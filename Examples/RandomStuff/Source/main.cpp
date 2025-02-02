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
 on 3/2/2018.
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