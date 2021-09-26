/**********************************************************************
 * 
 * This is the C++ part of the Ram library.
 * See RAM23LC512SPI for a full documentation.
 * 
 *********************************************************************/


#include "ram23LC512_spi.h"

// ==================================================================== //
//                                                                      //
//                         16 BITS                                      //
//                                                                      //
// ==================================================================== // 
uint16_t RAM23LC512SPI::new16( uint16_t val ){                 alloc( RamSPI_uint16_t );   buffer16 = val;     write16();    return( ramAdd );     }
void     RAM23LC512SPI::del16( uint16_t add ){                                               ramAdd = add;     free();                             }
/*
// ==================================================================== //
//                                                                      //
//                         32 BITS                                      //
//                                                                      //
// ==================================================================== // 
uint16_t RAM23LC512SPI::new32( uint32_t val ){                 uint16_t add = alloc( RamSPI_uint32_t );          VAR.put32( val );   return( add );       }
uint32_t RAM23LC512SPI::get32( uint16_t add ){                                focus( add );              raw32 = VAR.get32();        return( raw32 );     }
void     RAM23LC512SPI::set32( uint16_t add, uint32_t val ){                  focus( add );                      VAR.put32( val );                        }

// ==================================================================== //
//                                                                      //
//                         64 BITS                                      //
//                                                                      //
// ==================================================================== // 
uint16_t RAM23LC512SPI::new64( uint64_t val ){                 uint16_t add = alloc( RamSPI_uint64_t );          VAR.put64( val );   return( add );       }
uint64_t RAM23LC512SPI::get64( uint16_t add ){                                focus( add );              raw64 = VAR.get64();        return( raw64 );     }
void     RAM23LC512SPI::set64( uint16_t add, uint64_t val ){                  focus( add );                      VAR.put64( val );                        }
*/
// ==================================================================== //
//                                                                      //
//                         DOUBLE                                       //
//                                                                      //
// ==================================================================== // 
/*
uint16_t RAM23LC512SPI::newDouble( double val ){
  alloc( RamSPI_float_double );
  uint16_t add = RamSPI_dataAdd;
  focus( add );
  setDoubleHelper( val );
  return( add );
}

double RAM23LC512SPI::getDouble( uint16_t add ){
  focus( add );
  return( getDoubleHelper() );
}

void RAM23LC512SPI::setDouble( uint16_t add, double val ){
  focus( add );
  setDoubleHelper( val );
}

// ==================================================================== //
//                                                                      //
//                         FLOAT                                        //
//                                                                      //
// ==================================================================== // 
uint16_t RAM23LC512SPI::newFloat( float val ){
  alloc( RamSPI_float_double );
  uint16_t add = RamSPI_dataAdd;
  focus( add );
  setDoubleHelper( val );
  return( add );
}

float RAM23LC512SPI::getFloat( uint16_t add ){
  focus( add );
  return( getDoubleHelper() );
}

void RAM23LC512SPI::setFloat( uint16_t add, float val ){
  focus( add );
  setDoubleHelper( val );
}

// ==================================================================== //
//                                                                      //
//                         ARRAY 16                                     //
//                                                                      //
// ==================================================================== // 

uint16_t RAM23LC512SPI::newArray16( uint16_t q ){                          // 
  alloc( RamSPI_uint16_t, q );                                          // on alloue une quantité q de type uint16_t
  return( ramAdd );                                                     // on retourne l'adresse du point d'entrée
}
*/
// ==================================================================== //
//                                                                      //
//                         MEMORY                                       //
//                                                                      //
// ==================================================================== // 

void RAM23LC512SPI::focus( uint16_t add, uint16_t index ){              // 
  ramAdd = add;                                                         // on se positionne
  read16();                                                             // on récupère sa déclaration
  uint8_t size = (buffer16 >> 12) & RamSPI_size_mask ;                  // on isole la taille de chaque élément
  buffer16 &= RamSPI_count_mask;                                        // on isole la quantité déclarée
  ramAdd += 2;                                                          // on se place sur l'index
  if( index && buffer16 ){                                              // si un index a été indiqué et qu'une quantité a été déclarée
    buffer16 = index;                                                   //   on initialise l'index demandé
    write16();                                                          //   on mémorise l'index pour la prochaine lecture
  }                                                                     // 
  else if( buffer16 ){                                                  // si aucun index indiqué mais quantité déclarée
    read16();                                                           //   on récupère l'index positionnée précédement
  }                                                                     //   
  ramAdd += 2;                                                          // on se positionne sur le 1er éléménet
  ramAdd += buffer16 << size;                                           // on se décalle en mémoire suivant index
}

void RAM23LC512SPI::free(){                                             // libère l'espace add
  read16();                                                             // on récupère sa déclaration
  uint8_t size = (buffer16 >> 12) & RamSPI_size_mask ;                  // on isole la taille de chaque élément
  buffer16 &= RamSPI_count_mask ;                                       // on isole la quantité déclarée
  buffer16 <<= size;                                                    // on calcule l'espace occupé suivant la taille déclarée de chaque élément
  sectorTable( buffer16+2, false );                                     // on libère la taille totale (+2 pour la zone de déclaration et index)
}                                                                       //

