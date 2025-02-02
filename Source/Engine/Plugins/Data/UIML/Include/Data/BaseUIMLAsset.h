//
// Created by Dev on 1/28/2018.
//
#pragma once

#include "IUIMLAsset.h"
#include "UIMLContent.h"

namespace Djinn::Data {
    class DJINN_EXPORT_PLUGINS_DATA_UIML BaseUIMLAsset : public IUIMLAsset {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        BaseUIMLAsset(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        virtual UIMLContent *GetContent() override;

        /// Content
        UIMLContent content;
    };
}