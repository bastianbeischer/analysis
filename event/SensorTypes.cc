#include "SensorTypes.hh"

#include <cstring>
#include <cassert>

// expansion macro for enum to string conversion
#define ENUM_CASE(name,assign) case name: return #name;

// expansion macro for string to enum conversion
#define ENUM_STRCMP(name,assign) if (strcmp(string,#name) == 0) return name;

/// define the access function names
#define ENUM_CONVERTER_FUNCTIONS(EnumType,ENUM_DEF) \
  const char* convertToString(EnumType value) \
  { \
    switch(value) { \
      ENUM_DEF(ENUM_CASE) \
    default: \
      assert(false); \
      return ""; \
    } \
  } \
  EnumType convertFromString(const char* string) \
  { \
    ENUM_DEF(ENUM_STRCMP) \
    assert(false); \
    return (EnumType)0; \
  }

namespace SensorTypes {

  ENUM_CONVERTER_FUNCTIONS(Type,SENSOR_TYPES)

  std::vector<SensorTypes::Type> temperatureSensors()
  {
    std::vector<SensorTypes::Type> ret;
    ret.push_back(HPE_0x3000_TEMP);
    ret.push_back(HPE_0x3001_TEMP);
    ret.push_back(HPE_0x3100_TEMP);
    ret.push_back(HPE_0x3101_TEMP);
    ret.push_back(HPE_0x3300_TEMP);
    ret.push_back(HPE_0x3301_TEMP);
    ret.push_back(HPE_0x3700_TEMP);
    ret.push_back(HPE_0x3701_TEMP);
    ret.push_back(HPE_0x6000_TEMP);
    ret.push_back(HPE_0x6001_TEMP);
    ret.push_back(HPE_0x6100_TEMP);
    ret.push_back(HPE_0x6101_TEMP);
    ret.push_back(HPE_0x6200_TEMP);
    ret.push_back(HPE_0x6201_TEMP);
    ret.push_back(HPE_0x6300_TEMP);
    ret.push_back(HPE_0x6301_TEMP);
    ret.push_back(HPE_0x6400_TEMP);
    ret.push_back(HPE_0x6401_TEMP);
    ret.push_back(HPE_0x6500_TEMP);
    ret.push_back(HPE_0x6501_TEMP);
    ret.push_back(HPE_0x6600_TEMP);
    ret.push_back(HPE_0x6601_TEMP);
    ret.push_back(HPE_0x6700_TEMP);
    ret.push_back(HPE_0x6701_TEMP);
    ret.push_back(HPE_0x7800_TEMP);
    ret.push_back(HPE_0x7801_TEMP);
    ret.push_back(HPE_0x7900_TEMP);
    ret.push_back(HPE_0x7901_TEMP);
    ret.push_back(HPE_0x7a00_TEMP);
    ret.push_back(HPE_0x7a01_TEMP);
    ret.push_back(HPE_0x7b00_TEMP);
    ret.push_back(HPE_0x7b01_TEMP);
    ret.push_back(HPE_0x7c00_TEMP);
    ret.push_back(HPE_0x7c01_TEMP);
    ret.push_back(HPE_0x7d00_TEMP);
    ret.push_back(HPE_0x7d01_TEMP);
    ret.push_back(HPE_0x7e00_TEMP);
    ret.push_back(HPE_0x7e01_TEMP);
    ret.push_back(HPE_0x7f00_TEMP);
    ret.push_back(HPE_0x7f01_TEMP);
    ret.push_back(TOF_1_TEMP);
    ret.push_back(TOF_2_TEMP);
    ret.push_back(TOF_3_TEMP);
    ret.push_back(TOF_4_TEMP);
    ret.push_back(TOF_5_TEMP);
    ret.push_back(TOF_6_TEMP);
    ret.push_back(TOF_7_TEMP);
    ret.push_back(TOF_8_TEMP);
    ret.push_back(TRIGGER_BOARD_TEMP);
    ret.push_back(CPU_TEMP);
    ret.push_back(PC_TEMP);
    ret.push_back(TDC_TEMP);
    ret.push_back(TRD_UFE_BOTTOM_COLD_TEMP);
    ret.push_back(TRD_UFE_TOP_COLD_TEMP);
    ret.push_back(TRACKER_UFE_BOTTOM_COLD_TEMP);
    ret.push_back(TRACKER_UFE_TOP_COLD_TEMP);
    ret.push_back(TRACKER_UFE_TOP_HOT_TEMP);
    ret.push_back(TRACKER_UFE_BOTTOM_HOT_TEMP);
    ret.push_back(TRD_UFE_TOP_HOT_TEMP);
    ret.push_back(TRD_UFE_BOTTOM_HOT_TEMP);
    ret.push_back(POWER_UFE_TEMP);
    ret.push_back(USB_BOARD_TEMP);
    ret.push_back(POWER_MID_TEMP);
    ret.push_back(TRD_TUBE_TOP_HOT_TEMP);
    ret.push_back(TRD_TUBE_TOP_COLD_TEMP);
    ret.push_back(TRD_GAS_COLD_TEMP);
    ret.push_back(TRD_GAS_HOT_TEMP);
    ret.push_back(TRD_TUBE_BOTTOM_HOT_TEMP);
    ret.push_back(TRD_TUBE_BOTTOM_COLD_TEMP);
    ret.push_back(POWER_GAS_TEMP);
    ret.push_back(OUTLET_TEMP);
    ret.push_back(INLET_TEMP);
    ret.push_back(BAT_BOTTOM_TEMP);
    ret.push_back(BAT_TOP_TEMP);
    ret.push_back(EBASS_TEMP_OUT);
    ret.push_back(EBASS_TEMP_GAS);
    ret.push_back(HPE_0x1804_TEMP);
    ret.push_back(HPE_0x1805_TEMP);
    ret.push_back(HPE_0x1806_TEMP);
    ret.push_back(HPE_0x1c04_TEMP);
    ret.push_back(HPE_0x1c05_TEMP);
    ret.push_back(HPE_0x1c06_TEMP);
    return ret;
  }

  std::vector<SensorTypes::Type> beamSensors()
  {
    std::vector<SensorTypes::Type> ret;
    ret.push_back(BEAM_TRIGGER);
    ret.push_back(BEAM_CHERENKOV1);
    ret.push_back(BEAM_CHERENKOV2);
    return ret;
  }

  std::vector<SensorTypes::Type> voltageSensors()
  {
    std::vector<SensorTypes::Type> ret;
    ret.push_back(VOLTAGE_0x1800);
    ret.push_back(VOLTAGE_0x1801);
    ret.push_back(VOLTAGE_0x1802);
    ret.push_back(VOLTAGE_0x1803);
    ret.push_back(VOLTAGE_0x1c00);
    ret.push_back(VOLTAGE_0x1c01);
    ret.push_back(VOLTAGE_0x1c02);
    ret.push_back(VOLTAGE_0x1c03);
    return ret;
  }

}
