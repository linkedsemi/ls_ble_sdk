#ifndef SLEEP_H_
#define SLEEP_H_

void dcdc_on(void);

void dcdc_off(void);

void cpu_sleep_recover_init(void);

void deep_sleep(void);

#endif

