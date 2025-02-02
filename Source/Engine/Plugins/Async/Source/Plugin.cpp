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
#include <Base/DefaultLogs.h>
#include <Async/Worker.h>
#include <Async/Scheduler.h>
#include <Async/Framework.h>
#include <Platform/User.h>

DJINN_NS2(Async);

DJINN_C_EXPORT_PLUGINS_ASYNC COM::Result DJINN_PLUGINAPI_INSTALL(Host::IRegistry *registry, Host::LoadFlagSet flags, Host::IPlugin* plugin) {
    // Pipeline integration
    if (flags & Host::LoadFlag::ePipeline) {
        // Initialize scheduler
        Scheduler *scheduler;
        if (!registry->CreateClass(plugin, &scheduler) || !scheduler->Initialize()) {
            ErrorLog(registry).Write("Failed to install plugin, could not initialize async scheduler");
            return COM::kError;
        }

        // Initialize default lanes
        if (!scheduler->CreateLane(Async::kDefault, Math::Max(1U, Platform::User::GetCoreCount())) ||
            !scheduler->CreateLane(Async::kResource, Math::Max(1U, Platform::User::GetCoreCount()))) {
            ErrorLog(registry).Write("Failed to install plugin, could not create default async lanes");
            return COM::kError;
        }

        // Initialize framework
        Framework *framework;
        if (!registry->CreateClass(plugin, &framework) || !framework->Initialize(scheduler)) {
            ErrorLog(registry).Write("Failed to install plugin, could not initialize async framework");
            return COM::kError;
        }

        // Pipeline
        Check(registry->GetPipeline()->AddInterface(scheduler));
        Check(registry->GetPipeline()->AddClass(framework));
    }
    return COM::kOK;
}

DJINN_C_EXPORT_PLUGINS_ASYNC COM::Result DJINN_PLUGINAPI_DESTROY(Host::IRegistry *registry) {
    return COM::kOK;
}
