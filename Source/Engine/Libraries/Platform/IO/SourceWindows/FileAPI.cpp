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
 on 6/5/2017.
//

#include <Platform/FileAPI.h>
#include <Core/Common.h>
#include <Core/Windows.h>

DJINN_NS3(Platform, FileAPI);

FileHandle FileAPI::Open(FileMode mode, const Core::StringView &path) {
    // TODO: fopen without null terminator
    static const char *cModes[static_cast<int>(FileMode::eCount)] = {
            "rb",
            "wb",
            "rb+"
    };
    return fopen(path.ToString().AsCharArray(), cModes[static_cast<int>(mode)]);
}

bool FileAPI::IsEOF(const FileHandle &handle) {
    char ch;
    bool eof = !fread(&ch, 1, 1, handle);
    if (!eof) {
        FileAPI::SetCursor(handle, FileAPI::GetCursor(handle) - 1);
    }
    return eof;
}

bool FileAPI::IsGood(const FileHandle &handle) {
    return handle != nullptr;
}

void FileAPI::Close(const FileHandle &handle) {
    fclose(handle);
}

void FileAPI::Flush(const FileHandle &handle) {
    fflush(handle);
}

Int64 FileAPI::Read(const FileHandle &handle, void *buffer, USize size) {
    return fread(buffer, 1, size, handle);
}

Int64 FileAPI::Write(const FileHandle &handle, const void *buffer, USize size) {
    return fwrite(buffer, 1, size, handle);
}

USize FileAPI::GetCursor(const FileHandle &handle) {
    Int64 pos;
#ifndef DJINN_COMPILER_GNU
    fgetpos(handle, &pos);
#else
    fgetpos64(handle, &pos);
#endif
    return static_cast<USize>(pos);
}

USize FileAPI::GetSize(const FileHandle &handle) {
#ifndef DJINN_COMPILER_GNU
    Int64 previous = ftell(handle);
    fseek(handle, 0, SEEK_END);
    Int64 size = ftell(handle);
    fseek(handle, previous, SEEK_SET);
#else
    Int64 previous = ftello64(handle);
    fseeko64(handle, 0, SEEK_END);
    Int64 size = ftello64(handle);
    fseeko64(handle, previous, SEEK_SET);
#endif
    return static_cast<USize>(size);
}

Int64 FileAPI::GetUID(const FileHandle &handle) {
    BY_HANDLE_FILE_INFORMATION info;
    if (GetFileInformationByHandle(handle, &info) == 0) {
        return -1;
    }
    return (static_cast<Int64>(info.nFileIndexHigh) << 32) + static_cast<Int64>(info.nFileIndexLow);
}

void FileAPI::SetCursor(const FileHandle &handle, USize cursor) {
#ifndef DJINN_COMPILER_GNU
    fseek(handle, cursor, SEEK_SET);
#else
    fseeko64(handle, cursor, SEEK_SET);
#endif
}
