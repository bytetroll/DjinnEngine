//
// Created by Dev on 8/24/2018.
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