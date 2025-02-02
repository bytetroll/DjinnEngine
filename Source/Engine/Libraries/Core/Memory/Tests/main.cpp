//
// Created by Dev on 8/11/2018.
//

#include <iostream>
#include <Core/SharedPtr.h>
#include <Core/WeakPtr.h>

DJINN_NS();

int main() {
    auto a = Core::MakeShared<int>();

    std::cout << a.Get() << "\n";

    Core::SharedPtr<int> e;

    {
        auto b = a;
        {
            e = b;
        }

        std::cout << **b << "\n";
    }

    std::cout << e.Get() << "\n";

    {
        Core::WeakPtr<int> w;
        {
            auto d = Core::MakeShared<int>(5);

            w = d;

            std::cout << w.Get() << "\n";
        }

        std::cout << w.Get() << "\n";
    }

    return 0;
}