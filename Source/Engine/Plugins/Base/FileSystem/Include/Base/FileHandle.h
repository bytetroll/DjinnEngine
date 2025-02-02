//
// Created by Dev on 3/20/2018.
//
#pragma once

#include "IFileProxy.h"
#include "Paths.h"

namespace Djinn::Core {
    class IStream;
}

namespace Djinn::Base {
    class FileHandle {
    public:
        FileHandle() = default;

        /// Deconstruct
        ~FileHandle() {
            if (shouldRelease) {
                Release();
            }
        }

        /// Open the handle
        /// \param path
        /// \param stream
        /// \param deleter
        void Open(const PathID& path, Core::IStream* stream, IFileProxy* proxy) {
            this->path = path;
            this->stream = stream;
            this->proxy = proxy;
        }

        /// Manually release this file handle
        void Release() {
            if (proxy) {
                proxy->CloseStream(path, stream);
                stream = nullptr;
                proxy = nullptr;
            }
        }

        /// Mark this handle as manual release
        void ManualRelease() {
            shouldRelease = false;
        }

        /// Get path
        /// \return
        PathID GetPath() const {
            return path;
        }

        /// Get stream
        /// \return
        Core::IStream* GetStream() const {
            return stream;
        }

        /// Implicit stream
        /// \return
        operator Core::IStream*() const {
            return stream;
        }

    private:
        PathID path;
        Core::IStream* stream;
        IFileProxy* proxy;
        bool shouldRelease = true;
    };
}