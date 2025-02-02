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
 on 1/13/2018.
//

#include <Base/ConsoleLogStream.h>
#include <Core/Common.h>
#include <iostream>
#include <time.h>
#include <Platform/Mutex.h>
#include <Platform/LockGuard.h>

DJINN_NS2(Base);

namespace {
    Platform::Mutex consoleMtx;
}

ConsoleLogStream::ConsoleLogStream(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ILogStream(registry, outer, host) {
}

void ConsoleLogStream::WriteMessage(const Core::String& header, const Core::String &message) {
    Platform::LockGuard guard(consoleMtx);

    // Header
    std::cout << header.AsCharArray() << " ";

    // Prefix
    time_t t = time(0);
    struct tm *now = localtime(&t);
    std::cout << "[" << (now->tm_year + 1900) << '-' << (now->tm_mon + 1) << '-' << now->tm_mday << ", " << now->tm_hour << "h" << now->tm_min << "m" << now->tm_sec << "s] ";

    // Message
    std::cout << message.AsCharArray() << "\n";
}

bool ConsoleLogStream::IsEOS() const {
    return false;
}

bool ConsoleLogStream::IsGood() {
    return true;
}

void ConsoleLogStream::Close() {
}

void ConsoleLogStream::Flush() {
    std::cout.flush();
}

Int32 ConsoleLogStream::Peek() {
    return 0;
}

Int32 ConsoleLogStream::Read() {
    return 0;
}

Int64 ConsoleLogStream::Read(void *, USize) {
    return 0;
}

Int64 ConsoleLogStream::Write(const void *buffer, USize size) {
    std::cout.write(static_cast<const char*>(buffer), size);
    return size;
}

USize ConsoleLogStream::GetPosition() const {
    return 0;
}

USize ConsoleLogStream::GetSize() const {
    return 0;
}

void ConsoleLogStream::SetPosition(USize) {

}

