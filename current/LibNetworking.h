/*******************************************************************
 *
 *    PROJECT:     Library
 *
 *    FILE:        Networking.h
 *
 *    DESCRIPTION: Library for connecting and disconnecting Network drives
 *
 *    AUTHOR:      $Author$
 *
 *    ENVIRONMENT: CVI 7.1
 *
 *    DATE:        $Date$
 *
 *    HISTORY:
 *      $Log$
 *
 *******************************************************************/

#ifndef NETWORKING_H_INCLUDED
  #define NETWORKING_H_INCLUDED

#include <windows.h>

  #ifdef DLL_EXPORTS
    #define MYDLL_API __declspec(dllexport)
  #else
    #define MYDLL_API __declspec(dllimport)
  #endif

#ifndef _CVI_
  #undef WINAPI
  #define WINAPI
#endif // !_CVI_

/**
 * Netzwerkverbindung aufbauen
 *
 * @param pcUser Benutzername
 * @param pcPwd  Passwort
 * @param pcLocalName
 *               Laufwerksbuchstaben, welchem der Netzwerkpfad zugeordnet werden soll. Format X:
 * @param pcRemoteName
 *               Netzwerkpfad, Format: \\Freigabe\Ordner
 * @param pcCompanyName
 *               Name der Firma
 * @param pcProductName
 *               Name des Produkt
 * @param pcApplicationName
 *               Name der Vorrichtung
 * @param pcSWVersion
 *               Version der Software.
 *
 * @return TRUE bei Erfolg, sonst FALSE
 */
MYDLL_API BOOL WINAPI bNetworking_ConnectWait(char *pcUser,
                                              char *pcPwd,
                                              char *pcLocalName,
                                              char *pcRemoteName,
                                              char *pcCompanyName,
                                              char *pcProductName,
                                              char *pcApplicationName,
                                              char *pcSWVersion);

/**
 * Trennt ein Netzlaufwerk
 *
 * @param pcLocalName
 *               Laufwerkbuchstane
 * @param bForceDisconnect
 *               Trennen erzwingen
 *
 * @return TRUE bei Erfolg, sonst FALSE
 */
MYDLL_API BOOL WINAPI bNetworking_Disconnect(char *pcLocalName, BOOL bForceDisconnect);


/**
 * Gibt den letzten Fehler als String zurück.
 *
 * @return Letzter Fehler
 */
MYDLL_API char WINAPI *pcNetworking_GetLastErrorString(void);

/**
 * Gibt den letzten Fehlercode zurück, alles sind standard Windows Fehlercodes. 0 bedeutet kein Fehler.
 *
 * @return Letzten Fehlercode
 */
MYDLL_API UINT WINAPI uiNetworking_GetLastErrorCode(void);

#endif // NETWORKING_H_INCLUDED
