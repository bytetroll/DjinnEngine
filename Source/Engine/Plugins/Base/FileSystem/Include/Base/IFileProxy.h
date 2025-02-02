//
// Created by Dev on 10/24/2017.
//
#pragma once

#include <COM/IUnknown.h>
#include <Core/IStream.h>
#include <Platform/FileMode.h>
#include <Core/StringView.h>
#include "FileAttribute.h"
#include "Paths.h"

namespace Djinn::Base {
    /// Proxy for a file system
    /// Thread Safe
    class IFileProxy : public COM::TUnknown<IFileProxy> {
    public:
        DJINN_COM_INTERFACE();

        IFileProxy(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Open a stream to given path
        /// \param path
        /// \param mode - Incompatible mode may result in rejection
        /// \param out
        /// \return
        virtual COM::Result OpenStream(const PathID& path, Platform::FileMode mode, Core::IStream** out) = 0;

        /// Close an open stream
        /// \param path
        /// \param stream
        virtual void CloseStream(const PathID& path, Core::IStream* stream) = 0;

        /// Get absolute (real) path of given path, may not be available
        /// Must follow standardized path format (forward slashes)
        /// \param path
        /// \param out
        /// \return
        virtual COM::Result GetAbsolutePath(const PathID& path, Core::StringView* out) = 0;

        /// Get virtual absolute (real) path of given path
        /// Must follow standardized path format (forward slashes)
        /// \param path
        /// \param out
        /// \return
        virtual COM::Result GetVirtualPath(const PathID& path, Core::StringView* out) = 0;

        /// Get name of file
        /// \param path
        /// \param out
        /// \return
        virtual COM::Result GetName(const PathID& path, Core::StringView* out) = 0;

        /// Get time stamp of this file
        /// \param out
        /// \return
        virtual COM::Result GetStamp(const PathID& path, Int64* out) = 0;

        /// Get attributes of given path, may not be available
        /// \param path
        /// \param out
        /// \return
        virtual COM::Result GetAttributes(const PathID& path, FileAttributeSet* out) = 0;
    };
}