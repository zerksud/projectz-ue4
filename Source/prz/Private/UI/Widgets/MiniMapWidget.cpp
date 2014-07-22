#include "prz.h"
#include "UI/Widgets/MinimapWidget.h"

#include <map>

FLinearColor CellToColor(prz::mdl::EDungeonCell::Type cell) {
    using namespace prz::mdl;

    static std::map<char, FLinearColor> cellToColor = {
        {EDungeonCell::Unknown, FLinearColor(0.0f, 0.0f, 0.0f)},
        {EDungeonCell::SolidRock, FLinearColor(42 / 255.0f, 47 / 255.0f, 51 / 255.0f)},
        {EDungeonCell::Emptiness, FLinearColor(129 / 255.0f, 150 / 255.0f, 154 / 255.0f)},
        {EDungeonCell::DownStaircase, FLinearColor(170 / 255.0f, 198 / 255.0f, 203 / 255.0f)},
        {EDungeonCell::UpStaircase, FLinearColor(86 / 255.0f, 100 / 255.0f, 103 / 255.0f)},
        {EDungeonCell::Monster, FLinearColor(236 / 255.0f, 118 / 255.0f, 0.0f)},
    };

    auto pos = cellToColor.find(cell);
    if (pos != cellToColor.end()) {
        return pos->second;
    }

    return FLinearColor::Red;
}

void SMinimapWidget::Construct(const FArguments& InArgs) {
    OwnerHUD = InArgs._OwnerHUD;
}

int32 SMinimapWidget::OnPaint(const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const {
    if (OwnerHUD.IsValid() && mMinimap.IsValid()) {
        const FSlateBrush* brush = new FSlateBrush();
        const FVector2D cellSize = FVector2D(1, 1);

        int mapSize = mMinimap->GetSize();

        for (int32 x = 0; x < mapSize; ++x) {
            for (int32 y = 0; y < mapSize; ++y) {
                const FLinearColor color = CellToColor(mMinimap->GetCell(x, y));
                FSlateDrawElement::MakeBox(OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(FVector2D(x, y), cellSize), brush, MyClippingRect, ESlateDrawEffect::None, color);
            }
        }

    }

    return SCompoundWidget::OnPaint(AllottedGeometry, MyClippingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}

void SMinimapWidget::SetMinimap(const prz::mdl::ZMinimap& minimap) {
    using namespace prz::mdl;

    mMinimap = TSharedPtr<ZMinimap>(new ZMinimap(minimap));
}