void RAM23LC512SPI::alloc( uint16_t type, uint16_t q ){                 // on recherche un emplacement d'une quantité q de type
  Serial.println( "========= ram alloc =========" );
  uint16_t count = 4;                                                   // espace nécessaire : +1 pour le typer/quantifier +1 pour l'index
  if( q ){                                                              // si une quantité de variable a été spécifiée : on défini alors un tableau
    count += (q << (type & RamSPI_size_mask));                          //   on calcul l'espace nécessaire 
  }                                                                     // 
  else{                                                                 // si la quantité de variable est à 0 : on défini une variable simple
    count += (1 << (type & RamSPI_size_mask));                          //   on calcul l'espace nécessaire
  }                                                                     // 
  Serial.print( "look for " );  Serial.println( count );
  searchFreeSpace( count );                                             // on recherche un emplacement adhéquat
  sectorTable( count, true );                                           // on verrouille
  buffer16 = type << 12 | q;                                            // on défini sa déclaration
  write16();                                                            // on enregistre la déclaration
  Serial.print( "fount at " );  Serial.println( ramAdd );
  Serial.println( "=============================" );
}


// ==================================================================== //
//                                                                      //
//                         ROUTINE                                      //
//                                                                      //
// ==================================================================== // 
/*
void RAM23LC512SPI::setDoubleHelper( double val ){
  raw32 = val;                      // on stock la partie entière
  raw64 = raw32;                    // 
  raw64 <<= 32;                     // 
  val -= raw32;                     // on retranche la partie entière à val
  val *= 1000000000;                // on multiplie par 1 milliards pour ne garder que les 9 chiffres après la virgule
  raw32 = val;                      // on stock les 9 décimales
  raw64 |= raw32;                   // 
  write64();                        // on mémorise
}

double RAM23LC512SPI::getDoubleHelper(){
  read64();                         // on récupère l'intégralité
  raw32 = raw64;                    // on isole la partie décimale
  raw64 >>= 32;                     // on isole la partie entière
  double val = raw64;               // on place la partie entière dans la variable double
  val += (raw32 / 1000000000.0);    // on rajoute la partie décimale
  return( val );                    // on renvoie le double
}
*/
// ==================================================================== //
//                                                                      //
//                         CONSTRUCTEUR / DESTRUCTEUR                   //
//                                                                      //
// ==================================================================== // 

RAM23LC512SPI::RAM23LC512SPI(){}
RAM23LC512SPI::~RAM23LC512SPI(){}

// ==================================================================== //
//                                                                      //
//                         MFT                                          //
//       Attention chaque bit désigne un emplacement de 1 octets        //
// ==================================================================== //

void RAM23LC512SPI::searchFreeSpace( uint16_t spaceNeeded ){        // on recherche n espace libre. 
  Serial.println( "======= MFT alloc =======" );                    // 
  sectorAdd = sectorTableStartAdd;                                  // on se place au début de la table sector
  sectorBit = 0;                                                    // position binaire dans la table secteur
  uint16_t count = 0;                                               // compteur d'espace contigus
  while( count < spaceNeeded ){                                     // pour chaque octect de tableSector
    getSectorTable();                                               //   on récupère sa valeur
    while( sectorBit < 8 && count < spaceNeeded ){                  //   pour chaque bit du secteur récupéré
      if( count == 0 ){                                             //     si le compteur est à zéro 0, c'est l'adresse initiale
        ramAdd = dataStartAdd + (sectorAdd << 3) + sectorBit ;      //       on met alors l'adresse de départ de coté
      }                                                             // 
      if( sector == 0x00 && sectorBit == 0 ){                       //     si le secteur est à zéro et qu'on a pas encore décaler ( B00000001 >> 1 = B00000000 )
        count +=8;                                                  //       le secteur est complètement libre, donc +8 au compteur
        sectorBit = 8;                                              //       on met sectorBit pour sortir directement de la boucle while
      }                                                             // 
      else if( sector &  0x01 ){                                    //     si le secteur n'est pas à zéro mais qu'on rencontre un bit 1
        count = 0;                                                  //       c'est une adresse verrouillée donc retour à zéro
      }                                                             // 
      else{                                                         //     si le secteur n'est pas à zéro mais qu'on rencontre un bit 0
        count++;                                                    //       on rajoute 1 au compteur
      }                                                             // 
      sector >>= 1;                                                 //     on passe au bit suivant
      sectorBit++;                                                  //     
    }                                                               // 
    sectorAdd++;                                                    //   on passe au secteur suivant
    sectorBit = 0;                                                  //   on remet le compteur de bit à zéro
  }                                                                 // 
  Serial.print( "======= MFT alloc = " );                           // 
  Serial.println( ramAdd );                                         // 
}                                                                   // 

