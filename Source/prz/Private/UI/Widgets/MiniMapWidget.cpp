// Copyright 2016 Max Molodtsov <zerksud@gmail.com>
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "przPCH.h"
#include "UI/Widgets/MinimapWidget.h"

#include <map>

#include "UI/Styles/PrzStyleManager.h"
#include "UI/Styles/MinimapWidgetStyle.h"

typedef std::map<prz::mdl::EDungeonCell, FColor> DungeonCellColorMap;

DungeonCellColorMap GetDungeonCellColorMap() {
    using namespace prz::mdl;

    const FMinimapStyle* style = &FPrzStyleManager::Get().GetWidgetStyle<FMinimapStyle>("MinimapWidgetStyle");
    DungeonCellColorMap cellToColorMap = {
        {EDungeonCell::Unknown, style->UnknownCellColor},
        {EDungeonCell::SolidRock, style->SolidRockCellColor},
        {EDungeonCell::Emptiness, style->EmptyCellColor},
        {EDungeonCell::DownStaircase, style->DownStaircaseCellColor},
        {EDungeonCell::UpStaircase, style->UpStaircaseCellColor},
        {EDungeonCell::Monster, style->MonsterCellColor},
    };

    return cellToColorMap;
}

FColor CellToColor(const DungeonCellColorMap& cellToColorMap, prz::mdl::EDungeonCell cell) {
    auto pos = cellToColorMap.find(cell);
    if (pos != cellToColorMap.end()) {
        return pos->second;
    }

    return FColor(255, 0, 0);
}

void SMinimapWidget::Construct(const FArguments& InArgs) {
    OwnerHUD = InArgs._OwnerHUD;
}

int32 SMinimapWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const {
    if (OwnerHUD.IsValid() && mMinimap.IsValid()) {
        const FVector2D backgroundSize = FVector2D(1, 1);
        const FMinimapStyle* style = &FPrzStyleManager::Get().GetWidgetStyle<FMinimapStyle>("MinimapWidgetStyle");
        FPaintGeometry paintGeometry = AllottedGeometry.ToPaintGeometry(FVector2D(0, 0), FVector2D(32, 32));
        FSlateDrawElement::MakeBox(OutDrawElements, LayerId, paintGeometry, &style->BackgroundImage, MyClippingRect);

        DungeonCellColorMap cellToColorMap = GetDungeonCellColorMap();
        const FSlateBrush* brush = new FSlateBrush();
        const FVector2D cellSize = FVector2D(1, 1);
        int mapSize = mMinimap->GetSize();
        int dx = 2;
        int dy = 2;

        for (int32 x = 0; x < mapSize; ++x) {
            for (int32 y = 0; y < mapSize; ++y) {
                const FColor color = CellToColor(cellToColorMap, mMinimap->GetCell(x, y));
                FSlateDrawElement::MakeBox(OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(FVector2D(x + dx, y + dy), cellSize), brush, MyClippingRect, ESlateDrawEffect::None, color);
            }
        }

    }

    return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyClippingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}

void SMinimapWidget::SetMinimap(const prz::mdl::ZMinimap& minimap) {
    mMinimap = TSharedPtr<prz::mdl::ZMinimap>(new prz::mdl::ZMinimap(minimap));
}