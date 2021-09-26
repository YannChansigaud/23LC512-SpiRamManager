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

class RAM23LC512SPI{
  public:
    RAM23LC512SPI();
    ~RAM23LC512SPI();

    uint16_t ramAdd = 0;
    union{ uint64_t _64 ; uint32_t _32[2] ; uint16_t _16[4] ; uint8_t _8[8] ; } buffer, verif;

    void begin( uint8_t _cs );
    void focus( uint16_t add, uint16_t index=0 );

    // 16 bits
    uint16_t new16( uint16_t val=0 );
        void del16( uint16_t add );
/*
    // 32 bits
    uint16_t new32( uint32_t val=0 );
        void set32( uint16_t add, uint32_t val );
    // 64 bits
    uint16_t new64( uint64_t val=0 );
    uint64_t get64( uint16_t add );
        void set64( uint16_t add, uint64_t val );
    
    // double
    uint16_t newDouble(   double val=0 );
      double getDouble( uint16_t add );
        void setDouble( uint16_t add, double val );
    
    // float
    uint16_t newFloat(    float val=0 );
       float getFloat( uint16_t add );
        void setFloat( uint16_t add, float val );
*/
    // array 16
    uint16_t newArray16( uint16_t q );


    void write16();
    void read16();

  private:
    
    // =================================================
    // ====  MEMORY  ===================================
    // =================================================

    void alloc( uint16_t type, uint16_t q=0 );
    void free();

    // =================================================
    // ===  SPI  =======================================
    // =================================================

    uint8_t cs = 0;
    void send( uint8_t q );
    void get(  uint8_t q );
    
    // =================================================
    // ===  MFT  =======================================
    // =================================================

    uint8_t  sector      = 0;
    uint8_t  sectorBit   = 0;
    uint16_t sectorAdd   = 0;
    uint16_t sectorCount = 0;
    uint16_t farthestSectorLocked = 0;

    uint8_t   raw8 = 0;
    uint16_t raw16 = 0;

    void searchFreeSpace( uint16_t spaceNeeded );
    void sectorTable( uint16_t count, bool lock );
    void setSectorTable();
    void putSectorTable();
    void getSectorTable();

    void setDoubleHelper( double val );
    double getDoubleHelper();

    // =================================================
    // ===  SERIAL  ====================================
    // =================================================

    void printData16();
    void printSector();

};

#endif

#ifndef extRamDef
#define extRamDef
static RAM23LC512SPI extRam = RAM23LC512SPI();
#endif
