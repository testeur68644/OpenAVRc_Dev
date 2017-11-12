 /*
 **************************************************************************
 *                                                                        *
 *                 ____                ___ _   _____                      *
 *                / __ \___  ___ ___  / _ | | / / _ \____                 *
 *               / /_/ / _ \/ -_) _ \/ __ | |/ / , _/ __/                 *
 *               \____/ .__/\__/_//_/_/ |_|___/_/|_|\__/                  *
 *                   /_/                                                  *
 *                                                                        *
 *              This file is part of the OpenAVRc project.                *
 *                                                                        *
 *                         Based on code(s) named :                       *
 *             OpenTx - https://github.com/opentx/opentx                  *
 *             Deviation - https://www.deviationtx.com/                   *
 *                                                                        *
 *                Only AVR code here for visibility ;-)                   *
 *                                                                        *
 *   OpenAVRc is free software: you can redistribute it and/or modify     *
 *   it under the terms of the GNU General Public License as published by *
 *   the Free Software Foundation, either version 2 of the License, or    *
 *   (at your option) any later version.                                  *
 *                                                                        *
 *   OpenAVRc is distributed in the hope that it will be useful,          *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 *   GNU General Public License for more details.                         *
 *                                                                        *
 *       License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html          *
 *                                                                        *
 **************************************************************************
*/

#include "FrSkySimu.h"

FrSkySportSensorAss ass;                               // Create ASS sensor with default ID
FrSkySportSensorFcs fcs;                               // Create FCS-40A sensor with default ID (use ID8 for FCS-150A)
FrSkySportSensorFlvss flvss1;                          // Create FLVSS sensor with default ID
FrSkySportSensorFlvss flvss2(FrSkySportSensor::ID15);  // Create FLVSS sensor with given ID
FrSkySportSensorGps gps;                               // Create GPS sensor with default ID
FrSkySportSensorRpm rpm;                               // Create RPM sensor with default ID
FrSkySportSensorSp2uart sp2uart;                       // Create SP2UART Type B sensor with default ID
FrSkySportSensorVario vario;                           // Create Variometer sensor with default ID

//FrSkySportTelemetry telemetry;                         // Create telemetry object without polling
FrSkySportTelemetry telemetry(true);                 // Create telemetry object with polling


void frskySimuSetup()
{
  // Configure the telemetry serial port and sensors (remember to use & to specify a pointer to sensor)
  telemetry.begin( &ass, &fcs, &flvss1, &flvss2, &gps, &rpm, &sp2uart, &vario);
}

void frskySimuloop()
{
#define POLLING_ENABLED

  // Set airspeed sensor (ASS) data
  ass.setData(76.5);  // Airspeed in km/h

  // Set current/voltage sensor (FCS) data
  // (set Voltage source to FAS in menu to use this data for battery voltage,
  //  set Current source to FAS in menu to use this data for current readins)
  fcs.setData(25.3,   // Current consumption in amps
              12.6);  // Battery voltage in volts

  // Set LiPo voltage sensor (FLVSS) data (we use two sensors to simulate 8S battery
  // (set Voltage source to Cells in menu to use this data for battery voltage)
  flvss1.setData(4.07, 4.08, 4.09, 4.10, 4.11, 4.12);  // Cell voltages in volts (cells 1-6)
  flvss2.setData(4.13, 4.14);                          // Cell voltages in volts (cells 7-8)

  // Set GPS data
  gps.setData(48.858289, 2.294502,   // Latitude and longitude in degrees decimal (positive for N/E, negative for S/W)
              245.5,                 // Altitude in m (can be negative)
              100.0,                 // Speed in m/s
              90.23,                 // Course over ground in degrees (0-359, 0 = north)
              14, 9, 14,             // Date (year - 2000, month, day)
              12, 00, 00);           // Time (hour, minute, second) - will be affected by timezone setings in your radio

  // Set RPM/temperature sensor data
  // (set number of blades to 2 in telemetry menu to get correct rpm value)
  rpm.setData(200,    // Rotations per minute
              25.6,   // Temperature #1 in degrees Celsuis (can be negative, will be rounded)
              -7.8);  // Temperature #2 in degrees Celsuis (can be negative, will be rounded)

  // Set SP2UART sensor data
  // (values from 0.0 to 3.3 are accepted)
  sp2uart.setData(1.5,    // ADC3 voltage in volts
                  3.3);   // ADC4 voltage in volts

 // Set variometer data
  // (set Variometer source to VSpd in menu to use the vertical speed data from this sensor for variometer).
  vario.setData(250.5,  // Altitude in meters (can be negative)
                -1.5);  // Vertical speed in m/s (positive - up, negative - down)

  // Send the telemetry data, note that the data will only be sent for sensors
  // that are being polled at given moment
  telemetry.send();
}


