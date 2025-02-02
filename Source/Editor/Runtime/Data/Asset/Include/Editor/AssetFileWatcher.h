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

#pragma once

#include <Base/IFileWatcher.h>
#include <Core/String.h>
#include <Core/HashMap.h>
#include <Core/Delegate.h>
#include <Core/StringView.h>

namespace Djinn::Base {
    class IFileSystem;
}

namespace Djinn::Editor {
    class IAssetRegistry;
    class IIndexedDirectory;
    class IndexedDirectory;

    class DJINN_EXPORT_EDITOR_DATA_ASSET AssetFileWatcher : public Base::IFileWatcher {
    public:
        DJINN_COM_CLASS();

        AssetFileWatcher(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this watcher
        /// \return
        COM::Result Initialize();

        /// Overrides
        void OnEvent(Base::FileEvent event, const Base::PathID &path) override;

    private:
        IIndexedDirectory* GetDirectory(const Core::StringView& path);

    private:
        IAssetRegistry* assetRegistry;
        Base::IFileSystem* fs;
        Core::HashMap<Core::StringView, IndexedDirectory*> directories;
    };
}