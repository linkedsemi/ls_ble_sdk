#ifndef COMPILE_FLAG_H_
#define COMPILE_FLAG_H_

#define XIP_BANNED __attribute__((section(".xip_banned")))
#define RESET_RETAIN __attribute__((section(".reset_retain")))
#define NOINLINE __attribute__((noinline))
#if defined(__CC_ARM)
#define _Static_assert(expr) int __static_assert(int static_assert_failed[(expr)?1:-1])
#endif

#endif

