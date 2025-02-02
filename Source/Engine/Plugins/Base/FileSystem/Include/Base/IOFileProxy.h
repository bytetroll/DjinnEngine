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
#pragma once

#include <Core/HashMap.h>
#include <Core/Pool.h>
#include <Platform/SpinMutex.h>
#include "IFileProxy.h"
#include "Paths.h"

namespace Djinn::Platform {
    class FileStream;
}

namespace Djinn::Base {
    /// Standard io based file proxy
    class DJINN_EXPORT_PLUGINS_BASE_FILESYSTEM IOFileProxy : public IFileProxy {
    public:
        DJINN_COM_CLASS();

        IOFileProxy(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IFileProxy(registry, outer, host) {

        }

        /// Register a proxy
        /// \param path
        /// \param absolute
        /// \return
        COM::Result Register(const PathID &path, const Core::String &absolutePath, const Core::String &virtualPath);

        /// Deregister a proxy
        /// \param path
        /// \return
        COM::Result Deregister(const PathID &path);

        /// Overrides
        COM::Result OpenStream(const PathID &path, Platform::FileMode mode, Core::IStream **out) override;
        COM::Result GetName(const PathID &path, Core::StringView *out) override;
        void CloseStream(const PathID &path, Core::IStream *stream) override;
        COM::Result GetAbsolutePath(const PathID &path, Core::StringView *out) override;
        COM::Result GetVirtualPath(const PathID &path, Core::StringView *out) override;
        COM::Result GetAttributes(const PathID &path, FileAttributeSet *out) override;
        virtual COM::Result GetStamp(const PathID &path, Int64 *out) override;

    private:
        Platform::SpinMutex mutex;

        struct Proxy {
            Core::String absolutePath;
            Core::String virtualPath;
        };

        /// Proxy map
        Core::HashMap<PathID, Proxy> lut;

        /// Stream pool
        Core::ObjectPool<Platform::FileStream> streamPool;
    };
}