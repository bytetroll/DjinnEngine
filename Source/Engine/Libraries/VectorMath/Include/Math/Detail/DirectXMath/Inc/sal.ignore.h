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
 on 3/17/18.
//
#pragma once

#undef _Out_writes_
#undef _Out_writes_opt_
#undef _In_reads_
#undef _Out_writes_bytes_
#undef _In_reads_bytes_
#undef _Success_
#undef _Analysis_assume_
#undef _Use_decl_annotations_
#undef _In_
#undef _Out_
#undef _Out_opt_

#define _Out_writes_(...)
#define _Out_writes_opt_(...)
#define _In_reads_(...)
#define _Out_writes_bytes_(...)
#define _In_reads_bytes_(...)
#define _Success_(...)
#define _Analysis_assume_(...)

#define _Use_decl_annotations_

#define _In_
#define _Out_
#define _Out_opt_