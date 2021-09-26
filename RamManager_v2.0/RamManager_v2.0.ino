#include "ram23LC512_spi.h"
#include "ram23LC512_typedef.h"

  _uint16_t varA   = _uint16_t();
  _uint16_t varB   = _uint16_t();
//  _uint16_t arrayA = _uint16_t();

  uint16_t varAdd = 0;
  uint16_t value  = 0;
  uint16_t ram    = 0;

  uint32_t error = 0;
  uint32_t count = 0;
  uint32_t aff   = 0;

void setup() {
  Serial.begin(1000000);
  Serial.println( "debut setup" );
  SPI.begin();
  
  extRam.begin( 10 );
  delay(100);

/*
  varA.alloc();
  varB.alloc();
/*  
  arrayA.alloc( 20 );

  varA = 5;   Serial.println( varA.get() );   varA += 5;    Serial.println( varA.get() );
  varB = 5;   Serial.println( varB.get() );   varB = varA;  Serial.println( varB.get() );   varB++;   Serial.println( varB.get() );  
  
  arrayA[3] = 17;
  Serial.println( arrayA[3].get() );
  
  arrayA[3] = 22;
  Serial.println( arrayA[3].get() );
  
  */
  Serial.println( " ================================================ " );
  randomTest();
  Serial.println( " ================================================ " );
  varA.alloc();
  Serial.println( " ================================================ " );
  randomTest();
  Serial.println( " ================================================ " );
  Serial.println( "fin setup" );
}

void loop() {
}

void randomTest(){
  Serial.println( "random test :" );
  randomSeed(  analogRead(0) );
  varAdd = random( sectorTableEndAdd+1, 0xFFFF );
  value  = random( 0xFFFF );
  extRam.ramAdd = varAdd;
  extRam.buffer._16[0] = value;
  extRam.write16();
  extRam.buffer._16[0] = 0;
  extRam.read16();
  if( extRam.buffer._16[0] != value ){
    error++;
  }
  count++;
  Serial.print( error );    Serial.print( '/' );  Serial.println( count );
  
}

  
