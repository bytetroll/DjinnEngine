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
 on 10/22/2017.
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
