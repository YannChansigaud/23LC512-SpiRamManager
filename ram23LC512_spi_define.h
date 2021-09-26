#define RamSPI_read                 B00000011
#define RamSPI_write                B00000010
#define RamSPI_modeGet              B00000101
#define RamSPI_modeSet              B00000001

#define _RAM_CS_PIN_        10
#define _RAM_FREQUENCY_     4000000
#define _RAM_CONFIG_        SPI.beginTransaction( SPISettings( _RAM_FREQUENCY_, MSBFIRST, SPI_MODE0 ) );    digitalWrite( cs, LOW );    SPI.transfer( RamSPI_modeSet );         Serial.print( "cs conf " );
#define _RAM_READ_          SPI.beginTransaction( SPISettings( _RAM_FREQUENCY_, MSBFIRST, SPI_MODE0 ) );    digitalWrite( cs, LOW );    SPI.transfer( RamSPI_read    );         Serial.print( "cs read  " );
#define _RAM_WRITE_         SPI.beginTransaction( SPISettings( _RAM_FREQUENCY_, MSBFIRST, SPI_MODE0 ) );    digitalWrite( cs, LOW );    SPI.transfer( RamSPI_write   );         Serial.print( "cs write " );
#define _RAM_RELEASE_                                                                                       digitalWrite( cs, HIGH );   SPI.endTransaction();     delay(1);     Serial.println( "  release" );
#define _RAM_ADDRESS(a)         SPI.transfer16( a );    Serial.print( "add16" ); 
#define _RAM_SEND(a)            SPI.transfer16( a );    Serial.print( " send16:" );
#define _RAM_SEND8(a)           SPI.transfer( a );      Serial.print( "  send8:" );
#define _RAM_GET(a)         a = SPI.transfer16( 0 );    Serial.print( "  get16:" );
#define _RAM_GET8(a)        a = SPI.transfer( 0 );      Serial.print( "   get8:" );

#define RamSPI_byteMode             B00000000
#define RamSPI_pageMode             B10000000
#define RamSPI_sequentialMode       B01000000

#define RamSPI_type8bit             B00000000
#define RamSPI_type16bit            B00000001
#define RamSPI_type32bit            B00000010
#define RamSPI_type64bit            B00000011
#define RamSPI_type_mask            B00001111
#define RamSPI_size_mask            B00000011
#define RamSPI_count_mask           0x0FFF

#define RamSPI_uint8_t              B00000000
#define RamSPI_uint16_t             B00000001
#define RamSPI_uint32_t             B00000010
#define RamSPI_uint64_t             B00000011
#define RamSPI_int8_t               B00000100
#define RamSPI_int16_t              B00000101
#define RamSPI_int32_t              B00000110
#define RamSPI_int64_t              B00000111
#define RamSPI_float_double         B00001011

#define sectorTableStartAdd           0x0000
#define sectorTableEndAdd             0x07FF
#define dataStartAdd                  0x0800

#define sectorCount_mask              0x0FFF

//      VarsDeclaration             0xTCCC    bin : TTTT CCCC CCCC CCCC
//                              T : type
//                            CCC : COUNT max 4096

//                        type    : SxWW    S = signed    WW = wide : 01 = 1 octet    10 = 2 octets   11 = 4 octets
//                                : 0000    free sector
//                                : 0001    uint16_t    // 2 octects
//                                : 0010    uint32_t    // 4 octects
//                                : 0011    uint64_t    // 8 octects
//                                : 0101    int16_t     // 2 octects
//                                : 0110    int32_t     // 4 octects
//                                : 0111    int64_t     // 8 octects
//                                : 
//                                : 1000    float       // 4 octects
//                                : 1000    double      // 4 octects
//                                : 

//      VarsContruction           : 0xTCCC      // type declaration
//                                : 0xIIII      // index
//                                : 0xDDDD      // data
//                                : 0xDDDD      // data
//                                : 0xDDDD      // data
//                                : ....        // ...


#define dataSend16    send( 2 );
#define dataGet16     get(  2 );

#define buffer8       buffer._8[0]
#define bufferN8(a)   buffer._8[a]
#define verif8        verif._8[0]
#define verifN8(a)    verif._8[a]

#define buffer16      buffer._16[0]
#define bufferN16(a)  buffer._16[a]
#define verif16       verif._16[0]
#define verifN16(a)   verif._16[a]