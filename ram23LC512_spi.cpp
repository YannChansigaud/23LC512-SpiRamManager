/**********************************************************************
 * 
 * This is the C++ part of the Ram library.
 * See RAM23LC512SPI for a full documentation.
 * 
 *********************************************************************/


#include "ram23LC512_spi.h"

RAM23LC512SPI RAM;
//                                               ---------------- // -- SIMPLE -- | --- VERIF ---
//    SPI                                                         // ---- 2 ----- | --- 2 -------
uint8_t  RAM23LC512SPI::cs                   = 0 ;                //      1       |     1
bool     RAM23LC512SPI::started              = false ;            //      1       |     1

//    SECTOR                                     ---------------- // ---- 8 ----- | --- 8 -------
uint16_t RAM23LC512SPI::ramAdd               = 0 ;                //      2       |     2
uint8_t  RAM23LC512SPI::sector               = 0 ;                //      1       |     1
uint8_t  RAM23LC512SPI::sectorBit            = 0 ;                //      1       |     1
uint16_t RAM23LC512SPI::sectorAdd            = 0 ;                //      2       |     2
uint16_t RAM23LC512SPI::sectorCount          = dataStartAdd ;     //      2       |     2

//    BUFFER                                     ---------------- // ---- 8 ----- | --- 16 ------
unifiedBuffer RAM23LC512SPI::buffer          = { 0 } ;            //      8       |     8
#ifdef RAM_verif
unifiedBuffer RAM23LC512SPI::verif           = { 0 } ;            //      -       |     8
#endif
//    VARS                                       ---------------- // ---- 8 ----- | --- 12 ------
uint32_t    RAM23LC512SPI::size              = 0 ;                //      4       |     4
DECLARATION RAM23LC512SPI::vars              = { 0 } ;            //      4       |     4
#ifdef RAM_verif
DECLARATION RAM23LC512SPI::backup            = { 0 } ;            //      -       |     4
#endif
//                                                       TOTAL    // ==== 26 ==== | === 38 ===== 


// ==================================================================== //
//                                                                      //
//                         DYNAMIC ALLOCATION                           //
//                                                                      //
// ==================================================================== // 

uint16_t RAM23LC512SPI::allocNewVar( uint8_t type, uint16_t q ){
  alloc( type, q );
  return( ramAdd );
}
void     RAM23LC512SPI::del(){
  if( ramAdd != 0 ){
    free();
  }
}


// ==================================================================== //
//                                                                      //
//                         MEMORY                                       //
//                                                                      //
// ==================================================================== // 

void RAM23LC512SPI::focus( uint16_t add ){                              // 
  ramAdd = add;                                                         // on se positionne
  getVar();                                                             // on récupère sa déclaration et son index
  setVarsSize();                                                        // on détermine la taille de chaque élément
  ramAdd += ( vars.prop.type == RamSPI_userDefined ) ? 6 : 4;           // on se place sur le 1er index 
  ramAdd += vars.prop.index * size ;                                    // on décale la position en ram de la valeur de index/taille
}                                                                       // 

void RAM23LC512SPI::focus( uint16_t add, uint16_t index ){              // 
  ramAdd = add;                                                         // on se positionne
  getVar();                                                             // on récupère sa déclaration et son index
  index &= RamSPI_count_mask ;                                          // si l'index demandé dépasse la capacité du tableau, on reboucle à zéro
  index = ( index >= vars.prop.count ) ? 0 : index ;                    // si l'index demandé est supérieur à la quantité déclarée, on le met à 0
  vars.prop.index = index;                                              // on indique l'index demandé
  setVar( newIndex );                                                   // on le mémorise pour la prochaine lecture
}

void RAM23LC512SPI::free(){                                             // libère l'espace add
  setOverallSize();                                                     // on détermine la place totale occupée
  sectorTable( release );                                               // on libère la taille totale
}                                                                       //

