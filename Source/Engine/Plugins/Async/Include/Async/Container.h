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
 on 4/29/2018.
//
#pragma once

#include <Core/IndexArray.h>
#include <Core/Optional.h>
#include <Platform/RecursiveSpinMutex.h>
#include <Core/Buffer.h>
#include <Core/Pair.h>
#include "Event.h"
#include "Async.h"

namespace Djinn::Async {
    template<typename T>
    class Container {
    public:
        /// Add an object
        /// \param value
        void Add(const T &value) {
            Platform::LockGuard guard(mutex);
            pendingObjects.Add(value);
        }

        /// Remove an object
        /// \param value
        void Remove(const T &value) {
            Platform::LockGuard guard(mutex);
            pendingObjectRemovals.Add(value);
        }

        /// Clear all dependencies
        void ClearDependencies() {
            for (auto &&obj : objects) {
                obj.dependencies.Clear();
                obj.dependents = 0;
            }
        }

        /// Add a dependency
        /// \param value
        /// \param dependency
        /// \return
        void AddDependency(const T &value, const T &dependency) {
            Platform::LockGuard guard(mutex);
            pendingDependencies.Add({value, dependency});
        }

        /// Remove a dependency
        /// \param value
        /// \param dependency
        /// \return
        void RemoveDependency(const T &value, const T &dependency) {
            Platform::LockGuard guard(mutex);
            pendingDependencyRemoval.Add({value, dependency});
        }

        /// Run against all
        /// \tparam F
        /// \param action
        template<typename F>
        void All(F &&action) {
            for (auto &obj : objects) {
                action(obj.value);
            }
        }

        /// Apply all staged changes
        void ApplyChanges() {
            Platform::LockGuard guard(mutex);

            // Remove pending objects
            for (auto& value : pendingObjectRemovals) {
                for (USize i = 0; i < objects.ElementSize(); i++) {
                    if (objects[i] && objects[i].value == value) {
                        objects.Remove(i);
                        break;
                    }
                }
            }
            pendingObjectRemovals.Clear();

            // Push pending objects
            for (auto& value : pendingObjects) {
                Object obj;
                obj.value = value;
                obj.set = true;
                USize id = objects.Add(obj);
                objects[id].id = id;
            }
            pendingObjects.Clear();

            // Remove pending dependencies
            for (auto& dependency : pendingDependencyRemoval) {
                Core::Optional<USize> id;
                for (auto &&obj : objects) {
                    if (obj.value == dependency.second) {
                        id = obj.id;
                        obj.dependents--;
                        break;
                    }
                }

                if (!id) {
                    continue;
                }

                for (auto &&obj : objects) {
                    if (obj.value == dependency.first) {
                        obj.dependencies.RemoveValue(id.Get());
                        break;
                    }
                }
            }
            pendingDependencyRemoval.Clear();

            // Push pending dependencies
            for (auto& dependency : pendingDependencies) {
                Core::Optional<USize> id;
                for (auto &&obj : objects) {
                    if (obj.value == dependency.second) {
                        id = obj.id;
                        obj.dependents++;
                        break;
                    }
                }

                if (!id) {
                    continue;
                }

                for (auto &&obj : objects) {
                    if (obj.value == dependency.first) {
                        obj.dependencies.Add(id.Get());
                        break;
                    }
                }
            }
            pendingDependencies.Clear();
        }

        /// Run all objects
        /// \tparam F
        /// \param group
        /// \param action
        template<typename F>
        void Run(Async::Group &group, F &&action) {
            // Reset all events
            for (auto &obj : objects) {
                obj.event.Reset();
            }

            // Run all objects
            Async::Foreach(group, objects.ElementSize(), [&, action](USize i) {
                auto &obj = objects[i];
                if (!obj) {
                    return;
                }

                // Wait for dependencies
                for (auto dep : obj.dependencies) {
                    objects[dep].event.WaitRepeat();
                }

                // Run
                action(obj.value);

                // Signal event
                // TODO: Race condition somewhere...
                //if (obj.dependents > 0) {
                    obj.event.Signal();
                //}
            });
        }

    private:
        struct Object {
            Object(std::nullptr_t = nullptr) {

            }

            USize id;
            T value;
            Core::Array<USize> dependencies;
            Event event;
            USize dependents = 0;
            bool set = false;

            operator bool() const {
                return set;
            }
        };

        Platform::SpinMutex mutex;
        Core::Buffer<T> pendingObjects;
        Core::Buffer<T> pendingObjectRemovals;
        Core::Buffer<Core::Pair<T, T>> pendingDependencies;
        Core::Buffer<Core::Pair<T, T>> pendingDependencyRemoval;
        Core::IndexArray<Object> objects;
    };
}