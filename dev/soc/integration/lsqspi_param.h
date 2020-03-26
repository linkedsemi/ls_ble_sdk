#ifndef LSQSPI_PARAM_H_
#define LSQSPI_PARAM_H_
#include "lsqspi.h"
#define LSQSPI_MEM_MAP_BASE_ADDR (void *)0x18000000
#define LSQSPI_DUMMY_BYTES_CONVERT(x) ((x)-1)

uint8_t lsqspi_rd_cap_dly_get(struct lsqspi_instance *inst);

uint8_t lsqspi_baudrate_get(struct lsqspi_instance *inst);
#endif
