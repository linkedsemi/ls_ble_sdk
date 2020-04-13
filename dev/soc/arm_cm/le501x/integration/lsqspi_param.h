#ifndef LSQSPI_PARAM_H_
#define LSQSPI_PARAM_H_
#include "lsqspi.h"
#define LSQSPI_MEM_MAP_BASE_ADDR 0x18000000

void lsqspi_sw_rst(struct lsqspi_instance *inst);

void lsqspi_clk_set(struct lsqspi_instance *inst,bool enable);

uint8_t lsqspi_rd_cap_dly_get(struct lsqspi_instance *inst);

uint8_t lsqspi_baudrate_get(struct lsqspi_instance *inst);

uint32_t lsqspi_dly_get(struct lsqspi_instance *inst);

#endif
