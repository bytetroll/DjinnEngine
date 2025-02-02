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
 on 8/16/2017.
//

#include <Core/Common.h>
#include <Host/IRegistry.h>
#include <Host/PluginAPI.h>
#include <Graphics/IGraphicsSystem.h>
#include <Graphics/IGraph.h>
#include <Base/DefaultLogs.h>
#include <Graphics/IPipeline.h>
#include <Graphics/Output.h>
#include <Graphics/OutputPass.h>
#include <Graphics/OutputHost.h>
#include <Graphics/IProcess.h>
#include <Graphics/ScreenOutput.h>

DJINN_NS();

DJINN_C_EXPORT_PLUGINS_GRAPHICS_OUTPUT COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet flags, Host::IPlugin* plugin) {
    if (flags & Host::LoadFlag::ePipeline) {
        // Get graphics system
        Graphics::IGraphicsSystem *system;
        if (!registry->GetPipeline()->GetInterface(&system)) {
            ErrorLog(registry).Write("Failed to initialize output plugin, expected graphics system to be installed");
            return COM::kError;
        }

        // Get process
        auto process = system->GetGraph()->GetProcess();

        // Install output pass
        Graphics::OutputPass *pass;
        if (!registry->CreateClass(plugin, &pass) || !pass->Initialize() || !process->GetPipeline()->Register(pass)) {
            ErrorLog(registry).Write("Failed to initialize output plugin, failed to install output pass");
            return COM::kError;
        }

        // Install output worldHost
        Graphics::OutputHost* host;
        if (!registry->CreateClass(plugin, &host) || !host->Initialize(pass) || !registry->GetPipeline()->AddInterface(host)) {
            ErrorLog(registry).Write("Failed to initialize output plugin, failed to install output worldHost");
            return COM::kError;
        }
    }
    return COM::kOK;
}

DJINN_C_EXPORT_PLUGINS_GRAPHICS_OUTPUT COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    return COM::kOK;
}
