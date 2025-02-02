#include <Platform/LibraryAPI.h>
#include <Core/Common.h>
#include <dlfcn.h>

DJINN_NS2(Platform);

LibraryAPI::Handle LibraryAPI::GetCurrent() {
    return dlopen( nullptr, RTLD_LAZY );
}

LibraryAPI::Handle LibraryAPI::Load(const char* name) {
    return dlopen( name, RTLD_LAZY );
}

const char* LibraryAPI::LastError() {
    return dlerror();
}

void LibraryAPI::Free(const Handle& h) {
    dlclose( h );
}

void* LibraryAPI::GetNamedAddress(const Handle& h, const char* name) {
    return reinterpret_cast<void*>(dlsym( h, name ));
}