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
 on 10/22/2017.
//

#include <Platform/FileSystem.h>
#include <sys/stat.h>
#include <regex>
#include <Platform/FileStream.h>
#include <Core/Math.h>

#ifdef DJINN_PLATFORM_WINDOWS

#include <Core/Windows.h>

#else
#include <dirent.h>
#include <glob.h>
#include <unistd.h>

#endif

DJINN_NS2(Platform);

// Cleanup
#undef CreateDirectory

bool FileSystem::Exists(const Core::StringView &path) {
    // TODO: stat without null terminator
    struct stat info = {};
    return stat(path.ToString().AsCharArray(), &info) == 0;
}

bool FileSystem::IsDirectory(const Core::StringView &path) {
    // TODO: stat without null terminator
    struct stat info = {};
    if (stat(path.ToString().AsCharArray(), &info) != 0) {
        return false;
    }
    return (info.st_mode & S_IFDIR) != 0;
}

bool FileSystem::LoadTextFile(const Core::StringView &path, Core::String &out) {
    // Get file handle
    Platform::FileStream file(FileMode::eRead, path);
    if (!file.IsGood()) {
        return false;
    }

    // Read entire file
    if (file.GetSize() != 0) {
        // TODO: Move the data!!!
        char *ptr = new char[file.GetSize() + 1];
        file.Read(ptr, file.GetSize());
        ptr[file.GetSize()] = 0;
        out = ptr;
        delete[] ptr;
    } else {
        out = "";
    }

    // OK
    return true;
}

COM::Result FileSystem::Glob(Core::Array<Core::String> &out, const Core::StringView &pattern) {
    Core::String prefix = GetDirectory(pattern);

    // Prepare regex
#ifdef DJINN_PLATFORM_WINDOWS
    HANDLE dir;
    WIN32_FIND_DATA data = {};

    // Get directory
    // TODO: FindFirstFile without null terminator
    if ((dir = FindFirstFile(pattern.ToString().AsCharArray(), &data)) == INVALID_HANDLE_VALUE) {
        return kNoFilesInDirectory;
    }

    // Iterate files
    do {
        if (std::strcmp(data.cFileName, ".") == 0 || std::strcmp(data.cFileName, "..") == 0) {
            continue;
        }
        out.Add(prefix + data.cFileName);
    } while (FindNextFile(dir, &data));

    // ...
    FindClose(dir);
#else
    // Blob by pattern
    // TODO: Avoid null termiantor allocation
    glob_t globr = {};
    if (glob(pattern.ToString().AsCharArray(), GLOB_TILDE, nullptr, &globr)) {
        return COM::kError;
    }

    for (USize i = 0; i < globr.gl_pathc; i++) {
        out.Add(globr.gl_pathv[i]);
    }

    // Free
    globfree(&globr);
#endif

    return COM::kOK;
}

Int64 FileSystem::GetFileTimeStamp(const Core::StringView &path) {
    // TODO: Avoid null termiantor allocation
    struct stat result;
    if (stat(path.ToString().AsCharArray(), &result) == 0) {
        return result.st_mtime;
    }
    return 0;
}

