//
// Created by Dev on 11/2/2018.
//

#include <Host/Plugin.h>

DJINN_NS2(Host);

Plugin::Plugin(IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host) : IPlugin(registry, outer, host) {

}
