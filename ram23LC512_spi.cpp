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

//    SECTOR                                     ---------------- // ---- 8 ----- | --- 8 -------
uint16_t RAM23LC512SPI::ramAdd               = 0 ;                //      2       |     2
uint8_t  RAM23LC512SPI::sector               = 0 ;                //      1       |     1
uint8_t  RAM23LC512SPI::sectorBit            = 0 ;                //      1       |     1
uint16_t RAM23LC512SPI::sectorAdd            = 0 ;                //      2       |     2
uint16_t RAM23LC512SPI::sectorCount          = dataStartAdd ;     //      2       |     2

//    BUFFER                                     ---------------- // ---- 8 ----- | --- 16 ------
unifiedBuffer RAM23LC512SPI::bufferA         = { 0 } ;            //      8       |     8
unifiedBuffer RAM23LC512SPI::bufferB         = { 0 } ;            //      -       |     8
//    VARS                                       ---------------- // ---- 8 ----- | --- 12 ------
uint32_t    RAM23LC512SPI::size              = 0 ;                //      4       |     4
DECLARATION RAM23LC512SPI::varsA             = { 0 } ;            //      4       |     4
#ifdef RAM_verif
DECLARATION RAM23LC512SPI::varsB             = { 0 } ;            //      -       |     4
#endif
//                                                       TOTAL    // ==== 25 ==== | === 78 ===== 


bool RAM23LC512SPI::started(){
  if( cs == 0 ){
    begin();
  }
  return( cs ? true : false );
}

// ==================================================================== //
//                                                                      //
//                         DYNAMIC ALLOCATION                           //
//                                                                      //
// ==================================================================== // 

uint16_t RAM23LC512SPI::allocNewVar( uint8_t type, uint16_t q ){
  if( started() ){
    alloc( type, q );
    return( ramAdd );
  }
  return( 0 );
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
  getVar();                                                             // on r??cup??re sa d??claration et son index
  setVarsSize();                                                        // on d??termine la taille de chaque ??l??ment
  ramAdd += ( varsA.prop.type == RamSPI_userDefined ) ? 6 : 4;          // on se place sur le 1er index 
  ramAdd += varsA.prop.index * size ;                                   // on d??cale la position en ram de la valeur de index/taille
}                                                                       // 

void RAM23LC512SPI::focus( uint16_t add, uint16_t index ){              // 
  ramAdd = add;                                                         // on se positionne
  getVar();                                                             // on r??cup??re sa d??claration et son index
  index &= RamSPI_count_mask ;                                          // si l'index demand?? d??passe la capacit?? du tableau, on reboucle ?? z??ro
  index = ( index >= varsA.prop.count ) ? 0 : index ;                   // si l'index demand?? est sup??rieur ?? la quantit?? d??clar??e, on le met ?? 0
  varsA.prop.index = index;                                             // on indique l'index demand??
  setVar( newIndex );                                                   // on le m??morise pour la prochaine lecture
}