void RAM23LC512SPI::sectorTable( uint16_t count, bool lock ){
  Serial.print( "======= MFT sector = " ); 
  Serial.print( lock ? "lock " : "release " );
  Serial.print( count );
  Serial.print( " from " ); 
  Serial.println( ramAdd );

  sectorCount += lock ? count : -count ;
  if( ((ramAdd + count) > farthestSectorLocked) && (!lock) ){
    farthestSectorLocked = ramAdd + count;
  }

  sectorAdd = (ramAdd-dataStartAdd) >> 3;
  sectorBit = (ramAdd-dataStartAdd) % 8;

  getSectorTable();
  while( count-- ){
    if( lock ){  sector |=  ( 1 << sectorBit );  }
    else{        sector &= ~( 1 << sectorBit );  }
    sectorBit++;
    if( sectorBit==8 || count == 0 ){
      setSectorTable();
      if( count ){
        sectorBit=0;
        sectorAdd++;
        getSectorTable();
      }
    }
  }
}
void RAM23LC512SPI::setSectorTable(){
  uint8_t nbTry  = 0; 
  uint8_t backup = sector;
  putSectorTable();
  getSectorTable();
  while( (backup != sector) && (nbTry < 10) ){
    nbTry++;
    Serial.print( "try " );
    Serial.print( nbTry );
    Serial.print( " " );
    sector = backup;
    putSectorTable();
    getSectorTable();
  }
}
void RAM23LC512SPI::putSectorTable(){
  Serial.print( "set " );  printSector();
  _RAM_WRITE_
  _RAM_ADDRESS( sectorAdd )
  _RAM_SEND8( sector )
  _RAM_RELEASE_
}
void RAM23LC512SPI::getSectorTable(){
  _RAM_READ_
  _RAM_ADDRESS( sectorAdd )
  _RAM_GET8( sector )
  _RAM_RELEASE_
  Serial.print( "get " );  printSector();
}

// ==================================================================== //
//                                                                      //
//                         INITIALISATION SPI                           //
//                                                                      //
// ==================================================================== // 

void RAM23LC512SPI::begin( uint8_t _cs ){
  cs = _cs;
  pinMode( cs, OUTPUT );
  _RAM_CONFIG_
  _RAM_SEND8( RamSPI_sequentialMode );
  _RAM_RELEASE_
}


void RAM23LC512SPI::send( uint8_t q ){
  Serial.print( "send " );    Serial.println( q );
  _RAM_WRITE_                                 // on ouvre le canal en écriture
  _RAM_ADDRESS( ramAdd )                      // on envoi l'adresse
  while( q-- ){                               // pour chaque paquet de 8 bits
    _RAM_SEND8( bufferN8(q) )                 //   on l'envoi 8 bits q fois
    Serial.print( q );
  }                                           //
  _RAM_RELEASE_                               // on libère le canal
  Serial.print( "at   " );   printData16();
}

void RAM23LC512SPI::get( uint8_t q ){
  Serial.print( "get  " );    Serial.println( q );
  _RAM_READ_                                  // on ouvre le canal en écriture
  _RAM_ADDRESS( ramAdd )                      // on envoi l'adresse
  while( q-- ){                               // pour chaque paquet de 8 bits
    _RAM_GET8( bufferN8(q) )                  //   on récupère 8 bits q fois
    Serial.print( q );
  }                                           // 
  _RAM_RELEASE_                               // on libère le canal
  Serial.print( "from " );   printData16();
}


// ==================================================================== //
//                                                                      //
//                         ROUTINE PUBLIQUE                             //
//                                                                      //
// ==================================================================== // 


void RAM23LC512SPI::write16(){
  uint8_t nbTry = 0;                                        // essai max 10
  verif16 = buffer16;                                       // on sauvegarde la valeur
  dataSend16                                                // on envoi 16 bit
  dataGet16                                                 // on récupère 16 bits pour vérification
  while( (verif16 != buffer16) && nbTry < 10 ){             // 
    nbTry++;
    Serial.print( "try " );
    Serial.print( nbTry );
    Serial.print( " " );
    buffer16 = verif16;                                     // 
    dataSend16                                              // 
    dataGet16                                               // 
  }                                                         // 
}
void RAM23LC512SPI::read16(){
  dataGet16
}

void RAM23LC512SPI::printData16(){
  Serial.print( "add = " );
  uint8_t i = 16;
  uint16_t tmp = ramAdd;
  while( i-- ){
    Serial.print( (tmp & 0x8000) ? '1' : '0' );
    tmp <<= 1;
  }
  
  Serial.print( " data = " );
  i = 16;
  tmp = buffer16;
  while( i-- ){
    Serial.print( (tmp & 0x8000) ? '1' : '0' );
    tmp <<= 1;
  }

  Serial.println();
}

void RAM23LC512SPI::printSector(){
  Serial.print( "sector Add = " );
  uint8_t i = 16;
  uint16_t tmp = sectorAdd;
  while( i-- ){
    Serial.print( (tmp & 0x8000) ? '1' : '0' );
    tmp <<= 1;
  }
  
  Serial.print( " status = " );
  i = 8;
  tmp = sector;
  while( i-- ){
    Serial.print( (tmp & 0x80) ? '1' : '0' );
    tmp <<= 1;
  }

  Serial.println();
}
