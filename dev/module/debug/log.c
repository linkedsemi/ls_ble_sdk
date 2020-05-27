#include "log.h"
#include "segger_rtt.h"
//#include "compiler_flag.h"
//#include "ram_log.h"


const uint8_t hex_num_tab[] = "0123456789ABCDEF";

int SEGGER_RTT_vprintf(unsigned BufferIndex, const char * sFormat, va_list * pParamList);

void rtt_output(bool linefeed,const char *format,va_list *args)
{
    SEGGER_RTT_vprintf(0,format,args);
    if(linefeed)
    {
        SEGGER_RTT_vprintf(0,"\n",NULL);
    }
}

void log_output(bool linefeed,const char *format,...)
{
    va_list args;
    if(LOG_BACKEND&JLINK_RTT)
    {
        va_start(args,format);
        rtt_output(linefeed,format,&args);
    }
    if(LOG_BACKEND&UART1_LOG)
    {
        va_start(args,format);

    }
    if(LOG_BACKEND&RAM_LOG)
    {
        va_start(args,format);
//        ram_log_print(linefeed,format,&args);
    }
    va_end(args);
}

void log_init()
{
    if(LOG_BACKEND&JLINK_RTT)
    {
        SEGGER_RTT_Init();
    }
    if(LOG_BACKEND&UART1_LOG)
    {

    }
    if(LOG_BACKEND&RAM_LOG)
    {

    }
}



/**
 ****************************************************************************************
 * @brief  tools for printf to  log data.
 *
 * @param[in]  data_pointer      Pointer to the data to be printf.
 * @param[in]  data_length    The length of data to be printf.
 *
 ****************************************************************************************
 */
void log_hex_output(const void * data_pointer , uint16_t data_length)
{
    
    uint8_t *data = (uint8_t*)data_pointer;
    uint8_t  tmp_h,tmp_l;
    uint32_t total_length = data_length * 2 + 1;
    uint8_t  log_format_buff[total_length];
    uint8_t *bufptr=log_format_buff;
    //content
    for(uint16_t i=0;i<data_length;i++)
    {
        tmp_h = data[i] >> 4;
        tmp_l = data[i] & 0x0F;
        *bufptr = hex_num_tab[tmp_h];  bufptr++;
        *bufptr = hex_num_tab[tmp_l];  bufptr++;
    }
    *bufptr = '\n'; bufptr ++;
    
    //print
    if(LOG_BACKEND&JLINK_RTT)
    {
        SEGGER_RTT_Write(0,log_format_buff,total_length);
    }
    if(LOG_BACKEND&UART1_LOG)
    {

    }
    if(LOG_BACKEND&RAM_LOG)
    {
        //ram_log_write_buff(log_format_buff,total_length);
    }
}





