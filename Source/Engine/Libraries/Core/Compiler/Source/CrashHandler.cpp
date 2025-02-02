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
 on 4/8/2018.
//

#include <Core/Common.h>
#include <Core/CrashHandler.h>
#include <stdio.h>
#include <csignal>

#if DJINN_PLATFORM_WINDOWS

#   include <Windows.h>
#   include <dbghelp.h>
#   include <iostream>

#elif DJINN_PLATFORM_LINUX
#   include <stdlib.h>
#   include <sys/wait.h>
#   include <unistd.h>
#   include <cstdio>
#   include <sys/ucontext.h>
#   include <execinfo.h>

#   ifndef __USE_GNU
#       define __USE_GNU
#   endif

#   include <ucontext.h>
#endif

#if DJINN_PLATFORM_LINUX
// Errors.
#define DJINN_LINUX_IMAGE_BAD_READ -1

// Constants.
#define DJINN_LINUX_IMAGE_BUFFER_SIZE 1024

namespace Djinn {
    union LinuxImageDescriptor {
    public:
        LinuxImageDescriptor() {
            name = new char[ DJINN_LINUX_IMAGE_BUFFER_SIZE ];
        }

        ~LinuxImageDescriptor() {
            delete[] name;
            name = nullptr;
        }

    public:
        char* name;
    };

    unsigned int BindLinuxImage( LinuxImageDescriptor* const image ) {
        char linkNameBuffer[ 1024 ];
        std::snprintf( linkNameBuffer, sizeof( linkNameBuffer ), "/proc/%d/exe", getpid() );
        if( readlink( linkNameBuffer, image->name, sizeof( linkNameBuffer ) ) == DJINN_LINUX_IMAGE_BAD_READ ) {
            std::fprintf( stderr, "[Error] Failed to get stack trace of Linux image -- bad read.\n" );
            exit( -1 );
        }

        std::printf( "[Info] Linux image found: %s\n", image->name );
        return 0;
    }

    // @TODO: Fix memory leak here.
    const char* GetLinuxImageName() {
        LinuxImageDescriptor* const image = new LinuxImageDescriptor;
        BindLinuxImage( image );
        return image->name;
    }

    void LinuxSigHandler( const int sigCode, siginfo_t* const sigInfo, void* const secret ) {
        void* stackTrace[ 16 ];
        char** stackTraceMessages = static_cast< char** >( nullptr );
        int traceSize = 0;
        ucontext_t* const stackContext = static_cast< ucontext_t* >( secret );

        if( sigCode == SIGSEGV ) {
            // @TODO: Need to figure out the appropriate sigInfo struct field for the block addresses.
            //        Also need to figure out the correct register masks.
            std::printf( "[Stack Trace] Signal: SIGSEV, Signal Code: %d, Faulty Address (block): from {%p} to {%p}\n",
                sigCode, sigInfo->si_code, stackContext->uc_mcontext.gregs[ REG_ERR ] );
        } else {
            std::printf( "[Stack Trace] Signal: %d\n", sigCode );
        }

        traceSize = backtrace( stackTrace, sizeof( stackTrace ) ); // Should be 16.

        // Overwrite sigaction with caller's address.
        stackTrace[ 1 ] = reinterpret_cast< void* >( stackContext->uc_mcontext.gregs[ REG_ERR ] );
        stackTraceMessages = backtrace_symbols( stackTrace, traceSize );

        // SKip first stack frame, which will always point HERE.
        std::printf( "[Stack Trace] Execution Path:\n" );
        for( auto index = 0; index < traceSize; ++index ) {
            std::printf( "\t\\> %s\n", stackTraceMessages[ index ] );

            // File formatting.  Find first occurrence of '(' or " " in the string
            // and assume that everything before those tokens is the file name.
            // Make sure not to go past the string terminator.
            std::size_t tokenIndex = 0;
            while( ( stackTraceMessages[ index ][ tokenIndex ] != '(' ) &&
                   ( stackTraceMessages[ index ][ tokenIndex] != ' ' ) &&
                   ( stackTraceMessages[ index ][ tokenIndex ] != 0 ) ) {
                ++tokenIndex;
            }

            // Last parameter is the filename of the symbol.
            char sysComBuffer[ 256 ];
            std::sprintf( sysComBuffer, "addr2line %p -e %.*s", stackTrace[ index ], tokenIndex,
                stackTraceMessages[ index ] );
            system( sysComBuffer );
        }
    }

    // Installer.
    //
    // @NOTE: According to Miguel, crash handlers are installed in one of the plugins, but he ain't sure where.
    //        Since I don't have time for that shit, trick the compiler into installing the crash handler
    //        during the static initialization phase. -- NCY, 7.22.2018.
    enum class eCrashHandlerInstallerCode : unsigned int {
        OK
    };

    eCrashHandlerInstallerCode LinuxCrashHandlerInstallerCallback();
    using LinuxCrashHandlerInstaller = decltype( ( LinuxCrashHandlerInstallerCallback() ) );

#define DJINN_INSTALL_LINUX_CRASH_HANDLER( CALLBACK ) Djinn::LinuxCrashHandlerInstaller LinuxCrashInstaller = CALLBACK();
}

