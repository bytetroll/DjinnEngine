// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
#pragma once

// Example
//
// throw std::runtime_error( Formatter() << foo << 13 << ", bar" << myData );   Implicitly cast to std::string
// throw std::runtime_error( Formatter() << foo << 13 << ", bar" << myData >> Formatter::to_str );    Explicitly cast to std::string

#include <sstream>
#include <Core/String.h>

namespace Djinn {
    namespace Debug {
        class ExceptionFormatter {
        public:
            enum class FormatterToConversion {
                ToString
            };

        public:
            ExceptionFormatter() {
            }

            ~ExceptionFormatter() {
            }

            ExceptionFormatter& operator<<( const Core::String& Value ) {
                Stream << Value.AsCharArray();
                return *this;
            }

            template< typename Type >
            ExceptionFormatter& operator<<( const Type& Value ) {
                Stream << Value;
                return *this;
            }

            std::string AsString() const {
                return Stream.str();
            }

            operator std::string() const {
                return Stream.str();
            }

            std::string operator>>( const FormatterToConversion& ToConvertType ) {
                return Stream.str();
            }

        private:
            ExceptionFormatter( const ExceptionFormatter& Copy ) {
            }

            ExceptionFormatter& operator=( const ExceptionFormatter& Rhs ) {
                return* this;
            }

        private:
            std::stringstream Stream;
        };

    }
}