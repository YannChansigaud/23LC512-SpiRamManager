#define RAM_verif     

#ifndef RamSPI_cs
#define RamSPI_cs 10
#endif

#define RamSPI_read                 B00000011
#define RamSPI_write                B00000010
#define RamSPI_modeGet              B00000101
#define RamSPI_modeSet              B00000001

#define _RAM_FREQUENCY_     4000000
#define _RAM_CONFIG_        SPI.beginTransaction( SPISettings( _RAM_FREQUENCY_, MSBFIRST, SPI_MODE0 ) );    digitalWrite( cs , LOW );    SPI.transfer( RamSPI_modeSet );
#define _RAM_READ_          SPI.beginTransaction( SPISettings( _RAM_FREQUENCY_, MSBFIRST, SPI_MODE0 ) );    digitalWrite( cs , LOW );    SPI.transfer( RamSPI_read    );
#define _RAM_WRITE_         SPI.beginTransaction( SPISettings( _RAM_FREQUENCY_, MSBFIRST, SPI_MODE0 ) );    digitalWrite( cs , LOW );    SPI.transfer( RamSPI_write   );
#define _RAM_RELEASE_                                                                                       digitalWrite( cs , HIGH );   SPI.endTransaction();     delay(1);
#define _RAM_ADDRESS(a)         SPI.transfer16( a );
#define _RAM_SEND(a)            SPI.transfer16( a );
#define _RAM_SEND8(a)           SPI.transfer(   a );
#define _RAM_GET(a)         a = SPI.transfer16( 0 );
#define _RAM_GET8(a)        a = SPI.transfer(   0 );

#define RamSPI_byteMode             B00000000
#define RamSPI_pageMode             B10000000
#define RamSPI_sequentialMode       B01000000

#define RamSPI_free                 B00000000
#define RamSPI_type_mask            B00001111
#define RamSPI_size_mask            B00000011
#define RamSPI_count_mask           0x0FFF

#define RamSPI_uint16_t             B00000001
#define RamSPI_uint32_t             B00000010
#define RamSPI_uint64_t             B00000011
#define RamSPI_int16_t              B00000101
#define RamSPI_int32_t              B00000110
#define RamSPI_int64_t              B00000111
#define RamSPI_float_double         B00001010
#define RamSPI_userDefined          B00001100

#define sectorTableStartAdd           0x0000
#define sectorTableEndAdd             0x0FFF
#define dataStartAdd                  0x1000

//      VarsDeclaration             0xTCCC    bin : TTTT CCCC CCCC CCCC
//                              T : type
//                            CCC : COUNT max 4096

//                        type    : SxWW    S = signed    WW = wide : 01 = 1 octet    10 = 2 octets   11 = 4 octets
//                                : 0000    free sector
//                                : 0001    uint16_t    // 2 octets
//                                : 0010    uint32_t    // 4 octets
//                                : 0011    uint64_t    // 8 octets
//                                : 0101    int16_t     // 2 octets
//                                : 0110    int32_t     // 4 octets
//                                : 0111    int64_t     // 8 octets
//                                : 
//                                : 1010    float       // 4 octets    8 octets on arduino due
//                                : 1110    double      // 4 octets    8 octets on arduino due
//                                : 

//      VarsContruction           : 0xTCCC      // type declaration
//                                : 0xIIII      // index
//                                : 0xDDDD      // data
//                                : 0xDDDD      // data
//                                : 0xDDDD      // data
//                                : ....        // ...


#define buffer8       bufferA._8[0]
#define bufferN8(a)   bufferA._8[a]
#define verif8        bufferB._8[0]
#define verifN8(a)    bufferB._8[a]

#define buffer16      bufferA._16[0]
#define bufferN16(a)  bufferA._16[a]
#define verif16       bufferB._16[0]
#define verifN16(a)   bufferB._16[a]

#define buffer32      bufferA._32[0]
#define bufferN32(a)  bufferA._32[a]
#define verif32       bufferB._32[0]
#define verifN32(a)   bufferB._32[a]

#define bufferDBL     bufferA.dbl[0]
#define verifDBL      bufferB.dbl[0]

#define buffer64      bufferA._64[0]
#define verif64       bufferB._64[0]

#define newVar    true
#define newIndex  false

#define lockdown  true
#define release   false


#define opCode_affectation      1
#define opCode_addition         2
#define opCode_soustraction     3
#define opCode_multiplication   4
#define opCode_division         5
#define opCode_incrementation   6
#define opCode_decrementation   7

#define opType_notDefined   0
#define opType_int_on_dbl   1
#define opType_dbl_on_int   2
#define opType_int_on_int   3
#define opType_dbl_on_dbl   4

#define varsA_read    false
#define varsB_read    true

#define varsA_8       bufferA._8[0]
#define varsA_16      bufferA._16[0]
#define varsA_32      bufferA._32[0]
#define varsA_64      bufferA._64[0]
#define varsA_DBL     bufferA.dbl[0]

#define varsB_8       bufferB._8[0]
#define varsB_16      bufferB._16[0]
#define varsB_32      bufferB._32[0]
#define varsB_64      bufferB._64[0]
#define varsB_DBL     bufferB.dbl[0]