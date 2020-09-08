#include <stdint.h>
#include "reg_mdm2.h"
#include "field_manipulate.h"

#define SetWord32(a,d)      (* ( volatile uint32_t*)(a)=(d) )
#define GetWord32(a)        (* ( volatile uint32_t*)(a) )

uint32_t rf_rpl_reg_rd(uint16_t addr) {
    uint32_t rdat = 0;
    SetWord32(0x5001B004,0);
    SetWord32(0x5001B000,0x0B00|0x0000|addr);
    while(GetWord32(0x5001B014)!=1);
    while(GetWord32(0x5001B014)!=0);
    rdat = GetWord32(0x5001B010);
    return rdat;
}

void rf_rpl_reg_wr(uint16_t addr, uint32_t value) {
    SetWord32(0x5001B004,value);
    SetWord32(0x5001B000,0x0300|0xB000|addr);
    while(GetWord32(0x5001B014)!=1);
    while(GetWord32(0x5001B014)!=0);
    for(int i=0; i<10; i++) { __asm ("nop"); }
}

void rf_init_setting(void) {    
  rf_rpl_reg_wr(0x14, 0x000673ac);
  rf_rpl_reg_wr(0x02, 0x0f002460);
  rf_rpl_reg_wr(0x14, 0x000673fd);
  rf_rpl_reg_wr(0x00, 0xff7f013f);
  rf_rpl_reg_wr(0x01, 0x605a3317);
  rf_rpl_reg_wr(0x02, 0x07012a58);
  rf_rpl_reg_wr(0x03, 0x22e966d4);
  rf_rpl_reg_wr(0x04, 0xc0044005);
  rf_rpl_reg_wr(0x05, 0x80000000);
  rf_rpl_reg_wr(0x06, 0x50b00080);
  rf_rpl_reg_wr(0x07, 0x1f000980);
 // rf_rpl_reg_wr(0x08, 0x4c65824c);
	rf_rpl_reg_wr(0x08, 0x4c00824c);
  rf_rpl_reg_wr(0x09, 0x50752515);
  rf_rpl_reg_wr(0x0a, 0xaa32011c);
  rf_rpl_reg_wr(0x0b, 0xaa325f00);
  rf_rpl_reg_wr(0x0c, 0x00007444);
  rf_rpl_reg_wr(0x03, 0x24e966d2);
  rf_rpl_reg_wr(0x04, 0xc0244120);
  rf_rpl_reg_wr(0x00, 0xc77f0000);
	
	rf_rpl_reg_rd(0x00);
  rf_rpl_reg_rd(0x01);
  rf_rpl_reg_rd(0x02);
  rf_rpl_reg_rd(0x03);
  rf_rpl_reg_rd(0x04);
  rf_rpl_reg_rd(0x05);
  rf_rpl_reg_rd(0x06);
  rf_rpl_reg_rd(0x07);
  rf_rpl_reg_rd(0x08);
  rf_rpl_reg_rd(0x09);
  rf_rpl_reg_rd(0x0a);
  rf_rpl_reg_rd(0x0b);
  rf_rpl_reg_rd(0x0c);

}

static void rf_reg_init()
{
    rf_init_setting();
}

static void modem_reg_init()
{
    MDM2->REG08 = FIELD_BUILD(MDM2_MIN_MAG_CONF,0x474)
               | FIELD_BUILD(MDM2_DRIFT_COR_SET,1)
               | FIELD_BUILD(MDM2_IF_SHIFT,0x400);
    MDM2->REG20 = FIELD_BUILD(MDM2_LR_IF_SHIFT,0x400)
               | FIELD_BUILD(MDM2_LR_RX_INVERT,1)
               | FIELD_BUILD(MDM2_LR_IQ_INVERT,0)
               | FIELD_BUILD(MDM2_LR_ACC_INVERT,0);
}


void modem_rf_init()
{
    modem_reg_init();
    rf_reg_init();
}