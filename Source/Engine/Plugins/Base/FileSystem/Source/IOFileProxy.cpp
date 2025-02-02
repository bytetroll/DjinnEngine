//
// Created by Dev on 10/22/2017.
//

#include <Base/IOFileProxy.h>
#include <Core/Common.h>
#include <Platform/FileStream.h>
#include <Platform/FileSystem.h>
#include <Platform/LockGuard.h>

DJINN_NS2(Base);

COM::Result IOFileProxy::OpenStream(const PathID &path, Platform::FileMode mode, Core::IStream **out) {
    Platform::LockGuard guard(mutex);

    auto it = lut.Find(path);
    if (it == lut.End()) {
        return COM::kError;
    }

    // Attempt to open stream
    auto stream = streamPool.PopConstruct(mode, (*it).second.absolutePath);
    if (!stream->IsGood()) {
        return COM::kError;
    }

    // ...
    *out = stream;
    return COM::kOK;
}

void IOFileProxy::CloseStream(const PathID &path, Core::IStream *stream) {
    Platform::LockGuard guard(mutex);

    // TODO: Needed check?
    if (stream->IsGood()) {
        stream->Close();
    }
    streamPool.Push(static_cast<Platform::FileStream *>(stream));
}

COM::Result IOFileProxy::GetAbsolutePath(const PathID &path, Core::StringView *out) {
    Platform::LockGuard guard(mutex);

    auto it = lut.Find(path);
    if (it == lut.End()) {
        return COM::kError;
    }

    *out = (*it).second.absolutePath;
    return COM::kOK;
}

COM::Result IOFileProxy::GetVirtualPath(const PathID &path, Core::StringView *out) {
    Platform::LockGuard guard(mutex);

    auto it = lut.Find(path);
    if (it == lut.End()) {
        return COM::kError;
    }

    *out = (*it).second.virtualPath;
    return COM::kOK;
}

COM::Result IOFileProxy::GetName(const PathID &path, Core::StringView *out) {
    Platform::LockGuard guard(mutex);

    auto it = lut.Find(path);
    if (it == lut.End()) {
        return COM::kError;
    }

    *out = (*it).second.absolutePath;
    if (auto idx = out->LastIndexOf("/"); idx != -1) {
        *out = out->SubStr(idx + 1);
    }
    return COM::kOK;
}

COM::Result IOFileProxy::GetAttributes(const PathID &path, FileAttributeSet *out) {
    Platform::LockGuard guard(mutex);

    auto it = lut.Find(path);
    if (it == lut.End()) {
        return COM::kError;
    }

    *out = {};
    return COM::kOK;
}

COM::Result IOFileProxy::Register(const PathID &path, const Core::String &absolutePath, const Core::String &virtualPath) {
    Platform::LockGuard guard(mutex);

    Proxy proxy;

    proxy.absolutePath = absolutePath;
    proxy.absolutePath.Replace('\\', '/');
    proxy.absolutePath.Replace("//", "/");

    proxy.virtualPath = virtualPath;
    proxy.virtualPath.Replace('\\', '/');
    proxy.virtualPath.Replace("//", "/");

    lut.Add(path, proxy);
    return COM::kOK;
}

COM::Result IOFileProxy::Deregister(const PathID &path) {
    Platform::LockGuard guard(mutex);

    lut.Remove(path);
    return COM::kOK;
}

COM::Result IOFileProxy::GetStamp(const PathID &path, Int64 *out) {
    Platform::LockGuard guard(mutex);

    auto it = lut.Find(path);
    if (it == lut.End()) {
        return COM::kError;
    }

    *out = Platform::FileSystem::GetFileTimeStamp((*it).second.absolutePath);
    return COM::kOK;
}
