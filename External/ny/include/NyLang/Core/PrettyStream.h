//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <string>
#include <sstream>
#include "StdTypes.h"

namespace NyLang {
    class PrettyStream {
    public:
        template<class T, typename C = std::enable_if_t<std::is_arithmetic<T>::value>>
        PrettyStream &operator<<(const T &value) {
            operator<<(String(std::to_string(value).c_str()));
            return *this;
        }

        PrettyStream& operator<<(const char* ch) {
            operator<<(String(ch));
            return *this;
        }

        template<typename T, typename C = std::enable_if_t<!std::is_same<T, char>::value>>
        PrettyStream& operator<<(T* ptr) {
            std::ostringstream address;
            address << (void const *)ptr;
            operator<<(String(address.str()));
            return *this;
        }

        PrettyStream &operator<<(const String &str);

        const String &GetData() {
            return content;
        }

        void Indent(int offset) {
            indentLevel += offset;
            if (indentLevel < 0) {
                indentLevel = 0;
            }
        }

        void Push() {
            Indent(2);
        }

        void Pop() {
            Indent(-2);
        }

    private:
        String content = "";
        int indentLevel = 0;
    };
}
