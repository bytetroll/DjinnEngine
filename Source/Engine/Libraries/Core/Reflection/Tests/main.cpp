//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 2/12/2018.
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