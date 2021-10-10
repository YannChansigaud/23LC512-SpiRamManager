/**********************************************************************
 * 
 * This is the C++ part of the Ram typedef library.
 * See ram23K256-typedef.h for a full documentation.
 * 
 *********************************************************************/

#include "ram23LC512_typedef.h"


// ============================= 16 bits ============================= // 

_uint16_t::operator uint16_t() const{
  get() ;
  return( RAM.buffer16 );
}

void _uint16_t::alloc( uint16_t q ){
  ram_vars::alloc( RamSPI_uint16_t, q );
}

// ============================= 32 bits ============================= // 

_uint32_t::operator uint32_t() const{
  get() ;
  return( RAM.buffer32 );
}

void _uint32_t::alloc( uint16_t q ){
  ram_vars::alloc( RamSPI_uint32_t, q );
}


// ============================= double ============================= // 

_double::operator double() const{
  get() ;
  return( RAM.bufferDBL );
}

void _double::alloc( uint16_t q ){
  ram_vars::alloc( RamSPI_float_double, q );
}