#ifndef EPSSTMAPI_H_INCLUDED
  #define EPSSTMAPI_H_INCLUDED

// EPSStmApi.h
//
// Header zu EpsStmApi3.dll
//
///////////////////////////////////////////////////////////////////////////////
// ERROR CODE :
  #define SUCCESS                          0 // Success
  #define ERR_TYPE                       -10 // nType error
  #define ERR_OPENED                     -20 // Already opened
  #define ERR_NO_PRINTER                 -30 // There is not printer driver
  #define ERR_NO_TARGET                  -40 // Printer out of object
  #define ERR_NO_MEMORY                  -50 // No memory
  #define ERR_HANDLE                     -60 // Invalid handle
  #define ERR_TIMEOUT                    -70 // Ttime out
  #define ERR_ACCESS                     -80 // cannot read/write
  #define ERR_PARAM                      -90 // param error
  #define ERR_NOT_SUPPORT               -100 // not support
  #define ERR_OFFLINE                   -110 // now offline
  #define ERR_NOT_EPSON                 -120 // not epson printer
  #define ERR_WITHOUT_CB                -130 // without callback function
  #define ERR_BUFFER_OVER_FLOW          -140 // Read buffer over flow
  #define ERR_REGISTRY                  -150 // Registry error
  #define ERR_ENABLE                    -160 // BiOpenMonPrinter() is already called
  #define ERR_DISK_FULL                 -170 // Capacity of a disk is insufficient
  #define ERR_NO_IMAGE                  -180 // No image data
  #define ERR_ENTRY_OVER                -190 // Registration number-of-cases over
  #define ERR_CROPAREAID                -200 // No specific CropAreaID
  #define ERR_EXIST                     -210 // Already the same thing
  #define ERR_NOT_FOUND                 -220 // Not found
  #define ERR_IMAGE_FILEOPEN            -230 // Open failure
  #define ERR_IMAGE_UNKNOWNFORMAT       -240 // Format injustice
  #define ERR_IMAGE_FAILED              -250 // Image creation failure
  #define ERR_WORKAREA_NO_MEMORY        -260 // No memory for WORKAREA
  #define ERR_WORKAREA_UNKNOWNFORMAT    -270 // Image creation failure because of format injustice
  #define ERR_WORKAREA_FAILED           -280 // WORKAREA creation failure
  #define ERR_IMAGE_FILEREAD            -290 // Image file read error
  #define ERR_PAPERINSERT_TIMEOUT       -300 // Image paper insert error of timeout
  #define ERR_EXEC_FUNCTION             -310 // Other API is running (3.01)
  #define ERR_EXEC_MICR                 -320 // Now reading MICR (3.01)
  #define ERR_EXEC_SCAN                 -330 // Now scaning image(3.01)
  #define ERR_SS_NOT_EXIST              -340 // Status service not started (3.01)
  #define ERR_SPL_NOT_EXIST             -350 // Spooler service not started (3.01)
  #define ERR_SPL_PAUSED                -370 // Spooler service paused (3.01)
  #define ERR_RESET                     -400 // Now printer is reseting (3.01)
  #define ERR_THREAD                    -420 // Thread error (3.03)
  #define ERR_ABORT                     -430 // Call BiSCNMICRCancelFunction(3.03)
  #define ERR_MICR                      -440 // MICR error (3.03)
  #define ERR_SCAN                      -450 // SCAN error (3.03)
  #define ERR_LINE_OVERFLOW             -460 // Tranzaction print error (3.03)
  #define ERR_NOT_EXEC                  -470 // Not Process (3.03)
///////////////////////////////////////////////////////////////////////////////


