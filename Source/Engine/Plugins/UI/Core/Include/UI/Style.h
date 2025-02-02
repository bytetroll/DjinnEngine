//
// Created by Dev on 9/9/2018.
//
#pragma once

#include <COM/IUnknown.h>
#include <Core/Optional.h>
#include <Core/Variant.h>
#include <Math/Vector4.h>
#include <Core/HashMap.h>
#include <Base/Paths.h>
#include <Core/Pool.h>
#include <Core/StackArray.h>
#include <Reflection/Conversion.h>
#include <Base/DefaultLogs.h>

namespace Djinn {
    namespace UI {
        /// IDs
        DJINN_UNIQUEID(StyleID, UInt64);
        DJINN_UNIQUEID(StyleStateID, UInt64);
        DJINN_UNIQUEID(StyleClassID, UInt64);
    }

    /// Common hashes
    template<> struct Hasher<UI::StyleID> { USize operator()(const UI::StyleID& value) const { return Hasher<UInt64>{}(value.Get()); }};
    template<> struct Hasher<UI::StyleStateID> { USize operator()(const UI::StyleStateID& value) const { return Hasher<UInt64>{}(value.Get()); }};
    template<> struct Hasher<UI::StyleClassID> { USize operator()(const UI::StyleClassID& value) const { return Hasher<UInt64>{}(value.Get()); }};
}

namespace Djinn::Reflection {
    class IReflectionRegistry;
}

namespace Djinn::UI {
    class IUIElement;

    /// Style property
    using StyleValue = Core::Variant<Core::String, float, Math::Vector2, Math::Vector3, Math::Vector4, Base::PathID>;

    /// Binding mode for a style class
    enum class StyleBinding {
        eClass,
        eType,
        eID
    };

    struct StyleClass {
        StyleClass(StyleBinding binding, USize id, USize state) : binding(binding), id(id), state(state) {

        }

        /// Get a value if present otherwise default
        /// \param id
        /// \param value
        /// \return
        const StyleValue& GetOrDefault(const StyleID& id, const StyleValue& value) {
            if (auto ptr = map.GetNullable(id)) {
                return *ptr;
            }
            return value;
        }

        /// Get a value of type if present otherwise default
        /// \param id
        /// \param value
        /// \return
        template<typename T>
        const T& GetOrDefault(const StyleID& id, const T& value) {
            if (auto ptr = map.GetNullable(id)) {
                return ptr->Get<T>();
            }
            return value;
        }

        /// Add a value
        /// \param id
        /// \param value
        void Add(const StyleID& id, const StyleValue& value) {
            map.Add(id, value);
            values.Emplace(id, value);
        }

        /// Remove a value
        /// \param id
        void Remove(const StyleID& id) {
            map.Remove(id);
            values.RemoveAll([&](auto& kv) { return kv.first == id; });
        }

        /// Get the type of this class
        /// \return
        StyleBinding GetType() const {
            return binding;
        }

        /// Get the id of this class
        /// \return
        USize GetID() const {
            return id;
        }

        /// Get the state of this class
        /// \return
        USize GetState() const {
            return state;
        }

        /// Get all values
        /// \return
        const Core::Array<Core::Pair<USize, StyleValue>>& GetValues() const {
            return values;
        }

    private:
        StyleBinding binding;
        USize id;
        USize state;

        Core::Array<Core::Pair<USize, StyleValue>> values;
        Core::HashMap<StyleID, StyleValue> map;
    };

    class ElementStyle {
    public:
        /// Get a value if present otherwise default
        /// \param id
        /// \param value
        /// \return
        Core::Optional<StyleValue> Get(const StyleID& id) const {
            for (const auto& kv : values) {
                if (kv.first == id) {
                    return kv.second;
                }
            }
            return {};
        }

        /// Get a value if present otherwise default
        /// \param id
        /// \param value
        /// \return
        StyleValue GetOrDefault(const StyleID& id, const StyleValue& value) const {
            for (const auto& kv : values) {
                if (kv.first == id) {
                    return kv.second;
                }
            }
            return value;
        }

        /// Get a value of type if present otherwise default
        /// \param id
        /// \param value
        /// \return
        template<typename T>
        const T& GetOrDefault(const StyleID& id, const T& value) const {
            for (const auto& kv : values) {
                if (kv.first == id) {
                    if (!kv.second.IsOf<T>()) {
                        ErrorLog(registry).Write("Property '", id.GetString(), "' is not of type '", GetStaticTypeName<T>(), "'");
                        return value;
                    }
                    return kv.second.Get<T>();
                }
            }
            return value;
        }

        /// Get a value of type if present otherwise default and use reflection enum convertion
        /// \param id
        /// \param value
        /// \return
        template<typename T>
        T ConvertOrDefault(const StyleID &id, const T &value) const {
            for (const auto& kv : values) {
                if (kv.first == id) {
                    T converted;
                    if (!kv.second.IsOf<Core::String>() || !Reflection::ToEnum<T>(registry, kv.second.Get<Core::String>().Hash(), converted)) {
                        ErrorLog(registry).Write("Failed to convert '", kv.second.Get<Core::String>(), "' to enum of type '", GetStaticTypeName<T>(), "'");
                        return value;
                    }

                    return converted;
                }
            }
            return value;
        }

        /// Helper
        int GetOrDefault(const StyleID& id, int value) const {
            return static_cast<int>(GetOrDefault(id, static_cast<float>(value)));
        }

        Core::StackArray8<Core::Pair<USize, StyleValue>> values;
        Host::IRegistry* registry;
    };

    /// Style container
    class DJINN_EXPORT_PLUGINS_UI_CORE Style : public COM::IUnknown {
    public:
        DJINN_COM_CLASS();

        Style(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this style
        /// \return
        COM::Result Initialize();

        /// Create a class
        /// \param type
        /// \param id
        /// \param state
        /// \return
        StyleClass* GetOrCreateClass(StyleBinding type, USize id, USize state);

        /// Compile a style of an element
        /// \param element
        /// \return
        ElementStyle CompileStyle(IUIElement* element);

    private:
        Reflection::IReflectionRegistry* reflectionRegistry;
        Core::HashMap<USize, StyleClass*> classes;
        Core::LazyObjectPool<StyleClass> classPool;
    };
}