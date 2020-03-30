/*******************************************************************
 *
 *    PROJECT:     LinkUp EOL
 *
 *    EQUIPMENT:   EOL
 *
 *    FILE:        process.h
 *
 *    DESCRIPTION:
 *
 *    AUTHOR:      $Author
 *
 *    ENVIRONMENT: CVI 7.1
 *
 *    DATE:        $Author
 *
 *    HISTORY:
 *
 *
 *******************************************************************/

#include "defines.h"

#define ERR_NO_ERROR           0


#define ERR_I_MIN              1   // zu geringe Stromaufnahme
#define ERR_I_MAX              2   // zu hohe Stromaufnahme
#define ERR_KONTAKT            5   // Fehler bei Kontaktierung

#define ERR_PREV_TOOL         10   // Fehler: eine vorherige Vorrichtung wurde nicht durchlaufen

#define ERR_DIG_INPUT         20   // Fehler bei digitalem Input
#define ERR_ANALOG_INPUT      21   // Fehler bei analog Input
#define ERR_DIG_OUTPUT        22   // Fehler bei digitalem Output
#define ERR_FREQ_OUTPUTS      23   // Fehler bei Frequenz Output
#define ERR_NFC               24   // Fehler bei GPS
#define ERR_EASYLINK          25

#define ERR_SNR               35   // keine Snr vorhanden

#define ERR_LCD               40
//#define ERR_LED               41
#define ERR_ZG_POS_0          42
#define ERR_ZG_POS_2          43
#define ERR_OVERALL_IMPRESSION 44   // Anmutung schlecht


#define ERR_KOM_TIMEOUT       50   // Timeout bei Kommunikation mit Gerät

#define ERR_STEPPER_MOTOR     55

#define ERR_DICHT             70   // Dichtheistfehler

#define ERR_DEV_DAT           90   // Fehler in DeviceDat.ini
#define ERR_PROD_SELECT       91   // Fehler ausgewähltes Produkt
#define ERR_INTERNAL_HW       98   // Hardware-Fehler
#define ERR_INTERNAL_SW       99   // interner Software-Fehler
#define ERR_USER_ABORT       100   // von Benutzer abgebrochen


extern  char  *pcStatusTxt_g;           // Statusinformationen für den Techniker
extern  int   iTripSwitchState_g;         // Status TripSwitch


/**
 * Schritte der Statemachine
 */
typedef enum
{
  process_init                         =   10,
  warte_grundstellung                  =   20,
  warte_start                          =   50,

  warte_start_delayed                  =   60,
  warte_start_delayed_dut_weg          =   61,
  warte_auswahl1                       =   70,
  warte_auswahl2                       =   71,

  warte_pruefling                      =  100,

  warte_pruefling_start                =  112,

  kontakt_ein                          =  120,
  kontakt_ein_wait                     =  121,

  communication_init                   =  130,

  req_prod_data_init                   =  240,

  req_snr                              =  245,
  req_snr_read                         =  246,

  req_sw_version                       =  250,
  req_sw_version_read                  =  251,


  test_start                           =  290,

  /**
   * Stromaufnahme messen
   */
  mess_current_idle                    = 1000,
  mess_current_idle_init               = mess_current_idle+10,
  mess_current_idle_wait               = mess_current_idle+13,
  mess_current_idle_eval               = mess_current_idle+14,

  /**
   * Stromaufnahme messen
   */
  led_test                             = 1100,
  led_test_init                        = led_test+10,

  led_test_set                         = led_test+20,
  led_test_wait                        = led_test+21,

  led_test_reset                       = led_test+30,

  led_test_end                         = led_test+99,


  /**
   * analog Inputs testen
   */
  ai_test                              = 2000,

  ai_test_init                         = ai_test+10,
  ai_test_init_wait                    = ai_test+11,

  ai_test_set                          = ai_test+30,
  ai_test_set_wait                     = ai_test+31,

  ai_test_req                          = ai_test+40,
  ai_test_read                         = ai_test+41,
  ai_test_next                         = ai_test+42,

  ai_test_end                          = ai_test+99,


  /**
   * NFC testen
   */
  nfc_test                              = 2100,

  nfc_test_init                         = nfc_test+10,
  nfc_test_init_wait                    = nfc_test+11,

  nfc_test_set                          = nfc_test+30,
  nfc_test_set_wait                     = nfc_test+31,

  nfc_test_req                          = nfc_test+40,
  nfc_test_read                         = nfc_test+41,

  nfc_test_end                          = nfc_test+99,



  /* Bearbeitung abgeschlossen */
  process_end                          = 3000,


  prod_data_start                      = 3100,
  prod_data_write                      = 3110,
  prod_data_write_wait                 = 3111,
  prod_data_write_next                 = 3112,

#ifdef USE_LABEL_PRINTER
  printer_test                         = 3150,
  print_label                          = 3151,
#endif // USE_LABEL_PRINTER

  ignition_reset                       = 3240,
  ignition_reset2                      = 3241,
  interface_reset                      = 3245,

  spannen_reset_taste                  = 3250,
  spannen_reset                        = 3251,

  kontakt_aus                          = 3260,
  kontakt_aus_wait                     = 3261,

  wait_pruefling_weg                   = 3270,


  fehler                               = 3280,
  fehler_taste                         = 3281,
  fehler_auswertung                    = 3290,



  network_wait_for_reconnect           = 6000,
  network_wait_for_reconnect_wait      = 6001,

  /**
   * Sub-State-Machine
   */
  device_restart       = 5000,
  device_reset         = 5001,
  device_start_wait    = 5002,

  login_diag_mode      = 5020,
  login_diag_mode_wait = 5021,

  login_security       = 5030,
  login_security_wait  = 5031,

} EProcessState;



/**
 * Schritte der Statemachine für Dichtheitstest
 */
typedef enum
{
  dt_fill                 =  10,
  dt_fill_wait            =  20,
  dt_relax_wait           =  30,
  dt_wait                 =  40,
  dt_end                  =  50,
  dt_abort                =  60
} EDTState;


//  ------------------------------------------------------------------------------------------------
//    Globale Funktionen
//  ------------------------------------------------------------------------------------------------
extern  int   iProcess_g(void);   // Hauptablauf
//extern  char  *pcGetStatusInfoTxt_g(int iProcessStateNo);

