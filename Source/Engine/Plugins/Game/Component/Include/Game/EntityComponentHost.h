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

#include <Async/Async.h>
#include <COM/Result.h>
#include <Core/SlotArray.h>
#include <Core/SparseArray.h>
#include <Platform/RecursiveSpinMutex.h>
#include <Platform/LockGuard.h>
#include <Core/HashMap.h>
#include <Core/Buffer.h>
#include "EntityID.h"
#include "IEntityComponentHost.h"

namespace Djinn::Game {
    template<typename T>
    class EntityComponentHost : public IEntityComponentHost {
    public:
        /// Create a component
        /// \param eid
        /// \param component - Component value
        /// \param out - Resulting id
        /// \return
        COM::Result CreateComponent(const EntityComponentReference &ref) override {
            Platform::LockGuard guard(mutex);

            // Construct
            T com{};

            // Must not have header
            if (indices.Contains(ref.entity.id.id)) {
                return COM::kError;
            }

            // Create header
            Header header;
            header.isTemporary = true;
            indices[ref.entity.id.id] = header;

            // Set reference
            com.reference = ref;

            // Add
            temporaries.Add(ref.entity.id, com);

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

            if (singleComponents.Size() <= static_cast<USize>(header.offset)) {
                return COM::kError;
            }

            Object &obj = singleComponents[header.offset];

            // May already be deleted
            if (obj.deleted) {
                return COM::kOK;
            }

            // Mark as deleted
            obj.deleted = true;

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
                auto it = temporaries.Find(id.id);
                if (it != temporaries.End()) {
                    *out = &(*it).second;
                    return COM::kOK;
                }
                *out = nullptr;
                return COM::kError;
            }

            if (singleComponents.Size() <= static_cast<USize>(header.offset)) {
                return COM::kError;
            }

            *out = &singleComponents[header.offset].com;

            // OK
            return COM::kOK;
        }

        /// Update all components synchronously
        /// \tparam F
        /// \param functor
        /// \return
        template<typename F>
        COM::Result UpdateSync(F &&functor) {
            Platform::LockGuard guard(mutex);

            // Schedule single components
            for (auto &com : singleComponents) {
                functor(com.com);
            }

            // OK
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
            Async::Foreach(group, singleComponents.Size(), [&, functor](USize i) {
                functor(singleComponents[i].com);
            });

            // OK
            return COM::kOK;
        }

        /// Update all components
        /// \tparam F
        /// \param functor
        /// \return
        template<typename U, typename F, typename K>
        COM::Result UpdateAsyncChunked(Async::Group &group, U &&chunkBegin, K &&chunkEnd, F &&functor) {
            Platform::LockGuard guard(mutex);

            // Schedule single components
            Async::Chunked(group, singleComponents.Size(), [&, chunkBegin, chunkEnd, functor](USize begin, USize end) {
                if constexpr (Core::IsSame<decltype(chunkBegin(begin, end)), void>::kValue) {
                    chunkBegin(begin, end);
                    for (USize i = begin; i < end; i++) {
                        functor(singleComponents[i].com);
                    }
                    chunkEnd(begin, end);
                } else {
                    auto r = chunkBegin(begin, end);
                    for (USize i = begin; i < end; i++) {
                        functor(r, singleComponents[i].com);
                    }
                    chunkEnd(r, begin, end);
                }
            });

            // OK
            return COM::kOK;
        }

        /// Update all components with ids
        /// \tparam F
        /// \param functor
        /// \return.
        template<typename F>
        COM::Result UpdateAsyncID(Async::Group &group, F &&functor) {
            Platform::LockGuard guard(mutex);

            // Schedule single components
            Async::Foreach(group, singleComponents.Size(), [&, functor](USize i) {
                functor({singleComponents[i].cid, 0}, singleComponents[i].com);
            });

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
            Platform::LockGuard guard(mutex);

            for (auto id : deletions) {
                deletions.Add(id);

                Header header;
                if (!indices.Get(id.id, header)) {
                    continue;
                }

                // Remove index
                indices.Remove(id.id);

                // Remove component
                if (!singleComponents.Remove(header.offset)) {
                    continue;
                }

                // Update index array
                if (singleComponents.Size() > static_cast<USize>(header.offset)) {
                    indices[singleComponents[header.offset].cid].offset = header.offset;
                }
            }
            deletions.Clear();

            for (const auto &kv : temporaries) {
                Header &header = indices[kv.first.id];
                header.isTemporary = false;

                // Spawn
                Object obj;
                obj.com = kv.second;
                obj.cid = kv.first.id;
                singleComponents.Add(obj);

                // Create header
                header.offset = singleComponents.Size() - 1;
                header.count = 1;
                indices[kv.first.id] = header;
            }
            temporaries.Clear();

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

        struct Header {
            Int64 offset;
            bool isTemporary = false;
        };

        struct Object {
            T com;
            USize cid = 0;
            bool deleted = false;
        };

        /// Array for single components
        Core::SlotArray<Object> singleComponents;

        /// Index array
        Core::SparseArray<Header> indices;

        /// Buffered changes
        Core::Buffer<EntityID> deletions;
        Core::HashMap<EntityID, T> temporaries;
    };
}