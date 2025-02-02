//
// Created by Dev on 11/2/2018.
//

#include <Host/InstanceHost.h>
#include <iostream>

DJINN_NS2(Host);

void Detail::LiveInstanceWarning(const char* name, Int64 count) {
    std::cout << "Leak detected during instance worldHost shutdown: " << count << " live instances of '" << name << "'\n";
}