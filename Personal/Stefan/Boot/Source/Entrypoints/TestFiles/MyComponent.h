#pragma once

#include <UI/IControl.h>
#include <UI/UIMLEvent.h>
#include <UI/Elements/UILabel.h>
#include <UI/Elements/UITextbox.h>

DJINN_NS();

/// Delegates
using MyComponentApplyDelegate = Core::Delegate<void(const Core::StringView& number, const Core::StringView& crc)>;

// Declaring the UIML class makes this component globally available anywhere else
ClassAttr(UIML::Class("my-component"))
class MyComponent : public UI::IControl {
public:
    DJINN_COM_CLASS();

    using IControl::IControl;

    void StaticInitialize() override {
        Construct("Runtime/MyComponent.uiml"_path);
    }

    void Created() override {

    }

public:
    // Setters make properties of components available in UIML
    // These are only for component declarations like <my-component text="..."/>, not styling
    // You can also declare your own styling attributes using StyleEvent.
    Attr(UIML::Setter("text"))
    void SetText(const Core::StringView &text) {
        FindChild<UI::UILabel>("label")->SetText(text);
    }

    // This is bound from UIML
    // Btw! This is completely type safe, invalid signatures will generate an error in the log
    void OnApply() {
        apply.Signal(
            this,
            FindChild<UI::UITextbox>("ccn")->GetText(),
            FindChild<UI::UITextbox>("crc")->GetText()
        );
    }

public:
    // Declares a bindable event
    // From UIML all that's needed is <my-component apply="SomeFunctionName"/>
    DJINN_UIML_EVENT("apply", BindApply, MyComponentApplyDelegate, apply);
};
