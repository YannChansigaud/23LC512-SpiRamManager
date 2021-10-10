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


#ifndef ram_vars_h
#define ram_vars_h

class ram_vars {
  public:
    ram_vars(){
      
    };

    ~ram_vars(){
      del();
    }
    
// =========================================================== VARS_A and VARS_B manipulation ===============
    ram_vars &operator=(  const ram_vars &val ){
      val.get();
      focus();
      write();
      return( *this );
    }
    

// =========================================================== VARS_A with std vars type manipulation ===============
    template <typename T>   ram_vars &operator=(   const T val ){
      focus();
      read();
      RAM.doMath( opCode_affectation, val );
      write();
      return( *this );
    }

    template <typename T>   ram_vars &operator+=(  const T val ){
      focus();
      read();
      RAM.doMath( opCode_addition, val );
      write();
      return( *this );
    }

    template <typename T>   ram_vars &operator-=(  const T val ){
      focus();
      read();
      RAM.doMath( opCode_soustraction, val );
      write();
      return( *this );
    }

    template <typename T>   ram_vars &operator*=(  const T val ){
      focus();
      read();
      RAM.doMath( opCode_multiplication, val );
      write();
      return( *this );
    }

    template <typename T>   ram_vars &operator/=(  const T val ){
      focus();
      read();
      RAM.doMath( opCode_division, val );
      write();
      return( *this );
    }

    ram_vars &operator++(){
      focus();
      read();
      RAM.doMath( opCode_incrementation, 1 );
      write();
      return( *this );
    }

    ram_vars &operator--(){
      focus();
      read();
      RAM.doMath( opCode_decrementation, 1 );
      write();
      return( *this );
    }

    ram_vars &operator[]( const uint16_t index ){
      focus( index );
      return( *this );
    }

    void get(){
      focus();
      read();
    }


  protected:
    uint16_t add=0;

    void focus(){
      if( add ){
        RAM.focus( add );
      }
    }
    void focus( uint16_t index ){
      if( add ){
        RAM.focus( add, index );
      }
    }
    void read(){
      if( add ){
        RAM.read_partB();
      }
    }
    void write(){
      if( add ){
        RAM.write();
      }
    }

    bool alloc( uint8_t type, uint16_t q=0 ){
      if( add == 0 ){
        add = RAM.allocNewVar( type, q );
        return( add != 0 );
      }
      return( false );
    }
    void del(){
      focus( 0 );
      RAM.del();
      add = 0;
    }
};

#endif
