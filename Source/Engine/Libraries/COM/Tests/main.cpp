//
// Created by Dev on 8/12/2018.
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