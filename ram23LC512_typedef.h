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

    bool alloc( uint16_t q=0 );
    void del();

    _uint16_t &operator=(  const _uint16_t& val );
    _uint16_t &operator=(  const  uint16_t  val );
    _uint16_t &operator+=( const  uint16_t  val );
    _uint16_t &operator-=( const  uint16_t  val );
    _uint16_t &operator++();
    _uint16_t &operator--();
    _uint16_t &operator/=( const  uint16_t  val );
    _uint16_t &operator*=( const  uint16_t  val );
    _uint16_t &operator[]( const  uint16_t  tabIndex );

    operator uint16_t() const{ return( get() ); }
    uint16_t get();
    

  private:
    uint16_t add=0;

    inline void focus(){                    if( add ){    RAM.focus( add );         }   else{   alloc();    }    }
    inline void focus( uint16_t index ){    if( add ){    RAM.focus( add, index );  }   else{   alloc();    }    }
    inline void read(){                     if( add ){    RAM.read();               }   else{   alloc();    }    }
    inline void write(){                    if( add ){    RAM.write();              }   else{   alloc();    }    }
    
};

#endif


#ifndef _uint32_t_h
#define _uint32_t_h

class _uint32_t {
  public:
    _uint32_t();
    ~_uint32_t();

    bool alloc( uint16_t q=0 );
    void del();

    _uint32_t &operator=(  const _uint32_t& val );
    _uint32_t &operator=(  const  uint32_t  val );
    _uint32_t &operator+=( const  uint32_t  val );
    _uint32_t &operator-=( const  uint32_t  val );
    _uint32_t &operator++();
    _uint32_t &operator--();
    _uint32_t &operator/=( const  uint32_t  val );
    _uint32_t &operator*=( const  uint32_t  val );
    _uint32_t &operator[]( const  uint16_t  tabIndex );

    operator uint32_t() const{ return( get() ); }
    uint32_t get();
    
  private:
    uint16_t add=0;

    inline void focus(){                    if( add ){    RAM.focus( add );         }   else{   alloc();    }    }
    inline void focus( uint16_t index ){    if( add ){    RAM.focus( add, index );  }   else{   alloc();    }    }
    inline void read(){                     if( add ){    RAM.read();               }   else{   alloc();    }    }
    inline void write(){                    if( add ){    RAM.write();              }   else{   alloc();    }    }
    
};

#endif


/*

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

#ifndef _double_h
#define _double_h

class _double {
  public:
    _double();
    ~_double();

    bool alloc( uint16_t q=0 );
    void del();

    _double &operator=(  const _double&   val );
    _double &operator=(  const  double    val );
    _double &operator+=( const  double    val );
    _double &operator-=( const  double    val );
    _double &operator++();
    _double &operator--();
    _double &operator/=( const  double    val );
    _double &operator*=( const  double    val );
    _double &operator[]( const  uint16_t  tabIndex );

    operator double() const{ return( get() ); }
    double get();
    

  private:
    uint16_t add=0;

    inline void focus(){                    if( add ){    RAM.focus( add );         }   else{   alloc();    }    }
    inline void focus( uint16_t index ){    if( add ){    RAM.focus( add, index );  }   else{   alloc();    }    }
    inline void read(){                     if( add ){    RAM.read();               }   else{   alloc();    }    }
    inline void write(){                    if( add ){    RAM.write();              }   else{   alloc();    }    }
    
};

#endif



#ifndef _vars_t_h
#define _vars_t_h

class _vars_t {
  public:
    _vars_t();
    ~_vars_t();

    bool alloc( uint16_t q=0 );
    void del();

    _vars_t &operator=(  const _vars_t& val );
    _vars_t &operator=(  const  uint16_t  val );
    _vars_t &operator+=( const  uint16_t  val );
    _vars_t &operator-=( const  uint16_t  val );
    _vars_t &operator++();
    _vars_t &operator--();
    _vars_t &operator/=( const  uint16_t  val );
    _vars_t &operator*=( const  uint16_t  val );
    _vars_t &operator[]( const  uint16_t  tabIndex );

    

  private:
    uint16_t add=0;

    inline void focus(){                    if( add ){    RAM.focus( add );         }   else{   alloc();    }    }
    inline void focus( uint16_t index ){    if( add ){    RAM.focus( add, index );  }   else{   alloc();    }    }
    inline void read(){                     if( add ){    RAM.read();               }   else{   alloc();    }    }
    inline void write(){                    if( add ){    RAM.write();              }   else{   alloc();    }    }
    
};

#endif
