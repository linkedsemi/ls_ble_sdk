#ifndef LSQSPI_H_
#define LSQSPI_H_
#include <stdbool.h>
#include "reg_lsqspi.h"

struct lsqspi_instance
{
	reg_lsqspi_t *reg;
	uint8_t *mem_base;
};

struct lsqspi_direct_read_config_param
{
	uint8_t opcode;
	uint8_t dummy_bytes:2,
			quad_addr:1,
			quad_data:1,
			mode_bits_en:1;
};

struct lsqspi_direct_write_data_param
{
	uint8_t *data;
	uint32_t addr:24,
			opcode:8;
	uint16_t size;
	bool quad_data;
};

struct lsqspi_stig_read_data_param
{
	uint8_t *data;
	uint32_t addr:24,
			opcode:8;
	uint16_t size;
	uint8_t dummy_bytes:2,
			quad_addr:1,
			quad_data:1,
			mode_bits_en:1;
};

void lsqspi_init(struct lsqspi_instance *inst);

void lsqspi_mode_bits_set(struct lsqspi_instance *inst,uint8_t mode_bits);

void lsqspi_direct_read_config(struct lsqspi_instance *inst,struct lsqspi_direct_read_config_param *param);

void lsqspi_direct_write_data(struct lsqspi_instance *inst,struct lsqspi_direct_write_data_param *param);

void lsqspi_stig_read_data(struct lsqspi_instance *inst,struct lsqspi_stig_read_data_param *param);

void lsqspi_stig_write_register(struct lsqspi_instance *inst,uint8_t opcode,uint8_t *data,uint8_t length);

void lsqspi_stig_read_register(struct lsqspi_instance *inst,uint8_t opcode,uint8_t *data,uint8_t length);

#endif

