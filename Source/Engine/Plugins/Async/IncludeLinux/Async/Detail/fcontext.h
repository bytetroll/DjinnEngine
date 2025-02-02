//
// Created by dev on 9/12/17.
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