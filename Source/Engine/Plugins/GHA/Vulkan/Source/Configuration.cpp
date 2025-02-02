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
 on 12/21/2017.
//

#include <Host/IRegistry.h>
#include <GHA/Vulkan/Configuration.h>
#include <GHA/Vulkan/Instance.h>
#include <GHA/Vulkan/Interpreter.h>
#include <GHA/PipelineScheduler/Scheduler.h>

DJINN_NS3(GHA, Vulkan);

Configuration::Configuration(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IConfiguration(registry, outer, host) {

}

Core::String Configuration::GetDisplayName() {
    return "Vulkan";
}

COM::ClassID Configuration::GetInstanceCID() {
    return Instance::kCID;
}

COM::Result Configuration::GetSupportedSchedulerCIDs(UInt32 &count, COM::ClassID *out) {
    if (out) {
        *out = PipelineScheduler::Scheduler::kCID;
    } else {
        count = 1;
    }
    return COM::kOK;
}

COM::Result Configuration::GetSupportedInterpreterCIDs(const COM::ClassID &scheduler, UInt32 &count, COM::ClassID *out) {
    if (out) {
        if (count == 1) {
            *out = Interpreter::kCID;
        }
    } else {
        count = 1U * (scheduler == PipelineScheduler::Scheduler::kCID);
    }
    return COM::kOK;
}

COM::Result Configuration::Released() {
    // OK
    return COM::kOK;
}

