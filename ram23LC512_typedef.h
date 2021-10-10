/* RAM typedef library version 1.0
# 
# Writed by Chansigaud Yann
# ram23K256-typedef.h - register library
# 
*/



#include <inttypes.h>
#include <ram23LC512_spi.h>
#include <ram23LC512_type.h>

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#ifndef _uint16_t_h
#define _uint16_t_h

class _uint16_t : public ram_vars
{
  public:
    void alloc( uint16_t q=0 );
    operator uint16_t() const ;
};

#endif


#ifndef _uint32_t_h
#define _uint32_t_h

class _uint32_t : public ram_vars
{
  public:
    void alloc( uint16_t q=0 );
    operator uint32_t() const ;
};

#endif


#ifndef _double_h
#define _double_h

class _double : public ram_vars
{
  public:
    void alloc( uint16_t q=0 );
    operator double() const ;
};

#endif

