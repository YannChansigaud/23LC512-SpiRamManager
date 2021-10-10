/* RAM manager library version 1.0
# 
# Writed by Chansigaud Yann
# RAM23LC512SPI.h - ram library for 23LC512 from MicroShip
# 
*/



#ifndef RAM23LC512_h
#define RAM23LC512_h
#include <inttypes.h>
#include <SPI.h>

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "ram23LC512_spi_define.h"

// =================================================
// ===  TYPE DEF  ==================================
// =================================================

#if defined(ARDUINO_SAM_DUE)    
    typedef union{ uint64_t _64[1] ;   double dbl[1] ; uint32_t _32[2] ; uint16_t _16[4] ; uint8_t _8[8] ; }  unifiedBuffer;
#else
    typedef union{ uint64_t _64[1] ;   double dbl[2] ; uint32_t _32[2] ; uint16_t _16[4] ; uint8_t _8[8] ; }  unifiedBuffer;
#endif

typedef struct{
    uint32_t
            type: 4,
           count: 12,
           index: 16;
} File_structure; 

typedef union{ uint32_t definition ; struct{ uint16_t varField ; uint16_t varIndex ; }; File_structure prop ; }  DECLARATION;

// =================================================
// === CLASS  ======================================
// =================================================

class RAM23LC512SPI{
  public:

    static void begin();

    static unifiedBuffer bufferA;
    static unifiedBuffer bufferB;
    static void focus( uint16_t add );
    static void focus( uint16_t add, uint16_t index );

    static uint16_t allocNewVar( uint8_t type, uint16_t qte=0 );
    static void     del();

    static void write();
    static void read( bool part=false );
    static void read_partA();
    static void read_partB();

    static uint8_t varType();
  
    static uint16_t freeSpace();

    // =================================================
    // ===  MATH  ======================================
    // =================================================
    static void doMath( uint8_t op );

    template <typename T> static void doMath( uint8_t op, T val ){
      if( varsA.prop.type < RamSPI_userDefined ){
        switch( op ){
          case opCode_affectation     :{
            if( varsA.prop.type == RamSPI_float_double ){   varsA_DBL = val;    }
            else{                                           varsA_64  = val;    }
            break;
          }
          case opCode_addition        :{
            if( varsA.prop.type == RamSPI_float_double ){   varsA_DBL += val;    }
            else{                                           varsA_64  += val;    }
            break;
          }
          case opCode_soustraction    :{
            if( varsA.prop.type == RamSPI_float_double ){   varsA_DBL -= val;    }
            else{                                           varsA_64  -= val;    }
            break;
          }
          case opCode_multiplication  :{
            if( varsA.prop.type == RamSPI_float_double ){   varsA_DBL *= val;    }
            else{                                           varsA_64  *= val;    }
            break;
          }
          case opCode_division        :{
            if( varsA.prop.type == RamSPI_float_double ){   varsA_DBL /= val;    }
            else{                                           varsA_64  /= val;    }
            break;
          }
          case opCode_incrementation  :{
            if( varsA.prop.type == RamSPI_float_double ){   varsA_DBL ++;    }
            else{                                           varsA_64  ++;    }
            break;
          }
          case opCode_decrementation  :{
            if( varsA.prop.type == RamSPI_float_double ){   varsA_DBL --;    }
            else{                                           varsA_64  --;    }
            break;
          }
        }
      }
    }

  private:
    
    static bool started();
    // =================================================
    // ===  SPI  =======================================
    // =================================================

    static void send( uint8_t q );
    static void get(  uint8_t q );
    
    static uint8_t cs;

    // =================================================
    // ====  MEMORY  ===================================
    // =================================================

    static uint16_t ramAdd;         // désigne les adresses de 8 bits en ram 
    
    static uint32_t    size;        // désigne le nombre de bloc de 16 bits nécessaire
    static void        setVarsSize();
    static void        setOverallSize();
    static DECLARATION varsA;
    static DECLARATION varsB;
    
    static void alloc( uint16_t type, uint16_t q=0 );
    static void free();

    static void setVar( bool alreadySet=true );
    static void putVar( bool alreadySet );
    static void getVar();

#ifdef RAM_verif
    static void backupValue();
    static void retrieveValue();
    static bool checkValue();
#endif
    // =================================================
    // ===  MFT  =======================================
    // =================================================

    static uint8_t  sector;
    static uint8_t  sectorBit;
    static uint16_t sectorAdd;
    static uint16_t sectorCount;

    static void searchFreeSpace();
    static void format();
    static void sectorTable( bool lock );
    static void setSectorTable();
    static void putSectorTable();
    static void getSectorTable();

    static void setDoubleHelper();
    static void getDoubleHelper();


};

extern RAM23LC512SPI RAM ;

#endif