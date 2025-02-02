//
// Created by Dev on 8/22/2018.
//
#pragma once

#include <COM/ID.h>
#include <Core/ArrayView.h>
#include <Core/BitSet.h>

namespace Djinn::UI {
    class UIDocking;
    class UIDockingManager;
    class IUIElement;
    class IUILayer;

    enum class EDockingSlot {
        eNone = 0,
        eTop = 1<<0,
        eLeft = 1<<1,
        eBottom = 1<<2,
        eRight = 1<<3,
        eMerge = 1<<4
    };

    DJINN_CORE_BITSET(EDockingSlot);

    class IDockable {
    public:
        DJINN_COM_INTERFACEID();

        /// Get the render layer
        /// \return
        virtual IUILayer* GetLayer() = 0;

        /// Get the element
        /// \return
        virtual IUIElement* GetDockingElement() = 0;

        /// Get all child dockables
        /// \return
        virtual Core::ArrayView<IDockable*> GetDockables() = 0;

        /// Get the capabilities of this dockable
        /// \return
        virtual EDockingSlotSet GetDockingSlots() =  0;

        /// Add a dockable
        /// \param slot
        /// \param dockable
        virtual void AddDockable(EDockingSlot slot, IDockable* dockable) = 0;

    public:
        /// Get the current docking
        /// \return
        UIDocking* GetDocking() const {
            return docking;
        }

        /// Get the current docking
        /// \return
        UIDockingManager* GetDockingManager() const {
            return manager;
        }

        /// Set the current docking reference
        /// \param docking
        void SetDockingReference(UIDocking* docking) {
            this->docking = docking;
        }

        /// Set the current docking manager reference
        /// \param docking
        void SetDockingManagerReference(UIDockingManager* manager) {
            this->manager = manager;
        }

    private:
        UIDocking* docking = nullptr;
        UIDockingManager* manager = nullptr;
    };
}