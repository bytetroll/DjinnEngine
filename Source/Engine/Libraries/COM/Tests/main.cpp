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
 on 8/12/2018.
//

#include <COM/IUnknown.h>
#include <iostream>

DJINN_NS();

class ITest : public COM::TUnknown<ITest> {
public:
    DJINN_COM_INTERFACE();

    ITest(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

    }
};

class Test : public ITest {
public:
    DJINN_COM_CLASS();

    Test(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ITest(registry, outer, host) {

    }

    ~Test() {
        std::cout << "dead\n";
    }
};

int main() {
    Core::WeakPtr<Test> ptr;

    {
        auto test = new Test(nullptr, nullptr, nullptr);

        ptr = test->GetWeakPtr();

        auto t = test->GetWeakPtr();
        t->GetRegistry();

        std::cout << ptr.Get() << "\n";

        delete test;
    }

    std::cout << ptr.Get() << "\n";

    {
        Core::WeakPtr<Test> ptr2;

        auto outer = new Test(nullptr, nullptr, nullptr);

        {
            auto inner = new Test(nullptr, outer, nullptr);

            ptr2 = inner->GetWeakPtr();
            std::cout << ptr2.Get() << "\n";
        }

        delete outer;

        std::cout << ptr2.Get() << "\n";
    }

    {
        Core::SharedPtr<Test> ptr2;
        Core::SharedPtr<Test> ptr3;

        auto outer = new Test(nullptr, nullptr, nullptr);

        {
            auto inner = new Test(nullptr, outer, nullptr);

            ptr2 = inner->PromoteSharedPtr();
            ptr3 = inner->PromoteSharedPtr();
        }

        delete outer;

        std::cout << "----\n";
    }

    return 0;
}