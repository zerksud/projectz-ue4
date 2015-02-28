#include "przCorePCH.h"
#include "Model/FieldOfViewBuilder.h"

#include <utility>

#include "Model/Matrix2D.h"

#include "Utils/MatrixHelpers.h"
#include "Utils/LOG.h"

namespace prz {
namespace mdl {

struct FieldOfViewData {
    const ZPosition viewerPosition;
    const int viewDistance;
    const ZDungeonLevel& dungeonLevel;
    EDungeonCell::Type** cells;

    FieldOfViewData(const ZPosition& pViewerPosition, int pViewDistance, const ZDungeonLevel& pDungeonLevel) :
    viewerPosition(pViewerPosition),
    viewDistance(pViewDistance),
    dungeonLevel(pDungeonLevel) {
        int size = viewDistance * 2 + 1;
        utl::ZMatrix::Allocate(&cells, size, size, EDungeonCell::Unknown);
    }

    bool CellIsIsTooFar(const ZPositionDiff& positionDiff) const {
        return positionDiff.LengthSquare() >= viewDistance * viewDistance;
    }

    void MarkCellAsVisible(const ZPositionDiff& realSectorPositionDiff) {
        ZPosition position = viewerPosition + realSectorPositionDiff;
        cells[viewDistance + realSectorPositionDiff.GetdX()][viewDistance + realSectorPositionDiff.GetdY()] = dungeonLevel.GetCellType(position);
    }
    
    bool CellIsSolid(const ZPositionDiff& realSectorPositionDiff) {
        ZPosition position = viewerPosition + realSectorPositionDiff;
        return dungeonLevel.CellIsSolid(position);
    }
};

struct SectorScanData {
    FieldOfViewData* fieldOfViewData;
    const ZPositionDiff slopeDiff;
    const ZMatrix2D conversionMatrix;

    SectorScanData(FieldOfViewData* pFieldOfViewData, const ZMatrix2D pConversionMatrix):
    fieldOfViewData(pFieldOfViewData),
    conversionMatrix(pConversionMatrix) {
    }
};

namespace CellSlope {
    enum Type {
        Left,
        Right
    };
}

static float CalcSlope(const ZPositionDiff& positionDiff, CellSlope::Type slope) {
    static const float kSlopeRowDiff[] = {-0.5f, 0.5f};

    float slopeDeltaX = positionDiff.GetdX() + kSlopeRowDiff[slope];
    float slopeDeltaY = positionDiff.GetdY() - kSlopeRowDiff[slope];

    return slopeDeltaX / slopeDeltaY;
}

static const ZPositionDiff kLeftEdgeDiff = ZPositionDiff(-1, -1);
static const ZPositionDiff kRightEdgeDiff = ZPositionDiff(0, -1);
static const ZPositionDiff kRowDiff = kRightEdgeDiff - kLeftEdgeDiff;

void ScanSector(SectorScanData* sectorScanData, int rowNumber, float leftMaxSlope, float rightMinSlope) {
    LOGD("new sector scan from row #%d, slope %f -> %f", rowNumber, leftMaxSlope, rightMinSlope);
    if (leftMaxSlope < rightMinSlope) {
        LOGD("left slope < right slope; abort current scan");
        return;
    }

    FieldOfViewData* fieldOfViewData = sectorScanData->fieldOfViewData;

    float currentRowLeftMaxSlope = 0.0f;
    float nextRowLeftMaxSlope = leftMaxSlope;

    bool previousCellIsSolid = false;
    
    for (int currentRowNumber = rowNumber; currentRowNumber <= fieldOfViewData->viewDistance && !previousCellIsSolid; ++currentRowNumber) {
        ZPositionDiff rowFirstCellDiff = kLeftEdgeDiff * currentRowNumber;
        ZPositionDiff rowCellAfterLastDiff = kRightEdgeDiff * currentRowNumber + kRowDiff;

        currentRowLeftMaxSlope = nextRowLeftMaxSlope;
        
        LOGD("scan row #%d, slope %f -> %f", currentRowNumber, currentRowLeftMaxSlope, rightMinSlope);
        for (ZPositionDiff cellDiff = rowFirstCellDiff; cellDiff != rowCellAfterLastDiff; cellDiff += kRowDiff) {
            float cellRightSlope = CalcSlope(cellDiff, CellSlope::Right);
            if (cellRightSlope > currentRowLeftMaxSlope) {
                continue;
            }

            float cellLeftSlope = CalcSlope(cellDiff, CellSlope::Left);
            if (cellLeftSlope < rightMinSlope) {
                break;
            }
            
            if (fieldOfViewData->CellIsIsTooFar(cellDiff)) {
                continue;
            }
            
            LOGD("cell %s with slopes [%f; %f] is visible", cellDiff.ToString().c_str(), cellLeftSlope, cellRightSlope);
            ZPositionDiff realSectorCellDiff = sectorScanData->conversionMatrix * cellDiff;
            fieldOfViewData->MarkCellAsVisible(realSectorCellDiff);
            
            bool cellIsSolid = fieldOfViewData->CellIsSolid(realSectorCellDiff);
            if (cellIsSolid) {
                nextRowLeftMaxSlope = cellRightSlope;
                if (previousCellIsSolid) {
                    continue;
                } else {
                    LOGD("current cell is first solid; initiating subscan");
                    ScanSector(sectorScanData, currentRowNumber + 1, currentRowLeftMaxSlope, cellLeftSlope);
                }
            } else if (previousCellIsSolid) {
                currentRowLeftMaxSlope = nextRowLeftMaxSlope;
            }

            previousCellIsSolid = cellIsSolid;
        }
    }
}

static const int kSectorCount = 8;
static ZMatrix2D kFirstSectorConversionMatrices[] = {
    ZMatrix2D(),
    ZMatrix2D::FlipX(),
    ZMatrix2D::Rotate(1),
    ZMatrix2D::FlipX() * ZMatrix2D::Rotate(1),
    ZMatrix2D::Rotate(2),
    ZMatrix2D::FlipX() * ZMatrix2D::Rotate(2),
    ZMatrix2D::Rotate(3),
    ZMatrix2D::FlipX() * ZMatrix2D::Rotate(3)
};

ZFieldOfView ZFieldOfViewBuilder::Build(const ZPosition& position, int viewDistance, const ZDungeonLevel& dungeonLevel) {
    LOGD("build FOV from %s", position.ToString().c_str());
    
    FieldOfViewData fieldOfViewData(position, viewDistance, dungeonLevel);
    fieldOfViewData.MarkCellAsVisible(ZPositionDiff(0, 0));
    
    for (int sectorIndex = 0; sectorIndex < kSectorCount; ++sectorIndex) {
        SectorScanData sectorScaneData(&fieldOfViewData, kFirstSectorConversionMatrices[sectorIndex]);
        LOGD("Scan sector %d", sectorIndex);
        ScanSector(&sectorScaneData, 1, 1.0f, 0.0f);
    }

    return ZFieldOfView(viewDistance, fieldOfViewData.cells);
}

}
}
