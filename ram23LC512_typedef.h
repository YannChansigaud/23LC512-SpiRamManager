/* RAM typedef library version 1.0
# 
# Writed by Chansigaud Yann
# ram23K256-typedef.h - register library
# 
*/



#include <inttypes.h>
#include <ram23LC512_spi.h>

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#ifndef _uint16_t_h
#define _uint16_t_h

class _uint16_t {
  public:
    _uint16_t();
    ~_uint16_t();

    void     alloc();
    void     alloc( uint16_t q );
    uint16_t get();

    _uint16_t &operator=(  const _uint16_t& val );
    _uint16_t &operator=(  const  uint16_t  val );
    _uint16_t &operator+=( const  uint16_t  val );
    _uint16_t &operator-=( const  uint16_t  val );
    _uint16_t &operator++();
    _uint16_t &operator--();
    _uint16_t &operator/=( const  uint16_t  val );
    _uint16_t &operator*=( const  uint16_t  val );
    _uint16_t &operator[]( const  uint16_t  tabIndex );

  private:
    uint16_t add=0;
    
};

#endif

/*
#ifndef _uint32_t_h
#define _uint32_t_h

class _uint32_t {
  public:
    _uint32_t();
    ~_uint32_t();

    _uint32_t &operator=(  const _uint32_t& val );
    _uint32_t &operator=(  const  uint32_t  val );
    _uint32_t &operator+=( const  uint32_t  val );
    _uint32_t &operator-=( const  uint32_t  val );
    _uint32_t &operator++();
    _uint32_t &operator--();
    _uint32_t &operator/=( const  uint32_t  val );
    _uint32_t &operator*=( const  uint32_t  val );
    uint32_t get();

  private:
    uint16_t add=0;
    
};

#endif

#ifndef _uint64_t_h
#define _uint64_t_h

class _uint64_t {
  public:
    _uint64_t();
    ~_uint64_t();

    _uint64_t &operator=(  const _uint64_t& val );
    _uint64_t &operator=(  const  uint64_t  val );
    _uint64_t &operator+=( const  uint64_t  val );
    _uint64_t &operator-=( const  uint64_t  val );
    _uint64_t &operator++();
    _uint64_t &operator--();
    _uint64_t &operator/=( const  uint64_t  val );
    _uint64_t &operator*=( const  uint64_t  val );
    uint64_t get();

  private:
    uint16_t add=0;
    
};

#endif

*/