void RAM23LC512SPI::format(){
  uint32_t count = size;
  _RAM_WRITE_
  _RAM_ADDRESS( ramAdd+4 )
  while( count-- ){
    _RAM_SEND( 0 )
  }
  _RAM_RELEASE_
}

void RAM23LC512SPI::alloc( uint16_t type, uint16_t q ){                 // on recherche un emplacement d'une quantité q de type
  q &= RamSPI_count_mask ;                                              // on limite la quantité d'unitée
  vars.prop.type  = type;                                               // 
  vars.prop.count = q;                                                  // 
  vars.prop.index = 0;                                                  // 
  searchFreeSpace();                                                    // on recherche un emplacement adhéquat
  if( ramAdd >= dataStartAdd ){                                         //   si un emplacement a été trouvé
    sectorTable( lockdown );                                            //   on verrouille la zone occupé
    setVar( newVar );                                                   //   on enregistre la définition
    format();                                                           //   on format à zéro sa zone mémoire
  }
}

void RAM23LC512SPI::setOverallSize(){                                   // calcule la taille occupée en octets
  setVarsSize();                                                        // on récupère la taille d'une seule variable
  size *=  vars.prop.count ? vars.prop.count : 1 ;                      // il y a toujours au moins 1 éléments
  size += ( vars.prop.type == RamSPI_userDefined ) ? 6 : 4 ;            // on y rajoute les 4 octets pour type+quantifier et index et 2 de plus si c'est une variable spéciale pour l'emplacement de sa taille unitaire
}

void RAM23LC512SPI::setVarsSize(){
  size = 1 ;                                                            // il y a toujours, au moins 1 éléments
  if( vars.prop.type == RamSPI_userDefined ){                           // exception spécifique à l'utilisation des variables spéciales.
    uint16_t userDefinedSize = 0 ;                                      //   
    _RAM_READ_                                                          //   ouverture en lecture
    _RAM_ADDRESS( ramAdd+4 )                                            //   on se positionne après l'index
    _RAM_GET( userDefinedSize )                                         //   on récupère les 16 bits de la taille spécifique
    _RAM_RELEASE_                                                       //   on cloture
    size *= userDefinedSize;                                            //   on calcule la taille spécifique
  }                                                                     // 
  else{                                                                 // si c'est une variable standard
    size <<= ( vars.prop.type & RamSPI_size_mask ) ;                    //   on calcule directement la taille
  }
#if defined(ARDUINO_SAM_DUE)  
  size <<= 1;
#endif
}

// ==================================================================== //
//                                                                      //
//                         GESTION VARS                                 //
//                                                                      //
// ==================================================================== // 

void RAM23LC512SPI::setVar( bool isNewVar ){
#ifdef RAM_verif
  uint8_t nbTry  = 0; 
  backup.definition = vars.definition;
#endif
  putVar( isNewVar );
#ifdef RAM_verif
  getVar();
  while( (backup.definition != vars.definition) && (nbTry < 10) ){
    nbTry++;
    vars.definition = backup.definition;
    putVar( isNewVar );
    getVar();
  }
#endif
}

void RAM23LC512SPI::putVar( bool isNewVar ){
  _RAM_WRITE_
  _RAM_ADDRESS( ramAdd )
  if( isNewVar ){
    _RAM_SEND( vars.varField )
  }
  _RAM_SEND( vars.varIndex )
  _RAM_RELEASE_
}

void RAM23LC512SPI::getVar(){
  _RAM_READ_
  _RAM_ADDRESS( ramAdd )
  _RAM_GET( vars.varField )
  _RAM_GET( vars.varIndex )
  _RAM_RELEASE_
}

// ====================================================================== //
//                                                                        //
//                         MFT                                            //
//       Attention chaque bit désigne un emplacement de 16 bits           //
// ====================================================================== //

