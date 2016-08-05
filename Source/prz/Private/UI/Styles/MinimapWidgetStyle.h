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

#pragma once

#include "SlateBasics.h"
#include "SlateWidgetStyleContainerBase.h"

#include "UI/Styles/StyleHelpers.h"

#include "MinimapWidgetStyle.generated.h"

USTRUCT()
struct FMinimapStyle : public FSlateWidgetStyle {
    GENERATED_USTRUCT_BODY()

    FMinimapStyle() = default;
    virtual ~FMinimapStyle() = default;

    static const FName TypeName;
    virtual const FName GetTypeName() const override {
        return TypeName;
    }
    static const FMinimapStyle& GetDefault() {
        static FMinimapStyle instance;
        return instance;
    }

    UPROPERTY(EditAnywhere, Category = Appearance)
    FSlateBrush BackgroundImage;
    DECLARE_STYLE_FIELD_SETTER(FMinimapStyle, FSlateBrush, BackgroundImage)

    UPROPERTY(EditAnywhere, Category = Appearance)
    FColor UnknownCellColor;
    DECLARE_STYLE_FIELD_SETTER(FMinimapStyle, FColor, UnknownCellColor)

    UPROPERTY(EditAnywhere, Category = Appearance)
    FColor SolidRockCellColor;
    DECLARE_STYLE_FIELD_SETTER(FMinimapStyle, FColor, SolidRockCellColor)

    UPROPERTY(EditAnywhere, Category = Appearance)
    FColor EmptyCellColor;
    DECLARE_STYLE_FIELD_SETTER(FMinimapStyle, FColor, EmptyCellColor)

    UPROPERTY(EditAnywhere, Category = Appearance)
    FColor DownStaircaseCellColor;
    DECLARE_STYLE_FIELD_SETTER(FMinimapStyle, FColor, DownStaircaseCellColor)

    UPROPERTY(EditAnywhere, Category = Appearance)
    FColor UpStaircaseCellColor;
    DECLARE_STYLE_FIELD_SETTER(FMinimapStyle, FColor, UpStaircaseCellColor)

    UPROPERTY(EditAnywhere, Category = Appearance)
    FColor MonsterCellColor;
    DECLARE_STYLE_FIELD_SETTER(FMinimapStyle, FColor, MonsterCellColor)
};

UCLASS(hidecategories = Object, MinimalAPI)
class UMinimapWidgetStyle : public USlateWidgetStyleContainerBase {
    GENERATED_UCLASS_BODY()
public:
    UPROPERTY(EditAnywhere, Category = Appearance, meta = (ShowOnlyInnerProperties))
    FMinimapStyle MinimapStyleField;

    virtual const struct FSlateWidgetStyle* const GetStyle() const override {
        return static_cast<const struct FSlateWidgetStyle*>(&MinimapStyleField);
    }
};
