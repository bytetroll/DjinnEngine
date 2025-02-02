//
// Created by Dev on 10/18/2017.
//
#pragma once

#include <COM/IUnknown.h>
#include <Base/Paths.h>
#include <Core/IStream.h>
#include <Core/String.h>
#include <Data/Asset.h>
#include <Async/Async.h>

namespace Djinn::Data {
    class IAsset;
    class IAssetConfiguration;
    class IFileAssetCompiler;

    /// Common results
    DJINN_COM_RESULT(UnknownAssetExtension, true, "Data");

    class IAssetHost : public COM::TUnknown<IAssetHost> {
    public:
        DJINN_COM_INTERFACE();

        IAssetHost(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Compile an asset from a stream
        /// \param stream
        /// \param config
        /// \param out
        /// \return
        virtual COM::Result CompileAsset(Core::IStream *stream, IAssetConfiguration *config, IAsset **out) = 0;

        /// Compile asset from a file
        /// \param path
        /// \param config
        /// \param out
        /// \return
        virtual COM::Result CompileAsset(const Base::PathID &path, IAssetConfiguration *config, IAsset **out) = 0;

        /// Compile asset from a file and automatically determine the configuration
        /// \param path
        /// \param config
        /// \param out
        /// \return
        virtual COM::Result CompileAsset(const Base::PathID &path, IAsset **out) = 0;

        /// Compile an asset from a stream
        /// \param stream
        /// \param config
        /// \param out
        /// \return
        virtual AssetObject *CompileAssetObjectAsync(Core::IStream *stream, IAssetConfiguration *config) = 0;

        /// Compile asset from a file
        /// \param path
        /// \param config
        /// \param out
        /// \return
        virtual AssetObject *CompileAssetObjectAsync(const Base::PathID &path, IAssetConfiguration *config) = 0;

        /// Compile asset from a file and automatically determine the configuration
        /// \param path
        /// \param config
        /// \param out
        /// \return
        virtual AssetObject *CompileAssetObjectAsync(const Base::PathID &path) = 0;

        /// Load an asset from stream, does not query asset type
        /// \param path
        /// \return
        virtual COM::Result LoadAsset(Core::IStream *stream, IAsset **out) = 0;

        /// Load an asset from file
        /// \param path
        /// \return
        virtual COM::Result LoadAsset(const Base::PathID &path, IAsset **out) = 0;

        /// Save an asset to a stream
        /// \param path
        /// \return
        virtual COM::Result StoreAssetStream(Core::IStream *stream, IAsset *asset) = 0;

        /// Save an asset to a stream
        /// \param path
        /// \return
        virtual COM::Result StoreAsset(const Core::String &path, IAsset *asset) = 0;

        /// Load an asset from stream, does not query asset type
        /// \param path
        /// \return
        virtual AssetObject *LoadAssetObjectAsync(Core::IStream *stream) = 0;

        /// Load an asset from file
        /// \param path
        /// \return
        virtual AssetObject *LoadAssetObjectAsync(const Base::PathID &path) = 0;

        /// Async methods
        Asyncf(StoreAssetStream);
        Asyncf(StoreAsset);

        /// Register a file extension to an asset compiler
        /// \param extension
        /// \param compiler
        virtual void Register(const Core::String &extension, IFileAssetCompiler *compiler) = 0;

        /// Compile helper
        template<typename T>
        COM::Result CompileAsset(const Base::PathID &path, IAssetConfiguration *config, T **out) {
            IAsset *obj;
            if (COM::CheckedResult result = CompileAsset(path, config, &obj)) {
                return result;
            }

            if (!(*out = COM::Cast<T>(obj))) {
                return COM::kError;
            }
            return COM::kOK;
        }

        /// Compile helper
        template<typename T>
        COM::Result CompileAsset(Core::IStream *stream, IAssetConfiguration *config, T **out) {
            IAsset *obj;
            if (COM::CheckedResult result = CompileAsset(stream, config, &obj)) {
                return result;
            }

            if (!(*out = COM::Cast<T>(obj))) {
                return COM::kError;
            }
            return COM::kOK;
        }

        /// Compile helper
        template<typename T>
        COM::Result CompileAsset(const Base::PathID &path, T **out) {
            IAsset *obj;
            if (COM::CheckedResult result = CompileAsset(path, &obj)) {
                return result;
            }

            if (!(*out = COM::Cast<T>(obj))) {
                return COM::kError;
            }
            return COM::kOK;
        }

        /// Compile async helper
        template<typename T = Data::IAsset>
        Asset<T> CompileAssetAsync(const Base::PathID &path, IAssetConfiguration *config) {
            auto object = CompileAssetObjectAsync(path, config);
            return Asset<T>(object);
        }

        /// Compile async helper
        template<typename T = Data::IAsset>
        Asset<T> CompileAssetAsync(Core::IStream *stream, IAssetConfiguration *config) {
            auto object = CompileAssetObjectAsync(stream, config);
            return Asset<T>(object);
        }

        /// Compile async helper
        template<typename T = Data::IAsset>
        Asset<T> CompileAssetAsync(const Base::PathID &path) {
            auto object = CompileAssetObjectAsync(path);
            return Asset<T>(object);
        }

        /// Compile async helper
        template<typename T = Data::IAsset>
        Asset<T> LoadAsset(Core::IStream *stream) {
            auto object = LoadAssetObjectAsync(stream);
            return Asset<T>(object);
        }

        /// Compile async helper
        template<typename T = Data::IAsset>
        Asset<T> LoadAsset(const Base::PathID &path) {
            auto object = LoadAssetObjectAsync(path);
            return Asset<T>(object);
        }
    };
}