bool FileSystem::CreateDirectory(const Core::StringView &path) {
    // TODO: Avoid null termiantor allocation
#ifdef DJINN_PLATFORM_WINDOWS
    return static_cast<bool>(::CreateDirectoryA(path.ToString().AsCharArray(), nullptr));
#else
    return mkdir(path.ToString().AsCharArray(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0;
#endif
}

COM::Result FileSystem::GetExecutablePath(Core::String &out) {
#ifdef DJINN_PLATFORM_WINDOWS
    char buffer[512];
    if (GetModuleFileName(nullptr, buffer, sizeof(buffer)) == 0) {
        return COM::kError;
    }
    out = buffer;
    return COM::kOK;
#else
    char buffer[512] = {};
    readlink( "/proc/self/exe", buffer, sizeof(buffer) );
    out = buffer;
    return COM::kOK;
#endif
}

COM::Result FileSystem::GetExecutableDirectory(Core::String &out) {
    if (!GetExecutablePath(out)) {
        return COM::kError;
    }

#ifdef DJINN_PLATFORM_WINDOWS
    out = Core::String(out, 0, out.LastIndexOf('\\'));
#else
    out = Core::String(out, 0, out.LastIndexOf('/'));
#endif
    return COM::kOK;
}

Core::String FileSystem::GetDirectory(const Core::StringView &pattern) {
    if (IsDirectory(pattern)) {
#ifdef DJINN_PLATFORM_WINDOWS
        if (pattern.EndsWith("\\") || pattern.EndsWith("/")) {
            return pattern.ToString();
        } else {
            return pattern.ToString() + "\\";
        }
#else
        if (pattern.EndsWith("\\") || pattern.EndsWith("/")) {
            return pattern.ToString();
        } else {
            return pattern.ToString() + "/";
        }
#endif
    } else {
#ifdef DJINN_PLATFORM_WINDOWS
        return Core::String(pattern.Ptr(), 0, Math::Max(pattern.LastIndexOf('\\'), pattern.LastIndexOf('/')) + 1);
#else
        return pattern.SubStr(0, Math::Max(pattern.LastIndexOf('\\'), pattern.LastIndexOf('/')) + 1).ToString();
#endif
    }
}

Core::String FileSystem::GetFilename(const Core::StringView &pattern) {
    Int32 end = Math::Max(pattern.LastIndexOf('\\'), pattern.LastIndexOf('/'));
    if (end == Core::String::NPOS)
        return pattern.ToString();

    return pattern.SubStr(end + 1, pattern.Length() - end - 1).ToString();
}

Core::String FileSystem::RemoveExtension(const Core::StringView &pattern) {
    Int32 end = pattern.LastIndexOf('.');
    if (end == Core::String::NPOS)
        return pattern.ToString();

    return pattern.SubStr(0, end).ToString();
}

Core::StringView FileSystem::GetDrive(const Core::StringView &pattern) {
#ifdef DJINN_PLATFORM_WINDOWS
    return pattern.SubStr(0, pattern.IndexOf('\\') + 1);
#else
    return pattern.SubStr(0, pattern.IndexOf('/') + 1);
#endif
}

bool FileSystem::WaitDirectoryEvent(const Core::StringView &directory, UInt32 maxCount, UInt32 &count, FileEvent *events) {
#ifdef DJINN_PLATFORM_WINDOWS
    // Open directory
    // TODO: Avoid null termiantor allocation
    HANDLE dir = CreateFile(directory.ToString().AsCharArray(), FILE_LIST_DIRECTORY, FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
    if (dir == INVALID_HANDLE_VALUE) {
        return false;
    }

    // Max 1024 events
    count = Math::Min(1024u, maxCount);

    // Attempt to get
    FILE_NOTIFY_INFORMATION buffer[1024];
    DWORD bytes = 0;
    if (!ReadDirectoryChangesW(dir, &buffer, sizeof(buffer[0]) * count, TRUE, FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_ACTION_MODIFIED | FILE_ACTION_ADDED | FILE_ACTION_REMOVED, &bytes, NULL, NULL)) {
        return false;
    }

    // Copy events
    UInt32 index = 0;
    for (FILE_NOTIFY_INFORMATION *entry = buffer;; entry = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(reinterpret_cast<char *>(entry) + entry->NextEntryOffset), index++) {
        // Transform path
        char path[512] = {};
        std::transform(entry->FileName, entry->FileName + (entry->FileNameLength / sizeof(WCHAR)), path, [](auto w) {
            if (w == '\\') {
                return '/';
            }
            return static_cast<char>(w);
        });

        // Copy event
        events[index].path = Core::String(path);
        switch (entry->Action) {
            case FILE_NOTIFY_CHANGE_LAST_WRITE:
                events[index].type = FileEventType::eWrite;
                break;
            case FILE_ACTION_MODIFIED:
                events[index].type = FileEventType::eModified;
                break;
            case FILE_ACTION_ADDED:
                events[index].type = FileEventType::eCreated;
                break;
            case FILE_ACTION_REMOVED:
                events[index].type = FileEventType::eDeleted;
                break;
            default:
                // Unknown event
                index--;
                continue;
        }

        // End?
        if (entry->NextEntryOffset == 0) {
            count = index + 1;
            break;
        }
    }
#else
#warning( "[Linux] FileSystem::WaitDirectoryEvents needs an implementation." )
#endif

    return false;
}

bool FileSystem::IsSame(const Core::StringView& a, const Core::StringView& b) {
    auto ah = FileAPI::Open(FileMode::eRead, a);
    auto bh = FileAPI::Open(FileMode::eRead, b);
    if (!FileAPI::IsGood(ah) || !FileAPI::IsGood(bh)) {
        return false;
    }

    return FileAPI::GetUID(ah) == FileAPI::GetUID(bh);
}

Core::String FileSystem::Sanitize(const Core::StringView &_path) {
    Core::String path = _path.ToString();
#ifdef DJINN_PLATFORM_WINDOWS
    UnixToWindows(path);
#else
    WindowsToUnix(path);
#endif
    return path;
}

Core::String FileSystem::SanitizeFilename(const Core::StringView &_path, char replacement) {
    Core::String path = _path.ToString();
    path.Replace('\\', replacement);
    path.Replace('/', replacement);
    return path;
}

Core::String FileSystem::GetWorkingDirectory() {
#ifdef DJINN_PLATFORM_WINDOWS
    char path[MAX_PATH]{0};
    GetCurrentDirectory(MAX_PATH, path);
    return path;
#else
    char path[PATH_MAX]{0};
    getcwd(path, sizeof(path));
    return path;
#endif
}
