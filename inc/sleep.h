#ifndef SLEEP_H_
#define SLEEP_H_
#include <stdint.h>
#include <stdbool.h>
#define XTAL_STB_VAL (0x20)
#define XTAL_STARTUP_CYCLES (XTAL_STB_VAL + 4)

#define NORMAL_SLEEP           0x01
#define SLEEP_MOED0            0x00
#define SLEEP_MOED1            0x02
#define SLEEP_MOED2            0x04
#define SLEEP_MOED3            0x06

#define PB15_IO_WKUP                  0x00000001
#define PB15_IO_WKUP_EDGE_FALLING     0x00000000
#define PB15_IO_WKUP_EDGE_RISING      0x00000001
#define RTC_WKUP                      0x00000002
#define RTC_WKUP_EDGE_FALLING         0x00000000
#define RTC_WKUP_EDGE_RISING          0x00000002
#define NRST_IO_WKUP                  0x00000004
#define NRST_IO_WKUP_EDGE_FALLING     0x00000000
#define NRST_IO_WKUP_EDGE_RISING      0x00000004
#define BLE_WKUP                      0x00000008
#define BLE_WKUP_EDGE_FALLING         0x00000000
#define BLE_WKUP_EDGE_RISING          0x00000008
#define PA00_IO_WKUP                  0x00000010
#define PA00_IO_WKUP_EDGE_FALLING     0x00000000
#define PA00_IO_WKUP_EDGE_RISING      0x00000010
#define PA07_IO_WKUP                  0x00000020
#define PA07_IO_WKUP_EDGE_FALLING     0x00000000
#define PA07_IO_WKUP_EDGE_RISING      0x00000020
#define PB11_IO_WKUP                  0x00000040
#define PB11_IO_WKUP_EDGE_FALLING     0x00000000
#define PB11_IO_WKUP_EDGE_RISING      0x00000040
#define WDT_WKUP                      0x00000080
#define WDT_WKUP_EDGE_FALLING         0x00000000
#define WDT_WKUP_EDGE_RISING          0x00000080

#define WKUP_EN_POS                    0
#define WKUP_STATE_POS                 8
#define WKUP_EDGE_POS                  16

struct sleep_wakeup_type
{
    uint32_t trig_src;
    uint32_t trig_edge;
};

uint8_t get_deep_sleep_enable(void);

void dcdc_on(void);

void dcdc_off(void);

void cpu_sleep_recover_init(void);

void low_power_mode_init();

void deep_sleep(void);

void ble_sleep(void);

void enter_deep_sleep_lvl3_mode(struct sleep_wakeup_type *sleep_param);

void check_wkup_state(void);

void ble_irq_clr_and_enable(void);

void ble_wkup_status_set(bool);

bool ble_wkup_status_get(void);

void clr_ble_wkup_req(void);

void irq_disable_before_wfi(void);

#endif

