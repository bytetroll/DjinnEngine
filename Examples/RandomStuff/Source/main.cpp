//
// Created by Dev on 3/2/2018.
//

#include <Core/Array.h>
#include <Core/Timer.h>
#include <iostream>
#include <Core/HashMap.h>
#include <Core/SparseArray.h>
#include <Platform/Thread.h>
#include <Core/Atomic.h>
#include <Math/Vector2.h>

DJINN_NS();

int main() {
    UInt32 iterations = 1e7;
    UInt32 count = 1e6;

    Core::Timer timer;

    // ...
    Core::Array<int> arr0(count);
    Core::HashMap<int, int> arr1;
    Core::SparseArray<int> arr2;

    auto idx = [&]() { return static_cast<USize>(Math::RandFFast(0, count - 1)); };

    for (UInt32 i = 0; i < count; i++) {
        arr1[i] = 0;
    }
    for (UInt32 i = 0; i < count; i++) {
        arr2[i] = 0;
    }

    timer.Delta();
    for (UInt32 i = 0; i < iterations; i++) {
        arr0[idx()]++;
    }
    std::cout << "Raw: " << timer.Delta() << "s\n";

    timer.Delta();
    for (UInt32 i = 0; i < iterations; i++) {
        arr1[idx()]++;
    }
    std::cout << "Hash: " << timer.Delta() << "s\n";

    timer.Delta();
    for (UInt32 i = 0; i < iterations; i++) {
        arr2[idx()]++;
    }
    std::cout << "Sparse: " << timer.Delta() << "s\n";

    UInt32 sum = 0;
    for (UInt32 i = 0; i < iterations; i++) {
        sum += arr0[i % count] + arr1[i % count] + arr2[i % count];
    }
    std::cout << "... " << sum << "..." << "\n";

    return 0;
}