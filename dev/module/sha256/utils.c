#include "utils.h"
#include "constants.h"

#include <string.h>
#include <stdint.h>

#define MASK_TWENTY_SEVEN 0x1b

unsigned int _copy(uint8_t *to, unsigned int to_len,
                   const uint8_t *from, unsigned int from_len)
{
    if (from_len <= to_len)
    {
        (void)memcpy(to, from, from_len);
        return from_len;
    }
    else
    {
        return TC_CRYPTO_FAIL;
    }
}

void _set(void *to, uint8_t val, unsigned int len)
{
    (void)memset(to, val, len);
}

/*
 * Doubles the value of a byte for values up to 127.
 */
uint8_t _double_byte(uint8_t a)
{
    return ((a << 1) ^ ((a >> 7) * MASK_TWENTY_SEVEN));
}

int _compare(const uint8_t *a, const uint8_t *b, size_t size)
{
    const uint8_t *tempa = a;
    const uint8_t *tempb = b;
    uint8_t result = 0;

    for (unsigned int i = 0; i < size; i++)
    {
        result |= tempa[i] ^ tempb[i];
    }
    return result;
}