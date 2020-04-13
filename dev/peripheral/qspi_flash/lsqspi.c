#include <string.h>
#include "lsqspi.h"
#include "field_manipulate.h"
#include "ls_dbg.h"
#include "lsqspi_param.h"
#include "section_def.h"

XIP_BANNED void lsqspi_init(struct lsqspi_instance *inst)
{
    inst->mem_base = (void *)LSQSPI_MEM_MAP_BASE_ADDR;
    lsqspi_clk_set(inst,true);
    lsqspi_sw_rst(inst);
    inst->reg->DLY = lsqspi_dly_get(inst);
    inst->reg->RDCAP = FIELD_BUILD(LSQSPI_DLY_RD_CAP, lsqspi_rd_cap_dly_get(inst));
    inst->reg->CFG = FIELD_BUILD(LSQSPI_BAUDRATE,lsqspi_baudrate_get(inst))|FIELD_BUILD(LSQSPI_DAC_ENABLE,1)
        |FIELD_BUILD(LSQSPI_CPHA,0)|FIELD_BUILD(LSQSPI_CPOL,0)|FIELD_BUILD(LSQSPI_ENABLE,1);
}

XIP_BANNED void lsqspi_direct_read_config(struct lsqspi_instance *inst,struct lsqspi_direct_read_config_param *param)
{
    inst->reg->RDINS = FIELD_BUILD(LSQSPI_RD_OPCODE, param->opcode) | FIELD_BUILD(LSQSPI_RD_ADDR_TRANS_TYPE,param->quad_addr)
        | FIELD_BUILD(LSQSPI_RD_DATA_TRANS_TYPE,param->quad_data) | FIELD_BUILD(LSQSPI_RD_MODE_BIT_EN,param->mode_bits_en)
        | FIELD_BUILD(LSQSPI_RD_NUM_DUMMY, param->dummy_bytes);
}

XIP_BANNED void lsqspi_mode_bits_set(struct lsqspi_instance *inst,uint8_t mode_bits)
{
    inst->reg->MODE_BITS = mode_bits;
}

XIP_BANNED static void lsqspi_idle_status_check(struct lsqspi_instance *inst)
{
    while(REG_FIELD_RD(inst->reg->CFG, LSQSPI_IDLE)==0);
}

XIP_BANNED void lsqspi_direct_write_data(struct lsqspi_instance *inst,struct lsqspi_direct_write_data_param *param)
{
    inst->reg->WRINS = FIELD_BUILD(LSQSPI_WR_OPCODE, param->opcode)
        | FIELD_BUILD(LSQSPI_WR_ADDR_TRANS_TYPE,0) | FIELD_BUILD(LSQSPI_WR_DATA_TRANS_TYPE, param->quad_data) 
        | FIELD_BUILD(LSQSPI_WR_NUM_DUMMY, 0);
    inst->reg->DACWR_BYTES = FIELD_BUILD(LSQSPI_DACWR_BYTES_SET,1) | FIELD_BUILD(LSQSPI_DACWR_BYTES,param->size);
    memcpy(inst->mem_base + param->addr,param->data,param->size);
    lsqspi_idle_status_check(inst);
    inst->reg->DACWR_STAT = FIELD_BUILD(LSQSPI_DAC_WIP_CLR,1);
}

XIP_BANNED static void stig_read_data(struct lsqspi_instance *inst,uint32_t stig_cmd,bool hold_cs,uint32_t rd[2])
{
    inst->reg->STIG_CMD = stig_cmd;
    inst->reg->STIG_GO = FIELD_BUILD(LSQSPI_STIG_HOLD_CS,hold_cs) | FIELD_BUILD(LSQSPI_STIG_GO,1);    
    lsqspi_idle_status_check(inst);
    rd[0] = inst->reg->STIG_RD[0];
    rd[1] = inst->reg->STIG_RD[1];
}

