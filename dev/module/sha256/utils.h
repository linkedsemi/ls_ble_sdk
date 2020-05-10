/**
 * @file
 * @brief Interface to platform-dependent run-time operations.
 *
 */

#ifndef __TC_UTILS_H__
#define __TC_UTILS_H__

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
 * @brief Copy the the buffer 'from' to the buffer 'to'.
 * @return returns TC_CRYPTO_SUCCESS (1)
 *         returns TC_CRYPTO_FAIL (0) if:
 *                from_len > to_len.
 *
 * @param to OUT -- destination buffer
 * @param to_len IN -- length of destination buffer
 * @param from IN -- origin buffer
 * @param from_len IN -- length of origin buffer
 */
    unsigned int _copy(uint8_t *to, unsigned int to_len,
                       const uint8_t *from, unsigned int from_len);

    /**
 * @brief Set the value 'val' into the buffer 'to', 'len' times.
 *
 * @param to OUT -- destination buffer
 * @param val IN -- value to be set in 'to'
 * @param len IN -- number of times the value will be copied
 */
    void _set(void *to, uint8_t val, unsigned int len);

    /*
 * @brief AES specific doubling function, which utilizes
 * the finite field used by AES.
 * @return Returns a^2
 *
 * @param a IN/OUT -- value to be doubled
 */
    uint8_t _double_byte(uint8_t a);

    /*
 * @brief Constant-time algorithm to compare if two sequences of bytes are equal
 * @return Returns 0 if equal, and non-zero otherwise
 *
 * @param a IN -- sequence of bytes a
 * @param b IN -- sequence of bytes b
 * @param size IN -- size of sequences a and b
 */
    int _compare(const uint8_t *a, const uint8_t *b, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* __TC_UTILS_H__ */
