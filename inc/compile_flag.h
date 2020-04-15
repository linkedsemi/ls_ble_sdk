#ifndef COMPILE_FLAG_H_
#define COMPILE_FLAG_H_

#define XIP_BANNED __attribute__((section(".xip_banned")))
#define RESET_RETAIN __attribute__((section(".reset_retain")))
#define NOINLINE __attribute__((noinline))

#endif

