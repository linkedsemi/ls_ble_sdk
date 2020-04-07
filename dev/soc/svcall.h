#ifndef SVCALL_H_
#define SVCALL_H_
#include <stdint.h>

#if defined(__CC_ARM)
#define SVCALL(number, return_type, signature) return_type __svc(number) signature
#else
#define SVCALL(number, return_type, signature)          \
  _Pragma("GCC diagnostic push")                        \
  _Pragma("GCC diagnostic ignored \"-Wreturn-type\"")   \
  __attribute__((naked))                                \
  __attribute__((unused))                               \
  static return_type signature                          \
  {                                                     \
    __asm(                                              \
        "svc %0\n"                                      \
        "bx r14" : : "I" (number) : "r0"   \
    );                                                  \
  }                                                     \
  _Pragma("GCC diagnostic pop")
#endif

enum svcall_num_enum
{
    SVCALL_FLASH_PROGRAM,
    SVCALL_FLASH_SECTOR_ERASE,
    SVCALL_FLASH_FAST_READ,
    SVCALL_FLASH_QUAD_READ,
    SVCALL_FLASH_CHIP_ERASE,
    SVCALL_FLASH_WRITE_STATUS_REG,
    SVCALL_FLASH_ERASE_SECURITY,
    SVCALL_FLASH_PROGRAM_SECURITY,
    SVCALL_NUM_MAX,
};



#endif