void RAM23LC512SPI::searchFreeSpace(){                                      // on recherche n espace libre.
  if( started ){                                                            // 
    setOverallSize();                                                       // on détermine la taille occupée en octet
    size >>= 1;                                                             // attention chaque bit dans MFT désigne un emplacement de 16 bits donc division par 2
    sectorAdd = sectorTableStartAdd;                                        // on se place au début de la table sector
    sectorBit = 0;                                                          // position binaire dans le sector en cours
    ramAdd = 0;                                                             // on reset la position en ram pour les datas
    uint32_t count = 0;                                                     // compteur d'espace contigus
    _RAM_READ_                                                              // on ouvre en lecture
    _RAM_ADDRESS( sectorAdd )                                               // on envoi l'adresse
    while( count < size && sectorAdd < dataStartAdd ){                      // pour chaque octect de tableSector (temps qu'on reste dans la tableau)
      _RAM_GET8( sector )                                                   //   on récupère sa valeur
      if( count == 0 ){                                                     //     si le compteur est à zéro 0, c'est l'adresse initiale
        ramAdd = dataStartAdd + (sectorAdd << 4);                           //       on met alors l'adresse de départ de coté
      }                                                                     // 
      if( sector == 0xFF ){                                                 //     si le secteur est plein
        count = 0;                                                          //       on repart à 0
      }                                                                     //     
      else if( sector == 0x00 ){                                            //     si le secteur est vide
        count += 8;                                                         //       on rajoute 8
      }                                                                     // 
      else{                                                                 //     si ni vide ni plein, on va compter bit a bit
        while( sectorBit < 8 && count < size ){                             //       pour chaque bit du secteur récupéré
          if( count == 0 ){                                                 //         si le compteur est à zéro 0, c'est l'adresse initiale
            ramAdd = dataStartAdd + (sectorAdd << 4) + (sectorBit << 1) ;   //           on met alors l'adresse de départ de coté
          }                                                                 //         
          count = ( sector & 0x01 ) ? 0 : count+1 ;                         //         si on trouve un bit à 1 alors on repart à O sinon, on rajoute 1
          sector >>= 1;                                                     //         on passe au bit suivant
          sectorBit++;                                                      //     
        }                                                                   // 
      }                                                                     // 
      sectorAdd++;                                                          //   on passe au secteur suivant
      sectorBit = 0;                                                        //   on remet le compteur de bit à zéro
    }                                                                       // 
    if( count < size ){                                                     // si on a pas trouvé assez de place
      ramAdd = 0;                                                           //   on renvoi 0
    }                                                                       // 
  }                                                                         // 
  _RAM_RELEASE_                                                             // on cloture
}                                                                           // 

