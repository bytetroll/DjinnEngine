//
// Created by Dev on 6/1/2018.
//
#pragma once

#include <COM/IUnknown.h>
#include <Core/Array.h>

DJINN_NS();

class Boo : public COM::IUnknown {
public:
    DJINN_COM_CLASS();

    Boo(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : COM::IUnknown(registry, outer, host) {

    }

    float c;
};

class Foo : public Boo {
public:
    DJINN_COM_CLASS();

    Foo(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : Boo(registry, outer, host) {

    }

    int a[50];
    char b;
};

