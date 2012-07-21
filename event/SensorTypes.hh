#ifndef SensorTypes_hh
#define SensorTypes_hh

#include <vector>

// expansion macro for enum value definition
#define ENUM_VALUE(name,assign) name assign,

/// declare the access function and define enum values
#define DECLARE_ENUM(EnumType,ENUM_DEF) \
  enum EnumType { \
    ENUM_DEF(ENUM_VALUE) \
  }; \
  const char* convertToString(EnumType); \
  EnumType convertFromString(const char*); \

#define SENSOR_TYPES(XX) \
  XX(TRD_PRESSURE, = 0) /*first of our own sensors*/ \
  XX(TOF_VOLTAGE,) \
  XX(HPE_0x3000_TEMP,) \
  XX(HPE_0x3001_TEMP,) \
  XX(HPE_0x3100_TEMP,) \
  XX(HPE_0x3101_TEMP,) \
  XX(HPE_0x3300_TEMP,) \
  XX(HPE_0x3301_TEMP,) \
  XX(HPE_0x3700_TEMP,) \
  XX(HPE_0x3701_TEMP,) \
  XX(HPE_0x6000_TEMP,) \
  XX(HPE_0x6001_TEMP,) \
  XX(HPE_0x6100_TEMP,) \
  XX(HPE_0x6101_TEMP,) \
  XX(HPE_0x6200_TEMP,) \
  XX(HPE_0x6201_TEMP,) \
  XX(HPE_0x6300_TEMP,) \
  XX(HPE_0x6301_TEMP,) \
  XX(HPE_0x6400_TEMP,) \
  XX(HPE_0x6401_TEMP,) \
  XX(HPE_0x6500_TEMP,) \
  XX(HPE_0x6501_TEMP,) \
  XX(HPE_0x6600_TEMP,) \
  XX(HPE_0x6601_TEMP,) \
  XX(HPE_0x6700_TEMP,) \
  XX(HPE_0x6701_TEMP,) \
  XX(HPE_0x7800_TEMP,) \
  XX(HPE_0x7801_TEMP,) \
  XX(HPE_0x7900_TEMP,) \
  XX(HPE_0x7901_TEMP,) \
  XX(HPE_0x7a00_TEMP,) \
  XX(HPE_0x7a01_TEMP,) \
  XX(HPE_0x7b00_TEMP,) \
  XX(HPE_0x7b01_TEMP,) \
  XX(HPE_0x7c00_TEMP,) \
  XX(HPE_0x7c01_TEMP,) \
  XX(HPE_0x7d00_TEMP,) \
  XX(HPE_0x7d01_TEMP,) \
  XX(HPE_0x7e00_TEMP,) \
  XX(HPE_0x7e01_TEMP,) \
  XX(HPE_0x7f00_TEMP,) \
  XX(HPE_0x7f01_TEMP,) \
  XX(TOF_1_TEMP,) \
  XX(TOF_2_TEMP,) \
  XX(TOF_3_TEMP,) \
  XX(TOF_4_TEMP,) \
  XX(TOF_5_TEMP,) \
  XX(TOF_6_TEMP,) \
  XX(TOF_7_TEMP,) \
  XX(TOF_8_TEMP,) \
  XX(TRIGGER_BOARD_TEMP,) \
  XX(TRACKER_1_VOLTAGE,) \
  XX(TRACKER_2_VOLTAGE,) \
  XX(TRACKER_3_VOLTAGE,) \
  XX(TRD_VOLTAGE,) \
  XX(CPU_TEMP,) \
  XX(PC_TEMP,) \
  XX(TDC_TEMP,) \
  XX(TRIGGER_RATE,) \
  XX(TRD_UFE_BOTTOM_COLD_TEMP,) \
  XX(TRD_UFE_TOP_COLD_TEMP,) \
  XX(TRACKER_UFE_BOTTOM_COLD_TEMP,) \
  XX(TRACKER_UFE_TOP_COLD_TEMP,) \
  XX(TRACKER_UFE_TOP_HOT_TEMP,) \
  XX(TRACKER_UFE_BOTTOM_HOT_TEMP,) \
  XX(TRD_UFE_TOP_HOT_TEMP,) \
  XX(TRD_UFE_BOTTOM_HOT_TEMP,) \
  XX(POWER_UFE_TEMP,) \
  XX(USB_BOARD_TEMP,) \
  XX(POWER_MID_TEMP,) \
  XX(TRD_TUBE_TOP_HOT_TEMP,) \
  XX(TRD_TUBE_TOP_COLD_TEMP,) \
  XX(TRD_GAS_COLD_TEMP,) \
  XX(TRD_GAS_HOT_TEMP,) \
  XX(TRD_TUBE_BOTTOM_HOT_TEMP,) \
  XX(TRD_TUBE_BOTTOM_COLD_TEMP,) \
  XX(POWER_GAS_TEMP,) \
  XX(OUTLET_TEMP,) \
  XX(INLET_TEMP,) \
  XX(BAT_BOTTOM_TEMP,) \
  XX(BAT_TOP_TEMP,) \
  XX(ATC_LATITUDE,) /*first of ATC sensors*/ \
  XX(ATC_LONGITUDE,) \
  XX(ATC_HEIGHT,) \
  XX(ATC_HORIZONTAL_SPEED,) \
  XX(ATC_HEADING,) \
  XX(EBASS_LATITUDE,) /*first of EBASS sensors*/ \
  XX(EBASS_LONGITUDE,) \
  XX(EBASS_HEIGHT,) \
  XX(EBASS_HORIZONTAL_SPEED,) \
  XX(EBASS_HEADING,) \
  XX(EBASS_TEMP_OUT,) \
  XX(EBASS_TEMP_GAS,) \
  XX(EBASS_PRESSURE,) \
  XX(BEAM_TRIGGER,) /*first of Testbam 2011 sensors*/ \
  XX(BEAM_CHERENKOV1,) \
  XX(BEAM_CHERENKOV2,) \
  XX(TRD_PRESSURE_SMOOTHED,) \
  XX(VOLTAGE_0x1800,) \
  XX(VOLTAGE_0x1801,) \
  XX(VOLTAGE_0x1802,) \
  XX(VOLTAGE_0x1803,) \
  XX(VOLTAGE_0x1c00,) \
  XX(VOLTAGE_0x1c01,) \
  XX(VOLTAGE_0x1c02,) \
  XX(VOLTAGE_0x1c03,) \
  XX(HPE_0x1804_TEMP,) \
  XX(HPE_0x1805_TEMP,) \
  XX(HPE_0x1806_TEMP,) \
  XX(HPE_0x1c04_TEMP,) \
  XX(HPE_0x1c05_TEMP,) \
  XX(HPE_0x1c06_TEMP,) \
  XX(N_SENSOR_TYPES,) /* end marker */

namespace SensorTypes
{
  DECLARE_ENUM(Type,SENSOR_TYPES)

  std::vector<SensorTypes::Type> temperatureSensors();
  std::vector<SensorTypes::Type> beamSensors();
  std::vector<SensorTypes::Type> voltageSensors();
}

#endif /* SensorTypes_hh */