void RAM23LC512SPI::sectorTable( bool lock ){                               // 
  if( started ){                                                            // 
    uint32_t count = size;                                                  // l'unité de la ram est l'octet 8 bits (pas 16 bits)
    sectorCount += lock ? count : -count ;                                  // 
    count >>= 1;                                                            // l'unité de la MFT est le block de 16 bits donc division par 2
    sectorAdd =  (ramAdd-dataStartAdd) >> 4;                                // 
    sectorBit = ((ramAdd-dataStartAdd) >> 1) % 8;                           // 

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
}
void RAM23LC512SPI::setSectorTable(){
  uint8_t nbTry  = 0; 
  uint8_t backup = sector;
  putSectorTable();
  getSectorTable();
  while( (backup != sector) && (nbTry < 10) ){
    nbTry++;
    sector = backup;
    putSectorTable();
    getSectorTable();
  }
}
void RAM23LC512SPI::putSectorTable(){
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
}


// ==================================================================== //
//                                                                      //
//                         INITIALISATION SPI                           //
//                                                                      //
// ==================================================================== // 

void RAM23LC512SPI::begin( uint8_t _cs ){
  cs = _cs;
  if( cs > 0 ){
    pinMode( cs , OUTPUT );
    _RAM_CONFIG_
    _RAM_SEND8( RamSPI_sequentialMode );
    _RAM_RELEASE_
    uint16_t i = sectorTableEndAdd+1;
    _RAM_WRITE_
    _RAM_ADDRESS( sectorTableStartAdd )
    while( i-- ){
      _RAM_SEND8( 0 )
    }
    _RAM_RELEASE_

    started = true;
  }
}

void RAM23LC512SPI::send( uint8_t q ){
  if( started ){
    _RAM_WRITE_                                 // on ouvre le canal en écriture
    _RAM_ADDRESS( ramAdd )                      // on envoi l'adresse
    while( q-- ){                               // pour chaque paquet de 8 bits
      _RAM_SEND( bufferN16(q) )                 //   on l'envoi 8 bits q fois
    }                                           //
    _RAM_RELEASE_                               // on libère le canal
  }
}

void RAM23LC512SPI::get( uint8_t q ){
  if( started ){
    _RAM_READ_                                  // on ouvre le canal en écriture
    _RAM_ADDRESS( ramAdd )                      // on envoi l'adresse
    while( q-- ){                               // pour chaque paquet de 8 bits
      _RAM_GET( bufferN16(q) )                  //   on récupère 8 bits q fois
    }                                           // 
    _RAM_RELEASE_                               // on libère le canal
  }
}


// ==================================================================== //
//                                                                      //
//                         ROUTINE PUBLIQUE                             //
//                                                                      //
// ==================================================================== // 


void RAM23LC512SPI::write(){
  if( ramAdd > sectorTableEndAdd ){
#ifdef RAM_verif
    uint8_t nbTry = 0;
    backupValue();
#endif
    send( size );
#ifdef RAM_verif
    get( size );
    while( checkValue() && nbTry < 10 ){
      nbTry++;
      retrieveValue();
      send( size );
      get( size );
    }
#endif
  }
}

void RAM23LC512SPI::read(){
  if( ramAdd < dataStartAdd ){
    buffer64 = 0;
  }
  else{
    get( size );
  }
}

uint16_t RAM23LC512SPI::freeSpace(){
  return( 0xFFFF - sectorCount );
}

// ==================================================================== //
//                                                                      //
//                         ROUTINE                                      //
//                                                                      //
// ==================================================================== // 
/*
void RAM23LC512SPI::setDoubleHelper(){
  bufferN32(1) = DFVars  ;          // on stock la partie entière
  DFVars -= bufferN32(1) ;          // on retranche la partie entière à val
  DFVars *= 1000000000.0 ;          // on multiplie par 1 milliard pour ne garder que les 9 chiffres après la virgule
  bufferN32(0) = DFVars  ;          // on stock les 9 décimales
}

void RAM23LC512SPI::getDoubleHelper(){
  DFVars =  bufferN32(0) ;          // on place la partie décimale dans la variable double
  DFVars /= 1000000000.0 ;          // on divise par 1 milliard pour en faire la partie décimale
  DFVars += bufferN32(1) ;          // on rajoute la partie entière
}
*/
// ==================================================================== //
//                                                                      //
//                         ROUTINE BUFFER                               //
//                                                                      //
// ==================================================================== // 

#ifdef RAM_verif

void RAM23LC512SPI::backupValue(){
  switch( size ){
    case 1 :{   verif16 = buffer16 ;    break;  }
    case 2 :{   verif32 = buffer32 ;    break;  }
    default:{   verif64 = buffer64 ;    break;  }
  }
}

void RAM23LC512SPI::retrieveValue(){
  switch( size ){
    case 1 :{   buffer16 = verif16 ;    break;  }
    case 2 :{   buffer32 = verif32 ;    break;  }
    default:{   buffer64 = verif64 ;    break;  }
  }
}
bool RAM23LC512SPI::checkValue(){
  switch( size ){
    case 1 :{   return( buffer16 != verif16 ) ;    break;  }
    case 2 :{   return( buffer32 != verif32 ) ;    break;  }
    default:{   return( buffer64 != verif64 ) ;    break;  }
  }
  return( false );
}

#endif