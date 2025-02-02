//
// Created by Dev on 10/18/2017.
//
#pragma once

#include <COM/IUnknown.h>

namespace Djinn::Data {
    class IAssetCompiler;

    // Common results
    DJINN_COM_RESULT(NoCompiler, true, "Data");

    class IAssetCompilerRegistry : public COM::TUnknown<IAssetCompilerRegistry> {
    public:
        DJINN_COM_INTERFACE();

        IAssetCompilerRegistry(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Add a serializer with no attachments
        /// \param serializer
        /// \return
        virtual COM::Result AddCompiler(IAssetCompiler *compiler) = 0;

        /// Add a serializer mapped to id
        /// \param id - Class ID the serializer handles
        /// \param serializer
        /// \return
        virtual COM::Result AddCompiler(const COM::ClassID& id, IAssetCompiler* compiler) = 0;

        /// Add a serializer mapped to id
        /// \param id - Class ID the serializer handles
        /// \param serializer
        /// \return
        virtual COM::Result AddCompiler(const COM::ClassID& id, const COM::ClassID& compilerID) = 0;

        /// Remove a serializer from id
        /// \param id
        /// \return
        virtual COM::Result RemoveCompiler(const COM::ClassID& id) = 0;

        /// Get serializer from id
        /// \param id
        /// \return
        virtual COM::Result GetCompiler(const COM::ClassID& id, IAssetCompiler** out) = 0;

        /// Get serializer with id
        /// \param id
        /// \return
        virtual COM::Result GetCompilerWithID(const COM::ClassID& id, IAssetCompiler** out) = 0;
    };
}