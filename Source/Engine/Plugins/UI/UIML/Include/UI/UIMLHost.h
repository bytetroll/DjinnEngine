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
 on 5/9/2018.
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