#ifndef SYSTEM_UTILITIES_H
#define SYSTEM_UTILITIES_H

#include <Headers.hh>
#include <Typedefs.hh>

namespace SystemUtilities
{
/**
 * @brief roundUp
 * @param value1
 * @param value2
 * @return
 */
size_t roundUp(int value1, int value2 );

/**
 * @brief convertColorToRGBA
 * @param Color
 * @param r
 * @param g
 * @param b
 * @param a
 */
void convertColorToRGBA( uint Color,
                         uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a );
}

#endif // SYSTEM_UTILITIES_H