// ASB Bit :
  #define ASB_NO_RESPONSE                 0x00000001 //No response
  #define ASB_PRINT_SUCCESS               0x00000002 //Finish to print
  #define ASB_UNRECOVER_ERR               0x00002000 //Unrecoverable error
  #define ASB_AUTORECOVER_ERR             0x00004000 //Auto-Recoverable error
  #define ASB_OFF_LINE                    0x00000008 //Off-line
  #define ASB_WAIT_ON_LINE                0x00000100 //Waiting for on-line recovery
  #define ASB_WAIT_PEPRT_EJECT            0x00000100 // Waiting for slip remove (3.03)
  #define ASB_PANEL_SWITCH                0x00000200 //Panel switch
  #define ASB_PRINTER_FEED                0x00000040 //Paper is being fed
  #define ASB_HEAD_TEMPERATURE_ERR        0x00004000 //Mechanical error
  #define ASB_MECHANICAL_ERR              0x00000400 //Mechanical error
  #define ASB_LABEL_ERR                   0x00000400 //LABEL check error
  #define ASB_AUTOCUTTER_ERR              0x00000800 //Auto cutter error
  #define ASB_DRAWER_KICK                 0x00000004 //Drawer kick-out connector pin3 is HIGH
  #define ASB_PRESENTER_COVER             0x00000004 //Presenter cover is open
  #define ASB_JOURNAL_END                 0x00040000 //Journal paper roll end
  #define ASB_EJECT_SENSOR_NO_PAPER       0x00040000 //Eject sensor no paper (3.03)
  #define ASB_RECEIPT_END                 0x00080000 //Receipt paper roll end
  #define ASB_COVER_OPEN                  0x00000020 //Cover is open
  #define ASB_PLATEN_OPEN                 0x00000020 //Platen is open
  #define ASB_JOURNAL_NEAR_END            0x00010000 //Journal paper roll near-end
  #define ASB_JOURNAL_NEAR_END_FIRST      0x00010000 //Journal paper roll near-end-first
  #define ASB_NOT_CARD_INSERT             0x00010000 //Not card insert (3.03)
  #define ASB_RECEIPT_NEAR_END            0x00020000 //Receipt paper roll near-end
  #define ASB_RECEIPT_NEAR_END_FIRST      0x00020000 //Receipt paper roll near-end-first
  #define ASB_SLIP_TOF                    0x00200000 //SLIP TOF
  #define ASB_SLIP_TOF_2                  0x00400000 //SLIP TOF 2
  #define ASB_PSUPPLIER_END               0x00200000 //Paper supply end
  #define ASB_SLIP_BOF                    0x00400000 //SLIP BOF
  #define ASB_SLIP_BOF_2                  0x00200000 //SLIP BOF 2
  #define ASB_RECEIPT_NEAR_END_SECOND     0x00400000 //Receipt paper roll near-end-second
  #define ASB_ASF_PAPER                   0x00400000 //No paper ASF(3.03)
  #define ASB_SLIP_SELECTED               0x01000000 //Slip is not selected
  #define ASB_PRESENTER_TE                0x01000000 //Presenter T/E receipt end
  #define ASB_PRINT_SLIP                  0x02000000 //Cannot print on slip
  #define ASB_PRESENTER_TT                0x02000000 //Presenter T/T receipt end
  #define ASB_VALIDATION_SELECTED         0x04000000 //Validation is not selected
  #define ASB_RETRACTOR_R1JAM             0x04000000 //Retractor receipt end R1JAM
  #define ASB_PRINT_VALIDATION            0x08000000 //Cannot print on validation
  #define ASB_RETRACTOR_BOX               0x08000000 //Retractor box
  #define ASB_VALIDATION_TOF              0x20000000 //Validation TOF
  #define ASB_RETRACTOR_R2JAM             0x20000000 //Retractor receipt end R2JAM
  #define ASB_WAIT_INSERT                 0x20000000 //Wait insert (3.03)
  #define ASB_VALIDATION_BOF              0x40000000 //Validation BOF
  #define ASB_RETRACTOR_SENSOR3           0x40000000 //Retractor sensor NO.3
  #define ASB_VALIDATION_NO_PAPER         0x40000000 //Validation no paper (3.03)
  #define ASB_BATTERY_OFFLINE             0x00000004 //Off-line for BATTERY QUANTITY
  #define ASB_PAPER_FEED                  0x00000040 //Paper is now feeding by PF FW (3.01)
  #define ASB_PAPER_END                   0x00040000 //Detected paper roll end (3.01)
  #define INK_ASB_NEAR_END                0x00000001 //Ink near-end
  #define INK_ASB_END                     0x00000002 //Ink end
  #define INK_ASB_NO_CARTRIDGE            0x00000004 //Cartridge is not present
  #define INK_ASB_CLEANING                0x00000020 //Being cleaned
  #define INK_ASB_NEAR_END2               0x00000100 //Ink near-end2
  #define INK_ASB_END2                    0x00000200 //Ink end2

