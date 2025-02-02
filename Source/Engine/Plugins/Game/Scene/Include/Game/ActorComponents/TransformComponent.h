//
// Created by Dev on 8/30/2018.
//

#include "ITransformComponent.h"

namespace Djinn::Game {
    class DJINN_EXPORT_PLUGINS_GAME_SCENE TransformComponent : public ITransformComponent {
    public:
        DJINN_COM_CLASS();

        TransformComponent(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        void Update() override;
    };
}