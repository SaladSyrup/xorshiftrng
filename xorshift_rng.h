/******************************************************************************
 * xorshift_rng.h -- Declarations for the xorshift_rng unit.
 * 
 * Chris Fletcher
 * 28OCT17
 * 
 * Xorshift_rng provides an 8-bit pseudorandom number generator using the
 * xorshift algorithm. It is intended for use with 8-bit AVRs, though it should
 * run on most platforms--there's nothing AVR specific.
 * 
 * ----------------------------------------------------------------------------
 *      xorshift8_seed(seed)
 *          uint8_t seed
 * 
 * Sets the seed value for the 8-bit xorshift RNG. The seed must be non-zero.
 * 
 * The seed may be set at any time, and a default value will be used if none is
 * set.
 * 
 * ----------------------------------------------------------------------------
 *      xorshift8()
 * 
 * Returns an 8-bit pseudorandom number, 1-255, using the xorshift algorithm.
 * 
 * The 8-bit xorshift algorithm produces a repeating sequence of 255 numbers
 * (excluding zero). Each number occurs once during the sequence. The sequence
 * will do well using tests of entropy (ent). It should be noted that entropy
 * isn't a thorough test of randomness.
 * 
 * Setting the seed changes the location within the sequence; it does not
 * change the order of returned numbers.
 * 
 * ----------------------------------------------------------------------------
 *      xorshift8_uint(n)
 *          uint8_t n
 * 
 * Returns a pseudorandom sequence of n-bits using the xorshift algorithm. Bits
 * are returned as the low order bits of an unsigned byte. The sequence may be
 * any number of bits 1-8. If the requested number of bits is outside the
 * range of 1-8, zero will be returned and no bits will be used.
 * 
 * xorshift8_int shares the same seed and algorithm as xorshift8; calls to
 * xorshift8_int may affect the sequence generated by xorshift8 and
 * vice-versa. Additionally, xorshift8_uint and xorshift8_int share the same
 * bit buffer, and calls to either may affect the sequence generated by the
 * other.
 * 
 * ----------------------------------------------------------------------------
 *      xorshift8_int(n)
 *          uint8_t n
 * 
 * Returns a pseudorandom, signed n-bit number using the xorshift algorithm.
 * The returned number is in the range of +(2^n - 1) to -(2^n). For example,
 * the call xorshift8_int(2) will return numbers between +3 and -4.
 * 
 * n may be any value 1-7. If the requested number of bits is outside that
 * range, zero will be returned and no bits will be used. Successful calls to
 * xorshift8_int will consume n+1 bits.
 * 
 * xorshift8_int shares the same seed and algorithm as xorshift8; calls to
 * xorshift8_int may affect the sequence generated by xorshift8 and
 * vice-versa. Additionally, xorshift8_uint and xorshift8_int share the same
 * bit buffer, and calls to either may affect the sequence generated by the
 * other.
 * 
 *****************************************************************************/

#ifndef XORSHIFT_RNG_H
#define XORSHIFT_RNG_H

#include <stdint.h>

extern void xorshift8_seed(uint8_t seed);
extern uint8_t xorshift8();
extern uint8_t xorshift8_uint(uint8_t n);
extern int8_t xorshift8_int(uint8_t n);

#endif /* XORSHIFT_RNG_H */