/////////////////////////////////////////////////////////////////////////////////////
// GetBatteryStatus() :
  #define POWER_AC_ST                     0x30
  #define POWER_BT_ST                     0x31
  #define BATTERY_ST_H                    0x30
  #define BATTERY_ST_M                    0x31
  #define BATTERY_ST_L                    0x32
  #define BATTERY_ST_S                    0x33
  #define BATTERY_ST_N                    0x34

///////////////////////////////////////////////////////////////////////////////
// BiOpenMonPrinter() :
// Type
  #define TYPE_PORT                       1 // use port name
  #define TYPE_PRINTER                    2 // use printer driver name

///////////////////////////////////////////////////////////////////////////////
// BiOpenDrawer() :
// Drawer Number
  #define EPS_BI_DRAWER_1                 1 // Drawer 1
  #define EPS_BI_DRAWER_2                 2 // Drawer 2
// Start Time
  #define EPS_BI_PULSE_100                1 // 100 m sec
  #define EPS_BI_PULSE_200                2 // 200 m sec
  #define EPS_BI_PULSE_300                3 // 300 m sec
  #define EPS_BI_PULSE_400                4 // 400 m sec
  #define EPS_BI_PULSE_500                5 // 500 m sec
  #define EPS_BI_PULSE_600                6 // 600 m sec
  #define EPS_BI_PULSE_700                7 // 700 m sec
  #define EPS_BI_PULSE_800                8 // 800 m sec

///////////////////////////////////////////////////////////////////////////////
// BiSCNSetImageQuality() :
// Scale Option
  #define EPS_BI_SCN_1BIT                 1 // Monochrome
  #define EPS_BI_SCN_8BIT                 8 // Gray scale

// Color Option
  #define EPS_BI_SCN_MONOCHROME          48 // Monochrome
  #define EPS_BI_SCN_COLOR               49 // Color
// Extensive Option
  #define EPS_BI_SCN_AUTO                48 // Auto
  #define EPS_BI_SCN_MANUAL              49 // Manual
  #define EPS_BI_SCN_SHARP               50 // Sharpness

///////////////////////////////////////////////////////////////////////////////
// BiSCNSetImageFormat() :
// Format Option
  #define EPS_BI_SCN_TIFF                 1 // TIFF format(CCITT Group 4)
  #define EPS_BI_SCN_RASTER               2 // Raster Image
  #define EPS_BI_SCN_BITMAP               3 // Bitmap
  #define EPS_BI_SCN_TIFF256              4 // TIFF format(Glay scale)
  #define EPS_BI_SCN_JPEGHIGH             5 // Jpeg format(High complessed)
  #define EPS_BI_SCN_JPEGNORMAL           6 // Jpeg format(Normal)
  #define EPS_BI_SCN_JPEGLOW              7 // Jpeg format(Low complessed)

///////////////////////////////////////////////////////////////////////////////
// BiSCNReadImage() :
// Select Sheet Option
  #define EPS_BI_SCN_JOURNAL              1 // Select Journal
  #define EPS_BI_SCN_RECEIPT              2 // Select Receipt
  #define EPS_BI_SCN_SLIP                 4 // Select Slip (3.03)
  #define EPS_BI_SCN_CARD                16 // Select Card
  #define EPS_BI_SCN_CHECKPAPER          32 // Select Check Paper
// Timing Option
  #define EPS_BI_SCN_TRANSMIT_BATCH      48 // Batch mode
  #define EPS_BI_SCN_TRANSMIT_SEQUENCE   49 // Sequence mode
// Memory Option
  #define EPS_BI_SCN_NVMEMORY_NOTSAVE    48 // Not save (save to work area)
  #define EPS_BI_SCN_NVMEMORY_SAVE       49 // Save

///////////////////////////////////////////////////////////////////////////////
// BiSCNSelectScanUnit() :
// Scan unit ID
  #define EPS_BI_SCN_UNIT_CHECKPAPER     48 // Checkpaper scanner unit
  #define EPS_BI_SCN_UNIT_CARD           49 // Card scanner unit
///////////////////////////////////////////////////////////////////////////////
// BiESCNEnable() :
// Saved Method
  #define CROP_STORE_MEMORY               0 // Saved memory
  #define CROP_STORE_FILE                 1 // Saved file

