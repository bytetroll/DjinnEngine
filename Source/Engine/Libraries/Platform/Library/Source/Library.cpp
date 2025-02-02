#include <Platform/Library.h>
#include <Core/Common.h>

DJINN_NS2(Platform);

Library::Library() {

}

bool Library::Load(const char* name, const char** outError) {
    this->name = name;
    if ((handle = LibraryAPI::Load(name)) == LibraryAPI::kInvalid) {
        if (outError) {
            *outError = LibraryAPI::LastError();
        }
        return false;
    }
    return true;
}

void Library::Free() {
    LibraryAPI::Free(handle);
}
