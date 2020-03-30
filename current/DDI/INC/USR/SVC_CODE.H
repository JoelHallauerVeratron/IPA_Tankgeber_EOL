/*****************************************************************************
**                                                                          **
**              PHOENIX CONTACT GmbH & Co., W-32819 Blomberg                **
**                  Copyright Phoenix Contact 1996                          **
**                                                                          **
******************************************************************************
**                                                                          **
** Project      : IBS Controller Boards                                     **
** Sub-Project  : --                                                        **
**                                                                          **
*****************************************************************************/
/*
** File         : SVC_CODE.H
** Component(s) : -
** Device(s)    : independent
**              :
**         $Date:   05 Oct 2001 08:39:18  $
**              :
**     $Revision:   1.5  $
** Status       : -
**              :
** Description  : Codes for ibs master services
**              :
******************************************************************************
** Change Notes :
**
** Date      Version  Author        Description
** ---------------------------------------------------------------------------
**
*/

#ifndef INCL_SVC_CODES
  #define INCL_SVC_CODES

  #define S_GET_ERROR_INFO_REQ               0x0316
  #define S_GET_ERROR_INFO_CNF               0x8316
  #define S_CONFIRM_DIAG_REQ                 0x0760
  #define S_CONFIRM_DIAG_CNF                 0x8760
  #define S_GET_REVISION_INFO_REQ            0x032A
  #define S_GET_REVISION_INFO_CNF            0x832A
  #define S_GET_DIAG_INFO_REQ                0x032B
  #define S_GET_DIAG_INFO_CNF                0x832B

  #define S_START_DATA_TRANSFER_REQ          0x0701
  #define S_START_DATA_TRANSFER_CNF          0x8701
  #define S_STOP_DATA_TRANSFER_REQ           0x0702
  #define S_STOP_DATA_TRANSFER_CNF           0x8702
  #define S_ALARM_STOP_REQ                   0x1303
  #define S_ALARM_STOP_CNF                   0x9303

  #define S_ACTIVATE_CFG_REQ                 0x0711
  #define S_ACTIVATE_CFG_CNF                 0x8711
  #define S_DEACTIVATE_CFG_REQ               0x0712
  #define S_DEACTIVATE_CFG_CNF               0x8712
  #define S_CONTROL_ACTIVE_CFG_REQ           0x0713
  #define S_CONTROL_ACTIVE_CFG_CNF           0x8713
  #define S_CONTROL_DEVICE_FUNC_REQ          0x0714
  #define S_CONTROL_DEVICE_FUNC_CNF          0x8714
  #define S_READ_DEVICE_STATE_REQ            0x0315
  #define S_READ_DEVICE_STATE_CNF            0x8315

  #define S_INIT_LOAD_CFG_REQ                0x0306
  #define S_INIT_LOAD_CFG_CNF                0x8306
  #define S_LOAD_CFG_REQ                     0x0307
  #define S_LOAD_CFG_CNF                     0x8307
  #define S_TERM_LOAD_CFG_REQ                0x0308
  #define S_TERM_LOAD_CFG_CNF                0x8308
  #define S_READ_CFG_REQ                     0x0309
  #define S_READ_CFG_CNF                     0x8309
  #define S_COMPLETE_LOAD_CFG_REQ            0x030A
  #define S_COMPLETE_LOAD_CFG_CNF            0x830A
  #define S_COMPLETE_READ_CFG_REQ            0x030B
  #define S_COMPLETE_READ_CFG_CNF            0x830B
  #define S_DELETE_CFG_REQ                   0x030C
  #define S_DELETE_CFG_CNF                   0x830C
  #define S_SWITCH_CFG_REQ                   0x030D
  #define S_SWITCH_CFG_CNF                   0x830D
  #define S_CONTROL_PARAM_REQ                0x030E
  #define S_CONTROL_PARAM_CNF                0x830E
  #define S_CONTROL_STATISTICS_REQ           0x030F
  #define S_CONTROL_STATISTICS_CNF           0x830F

  #define S_INIT_PUT_PD_OV_REQ               0x0320
  #define S_INIT_PUT_PD_OV_CNF               0x8320
  #define S_PUT_PD_OV_REQ                    0x0321
  #define S_PUT_PD_OV_CNF                    0x8321
  #define S_TERM_PUT_PD_OV_REQ               0x0322
  #define S_TERM_PUT_PD_OV_CNF               0x8322
  #define S_GET_PD_OV_REQ                    0x0323
  #define S_GET_PD_OV_CNF                    0x8323
  #define S_INIT_LOAD_PDRL_REQ               0x0324
  #define S_INIT_LOAD_PDRL_CNF               0x8324
  #define S_LOAD_PDRL_REQ                    0x0325
  #define S_LOAD_PDRL_CNF                    0x8325
  #define S_TERM_LOAD_PDRL_REQ               0x0326
  #define S_TERM_LOAD_PDRL_CNF               0x8326
  #define S_READ_PDRL_REQ                    0x0327
  #define S_READ_PDRL_CNF                    0x8327
  #define S_COMPACT_LOAD_PDRL_REQ            0x0328
  #define S_COMPACT_LOAD_PDRL_CNF            0x8328
  #define S_COMPACT_READ_PDRL_REQ            0x0329
  #define S_COMPACT_READ_PDRL_CNF            0x8329

  #define S_SET_VALUE_REQ                    0x0750
  #define S_SET_VALUE_CNF                    0x8750
  #define S_READ_VALUE_REQ                   0x0351
  #define S_READ_VALUE_CNF                   0x8351

  #define S_CREATE_CFG_REQ                   0x0710
  #define S_CREATE_CFG_CNF                   0x8710

  #define S_RESET_CONTROLLER_REQ             0x0956

  #define S_GENERATE_ABORTS_REQ              0x0957

  #define S_INIT_LOAD_AH_OBJECT_REQ          0x0140
  #define S_INIT_LOAD_AH_OBJECT_CNF          0x8140
  #define S_LOAD_AH_OBJECT_REQ               0x0141
  #define S_LOAD_AH_OBJECT_CNF               0x8141
  #define S_TERMINATE_LOAD_AH_OBJECT_REQ     0x0142
  #define S_TERMINATE_LOAD_AH_OBJECT_CNF     0x8142
  #define S_READ_AH_OBJECT_REQ               0x0143
  #define S_READ_AH_OBJECT_CNF               0x8143
  #define S_DELETE_AH_OBJECT_REQ             0x0144
  #define S_DELETE_AH_OBJECT_CNF             0x8144
  #define S_AH_SET_VALUE_REQ                 0x0352
  #define S_AH_SET_VALUE_CNF                 0x8352
  #define S_AH_READ_VALUE_REQ                0x0353
  #define S_AH_READ_VALUE_CNF                0x8353

  #define S_INIT_LOAD_SH_OBJECT_REQ          0x0145
  #define S_INIT_LOAD_SH_OBJECT_CNF          0x8145
  #define S_LOAD_SH_OBJECT_REQ               0x0146
  #define S_LOAD_SH_OBJECT_CNF               0x8146
  #define S_TERMINATE_LOAD_SH_OBJECT_REQ     0x0147
  #define S_TERMINATE_LOAD_SH_OBJECT_CNF     0x8147
  #define S_READ_SH_OBJECT_REQ               0x0148
  #define S_READ_SH_OBJECT_CNF               0x8148
  #define S_DELETE_SH_OBJECT_REQ             0x0149
  #define S_DELETE_SH_OBJECT_CNF             0x8149

  #define S_INIT_LOAD_EH_OBJECT_REQ          0x014A
  #define S_INIT_LOAD_EH_OBJECT_CNF          0x814A
  #define S_LOAD_EH_OBJECT_REQ               0x014B
  #define S_LOAD_EH_OBJECT_CNF               0x814B
  #define S_TERMINATE_LOAD_EH_OBJECT_REQ     0x014C
  #define S_TERMINATE_LOAD_EH_OBJECT_CNF     0x814C
  #define S_READ_EH_OBJECT_REQ               0x014D
  #define S_READ_EH_OBJECT_CNF               0x814D
  #define S_DELETE_EH_OBJECT_REQ             0x014E
  #define S_DELETE_EH_OBJECT_CNF             0x814E

  #define S_MH_SET_VALUE_REQ                 0x0354
  #define S_MH_SET_VALUE_CNF                 0x8354
  #define S_MH_READ_VALUE_REQ                0x0355
  #define S_MH_READ_VALUE_CNF                0x8355

  #define S_CHANGE_EXCLUSIVE_RIGHTS_REQ      0x014F
  #define S_CHANGE_EXCLUSIVE_RIGHTS_CNF      0x814F
  #define S_FETCH_EXCLUSIVE_RIGHTS_REQ       0x0156
  #define S_FETCH_EXCLUSIVE_RIGHTS_CNF       0x8156

  #define S_SET_ONLINE_MODUS_REQ             0x0150
  #define S_SET_ONLINE_MODUS_CNF             0x8150

  #define S_DDI_V24_SET_VALUE_REQ            0x0151
  #define S_DDI_V24_SET_VALUE_CNF            0x8151
  #define S_DDI_READ_VALUE_REQ               0x0155
  #define S_DDI_READ_VALUE_CNF               0x8155

  #define S_SET_INDICATION_REQ               0x0152
  #define S_SET_INDICATION_CNF               0x8152

  #define S_WRITE_MPM_REQ                    0x0330
  #define S_WRITE_MPM_CNF                    0x8330
  #define S_READ_MPM_REQ                     0x0331
  #define S_READ_MPM_CNF                     0x8331
  #define S_WRITE_BYTE_MPM_REQ               0x0153
  #define S_WRITE_BYTE_MPM_CNF               0x8153
  #define S_READ_BYTE_MPM_REQ                0x0154
  #define S_READ_BYTE_MPM_CNF                0x8154

  #define S_SWITCH_VAR_ID_ACCESS_REQ         0x0157
  #define S_SWITCH_VAR_ID_ACCESS_CNF         0x8157
  #define S_SAVE_ACTION_REQ                  0x0158
  #define S_SAVE_ACTION_CNF                  0x8158

  #define S_CLEAR_RAM_CARD_REQ               0x0159
  #define S_CLEAR_RAM_CARD_CNF               0x8159
  #define S_SWITCH_ON_OFF_RAM_CARD_REQ       0x015A
  #define S_SWITCH_ON_OFF_RAM_CARD_CNF       0x815A

  #define S_L2_ACCESS_REQ                    0x0332
  #define S_L2_ACCESS_CNF                    0x8332
  #define S_PHYSICAL_READ_REQ                0x0333
  #define S_PHYSICAL_READ_CNF                0x8333
  #define S_MANUAL_DIAGNOSIS_REQ             0x0334
  #define S_MANUAL_DIAGNOSIS_CNF             0x8334
  #define S_INDICATION_START                 0x0390

  #define S_PCP_INITIATE_REQ                 0x008B
  #define S_PCP_INITIATE_IND                 0x408B
  #define S_PCP_INITIATE_RSP                 0xC08B
  #define S_PCP_INITIATE_CNF                 0x808B
  #define S_PCP_ABORT_REQ                    0x088D
  #define S_PCP_ABORT_IND                    0x488D
  #define S_PCP_REJECT_IND                   0x488E
  #define S_PCP_IDENTIFY_REQ                 0x0087
  #define S_PCP_IDENTIFY_CNF                 0x8087
  #define S_PCP_STATUS_REQ                   0x0086
  #define S_PCP_STATUS_CNF                   0x8086
  #define S_PCP_GET_OV_REQ                   0x0088
  #define S_PCP_GET_OV_CNF                   0x8088
  #define S_PCP_READ_REQ                     0x0081
  #define S_PCP_READ_IND                     0x4081
  #define S_PCP_READ_RSP                     0xC081
  #define S_PCP_READ_CNF                     0x8081
  #define S_PCP_WRITE_REQ                    0x0082
  #define S_PCP_WRITE_IND                    0x4082
  #define S_PCP_WRITE_RSP                    0xC082
  #define S_PCP_WRITE_CNF                    0x8082
  #define S_PCP_INFO_REPORT_REQ              0x0885
  #define S_PCP_INFO_REPORT_IND              0x4885
  #define S_PCP_START_REQ                    0x0083
  #define S_PCP_START_IND                    0x4083
  #define S_PCP_START_RSP                    0xC083
  #define S_PCP_START_CNF                    0x8083
  #define S_PCP_STOP_REQ                     0x0084
  #define S_PCP_STOP_IND                     0x4084
  #define S_PCP_STOP_RSP                     0xC084
  #define S_PCP_STOP_CNF                     0x8084
  #define S_PCP_RESUME_REQ                   0x0089
  #define S_PCP_RESUME_IND                   0x4089
  #define S_PCP_RESUME_RSP                   0xC089
  #define S_PCP_RESUME_CNF                   0x8089
  #define S_PCP_RESET_REQ                    0x008A
  #define S_PCP_RESET_IND                    0x408A
  #define S_PCP_RESET_RSP                    0xC08A
  #define S_PCP_RESET_CNF                    0x808A
  #define S_PCP_INIT_LOAD_KBL_LOC_REQ        0x0200
  #define S_PCP_INIT_LOAD_KBL_LOC_CNF        0x8200
  #define S_PCP_LOAD_KBL_LOC_REQ             0x0201
  #define S_PCP_LOAD_KBL_LOC_CNF             0x8201
  #define S_PCP_TERM_LOAD_KBL_LOC_REQ        0x0202
  #define S_PCP_TERM_LOAD_KBL_LOC_CNF        0x8202
  #define S_PCP_READ_KBL_LOC_REQ             0x0203
  #define S_PCP_READ_KBL_LOC_CNF             0x8203
  #define S_PCP_SET_VALUE_LOC_REQ            0x0210
  #define S_PCP_SET_VALUE_LOC_CNF            0x8210
  #define S_PCP_READ_VALUE_LOC_REQ           0x0211
  #define S_PCP_READ_VALUE_LOC_CNF           0x8211
  #define S_PCP_READ_STATUS_LOC_REQ          0x0212
  #define S_PCP_READ_STATUS_LOC_CNF          0x8212
  #define S_PCP_PNM7_RESET_REQ               0x0A20
  #define S_PCP_PNM7_RESET_CNF               0x8A20
  #define S_PCP_PNM7_EVENT_IND               0x4A21
  #define S_PCP_PNM7_INITIATE_REQ            0x00A0
  #define S_PCP_PNM7_INITIATE_CNF            0x80A0
  #define S_PCP_PNM7_ABORT_REQ               0x08A1
  #define S_PCP_PNM7_ABORT_IND               0x48A1
  #define S_PCP_INIT_LOAD_KBL_REM_REQ        0x00A2
  #define S_PCP_INIT_LOAD_KBL_REM_IND        0x40A2
  #define S_PCP_INIT_LOAD_KBL_REM_RSP        0xC0A2
  #define S_PCP_INIT_LOAD_KBL_REM_CNF        0x80A2
  #define S_PCP_LOAD_KBL_REM_REQ             0x00A3
  #define S_PCP_LOAD_KBL_REM_CNF             0x80A3
  #define S_PCP_TERM_LOAD_KBL_REM_REQ        0x00A4
  #define S_PCP_TERM_LOAD_KBL_REM_CNF        0x80A4
  #define S_PCP_READ_KBL_REM_REQ             0x00A5
  #define S_PCP_READ_KBL_REM_CNF             0x80A5
  #define S_PCP_AUTO_LOAD_KBL_LOC_REQ        0x0240
  #define S_PCP_AUTO_LOAD_KBL_LOC_CNF        0x8240
  #define S_PCP_COMP_READ_KBL_LOC_REQ        0x0241
  #define S_PCP_COMP_READ_KBL_LOC_CNF        0x8241
  #define S_PCP_AUTO_LOAD_KBL_REM_REQ        0x00C0
  #define S_PCP_AUTO_LOAD_KBL_REM_CNF        0x80C0
  #define S_PCP_LOAD_NET_KBL_LOC_REQ         0x0260
  #define S_PCP_LOAD_NET_KBL_LOC_CNF         0x8260
  #define S_PCP_INIT_PUT_OV_LOC_REQ          0x0261
  #define S_PCP_INIT_PUT_OV_LOC_CNF          0x8261
  #define S_PCP_PUT_OV_LOC_REQ               0x0262
  #define S_PCP_PUT_OV_LOC_CNF               0x8262
  #define S_PCP_TERM_PUT_OV_LOC_REQ          0x0263
  #define S_PCP_TERM_PUT_OV_LOC_CNF          0x8263
  #define S_PCP_LOAD_KBL_PAR_LOC_REQ         0x0264
  #define S_PCP_LOAD_KBL_PAR_LOC_CNF         0x8264

  #define S_API_FAULT_IND                    0x4B58
  #define S_EXCEPTION_IND                    0x73FE
  #define S_DEVICE_STATE_IND                 0x5340
  #define S_FAULT_IND                        0x4341
  #define S_BUS_ERROR_IND                    0x6342


#endif /*INCL_SVC_CODES*/
