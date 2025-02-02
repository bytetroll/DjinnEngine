//
// Created by Dev on 4/28/2017.
//

#include <Core/Common.h>
#include <Core/Array.h>

DJINN_NS2(Core);

void TestArray() {
    Array<int> arr;
    arr.Add(5);
    arr.AddFront(2);
    arr.Clear(true);
}

int main() {
    return 0;
}