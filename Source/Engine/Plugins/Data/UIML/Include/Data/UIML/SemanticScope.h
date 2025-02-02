//
// Created by Dev on 5/9/2018.
//
#pragma once

#include <Core/CompileHash.h>
#include <Core/FixedArray.h>

namespace Djinn::Data::UIML {
    struct SemanticScope {
        struct Entry {
            USize id;
            void* ptr;
        };

        template<typename T>
        T* Get() {
            auto id = GetTypeNameCRC64<T>();
            for (auto& entry : entries) {
                if (entry.id == id) {
                    return static_cast<T*>(entry.ptr);
                }
            }
            return nullptr;
        }

        template<typename T>
        void Remove() {
            auto id = GetTypeNameCRC64<T>();
            for (auto& entry : entries) {
                if (entry.id == id) {
                    entry.ptr = nullptr;
                }
            }
        }

        template<typename T>
        void Add(T* ptr) {
            auto id = GetTypeNameCRC64<T>();
            for (auto& entry : entries) {
                if (entry.id == id) {
                    entry.ptr = ptr;
                    return;
                }
            }

            Entry entry;
            entry.id = id;
            entry.ptr = ptr;
            entries.Add(entry);
        }

    private:
        Core::FixedArray32<Entry> entries;
    };
}
