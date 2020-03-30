/*******************************************************************
 *
 *    PROJECT:     LibDutCom
 *
 *    EQUIPMENT:   .
 *
 *    FILE:        DeviceSpecific.h
 *
 *    DESCRIPTION: prototypes for private functions to gather device
 *                 specific settings and values
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

#ifndef DEVICE_SPECIFIC_H_INCLUDED
  #define DEVICE_SPECIFIC_H_INCLUDED

  #include <windows.h>
  #include "LibDutCom_Interface.h"


  #define LIB_DUT_INFO       "CapaTankGeber NMEA2k"
  #define LIB_DUT_ID        55

  #define VERSION_MAJOR       0
  #define VERSION_MINOR       800

  /**
   * WARN_NOT_IMPLEMENTED_FUNCTIONS:
   * Print a warning message to stdio if a function is called which is not implemented
   *
   * Important: undef before release to production
   */
  //#define WARN_NOT_IMPLEMENTED_FUNCTIONS


/**
 * Function Prototypes
 */
  #include "../../0000/current/DeviceSpecific_0000.h"


/**
 * Number of pointers.
 */
  #define NUM_POINTERS 0

/**
 * Number of tell-tales.
 */
  #define NUM_TELLTALES 0

/**
 * Number of lcds.
 */
  #define NUM_LCDS     0

/**
 * Number of digital Inputs.
 */
  #define NUM_DIG_INPUTS     1

/**
 * Number of digital Outputs.
 */
  #define NUM_DIG_OUTPUTS     0

/**
 * Number of analog Inputs.
 */
  #define NUM_ANALOG_INPUTS     1

/**
 * Number of analog Outputs.
 */
  #define NUM_ANALOG_OUTPUTS     0

/**
 * Number of frequency Inputs.
 */
  #define NUM_FREQ_INPUTS     0

/**
 * Number of frequency Outputs.
 */
  #define NUM_FREQ_OUTPUTS     0

/**
 * Number of individually controllable illuminations.
 */
  #define NUM_BACKLIGHT_ILLUMINATIONS  0

/**
 * Number of individually controllable dial colors (on / off = 1, rgb = 3).
 * define to 0 if not existing
 */
  #define NUM_BACKLIGHT_ILLUMINATION_COLORS_0  0
  #define NUM_BACKLIGHT_ILLUMINATION_COLORS_1  0
  #define NUM_BACKLIGHT_ILLUMINATION_COLORS_2  0
  #define NUM_BACKLIGHT_ILLUMINATION_COLORS_3  0
  #define NUM_BACKLIGHT_ILLUMINATION_COLORS_4  0
  #define NUM_BACKLIGHT_ILLUMINATION_COLORS_5  0
  #define NUM_BACKLIGHT_ILLUMINATION_COLORS_6  0
  #define NUM_BACKLIGHT_ILLUMINATION_COLORS_7  0
  #define NUM_BACKLIGHT_ILLUMINATION_COLORS_8  0
  #define NUM_BACKLIGHT_ILLUMINATION_COLORS_9  0

/**
 * Number of individually controllable lcd colors (on / off = 1, rgb = 3).
 */
  #define NUM_BACKLIGHT_COLORS_LCD  0

/**
 * Number of different lcd test patterns (excluding pattern "All Segments").
 * define to 0 if not existing
 */
  #define NUM_LCD_TEST_PATTERN_0     0
  #define NUM_LCD_TEST_PATTERN_1     0
  #define NUM_LCD_TEST_PATTERN_2     0
  #define NUM_LCD_TEST_PATTERN_3     0

/**
 * Number of tools for Production-data-interlock
 */
  #define NUM_PROD_INTERLOCK_TOOLS   1

/**
 * Number of device specific self running tests
 */
  #define NUM_DEV_SPEC_TESTS     0


#endif // DEVICE_SPECIFIC_H_INCLUDED
