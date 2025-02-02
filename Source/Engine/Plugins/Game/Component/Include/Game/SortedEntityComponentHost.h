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
 on 1/17/2018.
//
#pragma once

#include <COM/Result.h>
#include <Core/SlotArray.h>
#include <Core/SparseArray.h>
#include <Platform/RecursiveSpinMutex.h>
#include <Platform/LockGuard.h>
#include <Core/HashMap.h>
#include <Core/Buffer.h>
#include <Async/Async.h>
#include "EntityID.h"
#include "IEntityComponentHost.h"

namespace Djinn::Game {
    DJINN_COM_RESULT(DuplicateSortedComponent, true, "Game");

    template<typename T>
    class SortedEntityComponentHost : public IEntityComponentHost {
    public:
        struct Header {
            UInt32 key;
            Int64 offset;
            bool isTemporary = false;
        };

        struct Object {
            T com;
            USize cid = 0;
            UInt32 weight = 0;
            bool deleted = false;
        };

        Header ExtractHeader(Int64 uid) {
            Header header;
            header.key = static_cast<UInt32>(uid & 0x7FFF);
            header.offset = (uid << 32);
            return header;
        }

        /// Create a component
        /// \param eid
        /// \param component - Component value
        /// \param out - Resulting id
        /// \return
        COM::Result CreateComponent(const EntityComponentReference &ref) override {
            Platform::LockGuard guard(mutex);

            T com{};
            com.reference = ref;

            // Has header?
            if (indices.Contains(ref.entity.id.id)) {
                return kDuplicateSortedComponent;
            }

            // Insert
            Header header;
            header.isTemporary = true;
            indices[ref.entity.id.id] = header;
            temporaries.Add(ref.entity.id, {com, 0});

            // OK
            return COM::kOK;
        }

        /// Destroy a component with an id
        /// \param id
        /// \return
        COM::Result DestroyComponent(const EntityID &id) override {
            Platform::LockGuard guard(mutex);

            // Attempt to get
            Header header;
            if (!indices.Get(id.id, header)) {
                return COM::kError;
            }

            // May already be marked for deletion
            if (keyArray[header.key][header.offset].deleted) {
                return COM::kOK;
            }

            // Mark as deleted
            keyArray[header.key][header.offset].deleted = true;

            // ...
            deletions.Add(id);

            // OK
            return COM::kOK;
        }

        /// Get a component pointer
        /// \param cid
        /// \param out
        /// \return
        COM::Result GetComponentPtr(const EntityID &id, IEntityComponentState **out) override {
            //Platform::LockGuard guard(mutex);

            // Attempt to get
            Header header;
            if (!indices.Get(id.id, header)) {
                *out = nullptr;
                return COM::kError;
            }

            // Temporary?
            if (header.isTemporary) {
                auto it = temporaries.Find(id);
                if (it != temporaries.End()) {
                    *out = &(*it).second.first;
                    return COM::kOK;
                }
                *out = nullptr;
                return COM::kError;
            }

            // Outside range?
            if (keyArray[header.key].Size() <= static_cast<USize>(header.offset)) {
                return COM::kError;
            }
            *out = &keyArray[header.key][header.offset].com;

            // OK
            return COM::kOK;
        }

        COM::Result ChangeWeight(const EntityID &id, UInt32 weight) {
            Platform::LockGuard guard(mutex);
            weightChanges.Add({id, weight});
            return COM::kOK;
        }

        COM::Result ChangeWeightImmediate(const EntityID &id, UInt32 weight) {
            Platform::LockGuard guard(mutex);

            // Attempt to get
            Header header;
            if (!indices.Get(id.id, header)) {
                return COM::kError;
            }

            if (header.key == weight) {
                return COM::kError;
            }

            T com = keyArray[header.key][header.offset].com;
            {

                // Get cid
                auto otherCid = keyArray[header.key][header.offset].cid;

                // Remove component
                if (!keyArray[header.key].Remove(header.offset)) {
                    return COM::kError;
                }

                // Update index array
                if (static_cast<USize>(header.offset) < keyArray[header.key].Size()) {
                    indices[keyArray[header.key][header.offset].cid].offset = header.offset;
                }
            }

            {
                // Spawn
                Object obj;
                obj.cid = id.id;
                obj.com = com;
                obj.weight = weight;
                keyArray[weight].Add(obj);

                // Create header
                header.key = weight;
                header.offset = keyArray[weight].Size() - 1;
                indices[id.id] = header;
            }

            return COM::kOK;
        }

