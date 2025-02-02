#pragma once

#include <Core/StringView.h>
#include <Platform/FileMode.h>
#include <Core/Types.h>
#include <Core/Size.h>

namespace Djinn {
    namespace Platform {
        namespace FileAPI {
            using FileHandle = FILE*;

            // Open file
            extern FileHandle Open(FileMode mode, const Core::StringView& path);

            // Is end of file
            extern bool IsEOF(const FileHandle& handle);

            // Is good
            extern bool IsGood(const FileHandle& handle);

            // Flush file
            extern void Flush(const FileHandle& handle);

            // Read from file
            extern Int64 Read(const FileHandle& handle, void *buffer, USize size);

            // Write to file
            extern Int64 Write(const FileHandle& handle, const void *buffer, USize size);

            // Get file cursor
            extern USize GetCursor(const FileHandle& handle);

            // Get file size
            extern USize GetSize(const FileHandle& handle);

            // Set file cursor
            extern void SetCursor(const FileHandle& handle, USize cursor);

            // Get file unique id
            extern Int64 GetUID(const FileHandle& handle);

            // Close file
            extern void Close(const FileHandle& handle);
        }
    }
}