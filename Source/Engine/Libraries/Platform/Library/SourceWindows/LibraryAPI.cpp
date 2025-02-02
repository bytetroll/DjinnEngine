#include <Platform/LibraryAPI.h>
#include <Core/Windows.h>

DJINN_NS2(Platform);

LibraryAPI::Handle LibraryAPI::GetCurrent() {
    return GetModuleHandle(nullptr);
}

LibraryAPI::Handle LibraryAPI::Load(const char* name) {
    return LoadLibrary(name);
}

const char* LibraryAPI::LastError() {
    return "";
}

void LibraryAPI::Free(const Handle& h) {
    FreeLibrary(h);
}

void* LibraryAPI::GetNamedAddress(const Handle& h, const char* name) {
    // Note: HMODULE cast is not redundant
    //       Remove it and see the face of satan
    //                           (Semi nice guy)
    return reinterpret_cast<void*>(GetProcAddress(static_cast<HMODULE>(h), name));
}
