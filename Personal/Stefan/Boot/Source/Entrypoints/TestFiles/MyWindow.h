#pragma once

#include <UI/IControl.h>
#include <UI/Elements/UILabel.h>

DJINN_NS();

// Declaring the UIML class makes this component globally available anywhere else
ClassAttr(UIML::Class("my-window"))
class MyWindow : public UI::IControl {
public:
    DJINN_COM_CLASS();

    using IControl::IControl;

    void StaticInitialize() override {
        Construct("Runtime/MyWindow.uiml"_path);
    }

    void Created() override {

    }

public:
    // This is bound from UIML
    // Btw! This is completely type safe, invalid signatures will generate an error in the log
    void ComponentApplied(const Core::StringView& number, const Core::StringView& crc) {
        InfoLog(this).Write("Stole #", number, " [", crc, "]");
    }
};
