//
// Created by Dev on 5/13/2018.
//
#pragma once

namespace Djinn::Core {
    class IStream;
}

namespace Djinn::Base::Memory {
    DJINN_EXPORT_PLUGINS_BASE_MEMORY void PrettyPrint(Core::IStream* stream);
}