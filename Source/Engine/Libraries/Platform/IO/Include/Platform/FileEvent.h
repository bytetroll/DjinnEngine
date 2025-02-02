//
// Created by Dev on 4/19/2018.
//

#include <Core/String.h>

namespace Djinn::Platform {
    enum class FileEventType {
        eCreated,
        eModified,
        eDeleted,
        eWrite
    };

    struct FileEvent {
        Core::String path;
        FileEventType type;
    };
}