//
// Created by Dev on 6/5/2017.
//

#include <Platform/FileAPI.h>
#include <Core/Common.h>
// #include <_G_config.h>
#include <sys/stat.h>

DJINN_NS3(Platform, FileAPI);

FileHandle FileAPI::Open(FileMode mode, const Core::StringView &path) {
    // TODO: fopen without null terminator
    static const char* cModes[static_cast<int>(FileMode::eCount)] = {
            "rb",
            "wb",
            "rb+"
    };
    return fopen(path.ToString().AsCharArray(), cModes[static_cast<int>(mode)]);
}

bool FileAPI::IsGood(const FileHandle &handle) {
    return handle != nullptr;
}

bool FileAPI::IsEOF(const FileHandle& handle) {
    char ch;
    bool eof = !fread(&ch, 1, 1, handle);
    if (!eof) {
        FileAPI::SetCursor(handle, FileAPI::GetCursor(handle) - 1);
    }
    return eof;
}

void FileAPI::Close(const FileHandle& handle) {
    fclose(handle);
}

void FileAPI::Flush(const FileHandle& handle) {
    fflush(handle);
}

Int64 FileAPI::Read(const FileHandle& handle, void *buffer, USize size) {
    return fread(buffer, 1, size, handle);
}

Int64 FileAPI::Write(const FileHandle& handle, const void *buffer, USize size) {
    return fwrite(buffer, 1, size, handle);
}

USize FileAPI::GetCursor(const FileHandle &handle) {
    _G_fpos64_t pos;
    fgetpos64(handle, &pos);
    return static_cast<USize>(pos.__pos);
}

USize FileAPI::GetSize(const FileHandle &handle) {
    struct stat buf;
    fstat(fileno(handle), &buf);
    return buf.st_size;
    /*Int64 previous = ftello64(handle);
    fseeko64(handle, 0, SEEK_END);
    Int64 size = ftello64(handle);
    fseeko64(handle, previous, SEEK_SET);
    return static_cast<USize>(size);*/
}

Int64 FileAPI::GetUID(const FileHandle &handle) {
    struct stat buf;
    fstat(fileno(handle), &buf);
    return static_cast<Int64>(buf.st_ino);
}

void FileAPI::SetCursor(const FileHandle& handle, USize cursor) {
    fseeko64(handle, cursor, SEEK_SET);
}
