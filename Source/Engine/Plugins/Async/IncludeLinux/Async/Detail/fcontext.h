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
 on 9/12/17.
//

/*              Original Copyright                      */
//////////////////////////////////////////////////////////

//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//////////////////////////////////////////////////////////
/*             /Original Copyright                      */

#pragma once

#include <Core/Common.h>
#include <cstddef>

namespace Djinn {
    namespace Async {
        namespace Detail {
            typedef void*   fcontext_t;

            struct transfer_t {
                fcontext_t  fctx;
                void    *   data;
            };

            // Externs
            DJINN_C_EXPORT_PLUGINS_ASYNC transfer_t /*DJINN_CDECL*/ jump_fcontext( fcontext_t const to, void * vp);
            DJINN_C_EXPORT_PLUGINS_ASYNC fcontext_t /*DJINN_CDECL*/ make_fcontext( void * sp, std::size_t size, void (* fn)( transfer_t) );
            DJINN_C_EXPORT_PLUGINS_ASYNC transfer_t /*DJINN_CDECL*/ ontop_fcontext( fcontext_t const to, void * vp, transfer_t (* fn)( transfer_t) );
        }
    }
}