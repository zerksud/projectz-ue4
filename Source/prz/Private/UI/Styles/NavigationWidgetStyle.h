#pragma once

#include "SlateBasics.h"
#include "SlateWidgetStyleContainerBase.h"

#include "UI/Styles/StyleHelpers.h"

#include "NavigationWidgetStyle.generated.h"

USTRUCT()
struct FNavigationStyle : public FSlateWidgetStyle {
    GENERATED_USTRUCT_BODY()

    FNavigationStyle() = default;
    virtual ~FNavigationStyle() = default;

    static const FName TypeName;
    virtual const FName GetTypeName() const override {
        return TypeName;
    }
    static const FNavigationStyle& GetDefault() {
        static FNavigationStyle instance;
        return instance;
    }

    UPROPERTY(EditAnywhere, Category = Appearance)
    FSlateBrush BackgroundImage;
    DECLARE_STYLE_FIELD_SETTER(FNavigationStyle, FSlateBrush, BackgroundImage)

    UPROPERTY(EditAnywhere, Category = Appearance)
    FButtonStyle ButtonStyle;
    DECLARE_STYLE_FIELD_SETTER(FNavigationStyle, FButtonStyle, ButtonStyle)

    UPROPERTY(EditAnywhere, Category = Appearance)
    FSlateBrush TurnLeftButtonIcon;
    DECLARE_STYLE_FIELD_SETTER(FNavigationStyle, FSlateBrush, TurnLeftButtonIcon)

    UPROPERTY(EditAnywhere, Category = Appearance)
    FSlateBrush MoveForwardButtonIcon;
    DECLARE_STYLE_FIELD_SETTER(FNavigationStyle, FSlateBrush, MoveForwardButtonIcon)

    UPROPERTY(EditAnywhere, Category = Appearance)
    FSlateBrush TurnRightButtonIcon;
    DECLARE_STYLE_FIELD_SETTER(FNavigationStyle, FSlateBrush, TurnRightButtonIcon)

    UPROPERTY(EditAnywhere, Category = Appearance)
    FSlateBrush StrafeLeftButtonIcon;
    DECLARE_STYLE_FIELD_SETTER(FNavigationStyle, FSlateBrush, StrafeLeftButtonIcon)

    UPROPERTY(EditAnywhere, Category = Appearance)
    FSlateBrush MoveBackwardButtonIcon;
    DECLARE_STYLE_FIELD_SETTER(FNavigationStyle, FSlateBrush, MoveBackwardButtonIcon)

    UPROPERTY(EditAnywhere, Category = Appearance)
    FSlateBrush StrafeRightButtonIcon;
    DECLARE_STYLE_FIELD_SETTER(FNavigationStyle, FSlateBrush, StrafeRightButtonIcon)
};

UCLASS(hidecategories = Object, MinimalAPI)
class UNavigationWidgetStyle : public USlateWidgetStyleContainerBase {
    GENERATED_UCLASS_BODY()
public:
    UPROPERTY(EditAnywhere, Category = Appearance, meta = (ShowOnlyInnerProperties))
    FNavigationStyle NavigationStyleField;

    virtual const struct FSlateWidgetStyle* const GetStyle() const override {
        return static_cast<const struct FSlateWidgetStyle*>(&NavigationStyleField);
    }
};
