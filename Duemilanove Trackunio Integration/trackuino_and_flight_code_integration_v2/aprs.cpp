/* 
 *  Edited by Omkar Savant on 11.18.15
 *  
 *  Description: Creates a char array for our comment based on the serial 
 *  input from the Mega, converts the input string to a char array and sends
 */

#include "config.h"
#include "ax25.h"
#include "gps.h"
#include "aprs.h"
#include "sensors_avr.h"
#include "sensors_pic32.h"
#include <stdio.h>
#include <stdlib.h>
#if (ARDUINO + 1) >= 100
#  include <Arduino.h>
#else
#  include <WProgram.h>
#endif

// Module functions
float meters_to_feet(float m)
{
  // 10000 ft = 3048 m
  return m / 0.3048;
}

// Exported functions
void aprs_send(String ourString)
{
  char temp[12];                   // Temperature (int/ext)
  char ourComment[50]; //The imu comment we are sending 
  
  const struct s_address addresses[] = { 
    {D_CALLSIGN, D_CALLSIGN_ID},  // Destination callsign
    {S_CALLSIGN, S_CALLSIGN_ID},  // Source callsign (-11 = balloon, -9 = car)
#ifdef DIGI_PATH1
    {DIGI_PATH1, DIGI_PATH1_TTL}, // Digi1 (first digi in the chain)
#endif
#ifdef DIGI_PATH2
    {DIGI_PATH2, DIGI_PATH2_TTL}, // Digi2 (second digi in the chain)
#endif
  };

  ax25_send_header(addresses, sizeof(addresses)/sizeof(s_address));
  ax25_send_byte('/');                // Report w/ timestamp, no APRS messaging. $ = NMEA raw data
  // ax25_send_string("021709z");     // 021709z = 2nd day of the month, 17:09 zulu (UTC/GMT)
  ax25_send_string(gps_time);         // 170915 = 17h:09m:15s zulu (not allowed in Status Reports)
  ax25_send_byte('h');
  ax25_send_string(gps_aprs_lat);     // Lat: 38deg and 22.20 min (.20 are NOT seconds, but 1/100th of minutes)
  ax25_send_byte('/');                // Symbol table
  ax25_send_string(gps_aprs_lon);     // Lon: 000deg and 25.80 min
  ax25_send_byte('O');                // Symbol: O=balloon, -=QTH
  snprintf(temp, 4, "%03d", (int)(gps_course + 0.5)); 
  ax25_send_string(temp);             // Course (degrees)
  ax25_send_byte('/');                // and
  snprintf(temp, 4, "%03d", (int)(gps_speed + 0.5));
  ax25_send_string(temp);             // speed (knots)
  ax25_send_string("/A=");            // Altitude (feet). Goes anywhere in the comment area
  snprintf(temp, 7, "%06ld", (long)(meters_to_feet(gps_altitude) + 0.5));
  ax25_send_string(temp);
  ax25_send_string("/Ti=");
  snprintf(temp, 6, "%d", sensors_int_lm60());
  ax25_send_string(temp);
  ax25_send_string("/Te=");
  snprintf(temp, 6, "%d", sensors_ext_lm60());
  ax25_send_string(temp);
  ax25_send_string("/V=");
  snprintf(temp, 6, "%d", sensors_vin());
  ax25_send_string(temp);
  ax25_send_byte(' ');
  snprintf(ourComment, 50, "%d", ourString.c_str()); 
  ax25_send_string(ourComment);     // Comment
  ax25_send_footer();

  ax25_flush_frame();                 // Tell the modem to go
}



