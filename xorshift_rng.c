/* xorshift_rng.c -- Body file for the xorshift_rng unit.
 * 
 * Chris Fletcher
 * 28OCT17
 */

#include "xorshift_rng.h"

#include <stdint.h>

#define DEFAULT_SEED 1

/* State variable for the xorshift8 rng. Saves the last returned value for use
 * in calculating the next value.
 */
static uint8_t xorshift8_last_value = DEFAULT_SEED;

/* State variables for the xorshift8_uint and xorshift8_int bit generators.
 * Saves the buffer, reserve, and the number of bits in each.
 */
static uint8_t buffer;
static uint8_t reserve;
static uint8_t num_buffer = 0;
static uint8_t num_reserve = 0;

/*
 * xorshift8_seed -- Sets the seed value for the 8-bit xorshift RNG.
 * 
 * Parameters
 *  seed        The seed value. Must be non-zero.
 * 
 * Updates the state variable to the seed value.
 */
void xorshift8_seed(uint8_t seed)
{
    if (seed != 0)
        xorshift8_last_value = seed;
    
    return;
}

/*
 * xorshift8 -- Returns an 8-bit pseudorandom number using the xorshift
 * algorithm.
 * 
 * Returns
 *  The next number in the sequence.
 * 
 * The total sequence length is 255 numbers, 1-255 (no zero), before repeating.
 * Each number in the range 1-255 occurs once during the sequence.
 * 
 * This implementation uses the shift sequence L1 R1 R2. See 'Xorshift
 * description.txt' and 'Recommended Xorshift Sequences.txt'.
 */
uint8_t xorshift8()
{
    uint8_t rnd_num;
    
    rnd_num = xorshift8_last_value;
    
    rnd_num ^= (rnd_num << 1);
    rnd_num ^= (rnd_num >> 1);
    rnd_num ^= (rnd_num >> 2);
    
    xorshift8_last_value = rnd_num;
    
    return rnd_num;
}


/*
 * xorshift8_uint -- Returns a sequence of pseudorandom bits.
 * 
 * Parameters
 *  n           The number of bits to be returned, 1-8.
 * 
 * Returns
 *  The sequence of bits, returned as the low order bits of an unsigned byte.
 *  If the requested number of bits are outside the range of 1-8, zero will be
 *  returned and no bits will be consumed.
 * 
 * The flow of bits is from the RNG -> reserve -> buffer. Requested bits are
 * first drawn from the buffer. If there are insufficient bits in the buffer,
 * additional bits will be drawn from the reserve. When the reserve runs empty,
 * it is refilled by a call to xorshift8.
 */
uint8_t xorshift8_uint(uint8_t n)
{
    uint8_t bits;
    uint8_t i;
    
    if ((n == 0) || (n > 8))
        return 0;
    
    while (num_buffer < n) {
    
        /* Refill the reserve if empty */
        if (num_reserve == 0) {
            reserve = xorshift8();
            num_reserve = 8;
        }
        
        /* Find number of bits to move from reserve to buffer--the lesser of
         available spaces in the buffer or available reserve bits. */
        if (num_reserve >= (8 - num_buffer))
            i = (8 - num_buffer);
        else
            i = num_reserve;
        
        buffer |= (reserve << num_buffer);
        num_buffer += i;

        reserve = reserve >> i;
        num_reserve -= i;
    }
    
    bits = buffer & (0xFF >> (8 - n));
    
    buffer = buffer >> n;
    num_buffer -= n;
    
    return bits;
}

/*
 * xorshift8_int -- Returns a pseudorandom, signed n-bit number.
 *
 * Parameters
 *  n       The number of bits to be returned, 1-7.
 *
 * Returns
 *  A pseudorandom, signed n-bit number using the xorshift algorithm.
 *  The returned number is in the range of +(2^n - 1) to -(2^n). For
 *  example, the call xorshift8_int(2) will return numbers between +3 and -4.
 *  If the requested number of bits is outside the range 1-7, zero will be
 *  returned and no bits will be used. A successful call to xorshift8_int will
 *  consume n+1 bits.
 * 
 * n+1 bits are drawn using a call to xorshift8_uint. If the MSB is set, the
 * number is negative and the remaining high order bits are set to make it a
 * full 8-bit signed number.
 */
int8_t xorshift8_int(uint8_t n)
{
    uint8_t num;
    
    if ((n == 0) || (n > 7))
        return 0;
    
    num = xorshift8_uint(n + 1);
    
    if (num & (0xFF << n)) {
        num = num | (0xFF << n);
    }
    
    return (int8_t)num;
}

