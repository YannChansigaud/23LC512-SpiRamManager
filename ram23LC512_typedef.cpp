/**********************************************************************
 * 
 * This is the C++ part of the Ram typedef library.
 * See ram23K256-typedef.h for a full documentation.
 * 
 *********************************************************************/

#include "ram23LC512_typedef.h"

// ============================= 16 bits ============================= // 
_uint16_t::_uint16_t(){}
_uint16_t::~_uint16_t(){  del();  }

bool _uint16_t::alloc( uint16_t q=0 ){
  if( add == 0 ){
    add = RAM.allocNewVar( RamSPI_uint16_t, q );
    return( add != 0 );
  }
  return( false );
}
void _uint16_t::del(){
  focus( 0 );
  RAM.del();
  add = 0;
}


_uint16_t &_uint16_t::operator=(  const _uint16_t &val ){     val.get();  focus();                                          write();   return( *this );              }
_uint16_t &_uint16_t::operator=(  const  uint16_t  val ){                 focus();                   RAM.buffer16  = val;   write();   return( *this );              }
_uint16_t &_uint16_t::operator+=( const  uint16_t  val ){                 focus();          read();  RAM.buffer16 += val;   write();   return( *this );              }
_uint16_t &_uint16_t::operator-=( const  uint16_t  val ){                 focus();          read();  RAM.buffer16 -= val;   write();   return( *this );              }
_uint16_t &_uint16_t::operator++(){                                       focus();          read();  RAM.buffer16 ++;       write();   return( *this );              }
_uint16_t &_uint16_t::operator--(){                                       focus();          read();  RAM.buffer16 --;       write();   return( *this );              }
_uint16_t &_uint16_t::operator*=( const  uint16_t  val ){                 focus();          read();  RAM.buffer16 *= val;   write();   return( *this );              }
_uint16_t &_uint16_t::operator/=( const  uint16_t  val ){                 focus();          read();  RAM.buffer16 /= val;   write();   return( *this );              }
_uint16_t &_uint16_t::operator[]( const  uint16_t  index ){               focus( index );                                              return( *this );              }
uint16_t   _uint16_t::get(){                                              focus();          read();                                    return( RAM.buffer16 );  }



// ============================= 32 bits ============================= // 
_uint32_t::_uint32_t(){}
_uint32_t::~_uint32_t(){  del();  }

bool _uint32_t::alloc( uint16_t q=0 ){
  if( add == 0 ){
    add = RAM.allocNewVar( RamSPI_uint32_t, q );
    return( add != 0 );
  }
  return( false );
}
void _uint32_t::del(){
  focus( 0 );
  RAM.del();
  add = 0;
}

_uint32_t &_uint32_t::operator=(  const _uint32_t &val ){     val.get();  focus();                                          write();   return( *this );              }
_uint32_t &_uint32_t::operator=(  const  uint32_t  val ){                 focus();                   RAM.buffer32  = val;   write();   return( *this );              }
_uint32_t &_uint32_t::operator+=( const  uint32_t  val ){                 focus();          read();  RAM.buffer32 += val;   write();   return( *this );              }
_uint32_t &_uint32_t::operator-=( const  uint32_t  val ){                 focus();          read();  RAM.buffer32 -= val;   write();   return( *this );              }
_uint32_t &_uint32_t::operator++(){                                       focus();          read();  RAM.buffer32 ++;       write();   return( *this );              }
_uint32_t &_uint32_t::operator--(){                                       focus();          read();  RAM.buffer32 --;       write();   return( *this );              }
_uint32_t &_uint32_t::operator*=( const  uint32_t  val ){                 focus();          read();  RAM.buffer32 *= val;   write();   return( *this );              }
_uint32_t &_uint32_t::operator/=( const  uint32_t  val ){                 focus();          read();  RAM.buffer32 /= val;   write();   return( *this );              }
_uint32_t &_uint32_t::operator[]( const  uint16_t  index ){               focus( index );                                              return( *this );              }
uint32_t   _uint32_t::get(){                                              focus();          read();                                    return( RAM.buffer32 );  }





