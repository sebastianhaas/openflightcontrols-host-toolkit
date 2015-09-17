#ifndef FIRST_H_INCLUDED
#define FIRST_H_INCLUDED

#include <stdint.h>

/**
 * \brief Merges two uint8_t into a uint16_t.
 * 
 * A helper function that merges two uint8_t into a unint16_t. It is assumed
 * that the given value is right-aligned. The parameter lowerBits will be the
 * first eight bits from the right, higherBits the last eight.
 * 
 * Example: Decimal 1000 is submitted as two uint8_t (0x03  and 0xE8).
 * 
 * 0 0 0 0 0 0 1 1 1 1 1 0 1 0 0 0
 * |             | |             |
 * --higherBits--- ---lowerBits---
 * 
 * \param lowerBits The lower part of the submitted value
 * \param higherBits The higher part of the submitted value
 * \returns A uint16_t with the value of the two uint8_t parameters merged
 */
uint16_t merge_uint8(uint8_t lowerBits, uint8_t higherBits);


/**
 * \brief Converts a 10-bit result from a PIC microcontroller into a uint16_t
 *        value.
 * 
 * This method expects the ADC register to be right aligned. On a PIC18F14K50
 * to achieve this, ADCON2bits.ADFM must be set to 1.
 */
uint16_t convert_adc_10bit(uint8_t ADRESH, uint8_t ADRESL);

/**
 * \brief Splits a uint16_t into two uint8_t.
 */
void split_uint16(uint8_t *result, uint16_t value);

#endif
