#pragma once

#include <COM/ID.h>
#include <COM/IUnknown.h>
#include <Core/Delegate.h>
#include <Core/StringView.h>
#include <Platform/Library.h>
#include "COM/IInstanceHost.h"
#include "InstanceHost.h"
#include "IPipeline.h"
#include "PluginAPI.h"

namespace Djinn {
    namespace Host {
        /// Common results
        DJINN_COM_RESULT(NoPluginInstallAddr, true, "Plugin");
        DJINN_COM_RESULT(NoPluginInfoAddr, true, "Plugin");
        DJINN_COM_RESULT(BadPluginInstall, true, "Plugin");
        DJINN_COM_RESULT(NoPluginDestroyAddr, true, "Plugin");
        DJINN_COM_RESULT(DuplicateClass, true, "Plugin");
        DJINN_COM_RESULT(UnknownClass, true, "Plugin");
        DJINN_COM_RESULT(PluginNotLoaded, true, "Plugin");

        /// Delegates
        using PluginResolvedDelegate = Core::Delegate<void(const Platform::Library &library)>;
        using PluginInstalledDelegate = Core::Delegate<void(const Core::StringView& module, double elapsed)>;
        using PluginDestroyedDelegate = Core::Delegate<void(const Core::StringView& module, double elapsed)>;
        using PluginFailedDelegate = Core::Delegate<void(const Core::StringView& module, const char *nativeError, LoadError error)>;
        using PluginResolveDelegate = Core::Delegate<void(UInt32)>;

        // Interface
        // Hosts plugins and classes
        class IRegistry {
        public:
            DJINN_COM_INTERFACEID();

            /// Load a plugin
            /// \param name
            /// \param flags
            /// \return
            virtual COM::Result LoadPlugin(const Core::StringView &name, LoadFlagSet flags = LoadFlag::eAll) = 0;

            /// Load all plugins
            /// \param flags
            /// \return
            virtual COM::Result LoadPlugins(const Core::StringView &pattern = "Plugins.*.dll", LoadFlagSet flags = LoadFlag::eAll) = 0;

            /// Unload a currently loaded plugin
            /// \param name
            /// \return
            virtual COM::Result UnloadPlugin(const Core::StringView &name) = 0;

            /// Unload all currently loaded plugins
            /// \param name
            /// \return
            virtual COM::Result UnloadPlugins() = 0;

            /// Install a class
            /// \param id
            /// \param iid - Top most interface
            /// \param host
            /// \param inheritHost
            /// \return
            virtual void InstallClass(const COM::ClassID &id, const COM::InterfaceID &iid, COM::IInstanceHost *host, bool inheritHost) = 0;

            /// Remove an installed class
            /// \param id
            /// \return
            virtual void RemoveClass(const COM::ClassID &id) = 0;

            /// Create an installed class
            /// \param id
            /// \param outer - Master outer class
            /// \param iid - IID of out parameter
            /// \param out
            /// \return
            virtual COM::Result CreateClass(const COM::ClassID &id, COM::IUnknown *outer, const COM::InterfaceID &iid, void **out) = 0;

            /// Get all installed classes with top most interface
            /// \param id
            /// \param count
            /// \param out
            /// \return
            virtual COM::Result GetClasses(const COM::InterfaceID &id, USize &count, COM::ClassID *out) = 0;

            /// Bind plugin installed event
            /// \param delegate
            virtual void BindPluginInstalled(const PluginInstalledDelegate &delegate) = 0;

            /// Bind plugin installed event
            /// \param delegate
            virtual void BindPluginDestroyed(const PluginDestroyedDelegate &delegate) = 0;

            /// Bind plugin failed event
            /// \param delegate
            virtual void BindPluginFailed(const PluginFailedDelegate &delegate) = 0;

            /// Bind begin loading
            /// \param delegate
            virtual void BindPluginResolve(const PluginResolveDelegate &delegate) = 0;

            /// Bind resolved
            /// \param delegate
            virtual void BindPluginResolved(const PluginResolvedDelegate &delegate) = 0;

            /// Check if this registry supports the given class id
            /// \param id
            /// \return
            virtual bool HasClass(const COM::ClassID &id) = 0;

            /// Get the default pipeline
            /// \return
            virtual IPipeline *GetPipeline() = 0;

            /// Install class
            template<class CLASS>
            void InstallClass(COM::IInstanceHost *host) {
                if constexpr(COM::HasKIID<CLASS>::kValue) {
                    InstallClass(CLASS::kCID, CLASS::kIID, host);
                } else {
                    InstallClass(CLASS::kCID, nullptr, host);
                }
            }

            /// Install class
            template<class CLASS>
            void InstallClass() {
                if constexpr(COM::HasKIID<CLASS>::kValue) {
                    InstallClass(CLASS::kCID, CLASS::kIID, new InstanceHost<CLASS>(), true);
                } else {
                    InstallClass(CLASS::kCID, nullptr, new InstanceHost<CLASS>(), true);
                }
            }

            /// Destroy class
            template<class CLASS>
            void RemoveClass() {
                RemoveClass(CLASS::kCID);
            }

            /// Create class
            template<class INTERFACE>
            COM::Result CreateClass(const COM::ClassID &cid, COM::IUnknown *outer, INTERFACE **out) {
                void *obj = nullptr;
                if (COM::CheckedResult result = CreateClass(cid, outer, INTERFACE::kIID, &obj)) {
                    return result;
                }
                *out = static_cast<INTERFACE *>(obj);
                return COM::kOK;
            }

            /// Create class
            template<class CLASS, class ICLASS>
            COM::Result CreateClass(COM::IUnknown *outer, CLASS **out) {
                void *obj = nullptr;
                if (COM::CheckedResult result = CreateClass(CLASS::kCID, outer, ICLASS::kIID, &obj)) {
                    return result;
                }
                *out = static_cast<CLASS *>(static_cast<ICLASS *>(obj));
                return COM::kOK;
            }

            /// Create class
            template<class CLASS>
            COM::Result CreateClass(COM::IUnknown *outer, CLASS **out) {
                void *obj = nullptr;
                if (COM::CheckedResult result = CreateClass(CLASS::kCID, outer, COM::IUnknown::kIID, &obj)) {
                    return result;
                }
                *out = static_cast<CLASS *>(static_cast<COM::IUnknown *>(obj));
                return COM::kOK;
            }
        };
    }
}