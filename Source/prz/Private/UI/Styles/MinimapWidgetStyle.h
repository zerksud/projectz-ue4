#pragma once

#include "Slate.h"
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
