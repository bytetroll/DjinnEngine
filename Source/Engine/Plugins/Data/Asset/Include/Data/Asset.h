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
 on 4/15/2018.
//
#pragma once

#include <Async/Task.h>

namespace Djinn::Data {
    class IAsset;

    struct AssetObject {
        /// Sync
        Platform::SpinMutex mutex;

        /// Current task
        Async::Task<IAsset *> task;
    };

    template<typename T>
    class Asset {
    public:
        Asset(AssetObject *object = nullptr) : object(object) {

        }

        /// Accessor, yields if incomplete
        /// \return
        T *operator->() {
            return Get();
        }

        /// Get raw pointer, yielded if incomplete
        /// \return
        T *Get() const {
            if (!object) {
                return nullptr;
            }
            auto raw = object->task.GetValue();
            if (raw) {
                return asset = COM::Cast<T>(raw);
            } else {
                return asset = nullptr;
            }
        }

        /// Get raw pointer, does not yield on incomplete assets
        /// \return - null if incomplete
        T *GetRaw() const {
            if (!object) {
                return nullptr;
            }
            if (object->task.HasCompleted()) {
                auto raw = object->task.GetRawValue();
                if (raw) {
                    return asset = COM::Cast<T>(raw);
                } else {
                    return asset = nullptr;
                }
            }
            return asset.Load();
        }

        /// Wait for the asset to complete compiling
        void Wait() {
            if (!object) {
                return;
            }
            object->task.Wait();
        }

        /// Has this asset been loaded? Does not account for recompilations
        /// \return
        bool IsLoaded() const {
            if (!object) {
                return false;
            }
            return (asset.Load() != nullptr) || object->task.HasCompleted();
        }

        /// Is this asset loading, also accounts for recompilations
        /// \return
        bool IsLoading() const {
            if (!object) {
                return false;
            }
            return !object->task.HasCompleted();
        }

        /// Is this asset valid?
        /// Does not care about the state
        /// \return
        bool IsValid() const {
            return object != nullptr;
        }

        /// Implicit
        /// \return
        operator bool() const {
            return IsLoaded();
        }

    private:
        AssetObject *object;

        /// Asset pointer, may not be in sync to task due to recompilation
        mutable Core::Atomic<T *> asset{nullptr};
    };

    template<typename T>
    class AssetView {
    public:
        AssetView(T* asset = nullptr) : asset(asset) {

        }

        AssetView(const Asset<T>& asset) : asset(asset.GetRaw()) {

        }

        /// Accessor
        /// \return
        T *operator->() const {
            return Get();
        }

        /// Get raw pointer
        /// \return
        T *Get() const {
            return asset;
        }

    private:
        T* asset;
    };
}