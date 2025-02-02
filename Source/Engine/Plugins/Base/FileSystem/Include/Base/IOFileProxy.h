//
// Created by Dev on 10/22/2017.
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