XIP_BANNED void lsqspi_stig_read_data(struct lsqspi_instance *inst,struct lsqspi_stig_read_data_param *param)
{
    inst->reg->STIG_ADDR = param->addr;
    uint32_t cmd = FIELD_BUILD(LSQSPI_CMD_OPCODE,param->opcode) | FIELD_BUILD(LSQSPI_DATA_XFER_TYPE,param->quad_data) 
        | FIELD_BUILD(LSQSPI_ADDR_XFER_TYPE, param->quad_addr) | FIELD_BUILD( LSQSPI_RDATA_EN , 1)    
        | FIELD_BUILD(LSQSPI_MODE_EN, param->mode_bits_en) | FIELD_BUILD(LSQSPI_ADDR_EN, 1) | FIELD_BUILD(LSQSPI_OPCODE_EN, 1);
    if(param->dummy_bytes)
    {
        cmd |= FIELD_BUILD(LSQSPI_NUM_DUMMY_BYTES,param->dummy_bytes - 1) | FIELD_BUILD(LSQSPI_DUMMY_EN, 1);
    }
    uint32_t reg_rd[2];
    if((uint32_t)param->data % 4)
    {
        uint8_t first_length = 4 - (uint32_t)param->data % 4;
        stig_read_data(inst,cmd | FIELD_BUILD(LSQSPI_NUM_RDATA_BYTES, first_length - 1),param->size > first_length,reg_rd);
        cmd = FIELD_BUILD( LSQSPI_RDATA_EN , 1);
        memcpy(param->data,&reg_rd,first_length);
        param->size -= first_length;
        param->data += first_length;
    }
    uint16_t cycle = param->size / 8;
    uint8_t remainder = param->size % 8;
    if(remainder == 0 && cycle > 0)
    {
        cycle -= 1;
        remainder = 8;
    }
    uint16_t i;
    for(i=0;i<cycle;++i)
    {
        stig_read_data(inst,cmd | FIELD_BUILD(LSQSPI_NUM_RDATA_BYTES, 8 - 1),true,(uint32_t *)param->data);        
        cmd = FIELD_BUILD( LSQSPI_RDATA_EN , 1);
        param->size -= 8;
        param->data += 8;
    }
    if(remainder)
    {
        stig_read_data(inst,cmd | FIELD_BUILD(LSQSPI_NUM_RDATA_BYTES, remainder - 1),false,reg_rd);
        memcpy(param->data,reg_rd,remainder);
    }
}

XIP_BANNED void lsqspi_stig_write_register(struct lsqspi_instance *inst,uint8_t opcode,uint8_t *data,uint8_t length)
{
    LS_RAM_ASSERT(length <= 4);
    if(length)
    {
        inst->reg->STIG_CMD = FIELD_BUILD(LSQSPI_CMD_OPCODE, opcode) | FIELD_BUILD(LSQSPI_NUM_WDATA_BYTES, length - 1)
            | FIELD_BUILD(LSQSPI_WDATA_EN, 1 ) | FIELD_BUILD(LSQSPI_OPCODE_EN, 1);
        uint32_t reg_wr[2];
        memcpy(reg_wr,data,length);
        inst->reg->STIG_WR[0] = reg_wr[0];
        inst->reg->STIG_WR[1] = reg_wr[1];
    }else
    {
        inst->reg->STIG_CMD = FIELD_BUILD(LSQSPI_CMD_OPCODE, opcode) 
            | FIELD_BUILD(LSQSPI_WDATA_EN, 0 ) | FIELD_BUILD(LSQSPI_OPCODE_EN, 1);
    }
    inst->reg->STIG_GO = FIELD_BUILD(LSQSPI_STIG_GO, 1);
    lsqspi_idle_status_check(inst);
}

XIP_BANNED void lsqspi_stig_read_register(struct lsqspi_instance *inst,uint8_t opcode,uint8_t *data,uint8_t length)
{
    LS_RAM_ASSERT(length && length <= 8);
    inst->reg->STIG_CMD = FIELD_BUILD(LSQSPI_CMD_OPCODE, opcode) | FIELD_BUILD(LSQSPI_NUM_RDATA_BYTES, length - 1)
            | FIELD_BUILD(LSQSPI_RDATA_EN, 1 ) | FIELD_BUILD(LSQSPI_OPCODE_EN, 1);
    inst->reg->STIG_GO = FIELD_BUILD(LSQSPI_STIG_GO, 1);
    lsqspi_idle_status_check(inst);
    uint32_t reg_rd[2];
    reg_rd[0] = inst->reg->STIG_RD[0];
    reg_rd[1] = inst->reg->STIG_RD[1];
    memcpy(data,reg_rd,length);
}

