#pragma once

// Get next instruction, outputs address (void*) into VAR
/*#if 0
#define DJINN_INSTRADDRESS(VAR) asm ("lea (%%rip),%0" : "=r" (VAR))
#else
#define DJINN_INSTRADDRESS(VAR) VAR = &&a; a:
#endif*/