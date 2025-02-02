#pragma once

#include <Core/Size.h>
#include <Core/StringView.h>
#include <Platform/FileAPI.h>
#include <Core/IStream.h>
#include <Core/Memory.h>
#include <Core/Buffer.h>
#include "FileMode.h"

namespace Djinn::Platform {
    /// A stream that operates on files with cached reading and writings
    class FileStream : public Core::IStream {
    public:
        /// FileStreams are not copyable
        DJINN_NOCOPY(FileStream);

        FileStream() {

        }

        /// Construct from file
        /// \param mode
        /// \param path
        /// \param cacheSize
        FileStream(FileMode mode, const Core::StringView &path, UInt64 cacheSize = 1014 * 100) : mode(mode) {
            // Open
            handle = FileAPI::Open(mode, path);
            if (!handle) {
                return;
            }

            // Get size
            size = FileAPI::GetSize(handle);

            // Prepare cache
            DJINN_ASSERT(cacheSize != 0, "File stream cache size cannot be zero");
            cached.Resize(cacheSize);
            Recache(0);
        }

        /// Deconstruct
        virtual ~FileStream() {
            Close();
        }

        /// Boolean operator
        /// \return
        operator bool() const {
            return handle != nullptr && FileAPI::IsGood(handle);
        }

        /// Override
        bool IsEOS() const override {
            return static_cast<USize>(offset + relativePosition) >= (size);
        }

        /// Override
        bool IsGood() override {
            return handle != nullptr && FileAPI::IsGood(handle);
        }

        /// Override
        void Close() override {
            if (handle != nullptr) {
                FlushCache();
                FileAPI::Close(handle);
                handle = nullptr;
            }
        }

        /// Override
        void Flush() override {
            FlushCache();
            FileAPI::Flush(handle);
        }

        /// Override
        Int64 Read(void *buffer, USize size) override {
            // Abnormal read size?
            if (size > cached.Size()) {
                FileAPI::SetCursor(handle, static_cast<USize>(offset + relativePosition));
                Int64 c = FileAPI::Read(handle, buffer, size);
                handlePos = offset + relativePosition + size;
                Recache(offset + relativePosition + size);
                return c;
            } else {
                // Can fetch?
                if (CanFetch(size)) {
                    Core::Memory::Copy(buffer, &cached[relativePosition], size);
                    SetRelativePosition(relativePosition + size);
                } else {
                    // Cache not within limits, recache at current position and read
                    Recache(GetPosition());
                    Core::Memory::Copy(buffer, &cached[relativePosition], size);
                    SetRelativePosition(relativePosition + size);
                }
            }
            return size;
        }

        /// Override
        Int64 Write(const void *buffer, USize size) override {
            // Abnormal write size?
            if (size > cached.Size()) {
                FlushCache();
                FileAPI::SetCursor(handle, static_cast<USize>(offset + relativePosition));
                FileAPI::Write(handle, buffer, size);
                handlePos = offset + relativePosition + size;
                Recache(offset + relativePosition + size);
            } else {
                // Can fetch?
                if (CanFetch(size)) {
                    Core::Memory::Copy(&cached[relativePosition], buffer, size);
                    SetRelativePosition(relativePosition + size);
                } else {
                    // Cache not within limits, recache at current position and write
                    Recache(GetPosition());
                    Core::Memory::Copy(&cached[relativePosition], buffer, size);
                    SetRelativePosition(relativePosition + size);
                }
            }
            if (GetPosition() > this->size) {
                this->size = GetPosition();
            }
            return size;
        }

        /// Override
        USize GetPosition() const override {
            return static_cast<USize>(offset + relativePosition);
        }

        /// Override
        USize GetSize() const override {
            return size;
        }

        /// Override
        Int32 Read() override {
            Int32 value;
            Read(&value, sizeof(Int32));
            return value;
        }

        /// Override
        Int32 Peek() override {
            return cached[relativePosition];
        }

        /// Override
        void SetPosition(USize n) override {
            bool sparseCheck = false;
            switch (mode) {
                case FileMode::eReadWrite:
                case FileMode::eWrite:
                    sparseCheck = true;
                    break;
                default:
                    break;
            }

            if (n < static_cast<USize>(offset) || n >= (offset + cached.Size()) || (sparseCheck && (n > static_cast<USize>(offset + usedCache)))) {
                Recache(n);
            } else {
                SetRelativePosition(n - offset);
            }
        }

        /// Is within buffer limits?
        /// \param size
        /// \return
        bool CanFetch(USize size) {
            return (relativePosition + size) < (cached.Size());
        }

        /// Recache at position
        /// \param pos
        void Recache(USize pos) {
            // Ensure previous results are flushed
            FlushCache();

            // Need to seek?
            if (static_cast<USize>(handlePos) != pos) {
                FileAPI::SetCursor(handle, pos);
                handlePos = pos;
            }

            // Reset positions
            offset = pos;
            relativePosition = 0;
            usedCache = 0;

            // Stream
            switch (mode) {
                case FileMode::eReadWrite:
                case FileMode::eRead:
                    FileAPI::Read(handle, cached.Ptr(), cached.Size());
                    handlePos += cached.Size();
                    break;
                default:
                    break;
            }
        }

        /// Flush cache
        void FlushCache() {
            // May need flush on write modes
            switch (mode) {
                case FileMode::eWrite:
                case FileMode::eReadWrite: {
                    if (usedCache > 0) {
                        FileAPI::Write(handle, cached.Ptr(), usedCache * sizeof(char));
                        handlePos += usedCache * sizeof(char);
                    }
                    break;
                }
                default:
                    break;
            }
        }

        /// Set relative position
        /// \param pos
        void SetRelativePosition(USize pos) {
            relativePosition = pos;
            if (relativePosition >= usedCache) {
                usedCache = relativePosition;
            }
        }

    private:
        FileMode mode;

        // Backend handle
        FileAPI::FileHandle handle = nullptr;

        // Cache
        Int64 offset = -1;
        Int64 relativePosition = -1;
        Int64 usedCache = -1;
        Int64 handlePos = 0;
        Core::Buffer<char> cached;

        // Cached size
        USize size;
    };
}