///////////////////////////////////////////////////////////////////////////////
// BiESCNSetAutoSize() :
// Enable/Disable Autosize
  #define CROP_AUTOSIZE_DISABLE           0 // Enabled auto size
  #define CROP_AUTOSIZE_ENABLE            1 // Disabled auto size

///////////////////////////////////////////////////////////////////////////////
// BiESCNSetRotate() :
// Enable/Disable Rotate
  #define CROP_ROTATE_DISABLE             0 // Enabled rotate
  #define CROP_ROTATE_ENABLE              1 // Disabled rotate

///////////////////////////////////////////////////////////////////////////////
// BiESCNDefineCropArea() :
// CropArea Setting
  #define CROP_AREA_RESET_ALL             0 // Delete all crop area
  #define CROP_AREA_ENTIRE_IMAGE          1 // CropArea entire image
  #define CROP_AREA_RIGHT             65535 // End X Point
  #define CROP_AREA_BOTTOM            65535 // End Y Point

///////////////////////////////////////////////////////////////////////////////
// BiESCNClearImage() :
// Delete Method
  #define CROP_CLEAR_ALL_IMAGE            0 // Delete all image
  #define CROP_CLEAR_BY_FILEINDEX         1 // Clear by fileIndex
  #define CROP_CLEAR_BY_FILEID            2 // Clear by fileId
  #define CROP_CLEAR_BY_IMAGETAGDATA      3 // by imageTagData

///////////////////////////////////////////////////////////////////////////////
// BiAutoPowerOffTime() :
  #define EPS_BI_GET 0
  #define EPS_BI_SET 1


/* Druckerport = "ESDPRT001:" */

#define EXPORT   // WeU: work-around for incompatible Win-SDK

