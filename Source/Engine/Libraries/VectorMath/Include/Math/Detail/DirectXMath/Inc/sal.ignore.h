//
// Created by dev on 3/17/18.
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