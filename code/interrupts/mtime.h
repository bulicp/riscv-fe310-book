#ifndef MTIME_H
#define MTIME_H


#include <stdint.h>


#define CLINT_BASE                	0x02000000
#define CLINT_MSIP               	0x02000000
#define CLINT_MTIME               	0x0200bff8
#define CLINT_MTIME_CMP         	0x02004000

#define MTIME_ADDRESS ((uint64_t *)CLINT_MTIME)
#define MTIMECMP_ADDRESS ((uint64_t *)CLINT_MTIME_CMP)

__attribute__ ((weak, interrupt)) void mtime_handler (void);

void mtime_delay(uint32_t miliseconds);
void mtime_set_mtimecmp(uint64_t delay);

#endif
