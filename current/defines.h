/*******************************************************************
 *
 *    PROJECT:     Tankf�llstandsgeber EOL
 *
 *    EQUIPMENT:   EOL
 *
 *    FILE:        defines.h
 *
 *    DESCRIPTION:
 *
 *    AUTHOR:      HaJ
 *
 *    ENVIRONMENT: CVI 7.1
 *
 *    DATE:        $Date$
 *
 *    HISTORY:
 *      $Log$
 *
 *******************************************************************/

#ifndef DEFINES_H_INCLUDED
  #define DEFINES_H_INCLUDED


  #define PROGRAM_VERSION_MAJOR  0
  #define PROGRAM_VERSION_MINOR  801

#define TEST_OFFLINE  // Software ohne Vorichtung testen

//**
// LibTester Gui Defines
//**
  #define TESTER_GUI_DO_ANIMATION   TRUE
  #define TESTER_GUI_ANIMATION_TIME 1.0

// Komponenten individuell aktivieren / deaktivieren

#define USE_LABEL_PRINTER
#define USE_NFC

//#define USE_SLIP_PRINTER   // Bei Fehler einen Beleg drucken

// Komponenten in Abh�ngigkeit aktivieren / deaktivieren
  #ifdef TEST_OFFLINE
    #define IGNORE_FLIL       // Alle Eing�nge ignorieren
    #define SIMULATE_BOX      // Keine EA's => Bedienbox auf Oberfl�che simulieren
    #define IGNORE_IVCC_ERROR // Strommessung ignorieren
    #undef USE_SLIP_PRINTER   // Bei Fehler einen Beleg drucken
    #undef USE_LABEL_PRINTER
    #undef USE_NFC   // TODO: un-comment this
    #define IGNORE_GPS_SIMULATOR
  #endif // TEST_OFFLINE

  #ifdef IGNORE_FLIL
    #define IGNORE_FLIL_EAS   // Alle Eing�nge signorieren
    #define IGNORE_IVCC_ERROR // Strommessung ignorieren
  #endif // IGNORE_FLIL

  #ifdef IGNORE_FLIL_EAS
    #define SIMULATE_BOX   // Keine EA's => Bedienbox auf Oberfl�che simulieren
    #define IGNORE_IVCC_ERROR  // Strommessung ignorieren
  #endif // IGNORE_FLIL_EAS

// Softwarekomponenten f�r Touch adaptieren
  #define USE_TOUCH_PANEL


  #define INVALID_PANEL_HANDLE -1


#endif // DEFINES_H_INCLUDED
