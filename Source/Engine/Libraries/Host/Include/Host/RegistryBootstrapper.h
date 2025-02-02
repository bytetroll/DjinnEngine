//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//>> Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//>> RegisterBootstrapper.h
//>>
//>>    Allows the bootstrapping of registry IUnknown COM classes without having
//>>    to be in a plugin.
//>>
//>>    NOTE: IRegistry is designed to work with the plugin system.  Using the bootstrapper
//>>          is not the recommend way of registering a class to the registery.  This class
//>>          is designed for and used by specific registry plugins for special cases.
//>>
//>>    @TODO: This still needs a lot of work since bootstrapping a registry plugin
//>>           requires a registry instance passed by to the dynamic library on load.
//>>
//>> (Revision)
//>>    + 10/14/2018 - Nathan Young - Initial creation.
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#pragma once

// Engine core.
#include <Core/Common.h>
#include <Core/Array.h>
#include <Core/String.h>

// Engine COM.
#include <COM/IUnknown.h>

// Engine worldHost.
#include <Host/IRegistry.h>

namespace Djinn {
    namespace Host {
        enum class RegistryBootstrapResult : UInt32 {
            eSuccessful,
            eFail
        };

        RegistryBootstrapResult RegistryBootstrapCallback();
        using RegistryBootstrap = decltype ( ( RegistryBootstrapCallback() ) );

        #if DJINN_USE_REGISTRY_BOOTSTRAPPER
            #define DJINN_BOOTSTRAP( CALLBACK ) RegistryBootstrapResult DjinnB_Bootstrap_Callback##CALLBACK = CALLBACK();
        #else
            #define DJINN_BOOTSTRAP( CALLBACK )
        #endif
    }
}