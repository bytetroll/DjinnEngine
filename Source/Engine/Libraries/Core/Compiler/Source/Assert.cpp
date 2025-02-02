//
// Created by Dev on 6/2/2018.
//

#include <iostream>
#include <Core/Assert.h>
#include <Core/Common.h>

DJINN_NS();

void Djinn::CErr(const char* str) {
    std::cerr << str;
}