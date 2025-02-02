//
// Created by Dev on 2/12/2018.
//

#include <Core/DynamicDelegate.h>
#include <Core/Common.h>
#include <iostream>
#include <Core/String.h>

DJINN_NS2(Core);

int CallMe() {
    std::cout << "ass";
    return 0;
}

void NoRef(Core::String str) {
    std::cout << str.AsCharArray() << "\n";
}

void Ref(Core::String& str) {
    std::cout << str.AsCharArray() << "\n";
}

void Ref2(Core::String&& str) {
    std::cout << str.AsCharArray() << "\n";
}

void test(Core::String&& str) {
    Ref(static_cast<Core::String&>(str));
    Ref2(static_cast<Core::String&&>(str));
}

int main() {
    Core::String a;

    DynamicDelegate noRef = Bindg(NoRef);
    DynamicDelegate ref = Bindg(Ref);
    DynamicDelegate ref2 = Bindg(Ref2);

    Core::String msg = "message";
    noRef.Invoke(msg);
    ref.Invoke<void, Core::String&>(msg);
    ref2.Invoke<void, Core::String>("message");

    std::string name = "data";
    std::string *pname = &name;

    DynamicDelegate del = [=]() {
        std::cout << *pname << "\n";
    };

    DynamicDelegate del2;
    del2 = del;
    del2();
    DynamicDelegate del3(del);
    del3();

    del();

    Delegate<int()> ass = Bindg(CallMe);
    ass.Invoke();

    return 0;
}