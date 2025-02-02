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
 on 12/11/2017.
//
#pragma once

#include <COM/Result.h>
#include <Core/Delegate.h>
#include <Platform/Thread.h>
#include <Platform/Event.h>
#include <Core/Array.h>
#include <Math/Vector2I.h>
#include "WindowDesc.h"
#include "WindowHandle.h"
#include "WindowEvent.h"

namespace Djinn::Platform {
    class Window {
    public:
        Window();

        /// Initialize window
        /// \param desc
        /// \return
        COM::Result Initialize(const WindowDesc &desc);

        /// Calculate aspect ratio
        /// \return
        float GetAspectRatio() const;

        /// Set the window visibility
        /// \param isVisible
        void Show(bool isVisible = true);

        /// Set this window as the focus window
        void SetFocus();

        /// Set this window as the foreground window
        void SetForegroundWindow();

        /// Minimize this window
        void Minimize();

        /// Maximize this window
        void Maximize();

        /// Close this window
        void Close();

        /// Wait for this window to exit
        void Wait();

        /// Set the relative mouse position
        /// \param position
        void SetLocalMousePosition(const Math::Vector2I &position);

        /// Get the window size
        /// \return
        Math::Vector2I GetSize();

        /// Get the window position
        /// \return
        Math::Vector2I GetPosition();

        /// Get events
        /// \param count
        /// \param out
        /// \return
        COM::Result PoolEvents(UInt32 &count, WindowEvent *out);

        /// Get desc
        /// \return
        const WindowDesc &GetDesc() const {
            return desc;
        }

        /// Get handle container
        /// \return
        const WindowHandle &GetHandle() const {
            return handle;
        }

#ifdef DJINN_PLATFORM_WINDOWS
        LRESULT OnMessage(HWND hWnd, UINT umessage, WPARAM wparam, LPARAM lparam);
#endif

    protected:
        /// Initialize window
        /// \return
        void InitializeWindow(COM::Result *result);

    private:
        WindowDesc desc;

        // Last mouse position
        Math::Vector2I mousePosition;

        // Cached events
        Mutex cachedEventMtx;
        Core::Array<WindowEvent> cachedEvents;

        // Handle
        WindowHandle handle;

        // Thread
        Thread thread;

        // Initialized event
        Event event;

        // Finish event
        Event finish;
    };
}