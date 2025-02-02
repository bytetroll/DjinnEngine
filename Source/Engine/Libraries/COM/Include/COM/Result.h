#pragma once

#include <Core/Common.h>
#include <Core/Types.h>
#include <Core/CompileHash.h>

namespace Djinn {
    namespace COM {
        struct [[nodiscard]] Result {
            constexpr Result() : error(0U), system(0), value(0) {

            }

            constexpr Result(const Result& rhs) = default;

            constexpr Result(bool Error, UInt16 System, UInt16 Value)
                    : error(static_cast<UInt32>(Error)), system(static_cast<UInt32>(System)), value(static_cast<UInt32>(Value)) {
            }

            // Compare
            DJINN_INLINE bool operator==(const Result &Other) const {
                return (error == Other.error) && (system == Other.system) && (value == Other.value);
            }

            // Compare
            DJINN_INLINE bool operator!=(const Result &Other) const {
                return (error != Other.error) && (system != Other.system) && (value != Other.value);
            }

            // Is error?
            DJINN_INLINE operator bool() const {
                return error == 0;
            }

            /// Sink this result
            void Ignore() const { }

            // Is this result an error?
            UInt32 error : 1;

            // In which system did this error occur?
            UInt32 system : 16;

            // Actual Error
            UInt32 value : 15;
        };
        static_assert(sizeof(Result) == 4, "Unexpected size");

        struct CheckedResult {
            CheckedResult(Result result) : result(result) {

            }

            operator bool() {
                return !static_cast<bool>(result);
            }

            operator const Result&() const {
                return result;
            }

            operator Result&() {
                return result;
            }

        private:
            Result result;
        };

        // Declare result
#define DJINN_COM_RESULT(NAME, IS_ERROR, SYSTEM) static constexpr Djinn::COM::Result k##NAME = Djinn::COM::Result(IS_ERROR, SYSTEM##_crc16, #NAME##_crc16)

        // Common Results
        DJINN_COM_RESULT(OK, false, "COM");
        DJINN_COM_RESULT(InvalidParameter, true, "COM");
        DJINN_COM_RESULT(UnknownInterface, true, "COM");
        DJINN_COM_RESULT(NotImplemented, true, "COM");
        DJINN_COM_RESULT(OutOfMemory, true, "COM");
        DJINN_COM_RESULT(Error, true, "COM");
        DJINN_COM_RESULT(Uninitialized, true, "COM");

        /// Error message check helper
#define Check(CODE) {if(Djinn::COM::CheckedResult result = (CODE)) { return static_cast<COM::Result>(result); }}

        /// Error message check helper
#define CheckVoid(CODE) {if(Djinn::COM::CheckedResult result = (CODE)) { return; }}
    }
}