        /// Update all components
        /// \tparam F
        /// \param functor
        /// \return
        template<typename F>
        COM::Result UpdateAsync(Async::Group &group, F &&functor) {
            Platform::LockGuard guard(mutex);

            // Schedule single components
            for (auto it = keyArray.It(); it; ++it) {
                Async::Foreach(group, it->Size(), [&, it, functor](USize i) {
                    functor(it->Get(i).com);
                });
            }

            // OK
            return COM::kOK;
        }

        /// Update all components
        /// \tparam F
        /// \param functor
        /// \return
        template<typename F>
        COM::Result UpdateAscending(F &&functor) {
            Platform::LockGuard guard(mutex);

            // Schedule single components
            for (auto it = keyArray.It(); it; ++it) {
                for (USize i = 0; i < it->Size(); i++) {
                    if (functor(EntityID{it->Get(i).cid}, it->Get(i).com)) {
                        return COM::kOK;
                    }
                }
            }

            // OK
            return COM::kOK;
        }

        /// Update all components
        /// \tparam F
        /// \param functor
        /// \return
        template<typename F>
        COM::Result UpdateDescending(F &&functor) {
            Platform::LockGuard guard(mutex);

            // Schedule single components
            for (auto it = keyArray.RIt(); it; ++it) {
                for (USize i = 0; i < it->Size(); i++) {
                    if (functor(EntityID{it->Get(i).cid}, it->Get(i).com)) {
                        return COM::kOK;
                    }
                }
            }

            // OK
            return COM::kOK;
        }

        /// Does this container need an update?
        /// \return
        bool NeedsUpdate() {
            Platform::LockGuard guard(mutex);
            return indices.Size() > 0;
        }

        /// Apply all changes
        /// \return
        COM::Result ApplyChanges() override {
            {
                Platform::LockGuard guard(mutex);

                for (auto id : deletions) {
                    Header header;
                    if (!indices.Get(id.id, header)) {
                        continue;
                    }

                    // Remove index
                    indices.Remove(id.id);

                    // Remove component
                    if (!keyArray[header.key].Remove(header.offset)) {
                        continue;
                    }

                    // Update index array
                    if (keyArray[header.key].Size() > static_cast<USize>(header.offset)) {
                        indices[keyArray[header.key][header.offset].cid].offset = header.offset;
                    }
                }
                deletions.Clear();

                for (const auto &kv : temporaries) {
                    // Has header?
                    Header header;
                    if (!indices.Get(kv.first.id, header)) {
                        continue;
                    }

                    // Spawn
                    Object obj;
                    obj.cid = kv.first.id;
                    obj.com = kv.second.first;
                    obj.weight = kv.second.second;
                    keyArray[kv.second.second].Add(obj);

                    // Create header
                    header.key = kv.second.second;
                    header.offset = keyArray[kv.second.second].Size() - 1;
                    header.isTemporary = false;
                    indices[kv.first.id] = header;
                }
                temporaries.Clear();
            }

            for (const auto &kv : weightChanges) {
                ChangeWeightImmediate(kv.first, kv.second);
            }
            weightChanges.Clear();

            // OK
            return COM::kOK;
        }

        COM::Result Enable(const EntityID &id) override {
            return COM::kError;
        }

        COM::Result Disable(const EntityID &id) override {
            return COM::kError;
        }

    private:
        Platform::RecursiveSpinMutex mutex;

        /// Array for single components
        Core::SparseArray<Core::SlotArray<Object>, 32> keyArray;

        /// Index array
        Core::SparseArray <Header> indices;

        /// Buffered changes
        Core::Buffer <EntityID> deletions;
        Core::HashMap <EntityID, Core::Pair<T, UInt32>> temporaries;
        Core::Buffer <Core::Pair<EntityID, UInt32>> weightChanges;
    };
}