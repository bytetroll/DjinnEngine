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
 on 8/24/2018.
//
#pragma once

#include <COM/ID.h>
#include <Core/StackArray.h>
#include <Core/Pair.h>

namespace Djinn::Base {
    class ISystem;

    enum class PipelineBuilderMode {
        eAfter,
        eBefore
    };

    class PipelineBuilder {
    public:
        /// Read to system
        /// \param system
        void Reads(ISystem *other) {

        }

        /// Writes to system
        /// \param system
        void Writes(ISystem *other) {

        }

        /// Must run before system
        /// \param system
        void Before(ISystem* system) {
            systemDependencies.Add({PipelineBuilderMode::eBefore, system});
        }

        /// Must run after system
        /// \param system
        void After(ISystem* system) {
            systemDependencies.Add({PipelineBuilderMode::eAfter, system});
        }

        /// Must run before class
        /// \param system
        void Before(COM::ClassID cid) {
            classDependencies.Add({PipelineBuilderMode::eBefore, cid});
        }

        /// Must run before interface
        /// \param system
        void Before(COM::InterfaceID iid) {
            interfaceDependencies.Add({PipelineBuilderMode::eBefore, iid});
        }

        /// Must run after class
        /// \param system
        void After(COM::ClassID cid) {
            classDependencies.Add({PipelineBuilderMode::eAfter, cid});
        }

        /// Must run after interface
        /// \param system
        void After(COM::InterfaceID iid) {
            interfaceDependencies.Add({PipelineBuilderMode::eAfter, iid});
        }

        /// Helper
        template<typename T>
        void Reads() {
        }

        /// Helper
        template<typename T>
        void Writes() {

        }

        /// Helper
        template<typename T>
        void Before() {
            if constexpr(COM::HasKCID<T>::kValue) {
                Before(T::kCID);
            } else {
                Before(T::kIID);
            }
        }

        /// Helper
        template<typename T>
        void After() {
            if constexpr(COM::HasKCID<T>::kValue) {
                Before(T::kCID);
            } else {
                Before(T::kIID);
            }
        }

        Core::StackArray4<Core::Pair<PipelineBuilderMode, COM::ClassID>> classDependencies;
        Core::StackArray4<Core::Pair<PipelineBuilderMode, COM::InterfaceID>> interfaceDependencies;
        Core::StackArray4<Core::Pair<PipelineBuilderMode, ISystem *>> systemDependencies;
    };
}