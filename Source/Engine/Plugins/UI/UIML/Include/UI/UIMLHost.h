//
// Created by Dev on 5/9/2018.
//
#pragma once

#include <Core/HashMap.h>
#include <Data/UIMLContent.h>
#include <Core/StackArray.h>
#include <Core/Buffer.h>
#include <Game/Entity.h>
#include <Core/StringView.h>
#include <Platform/RecursiveMutex.h>
#include "IUIMLHost.h"

namespace Djinn::Reflection {
    class IReflectionRegistry;
    class IClassInfo;
    class IMethodInfo;
    class IMemberInfo;
}

namespace Djinn::Game {
    class IActorSystem;
    class IScene;
    class IActorSystemInstance;
}

namespace Djinn::UI {
    class IControl;
    class IUITransformComponent;

    class DJINN_EXPORT_PLUGINS_UI_UIML UIMLHost : public IUIMLHost {
    public:
        DJINN_COM_CLASS();

        UIMLHost(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this worldHost
        /// \return
        COM::Result Initialize();

        /// Overrides
        void Register(COM::ClassID cid, const Core::StringView &name) override;
        COM::Result Build(IUICanvas* canvas, Data::IUIMLAsset *asset, COM::IUnknown* model, const Core::Visitor<void(IUIElement*)>& visitor) override;

    private:
        using Setter = Core::Delegate<void(void *frame, const Core::StringView &value)>;
        using Event = Core::Delegate<void(void *frame, Game::IActorSystemInstance *instance, const Core::DynamicDelegate &delegate)>;
        using Bind = Core::Delegate<void(void *frame, void* value)>;
        using Socket = Core::Delegate<void(void *frame, IUITransformComponent* transform)>;

        /// Component builder
        struct NativeComponent {
            COM::ClassID classID;
            Core::HashMap<USize, Setter> setters;
            Core::HashMap<USize, Event> events;
            Core::HashMap<USize, Socket> socketAttachs;
            Core::HashMap<USize, Socket> socketDetachs;
            Socket defaultSocketAttach;
            Socket defaultSocketDetach;
        };

        /// Info about the native component
        struct NativeComponentClass {
            Core::HashMap<USize, Bind> binds;
        };

        /// Model state
        struct ModelState {
            COM::IUnknown *instance = nullptr;
            Reflection::IClassInfo *modelClassInfo = nullptr;
            NativeComponentClass *nativeClass = nullptr;
        };

        /// Meta
        struct Meta {
            Game::IActorSystem *system;
            Game::IScene *scene;
            ModelState model;
            USize bindingIndex = 0;
        };

        /// Build a compiled body
        /// \param body
        /// \param parent
        /// \param state
        /// \param outerModel - May be null
        /// \return
        COM::Result BuildBody(const Data::UIML::CompiledBody &body, const Core::Delegate<void(IUITransformComponent *)> &parent, Meta meta);

    private:
        Platform::RecursiveMutex mutex;
        Reflection::IReflectionRegistry *reflectionRegistry;

        /// Compiled styles
        Core::HashMap<USize, Data::UIML::CompiledStyle> styles;

        /// Compiled groups
        Core::HashMap<USize, Data::UIML::CompiledGroup> groups;

        /// Compiled component builders
        Core::HashMap<USize, NativeComponent> nativeComponents;
        Core::HashMap<USize, NativeComponentClass> nativeComponentClasses;
    };
}