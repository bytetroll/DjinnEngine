//
// Created by Dev on 5/29/2018.
//
#pragma once

#include <Core/Size.h>
#include <Core/BitSet.h>
#include <Core/Assert.h>
#include <Core/FixedArray.h>
#include <Core/String.h>
#include "ID.h"

namespace Djinn::Reflection {
    /// Type qualifier
    enum class TypeQualifier {
        eConst,
    };
    DJINN_CORE_BITSET(TypeQualifier);

    /// A single type layer
    struct TypeLayer {
        /// Type of this layer
        enum Type {
            eNone,
            eClass,
            eVoid,
            eBool,
            eUChar,
            eChar,
            eShort,
            eUShort,
            eLong,
            eLongLong,
            eULong,
            eULongLong,
            eWChar,
            eFloat,
            eDouble,
            eInt,
            eUInt,
            eRef,
            eArray,
            ePointer,
            eFPtr
        } type;

        TypeLayer(Type type = eNone) : type(type) {

        }

        TypeLayer(const TypeLayer &other) : type(other.type) {
            switch (type) {
                case eArray: {
                    arraySize = other.arraySize;
                    break;
                }
                case eClass: {
                    _class.id = other._class.id;
                    _class.qualifiedName = other._class.qualifiedName;
                    break;
                }
                default:
                    break;
            }
        }

        ~TypeLayer() {

        }

        TypeLayer &operator=(const TypeLayer &other) {
            type = other.type;
            switch (type) {
                case eArray: {
                    arraySize = other.arraySize;
                    break;
                }
                case eClass: {
                    _class.id = other._class.id;
                    _class.qualifiedName = other._class.qualifiedName;
                    break;
                }
                case eFPtr: {
                    fptr.returnType = other.fptr.returnType;
                    fptr.argumentCount = other.fptr.argumentCount;
                    fptr.argumentTypes = other.fptr.argumentTypes;
                    fptr.isShorthand = other.fptr.isShorthand;
                    fptr.isConst = other.fptr.isConst;
                    break;
                }
                default:
                    break;
            }
            return *this;
        }

        static TypeLayer Primitive(Type type) {
            return TypeLayer(type);
        }

        static TypeLayer Array(USize count) {
            auto layer = TypeLayer(eArray);
            layer.arraySize = count;
            return layer;
        }

        static TypeLayer Class(ClassID id, const char *qualifiedName) {
            auto layer = TypeLayer(eClass);
            layer._class.id = id;
            layer._class.qualifiedName = qualifiedName;
            return layer;
        }

        template<typename... T>
        static TypeLayer FPtr(class TypeInfo *returnType, bool isConst, bool isShorthand, T... args) {
            auto layer = TypeLayer(eFPtr);
            layer.fptr.returnType = returnType;
            layer.fptr.argumentTypes = new TypeInfo *[sizeof...(T)];
            layer.fptr.isConst = isConst;
            layer.fptr.isShorthand = isShorthand;
            layer.fptr.argumentCount = 0;
            layer.Add(args...);
            return layer;
        }

        /// Get the array size
        /// \return
        USize GetArraySize() const {
            DJINN_ASSERT(type == eArray);
            return arraySize;
        }

        /// Get the target class id
        /// \return
        ClassID GetClassID() const {
            DJINN_ASSERT(type == eClass);
            return _class.id;
        }

        /// Get the target class qualified name
        /// \return
        const char *GetClassQualifiedName() const {
            DJINN_ASSERT(type == eClass);
            return _class.qualifiedName;
        }

        /// Is this layer constant?
        /// \return
        bool IsConst() const {
            return isConst;
        }

        /// Data
        union {
            USize arraySize;
            struct {
                ClassID id;
                const char *qualifiedName;
            } _class;
            struct {
                class TypeInfo *classType;
                class TypeInfo *returnType;
                class TypeInfo **argumentTypes;
                UInt32 argumentCount;
                bool isConst;
                bool isShorthand;
            } fptr;
        };

        /// Is this layer constant?
        bool isConst = false;

    private:
        template<typename T0, typename T1, typename... TX>
        void Add(T0 &&t0, T1 &&t1, TX &&... tx) {
            Add(t0);
            Add(t1, tx...);
        }

        template<typename T0>
        void Add(T0 &&t0) {
            fptr.argumentTypes[fptr.argumentCount++] = t0;
        }
    };

    class TypeInfo {
    public:
        /// Make from layers
        /// \param layers
        /// \return
        template<typename T, typename... TX>
        static TypeInfo Make(TX &&... layers) {
            USize size = 0;
            if constexpr(!Core::IsSame<T, void>::kValue) {
                size = sizeof(T);
            }
            auto info = TypeInfo(size, GetTypeNameCRC64<T>(), GetStaticTypeName<T>());
            info.Add(layers...);
            return info;
        }

        /// Get the full decorated name
        /// \return
        const char *GetDecoratedName() const {
            return name;
        }

        /// Get the actual target byte size
        /// \return
        USize GetByteSize() const {
            return byteSize;
        }

        /// Get the number of layers
        /// \return
        UInt8 GetLayerCount() const {
            return static_cast<UInt8>(layers.Size());
        }

        /// Get the layers
        /// \return
        TypeLayer *GetLayers() {
            return layers.Ptr();
        }

        /// Get the combined hash
        /// \return
        USize GetHash() const {
            return hash;
        }

        /// Check if a runtime type is of a compile time
        /// \tparam T
        /// \return
        template<typename U>
        bool Is() const {
            return hash == GetTypeNameCRC64<U>();
        }

        /// Is this type a primitive type?
        /// \return
        bool IsPrimitive() const {
            return layers[layers.Size() - 1].type != TypeLayer::eClass;
        }

    private:
        TypeInfo(USize byteSize, USize hash, const char *name) : byteSize(byteSize), hash(hash), name(name) {

        }

        template<typename T0, typename T1, typename... TX>
        void Add(T0 &&t0, T1 &&t1, TX &&... tx) {
            Add(t0);
            Add(t1, tx...);
        }

        template<typename T0>
        void Add(T0 &&t0) {
            layers.Add(t0);
        }

        void Add() {

        }

        USize byteSize = 0;
        USize hash = 0;
        Core::FixedArray12<TypeLayer> layers;
        const char *name;
    };
}