Djinn::LinuxCrashHandlerInstaller InstallLinuxCrashHandler() {
    struct sigaction sigAction;
    sigAction.sa_sigaction = Djinn::LinuxSigHandler; //reinterpret_cast< void* >( &Djinn::LinuxSigHandler );
    sigemptyset( &sigAction.sa_mask );
    sigAction.sa_flags = SA_RESTART | SA_SIGINFO;

    // Add more signals here.
    sigaction( SIGSEGV, &sigAction, nullptr );
    sigaction( SIGUSR1, &sigAction, nullptr );

    std::printf( "[Stack Trace] Linux crash handler installed for %s...\n", Djinn::GetLinuxImageName() );
    return Djinn::eCrashHandlerInstallerCode::OK;
}
DJINN_INSTALL_LINUX_CRASH_HANDLER( InstallLinuxCrashHandler );
#endif

#ifdef DJINN_PLATFORM_WINDOWS
static WINAPI LONG WindowsHandler(PEXCEPTION_POINTERS ExceptionInfo) {
    printf("\nA fatal exception has occurred, stack trace:\n");

    BOOL result;
    HANDLE process;
    HANDLE thread;
    HMODULE hModule;

    STACKFRAME64 stack;
    ULONG frame;
    DWORD64 displacement;

    DWORD disp;
    IMAGEHLP_LINE64 *line;

    char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
    char name[256];
    char module[256];
    PSYMBOL_INFO pSymbol = reinterpret_cast<PSYMBOL_INFO>(buffer);

    memset(&stack, 0, sizeof(STACKFRAME64));

    process = GetCurrentProcess();
    thread = GetCurrentThread();
    displacement = 0;
#if !defined(DJINN_64)
#   error 32 bit not supported

    stack.AddrPC.Offset    = (*ctx).Eip;
    stack.AddrPC.Mode      = AddrModeFlat;
    stack.AddrStack.Offset = (*ctx).Esp;
    stack.AddrStack.Mode   = AddrModeFlat;
    stack.AddrFrame.Offset = (*ctx).Ebp;
    stack.AddrFrame.Mode   = AddrModeFlat;
#endif

    SymInitialize(process, NULL, TRUE); //load symbols

    HMODULE lastModule = nullptr;

    for (frame = 0;; frame++) {
        //get next call from stack
        result = StackWalk64(
#if defined(_M_AMD64)
                IMAGE_FILE_MACHINE_AMD64,
#else
                IMAGE_FILE_MACHINE_I386,
#endif
                process,
                thread,
                &stack,
                ExceptionInfo->ContextRecord,
                NULL,
                SymFunctionTableAccess64,
                SymGetModuleBase64,
                NULL
        );

        if (!result) break;

        //get symbol name for address
        pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
        pSymbol->MaxNameLen = MAX_SYM_NAME;
        SymFromAddr(process, static_cast<ULONG64>(stack.AddrPC.Offset), &displacement, pSymbol);

        line = static_cast<IMAGEHLP_LINE64 *>(malloc(sizeof(IMAGEHLP_LINE64)));
        line->SizeOfStruct = sizeof(IMAGEHLP_LINE64);

        if (lastModule != hModule) {
            lastModule = hModule;

            hModule = NULL;
            lstrcpyA(module, "");
            GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, reinterpret_cast<LPCTSTR>(stack.AddrPC.Offset), &hModule);
            if (hModule != NULL) GetModuleFileNameA(hModule, module, 256);
            printf("\t%s\n", module);
        }

        //try to get line
        if (SymGetLineFromAddr64(process, stack.AddrPC.Offset, &disp, line)) {
            printf("\t\tat %s in %s: line: %lu: address: 0x%0llX\n", pSymbol->Name, line->FileName, line->LineNumber, pSymbol->Address);
        } else {
            int status;
            printf("\t\t0x%0llX - %s\n", pSymbol->Address, pSymbol->Name);
        }

        free(line);
        line = NULL;
    }

    printf("\nPress enter to exit... ");
    std::cin.ignore();
    return EXCEPTION_CONTINUE_SEARCH;
};
#endif

void Djinn::InstallCrashHandler() {
    // Linux specific handlers
#ifdef DJINN_PLATFORM_LINUX
    //    std::signal(SIGSEGV, [](int sig) {
    //        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    //        printf("A fatal exception has occurred, stack trace:\n");
    //        //Djinn::PrintStackTrace();
    //    });
    //    std::signal(SIGFPE, [](int sig) {
    //        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    //        printf("A floating point exception has occurred, stack trace:\n");
    //        //Djinn::PrintStackTrace();
    //    });
    //    std::signal(SIGILL, [](int sig) {
    //        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    //        printf("An illegal instruction was executed, stack trace:\n");
    //        //Djinn::PrintStackTrace();
    //    });
    //    std::signal(SIGABRT, [](int sig) {
    //        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    //        printf("Abortion called, stack trace:\n");
    //        //Djinn::PrintStackTrace();
    //    });
#endif

    // Windows specific handlers
#ifdef DJINN_PLATFORM_WINDOWS
    SetUnhandledExceptionFilter(WindowsHandler);
#endif
}
