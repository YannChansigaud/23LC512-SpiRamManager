/**********************************************************************
 * 
 * This is the C++ part of the Ram typedef library.
 * See ram23K256-typedef.h for a full documentation.
 * 
 *********************************************************************/

#include "ram23LC512_typedef.h"

// ============================= 16 bits ============================= // 
_uint16_t::_uint16_t(){}
_uint16_t::~_uint16_t(){}
void       _uint16_t::alloc(){                                  add = extRam.new16();             Serial.print( "_uint16_t add " );   Serial.println( add );          }
void       _uint16_t::alloc( uint16_t q ){                      add = extRam.newArray16( q );     Serial.print( "array add " );       Serial.println( add );          }

_uint16_t &_uint16_t::operator=(  const _uint16_t &val ){       extRam.focus( add );                           extRam.buffer._16[0] = val.get();  extRam.write16();  return( *this );                  }
_uint16_t &_uint16_t::operator=(  const  uint16_t  val ){       extRam.focus( add );                           extRam.buffer._16[0] = val;        extRam.write16();  return( *this );                  }

_uint16_t &_uint16_t::operator+=( const  uint16_t  val ){       extRam.focus( add );         extRam.read16();  extRam.buffer._16[0] += val;       extRam.write16();  return( *this );                  }
_uint16_t &_uint16_t::operator-=( const  uint16_t  val ){       extRam.focus( add );         extRam.read16();  extRam.buffer._16[0] -= val;       extRam.write16();  return( *this );                  }
_uint16_t &_uint16_t::operator++(){                             extRam.focus( add );         extRam.read16();  extRam.buffer._16[0] ++;           extRam.write16();  return( *this );                  }
_uint16_t &_uint16_t::operator--(){                             extRam.focus( add );         extRam.read16();  extRam.buffer._16[0] --;           extRam.write16();  return( *this );                  }
_uint16_t &_uint16_t::operator*=( const  uint16_t  val ){       extRam.focus( add );         extRam.read16();  extRam.buffer._16[0] *= val;       extRam.write16();  return( *this );                  }
_uint16_t &_uint16_t::operator/=( const  uint16_t  val ){       extRam.focus( add );         extRam.read16();  extRam.buffer._16[0] /= val;       extRam.write16();  return( *this );                  }
_uint16_t &_uint16_t::operator[]( const  uint16_t  index ){     extRam.focus( add, index );                                                                          return( *this );                  }
uint16_t   _uint16_t::get(){                                    extRam.focus( add );         extRam.read16();                                                        return( extRam.buffer._16[0] );   }

/*
// ============================= 32 bits ============================= // 
_uint32_t::_uint32_t(){}
_uint32_t::~_uint32_t(){}
_uint32_t &_uint32_t::operator=(  const _uint32_t &val ){                                     extRam.set32( add, val.get()   );    return( *this );          }
_uint32_t &_uint32_t::operator=(  const  uint32_t  val ){                                     extRam.set32( add, val         );    return( *this );          }
_uint32_t &_uint32_t::operator+=( const  uint32_t  val ){   uint32_t v = get();   v += val;   extRam.set32( add, v           );    return( *this );          }
_uint32_t &_uint32_t::operator-=( const  uint32_t  val ){   uint32_t v = get();   v -= val;   extRam.set32( add, v           );    return( *this );          }
_uint32_t &_uint32_t::operator++(){                         uint32_t v = get();   v ++;       extRam.set32( add, v           );    return( *this );          }
_uint32_t &_uint32_t::operator--(){                         uint32_t v = get();   v --;       extRam.set32( add, v           );    return( *this );          }
_uint32_t &_uint32_t::operator*=( const  uint32_t  val ){   uint32_t v = get();   v *= val;   extRam.set32( add, v           );    return( *this );          }
_uint32_t &_uint32_t::operator/=( const  uint32_t  val ){   uint32_t v = get();   v /= val;   extRam.set32( add, v           );    return( *this );          }
uint32_t _uint32_t::get(){                                                                    extRam.get32( add              );    return( extRam.raw32 );   }


// ============================= 64 bits ============================= // 
_uint64_t::_uint64_t(){}
_uint64_t::~_uint64_t(){}
_uint64_t &_uint64_t::operator=(  const _uint64_t &val ){                                     extRam.set64( add, val.get()   );    return( *this );          }
_uint64_t &_uint64_t::operator=(  const  uint64_t  val ){                                     extRam.set64( add, val         );    return( *this );          }
_uint64_t &_uint64_t::operator+=( const  uint64_t  val ){   uint64_t v = get();   v += val;   extRam.set64( add, v           );    return( *this );          }
_uint64_t &_uint64_t::operator-=( const  uint64_t  val ){   uint64_t v = get();   v -= val;   extRam.set64( add, v           );    return( *this );          }
_uint64_t &_uint64_t::operator*=( const  uint64_t  val ){   uint64_t v = get();   v *= val;   extRam.set64( add, v           );    return( *this );          }
_uint64_t &_uint64_t::operator/=( const  uint64_t  val ){   uint64_t v = get();   v /= val;   extRam.set64( add, v           );    return( *this );          }
uint64_t _uint64_t::get(){                                                                    extRam.get64( add              );    return( extRam.raw64 );   }

*/