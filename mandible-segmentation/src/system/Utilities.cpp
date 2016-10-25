#include "Utilities.h"

size_t SystemUtilities::roundUp( int value1, int value2 )
{
    int result = value2 % value1;
    return result == 0 ? ( value2 ) : ( value2 + value1 - result );
}

void SystemUtilities::convertColorToRGBA( uint Color,
                                          uint8_t &r,
                                          uint8_t &g,
                                          uint8_t &b,
                                          uint8_t &a )
{
    b = Color & 0xFF; Color >>= 8;
    g = Color & 0xFF; Color >>= 8;
    r = Color & 0xFF; Color >>= 8;
    a = Color & 0xFF;
}
