#include "prz.h"
#include "UI/Widgets/MiniMapWidget.h"

#include <map>

static const int32 FAKE_MAP_SIZE = 15;
static const char* FAKE_MAP = 
    ".#####...#     "
    ".........#     "
    "..#.....##     "
    "  #.....###    "
    "####......###  "
    "............#  "
    "#######...#...."
    "      #@### ..."
    "#######.#   ..."
    "#.......###.#  "
    "#.##........#  "
    "..##........#  "
    "..######....#  "
    "..#    #....#  "
    "..#    #....#  "
    ;

FLinearColor CharToColor(const char ch) {
    std::map<char, FLinearColor> charToColor = {
        {' ', FLinearColor(0.0f, 0.0f, 0.0f)},
        {'#', FLinearColor(42 / 255.0f, 47 / 255.0f, 51 / 255.0f)},
        {'.', FLinearColor(129 / 255.0f, 150 / 255.0f, 154 / 255.0f)},
        {'@', FLinearColor(236 / 255.0f, 118 / 255.0f, 0.0f)},
    };

    auto pos = charToColor.find(ch);
    if (pos != charToColor.end()) {
        return pos->second;
    }

    return FLinearColor::Red;
}

void SMiniMapWidget::Construct(const FArguments& InArgs) {
    OwnerHUD = InArgs._OwnerHUD;
}

int32 SMiniMapWidget::OnPaint(const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const {
    if (OwnerHUD.IsValid()) {
        const FSlateBrush* brush = new FSlateBrush();
        const FVector2D cellSize = FVector2D(2, 2);
        for (int32 x = 0; x < FAKE_MAP_SIZE; ++x) {
            for (int32 y = 0; y < FAKE_MAP_SIZE; ++y) {
                const int32 linearIndex = x + y * FAKE_MAP_SIZE;
                const FLinearColor color = CharToColor(FAKE_MAP[linearIndex]);
                FSlateDrawElement::MakeBox(OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(FVector2D(x * 2, y * 2), cellSize), brush, MyClippingRect, ESlateDrawEffect::None, color);
            }
        }

    }
    return SCompoundWidget::OnPaint(AllottedGeometry, MyClippingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}