//
// Created by Dev on 10/22/2017.
//
#pragma once

#include <Core/Common.h>
#include <Core/String.h>
#include <Core/StringView.h>
#include <Core/Array.h>
#include <COM/Result.h>
#include "FileEvent.h"
#include <Core/Delegate.h>

// Cleanup
#undef CreateDirectory

namespace Djinn::Platform {
    // Common results
    DJINN_COM_RESULT(NoFilesInDirectory, true, "Platform");

    class FileSystem {
    public:
        DJINN_STATICCLASS(FileSystem);

        /// Check if a path exists
        /// \param path
        /// \return
        static bool Exists(const Core::StringView &path);

        /// Check if a path is a directory
        /// \param path
        /// \return
        static bool IsDirectory(const Core::StringView &path);

        /// Create a directory
        /// \param path
        static bool CreateDirectory(const Core::StringView &path);

        /// Get last modified time stamp of file
        /// \param path
        /// \return
        static Int64 GetFileTimeStamp(const Core::StringView &path);

        /// Load text file
        /// \param path
        /// \param out
        /// \return
        static bool LoadTextFile(const Core::StringView &path, Core::String &out);

        /// Get files in a directory
        /// \param directory
        /// \param out
        /// \return
        static COM::Result Glob(Core::Array<Core::String> &out, const Core::StringView &pattern);

        /// Get the current working directory
        /// \param pattern
        /// \return
        static Core::String GetWorkingDirectory();

        /// Get directory of path
        /// \param pattern
        /// \return
        static Core::String GetDirectory(const Core::StringView &path);

        /// Get the filename of a path without directory, includes extension
        /// \param pattern
        /// \return
        static Core::String GetFilename(const Core::StringView &path);

        /// Removes the extension from a filename
        /// \param pattern
        /// \return
        static Core::String RemoveExtension(const Core::StringView &path);

        /// Get drive of path
        /// \param path
        /// \return
        static Core::StringView GetDrive(const Core::StringView &path);

        /// Get path of the current executable
        /// \return
        static COM::Result GetExecutablePath(Core::String &out);

        /// Get directory of the current executable
        /// \return
        static COM::Result GetExecutableDirectory(Core::String &out);

        /// Wait for a file event
        /// \param count
        /// \param events
        /// \return
        static bool WaitDirectoryEvent(const Core::StringView &directory, UInt32 maxCount, UInt32 &count, FileEvent *events);

        /// Check if two paths are referencing the same file
        /// \param path
        /// \return
        static bool IsSame(const Core::StringView& a, const Core::StringView& b);

        /// Combine paths
        /// \param t0
        /// \param t1
        /// \return
        template<typename T0, typename T1>
        static Core::String CombinePaths(const T0 &_t0, const T1 &_t1) {
            Core::String t0 = _t0;
            Core::String t1 = _t1;
#ifdef DJINN_PLATFORM_WINDOWS
            if (t0.EndsWith("\\") || t1.StartsWith("\\")) {
                return t0 + t1;
            }
            return t0 + Core::String("\\") + t1;
#else
            if (t0.EndsWith("/") || t1.StartsWith("/")) {
                return t0 + t1;
            }
            return t0 + Core::String("/") + t1;
#endif
        }

        /// Combine paths
        /// \param t0
        /// \param t1
        /// \param t2
        /// \param tx
        /// \return
        template<typename T0, typename T1, typename T2, typename... TX>
        static Core::String CombinePaths(const T0 &t0, const T1 &t1, const T2 &t2, const TX &... tx) {
            return CombinePaths(CombinePaths(t0, t1), t2, tx...);
        }

        /// Sanitize a path
        /// \param path
        /// \return
        static Core::String Sanitize(const Core::StringView& path);

        /// Sanitize a filename
        /// \param path
        /// \param replacement - Replacement character for reserved filename characters
        /// \return
        static Core::String SanitizeFilename(const Core::StringView& path, char replacement = '_');

        static void UnixToWindows( Core::String& Path ) {
            Path.Replace( "/", "\\" );
        }

        static void WindowsToUnix( Core::String& Path ) {
            Path.Replace( "\\", "/" );
        }
    };
}