/*
// ============================= 64 bits ============================= // 
_uint64_t::_uint64_t(){}
_uint64_t::~_uint64_t(){}
_uint64_t &_uint64_t::operator=(  const _uint64_t &val ){                                     RAM.set64( add, val.get()   );    return( *this );          }
_uint64_t &_uint64_t::operator=(  const  uint64_t  val ){                                     RAM.set64( add, val         );    return( *this );          }
_uint64_t &_uint64_t::operator+=( const  uint64_t  val ){   uint64_t v = get();   v += val;   RAM.set64( add, v           );    return( *this );          }
_uint64_t &_uint64_t::operator-=( const  uint64_t  val ){   uint64_t v = get();   v -= val;   RAM.set64( add, v           );    return( *this );          }
_uint64_t &_uint64_t::operator*=( const  uint64_t  val ){   uint64_t v = get();   v *= val;   RAM.set64( add, v           );    return( *this );          }
_uint64_t &_uint64_t::operator/=( const  uint64_t  val ){   uint64_t v = get();   v /= val;   RAM.set64( add, v           );    return( *this );          }
uint64_t _uint64_t::get(){                                                                    RAM.get64( add              );    return( RAM.raw64 );   }

*/



// ============================= DOUBLE =============================== // 

_double::_double(){}
_double::~_double(){  del();  }

bool _double::alloc( uint16_t q=0 ){
  if( add == 0 ){
    add = RAM.allocNewVar( RamSPI_uint32_t, q );
    return( add != 0 );
  }
  return( false );
}
void _double::del(){
  focus( 0 );
  RAM.del();
  add = 0;
}

_double &_double::operator=(  const _double   &val ){     val.get();  focus();                                           write();   return( *this );          }
_double &_double::operator=(  const  double    val ){                 focus();                   RAM.bufferDBL  = val;   write();   return( *this );          }
_double &_double::operator+=( const  double    val ){                 focus();          read();  RAM.bufferDBL += val;   write();   return( *this );          }
_double &_double::operator-=( const  double    val ){                 focus();          read();  RAM.bufferDBL -= val;   write();   return( *this );          }
_double &_double::operator++(){                                       focus();          read();  RAM.bufferDBL ++;       write();   return( *this );          }
_double &_double::operator--(){                                       focus();          read();  RAM.bufferDBL --;       write();   return( *this );          }
_double &_double::operator*=( const  double    val ){                 focus();          read();  RAM.bufferDBL *= val;   write();   return( *this );          }
_double &_double::operator/=( const  double    val ){                 focus();          read();  RAM.bufferDBL /= val;   write();   return( *this );          }
_double &_double::operator[]( const  uint16_t  index ){               focus( index );                                               return( *this );          }
double   _double::get(){                                              focus();          read();                                     return( RAM.bufferDBL );  }




// ============================= VARS =============================== // 

_vars_t::_vars_t(){}
_vars_t::~_vars_t(){  del();  }

bool _vars_t::alloc( uint16_t q=0 ){
  if( add == 0 ){
    add = RAM.allocNewVar( RamSPI_userDefined, q );
    return( add != 0 );
  }
  return( false );
}
void _vars_t::del(){
  focus( 0 );
  RAM.del();
  add = 0;
}

_vars_t &_vars_t::operator=(  const _vars_t   &val ){     val.get();  focus();                                           write();   return( *this );          }
_vars_t &_vars_t::operator=(  const  double    val ){                 focus();                   RAM.bufferDBL  = val;   write();   return( *this );          }
_vars_t &_vars_t::operator+=( const  double    val ){                 focus();          read();  RAM.bufferDBL += val;   write();   return( *this );          }
_vars_t &_vars_t::operator-=( const  double    val ){                 focus();          read();  RAM.bufferDBL -= val;   write();   return( *this );          }
_vars_t &_vars_t::operator++(){                                       focus();          read();  RAM.bufferDBL ++;       write();   return( *this );          }
_vars_t &_vars_t::operator--(){                                       focus();          read();  RAM.bufferDBL --;       write();   return( *this );          }
_vars_t &_vars_t::operator*=( const  double    val ){                 focus();          read();  RAM.bufferDBL *= val;   write();   return( *this );          }
_vars_t &_vars_t::operator/=( const  double    val ){                 focus();          read();  RAM.bufferDBL /= val;   write();   return( *this );          }
_vars_t &_vars_t::operator[]( const  uint16_t  index ){               focus( index );                                               return( *this );          }
double   _vars_t::get(){                                              focus();          read();                                     return( RAM.bufferDBL );  }


