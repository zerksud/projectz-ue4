#pragma once

#include "SlateWidgetStyleContainerBase.h"
#include "NavigationWidgetStyle.generated.h"

USTRUCT()
struct FNavigationStyle : public FSlateWidgetStyle {
    GENERATED_USTRUCT_BODY()

    FNavigationStyle() = default;
    virtual ~FNavigationStyle() = default;

    static const FName TypeName;
    virtual const FName GetTypeName() const OVERRIDE{
        return TypeName;
    }
    static const FNavigationStyle& GetDefault() {
        static FNavigationStyle instance;
        return instance;
    }

    UPROPERTY(EditAnywhere, Category = Appearance)
    FButtonStyle ButtonStyle;
    FNavigationStyle& SetButtonStyle(const FButtonStyle& InButtonStyle) {
        ButtonStyle = InButtonStyle;
        return *this;
    }

    UPROPERTY(EditAnywhere, Category = Appearance)
    FSlateBrush TurnLeftButtonIcon;
    FNavigationStyle& SetTurnLeftButtonIcon(const FSlateBrush& value) {
        TurnLeftButtonIcon = value;
        return *this;
    }

    UPROPERTY(EditAnywhere, Category = Appearance)
    FSlateBrush MoveForwardButtonIcon;
    FNavigationStyle& SetForwardButton(const FSlateBrush& value) {
        MoveForwardButtonIcon = value;
        return *this;
    }

    UPROPERTY(EditAnywhere, Category = Appearance)
    FSlateBrush TurnRightButtonIcon;
    FNavigationStyle& SetTurnRightButtonIcon(const FSlateBrush& value) {
        TurnRightButtonIcon = value;
        return *this;
    }

    UPROPERTY(EditAnywhere, Category = Appearance)
    FSlateBrush StrafeLeftButtonIcon;
    FNavigationStyle& SetStrafeLeftButtonIcon(const FSlateBrush& value) {
        StrafeLeftButtonIcon = value;
        return *this;
    }

    UPROPERTY(EditAnywhere, Category = Appearance)
    FSlateBrush MoveBackwardButtonIcon;
    FNavigationStyle& SetMoveBackwardButtonIcon(const FSlateBrush& value) {
        MoveBackwardButtonIcon = value;
        return *this;
    }

    UPROPERTY(EditAnywhere, Category = Appearance)
    FSlateBrush StrafeRightButtonIcon;
    FNavigationStyle& SetStrafeRightButtonIcon(const FSlateBrush& value) {
        StrafeRightButtonIcon = value;
        return *this;
    }
};

UCLASS(hidecategories = Object, MinimalAPI)
class UNavigationWidgetStyle : public USlateWidgetStyleContainerBase {
    GENERATED_UCLASS_BODY()
public:
    UPROPERTY(EditAnywhere, Category = Appearance, meta = (ShowOnlyInnerProperties))
    FNavigationStyle NavigationStyleField;

    virtual const struct FSlateWidgetStyle* const GetStyle() const OVERRIDE{
        return static_cast<const struct FSlateWidgetStyle*>(&NavigationStyleField);
    }
};
