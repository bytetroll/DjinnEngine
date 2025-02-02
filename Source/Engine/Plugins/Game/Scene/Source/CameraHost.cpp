//
// Created by Dev on 3/2/2018.
//

#include <Game/CameraHost.h>
#include <Base/DefaultLogs.h>

DJINN_NS2(Game);

CameraHost::CameraHost(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ICameraHost(registry, outer, host) {

}

COM::Result CameraHost::Initialize() {
    // Diagnostic
    InfoLog(this).Write("Initialized");
    return COM::kOK;
}

COM::Result CameraHost::Register(ICamera* camera) {
    batchers.Add(camera);
    return COM::kOK;
}

COM::Result CameraHost::Deregister(ICamera* camera) {
    batchers.RemoveValue(camera);
    return COM::kOK;
}

Core::ArrayView<ICamera *> CameraHost::GetCameras() {
    return batchers;
}


