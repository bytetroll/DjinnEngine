#pragma once

namespace Djinn::Async {
    struct ContextData {
        void* owner = nullptr;
        void* userData = nullptr;
    };
}