#pragma once

#include <NyLang/Core/Exception.h>
#include <NyLang/Core/StdTypes.h>
#include <NyLang/Core/COM/IUnknown.h>
#include <unordered_map>
#include <vector>
#include <NyLang/Core/Context.h>

namespace NyLang {
    LANG_EXCEPTION(RegistryMissingClass);

    class Registry {
    public:
        template <typename X>
        void RemoveClass() {
            auto&& it = instanceMap.find(X::kCID);
            if (it != instanceMap.end()) {
                for (int i = instances.size() - 1; i >= 0; i--) {
                    if (instances[i] == it->second) {
                        instances.erase(instances.begin() + i);
                    }
                }
                instanceMap.erase(it);
            }
        }

        template <typename X>
        X* AddClass(X* instance) {
            instanceMap[X::kCID] = static_cast<IUnknown*>(instance);
            instances.push_back(static_cast<IUnknown*>(instance));
            return instance;
        }

        template <typename X>
        X* AddClass(ClassID id, X* instance) {
            instanceMap[id] = static_cast<IUnknown*>(instance);
            instances.push_back(instance);
            return instance;
        }

        template<typename X>
        X* GetClass() {
            return static_cast<X*>(instanceMap[X::kCID]);
        }
        
        template <typename X>
        void RemoveInterface() {
            auto&& it = interfaceInstanceMap.find(X::kIID);
            if (it != interfaceInstanceMap.end()) {
                for (int i = instances.size() - 1; i >= 0; i--) {
                    if (interfaceInstances[i] == it->second) {
                        interfaceInstances.erase(interfaceInstances.begin() + i);
                    }
                }
                interfaceInstanceMap.erase(it);
            }
        }

        template <typename X>
        X* AddInterface(X* instance) {
            interfaceInstanceMap[X::kIID] = static_cast<IUnknown*>(instance);
            interfaceInstances.push_back(static_cast<IUnknown*>(instance));
            return instance;
        }

        template <typename X>
        X* AddInterface(InterfaceID id, X* instance) {
            interfaceInstanceMap[id] = static_cast<IUnknown*>(instance);
            interfaceInstances.push_back(instance);
            return instance;
        }

        template<typename X>
        X* GetInterface() {
            return static_cast<X*>(interfaceInstanceMap[X::kIID]);
        }

        Context context;
        UnorderedMap<ClassID::Type, IUnknown*> instanceMap;
        UnorderedMap<ClassID::Type, IUnknown*> interfaceInstanceMap;
        Vector<IUnknown*> instances;
        Vector<IUnknown*> interfaceInstances;
    };
}