void RAM23LC512SPI::free(){                                             // lib??re l'espace add
  setOverallSize();                                                     // on d??termine la place totale occup??e
  sectorTable( release );                                               // on lib??re la taille totale
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

void RAM23LC512SPI::alloc( uint16_t type, uint16_t q ){                 // on recherche un emplacement d'une quantit?? q de type
  q &= RamSPI_count_mask ;                                              // on limite la quantit?? d'unit??e
  varsA.prop.type  = type;                                              // 
  varsA.prop.count = q;                                                 // 
  varsA.prop.index = 0;                                                 // 
  searchFreeSpace();                                                    // on recherche un emplacement adh??quat
  if( ramAdd >= dataStartAdd ){                                         //   si un emplacement a ??t?? trouv??
    sectorTable( lockdown );                                            //   on verrouille la zone occup??
    setVar( newVar );                                                   //   on enregistre la d??finition
    format();                                                           //   on format ?? z??ro sa zone m??moire
  }
}

void RAM23LC512SPI::setOverallSize(){                                   // calcule la taille occup??e en octets
  setVarsSize();                                                        // on r??cup??re la taille d'une seule variable
  size *=  varsA.prop.count ? varsA.prop.count : 1 ;                    // il y a toujours au moins 1 ??l??ments
  size += ( varsA.prop.type == RamSPI_userDefined ) ? 6 : 4 ;           // on y rajoute les 4 octets pour type+quantifier et index et 2 de plus si c'est une variable sp??ciale pour l'emplacement de sa taille unitaire
}

void RAM23LC512SPI::setVarsSize(){
  size = 1 ;                                                            // il y a toujours, au moins 1 ??l??ments
  if( varsA.prop.type == RamSPI_userDefined ){                          // exception sp??cifique ?? l'utilisation des variables sp??ciales.
    uint16_t userDefinedSize = 0 ;                                      //   
    _RAM_READ_                                                          //   ouverture en lecture
    _RAM_ADDRESS( ramAdd+4 )                                            //   on se positionne apr??s l'index
    _RAM_GET( userDefinedSize )                                         //   on r??cup??re les 16 bits de la taille sp??cifique
    _RAM_RELEASE_                                                       //   on cloture
    size *= userDefinedSize;                                            //   on calcule la taille sp??cifique
  }                                                                     // 
  else{                                                                 // si c'est une variable standard
    size <<= ( varsA.prop.type & RamSPI_size_mask ) ;                   //   on calcule directement la taille
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
  varsB.definition = varsA.definition;
#endif
  putVar( isNewVar );
#ifdef RAM_verif
  getVar();
  while( (varsB.definition != varsA.definition) && (nbTry < 10) ){
    nbTry++;
    varsA.definition = varsB.definition;
    putVar( isNewVar );
    getVar();
  }
#endif
}

void RAM23LC512SPI::putVar( bool isNewVar ){
  _RAM_WRITE_
  _RAM_ADDRESS( ramAdd )
  if( isNewVar ){
    _RAM_SEND( varsA.varField )
  }
  _RAM_SEND( varsA.varIndex )
  _RAM_RELEASE_
}

void RAM23LC512SPI::getVar(){
  _RAM_READ_
  _RAM_ADDRESS( ramAdd )
  _RAM_GET( varsA.varField )
  _RAM_GET( varsA.varIndex )
  _RAM_RELEASE_
}



// ====================================================================== //
//                                                                        //
//                         MFT                                            //
//       Attention chaque bit d??signe un emplacement de 16 bits           //
// ====================================================================== //

void RAM23LC512SPI::searchFreeSpace(){                                      // on recherche n espace libre.
  if( started() ){                                                          // 
    setOverallSize();                                                       // on d??termine la taille occup??e en octet
    size >>= 1;                                                             // attention chaque bit dans MFT d??signe un emplacement de 16 bits donc division par 2
    sectorAdd = sectorTableStartAdd;                                        // on se place au d??but de la table sector
    sectorBit = 0;                                                          // position binaire dans le sector en cours
    ramAdd = 0;                                                             // on reset la position en ram pour les datas
    uint32_t count = 0;                                                     // compteur d'espace contigus
    _RAM_READ_                                                              // on ouvre en lecture
    _RAM_ADDRESS( sectorAdd )                                               // on envoi l'adresse
    while( count < size && sectorAdd < dataStartAdd ){                      // pour chaque octect de tableSector (temps qu'on reste dans la tableau)
      _RAM_GET8( sector )                                                   //   on r??cup??re sa valeur
      if( count == 0 ){                                                     //     si le compteur est ?? z??ro 0, c'est l'adresse initiale
        ramAdd = dataStartAdd + (sectorAdd << 4);                           //       on met alors l'adresse de d??part de cot??
      }                                                                     // 
      if( sector == 0xFF ){                                                 //     si le secteur est plein
        count = 0;                                                          //       on repart ?? 0
      }                                                                     //     
      else if( sector == 0x00 ){                                            //     si le secteur est vide
        count += 8;                                                         //       on rajoute 8
      }                                                                     // 
      else{                                                                 //     si ni vide ni plein, on va compter bit a bit
        while( sectorBit < 8 && count < size ){                             //       pour chaque bit du secteur r??cup??r??
          if( count == 0 ){                                                 //         si le compteur est ?? z??ro 0, c'est l'adresse initiale
            ramAdd = dataStartAdd + (sectorAdd << 4) + (sectorBit << 1) ;   //           on met alors l'adresse de d??part de cot??
          }                                                                 //         
          count = ( sector & 0x01 ) ? 0 : count+1 ;                         //         si on trouve un bit ?? 1 alors on repart ?? O sinon, on rajoute 1
          sector >>= 1;                                                     //         on passe au bit suivant
          sectorBit++;                                                      //     
        }                                                                   // 
      }                                                                     // 
      sectorAdd++;                                                          //   on passe au secteur suivant
      sectorBit = 0;                                                        //   on remet le compteur de bit ?? z??ro
    }                                                                       // 
    if( count < size ){                                                     // si on a pas trouv?? assez de place
      ramAdd = 0;                                                           //   on renvoi 0
    }                                                                       // 
  }                                                                         // 
  _RAM_RELEASE_                                                             // on cloture
}                                                                           // 

void RAM23LC512SPI::sectorTable( bool lock ){                               // 
  if( started() ){                                                            // 
    uint32_t count = size;                                                  // l'unit?? de la ram est l'octet 8 bits (pas 16 bits)
    sectorCount += lock ? count : -count ;                                  // 
    count >>= 1;                                                            // l'unit?? de la MFT est le block de 16 bits donc division par 2
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

void RAM23LC512SPI::begin(){
  cs = RamSPI_cs;
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
  }
}

void RAM23LC512SPI::send( uint8_t q ){
  if( started() ){
    _RAM_WRITE_                                 // on ouvre le canal en ??criture
    _RAM_ADDRESS( ramAdd )                      // on envoi l'adresse
    while( q-- ){                               // pour chaque paquet de 8 bits
      _RAM_SEND( bufferN16(q) )                 //   on l'envoi 8 bits q fois
    }                                           //
    _RAM_RELEASE_                               // on lib??re le canal
  }
}

void RAM23LC512SPI::get( uint8_t q ){
  if( started() ){
    _RAM_READ_                                  // on ouvre le canal en ??criture
    _RAM_ADDRESS( ramAdd )                      // on envoi l'adresse
    while( q-- ){                               // pour chaque paquet de 8 bits
      _RAM_GET( bufferN16(q) )                  //   on r??cup??re 8 bits q fois
    }                                           // 
    _RAM_RELEASE_                               // on lib??re le canal
  }
}


// ==================================================================== //
//                                                                      //
//                         ROUTINE PUBLIQUE                             //
//                                                                      //
// ==================================================================== // 


void RAM23LC512SPI::write(){
  if( ramAdd > sectorTableEndAdd                                                                        // si l'adresse en ram est d??finie
  && varsA.prop.type != RamSPI_userDefined                                                              // et que la variable actuelle n'est pas un format d??fini par l'utilisateur
  && varsB.prop.type != RamSPI_userDefined ){                                                           // et que la variable pr??c??dente n'??tait pas non plus un format d??fini par l'utilisateur
    verif64 = buffer64;                                                                                 //   conversion de double -> int ou int -> double
    if( ( varsB.prop.type > RamSPI_int64_t ) && ( varsA.prop.type < RamSPI_float_double ) ){            //   si c'est un double ?? ranger dans un entier
      switch( varsA.prop.type ){                                                                        //     suivant le type de la variable de destination
        case RamSPI_uint16_t      :{   buffer16 = verifDBL ;   break;  }                                //       double dans un entier 16 bits non sign??
        case RamSPI_uint32_t      :{   buffer32 = verifDBL ;   break;  }                                //       - - - - - - - - - - - 32 bits - - - - -
        case RamSPI_uint64_t      :{   buffer64 = verifDBL ;   break;  }                                //       - - - - - - - - - - - 64 bits - - - - -
        case RamSPI_int16_t       :{   buffer16 = verifDBL ;   break;  }                                //       - - - - - - - - - - - 16 bits sign??
        case RamSPI_int32_t       :{   buffer32 = verifDBL ;   break;  }                                //       - - - - - - - - - - - 32 bits - - -
        case RamSPI_int64_t       :{   buffer64 = verifDBL ;   break;  }                                //       - - - - - - - - - - - 64 bits - - -
      }                                                                                                 // 
    }                                                                                                   // 
    else if( ( varsB.prop.type < RamSPI_float_double ) && ( varsA.prop.type > RamSPI_int64_t ) ) {      //   si la pr??c??dente valeur lue ??tait de type enti??re et qu'on s'appr??te ?? ??crire dans un double
      switch( varsB.prop.type ){                                                                        //     suivant le type de la pr??c??dente valeur
        case RamSPI_uint16_t      :{   bufferDBL = verif16 ;   break;  }                                //       16 bits non sign?? dans un double
        case RamSPI_uint32_t      :{   bufferDBL = verif32 ;   break;  }                                //       32 bits - - - - - - - - - - - - -
        case RamSPI_uint64_t      :{   bufferDBL = verif64 ;   break;  }                                //       64 bits - - - - - - - - - - - - -
        case RamSPI_int16_t       :{   bufferDBL = verif16 ;   break;  }                                //       16 bits sign?? dans un double
        case RamSPI_int32_t       :{   bufferDBL = verif32 ;   break;  }                                //       32 bits - - - - - - - - - - -
        case RamSPI_int64_t       :{   bufferDBL = verif64 ;   break;  }                                //       64 bits - - - - - - - - - - -
      }                                                                                                 // 
    }                                                                                                   // 
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


void RAM23LC512SPI::read_partA(){   read( false );    }
void RAM23LC512SPI::read_partB(){   read( true  );    }

void RAM23LC512SPI::read( bool part ){
  varsA_64 = 0;
  if( ramAdd > sectorTableEndAdd ){
    get( size );
    if( part ){
      varsB_64 = varsA_64 ;
      varsB.definition = varsA.definition;
    }
  }
}


void RAM23LC512SPI::doMath( uint8_t op ){
  if( varsA.prop.type < RamSPI_userDefined && varsB.prop.type < RamSPI_userDefined ){
    uint8_t opType = opType_notDefined;
    if( varsA.prop.type == RamSPI_float_double && varsB.prop.type == RamSPI_float_double ){   opType = opType_dbl_on_dbl;   }
    if( varsA.prop.type  < RamSPI_float_double && varsB.prop.type == RamSPI_float_double ){   opType = opType_dbl_on_int;   }
    if( varsA.prop.type == RamSPI_float_double && varsB.prop.type  < RamSPI_float_double ){   opType = opType_int_on_dbl;   }
    if( varsA.prop.type  < RamSPI_float_double && varsB.prop.type  < RamSPI_float_double ){   opType = opType_int_on_int;   }
    switch( opType ){
      case opType_dbl_on_dbl :{
        switch( op ){
          case opCode_affectation     :{    varsA_DBL  = varsB_DBL;  break;  }
          case opCode_addition        :{    varsA_DBL += varsB_DBL;  break;  }
          case opCode_soustraction    :{    varsA_DBL -= varsB_DBL;  break;  }
          case opCode_multiplication  :{    varsA_DBL *= varsB_DBL;  break;  }
          case opCode_division        :{    varsA_DBL /= varsB_DBL;  break;  }
        }
        break;
      }
      case opType_int_on_dbl :{
        switch( op ){
          case opCode_affectation     :{    varsA_DBL  = varsB_64;  break;  }
          case opCode_addition        :{    varsA_DBL += varsB_64;  break;  }
          case opCode_soustraction    :{    varsA_DBL -= varsB_64;  break;  }
          case opCode_multiplication  :{    varsA_DBL *= varsB_64;  break;  }
          case opCode_division        :{    varsA_DBL /= varsB_64;  break;  }
        }
        break;
      }
      case opType_dbl_on_int :{
        switch( op ){
          case opCode_affectation     :{    varsA_64  = varsB_DBL;  break;  }
          case opCode_addition        :{    varsA_64 += varsB_DBL;  break;  }
          case opCode_soustraction    :{    varsA_64 -= varsB_DBL;  break;  }
          case opCode_multiplication  :{    varsA_64 *= varsB_DBL;  break;  }
          case opCode_division        :{    varsA_64 /= varsB_DBL;  break;  }
        }
        break;
      }
      case opType_int_on_int :{
        switch( op ){
          case opCode_affectation     :{    varsA_64  = varsB_64;  break;  }
          case opCode_addition        :{    varsA_64 += varsB_64;  break;  }
          case opCode_soustraction    :{    varsA_64 -= varsB_64;  break;  }
          case opCode_multiplication  :{    varsA_64 *= varsB_64;  break;  }
          case opCode_division        :{    varsA_64 /= varsB_64;  break;  }
        }
        break;
      }
    }
  }
}

uint8_t RAM23LC512SPI::varType(){
  return( varsA.prop.type );
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
  bufferN32(1) = DFVars  ;          // on stock la partie enti??re
  DFVars -= bufferN32(1) ;          // on retranche la partie enti??re ?? val
  DFVars *= 1000000000.0 ;          // on multiplie par 1 milliard pour ne garder que les 9 chiffres apr??s la virgule
  bufferN32(0) = DFVars  ;          // on stock les 9 d??cimales
}

void RAM23LC512SPI::getDoubleHelper(){
  DFVars =  bufferN32(0) ;          // on place la partie d??cimale dans la variable double
  DFVars /= 1000000000.0 ;          // on divise par 1 milliard pour en faire la partie d??cimale
  DFVars += bufferN32(1) ;          // on rajoute la partie enti??re
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