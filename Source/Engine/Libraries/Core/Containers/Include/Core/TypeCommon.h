#pragma once

namespace Djinn {
    namespace Core {
        template<typename T>
        T Copy(T&& value) {
            return value;
        }

        template<typename T>
        T&& Forward(T& value) {
            return value;
        }

        template<typename T>
        T& Forward(T&& value) {
            return value;
        }
    }
}