typedef int (WINAPI* DLL_BiOpenMonPrinter)(int, LPSTR);
typedef int (WINAPI* DLL_BiSetMonInterval)(int, WORD, WORD);
typedef int (WINAPI* DLL_BiGetStatus)(int, LPDWORD);
typedef int (WINAPI* DLL_BiSetStatusBackFunction)(int, int (CALLBACK EXPORT *pStatusCB)(DWORD dwStatus));
typedef int (WINAPI* DLL_BiSetStatusBackWnd)(int, long, LPDWORD);
typedef int (WINAPI* DLL_BiCancelStatusBack)(int );
typedef int (WINAPI* DLL_BiDirectIO)(int, BYTE, LPBYTE, LPBYTE, LPBYTE, DWORD, BOOL);
typedef int (WINAPI* DLL_BiDirectIOEx)(int, DWORD, LPBYTE, LPDWORD, LPBYTE, DWORD, BOOL, BYTE);
typedef int (WINAPI* DLL_BiResetPrinter)(int);
typedef int (WINAPI* DLL_BiGetCounter)(int, WORD, LPDWORD);
typedef int (WINAPI* DLL_BiResetCounter)(int, WORD);
typedef int (WINAPI* DLL_BiCancelError)(int);
typedef int (WINAPI* DLL_BiGetType)(int, LPBYTE, LPBYTE, LPBYTE, LPBYTE);
typedef int (WINAPI* DLL_BiGetOfflineCode)(int, LPBYTE);
typedef int (WINAPI* DLL_BiGetInkStatus)(int, LPWORD);
typedef int (WINAPI* DLL_BiSetInkStatusBackFunction)(int, int (CALLBACK EXPORT *pInkCB)(WORD wStatus));
typedef int (WINAPI* DLL_BiSetInkStatusBackWnd)(int, long, LPDWORD);
typedef int (WINAPI* DLL_BiCancelInkStatusBack)(int);
typedef int (WINAPI* DLL_BiMICRSetReadBackFunction)(int, int (CALLBACK EXPORT *pMicrCB)(void), LPBYTE, LPBYTE, LPBYTE, LPBYTE);
typedef int (WINAPI* DLL_BiMICRSetReadBackWnd)(int, long, LPBYTE, LPBYTE, LPBYTE, LPBYTE);
typedef int (WINAPI* DLL_BiMICRSelectDataHandling)(int, BYTE, BYTE, BYTE);
typedef int (WINAPI* DLL_BiMICRReadCheck)(int, BYTE, BYTE);
typedef int (WINAPI* DLL_BiMICRCancelWaitChaeckInsertion)(int);
typedef int (WINAPI* DLL_BiMICRCancelWaitCheckInsertion)(int);
typedef int (WINAPI* DLL_BiMICRCancelReadBack)(int);
typedef int (WINAPI* DLL_BiMICRRetransmissionCheckData)(int, LPBYTE, LPBYTE, LPBYTE, LPBYTE, DWORD);
typedef int (WINAPI* DLL_BiMICRGetStatus)(int, LPBYTE);
typedef int (WINAPI* DLL_BiMICREjectCheck)(int);
typedef int (WINAPI* DLL_BiMICRLoadCheck)(int);
typedef int (WINAPI* DLL_BiMICRCleaning)(int);
typedef int (WINAPI* DLL_BiOpenDrawer)(int, BYTE, BYTE);
typedef int (WINAPI* DLL_BiSetMonEtherInterval)(int, WORD);
typedef int (WINAPI* DLL_BiSetDefaultEchoTime)(BYTE, WORD);
typedef int (WINAPI* DLL_BiSetEtherEchoTime)(int, BYTE, WORD);
typedef int (WINAPI* DLL_BiEjectSheet)(int);
typedef int (WINAPI* DLL_BiRetractSheet)(int);
typedef int (WINAPI* DLL_BiSetPresenterTimeOut)(int, BYTE, BYTE);
typedef int (WINAPI* DLL_BiSetPresenterEventBackFunction)(int, int (CALLBACK EXPORT *pEventCB)(BYTE returncode));
typedef int (WINAPI* DLL_BiSetPresenterEventBackWnd)(int, long, LPBYTE);
typedef int (WINAPI* DLL_BiCancelPresenterEventBack)(int);
typedef int (WINAPI* DLL_BiCheckPrint)(int, LPBYTE);
typedef int (WINAPI* DLL_BiPreparePaperChange)(int);
typedef int (WINAPI* DLL_BiSCNSetImageQuality)(int, BYTE, char, BYTE, BYTE);
typedef int (WINAPI* DLL_BiSCNSetImageFormat)(int, BYTE);
typedef int (WINAPI* DLL_BiSCNSetScanArea)(int, BYTE, BYTE, BYTE, BYTE);
typedef int (WINAPI* DLL_BiSCNPreScan)(int, LPBYTE, char *);
typedef int (WINAPI* DLL_BiSCNGetImageQuality)(int, LPBYTE, char *, LPBYTE, LPBYTE);
typedef int (WINAPI* DLL_BiSCNGetImageFormat)(int, LPBYTE);
typedef int (WINAPI* DLL_BiSCNGetScanArea)(int, LPBYTE, LPBYTE, LPBYTE, LPBYTE);
typedef int (WINAPI* DLL_BiSCNSetReadBackFunction)(int, int (CALLBACK EXPORT *pScanCB)(void), LPDWORD, LPBYTE, LPBYTE, LPBYTE, LPBYTE);
typedef int (WINAPI* DLL_BiSCNSetReadBackWnd)(int, long, LPDWORD, LPBYTE, LPBYTE, LPBYTE, LPBYTE);
typedef int (WINAPI* DLL_BiSCNCancelReadBack)(int);
typedef int (WINAPI* DLL_BiSCNReadImage)(int, WORD, BYTE, BYTE, BYTE, LPBYTE, BYTE);
typedef int (WINAPI* DLL_BiSCNRetransmissionImage)(int, WORD, LPDWORD, LPBYTE, LPBYTE, LPBYTE, LPBYTE, DWORD);
typedef int (WINAPI* DLL_BiSCNGetClumpStatus)(int, LPBYTE);
typedef int (WINAPI* DLL_BiSCNClumpPaper)(int);
typedef int (WINAPI* DLL_BiSCNSetCroppingArea)(int, BYTE, BYTE, BYTE, BYTE, BYTE);
typedef int (WINAPI* DLL_BiSCNGetCroppingArea)(int, LPWORD, LPBYTE);
typedef int (WINAPI* DLL_BiSCNDeleteCroppingArea)(int, BYTE);
typedef int (WINAPI* DLL_BiSCNDeleteImage)(int, WORD);
typedef int (WINAPI* DLL_BiSCNGetImageList)(int, LPWORD, LPWORD);
typedef int (WINAPI* DLL_BiSCNGetImageRemainingCapacity)(int, LPDWORD);
typedef int (WINAPI* DLL_BiGetPrnCapability)(int, BYTE, LPBYTE, LPBYTE);
typedef int (WINAPI* DLL_BiCloseMonPrinter)(int );
typedef int (WINAPI* DLL_BiSCNSelectScanUnit)(int, BYTE);
typedef int (WINAPI* DLL_BiGetRealStatus)(int, LPWORD);
typedef int (WINAPI* DLL_BiSendDataFile)(int, LPCSTR);
typedef int (WINAPI* DLL_BiDirectSendRead)(int, LPCSTR, LPCSTR, LPDWORD, LPBYTE, DWORD, BOOL);
typedef int (WINAPI* DLL_BiSetStatusBackFunctionEx)(int, int (CALLBACK EXPORT *pStatusCB)(DWORD dwStatus, LPSTR lpcPortName));
typedef int (WINAPI* DLL_BiSetInkStatusBackFunctionEx)(int, int (CALLBACK EXPORT *pStatusCB)(WORD wStatus, LPSTR lpPortName));
typedef int (WINAPI* DLL_BiSetPresenterEventBackFunctionEx)(int, int (CALLBACK EXPORT *pEventCB)(BYTE returncode, LPSTR lpPortName));
typedef int (WINAPI* DLL_BiMICRSetReadBackFunctionEx)(int, int (CALLBACK EXPORT *pMicrCB)(LPSTR lpPortName), LPBYTE, LPBYTE, LPBYTE, LPBYTE pDetail);
typedef int (WINAPI* DLL_BiSCNSetReadBackFunctionEx)(int, int (CALLBACK EXPORT *pScnCB)(LPSTR lpPortName), LPDWORD, LPBYTE, LPBYTE, LPBYTE, LPBYTE);
typedef int (WINAPI* DLL_BiGetBatteryStatus)(int, LPBYTE, LPBYTE);
typedef int (WINAPI* DLL_BiSetBatteryStatusBackFunctionEx)(int, int (CALLBACK EXPORT *pBattCB)(BYTE, BYTE, LPSTR));
typedef int (WINAPI* DLL_BiSetBatteryStatusBackWndEx)(int, long, LPBYTE, LPBYTE, LPSTR);
typedef int (WINAPI* DLL_BiCancelBatteryStatusBack)(int);
typedef int (WINAPI* DLL_BiAutoPowerOffTime)(int, BYTE, LPBYTE);
typedef int (WINAPI* DLL_BiPowerOff)(int);
typedef int (WINAPI* DLL_BiESCNEnable)(BYTE);
typedef int (WINAPI* DLL_BiESCNGetAutoSize)(int, LPBYTE);
typedef int (WINAPI* DLL_BiESCNSetAutoSize)(int, BYTE);
typedef int (WINAPI* DLL_BiESCNGetCutSize)(int, LPWORD);
typedef int (WINAPI* DLL_BiESCNSetCutSize)(int, WORD);
typedef int (WINAPI* DLL_BiESCNGetRotate)(int, LPBYTE);
typedef int (WINAPI* DLL_BiESCNSetRotate)(int, BYTE);
typedef int (WINAPI* DLL_BiESCNGetDocumentSize)(int, LPWORD, LPWORD);
typedef int (WINAPI* DLL_BiESCNSetDocumentSize)(int, WORD, WORD);
typedef int (WINAPI* DLL_BiESCNDefineCropArea)(int, BYTE, WORD, WORD, WORD, WORD);
typedef int (WINAPI* DLL_BiESCNGetMaxCropAreas)(int, LPBYTE);
typedef int (WINAPI* DLL_BiESCNStoreImage)(int, DWORD, LPSTR, LPSTR, BYTE);
typedef int (WINAPI* DLL_BiESCNRetrieveImage)(int, DWORD, LPSTR, LPSTR, LPDWORD, LPBYTE);
typedef int (WINAPI* DLL_BiESCNClearImage)(int, BYTE, DWORD, LPSTR, LPSTR);
typedef int (WINAPI* DLL_BiESCNGetRemainingImages)(int, LPBYTE);
typedef int (WINAPI* DLL_BiSCNMICRFunction)(int iHandle, LPVOID lpvStruct, WORD wFunction);
typedef int (WINAPI* DLL_BiSCNMICRCancelFunction)(int iHandle, WORD wEjectType);
typedef int (WINAPI* DLL_BiSCNSelectScanFace)(int iHandle, BYTE bFace);

#endif // !EPSSTMAPI_H_INCLUDED
