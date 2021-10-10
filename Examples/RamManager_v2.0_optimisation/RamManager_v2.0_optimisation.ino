#define RamSPI_cs 10

#include "ram23LC512_spi.h"
#include "ram23LC512_typedef.h"


  
  _uint16_t varA;
  _uint16_t varB;
  _double   varC;


void setup() {
  Serial.begin(1000000);
  Serial.println( "debut setup" );  
  SPI.begin();
  RAM.begin();

  varA.alloc();
  varB.alloc();
  varC.alloc();

  varA = 12;
  varB = 5;
  varC = 5.7;
  Serial.println( varA );
  varA /= 2;
  Serial.println( varA );
  varA += varB;
  Serial.println( varA );
  varC += varB;
  Serial.println( varC );
  varA += varC;
  Serial.println( varA );
  Serial.println( "fin setup" );
}

void loop() {

}

  
