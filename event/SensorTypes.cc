#include "SensorTypes.hh"

#include <cstring>
#include <cassert>

namespace SensorTypes {

  Type convertFromString(const char* string)
  {
    // Our own sensors
    if (strcmp(string, "TRD_PRESSURE") == 0) return TRD_PRESSURE;
    else if (strcmp(string, "TOF_VOLTAGE") == 0) return TOF_VOLTAGE;
    else if (strcmp(string, "HPE_0x3000_TEMP") == 0) return HPE_0x3000_TEMP;
    else if (strcmp(string, "HPE_0x3001_TEMP") == 0) return HPE_0x3001_TEMP;
    else if (strcmp(string, "HPE_0x3100_TEMP") == 0) return HPE_0x3100_TEMP;
    else if (strcmp(string, "HPE_0x3101_TEMP") == 0) return HPE_0x3101_TEMP;
    else if (strcmp(string, "HPE_0x3300_TEMP") == 0) return HPE_0x3300_TEMP;
    else if (strcmp(string, "HPE_0x3301_TEMP") == 0) return HPE_0x3301_TEMP;
    else if (strcmp(string, "HPE_0x3700_TEMP") == 0) return HPE_0x3700_TEMP;
    else if (strcmp(string, "HPE_0x3701_TEMP") == 0) return HPE_0x3701_TEMP;
    else if (strcmp(string, "HPE_0x6000_TEMP") == 0) return HPE_0x6000_TEMP;
    else if (strcmp(string, "HPE_0x6001_TEMP") == 0) return HPE_0x6001_TEMP;
    else if (strcmp(string, "HPE_0x6100_TEMP") == 0) return HPE_0x6100_TEMP;
    else if (strcmp(string, "HPE_0x6101_TEMP") == 0) return HPE_0x6101_TEMP;
    else if (strcmp(string, "HPE_0x6200_TEMP") == 0) return HPE_0x6200_TEMP;
    else if (strcmp(string, "HPE_0x6201_TEMP") == 0) return HPE_0x6201_TEMP;
    else if (strcmp(string, "HPE_0x6300_TEMP") == 0) return HPE_0x6300_TEMP;
    else if (strcmp(string, "HPE_0x6301_TEMP") == 0) return HPE_0x6301_TEMP;
    else if (strcmp(string, "HPE_0x6400_TEMP") == 0) return HPE_0x6400_TEMP;
    else if (strcmp(string, "HPE_0x6401_TEMP") == 0) return HPE_0x6401_TEMP;
    else if (strcmp(string, "HPE_0x6500_TEMP") == 0) return HPE_0x6500_TEMP;
    else if (strcmp(string, "HPE_0x6501_TEMP") == 0) return HPE_0x6501_TEMP;
    else if (strcmp(string, "HPE_0x6600_TEMP") == 0) return HPE_0x6600_TEMP;
    else if (strcmp(string, "HPE_0x6601_TEMP") == 0) return HPE_0x6601_TEMP;
    else if (strcmp(string, "HPE_0x6700_TEMP") == 0) return HPE_0x6700_TEMP;
    else if (strcmp(string, "HPE_0x6701_TEMP") == 0) return HPE_0x6701_TEMP;
    else if (strcmp(string, "HPE_0x7800_TEMP") == 0) return HPE_0x7800_TEMP;
    else if (strcmp(string, "HPE_0x7801_TEMP") == 0) return HPE_0x7801_TEMP;
    else if (strcmp(string, "HPE_0x7900_TEMP") == 0) return HPE_0x7900_TEMP;
    else if (strcmp(string, "HPE_0x7901_TEMP") == 0) return HPE_0x7901_TEMP;
    else if (strcmp(string, "HPE_0x7a00_TEMP") == 0) return HPE_0x7a00_TEMP;
    else if (strcmp(string, "HPE_0x7a01_TEMP") == 0) return HPE_0x7a01_TEMP;
    else if (strcmp(string, "HPE_0x7b00_TEMP") == 0) return HPE_0x7b00_TEMP;
    else if (strcmp(string, "HPE_0x7b01_TEMP") == 0) return HPE_0x7b01_TEMP;
    else if (strcmp(string, "HPE_0x7c00_TEMP") == 0) return HPE_0x7c00_TEMP;
    else if (strcmp(string, "HPE_0x7c01_TEMP") == 0) return HPE_0x7c01_TEMP;
    else if (strcmp(string, "HPE_0x7d00_TEMP") == 0) return HPE_0x7d00_TEMP;
    else if (strcmp(string, "HPE_0x7d01_TEMP") == 0) return HPE_0x7d01_TEMP;
    else if (strcmp(string, "HPE_0x7e00_TEMP") == 0) return HPE_0x7e00_TEMP;
    else if (strcmp(string, "HPE_0x7e01_TEMP") == 0) return HPE_0x7e01_TEMP;
    else if (strcmp(string, "HPE_0x7f00_TEMP") == 0) return HPE_0x7f00_TEMP;
    else if (strcmp(string, "HPE_0x7f01_TEMP") == 0) return HPE_0x7f01_TEMP;
    else if (strcmp(string, "TOF_1_TEMP") == 0) return TOF_1_TEMP;
    else if (strcmp(string, "TOF_2_TEMP") == 0) return TOF_2_TEMP;
    else if (strcmp(string, "TOF_3_TEMP") == 0) return TOF_3_TEMP;
    else if (strcmp(string, "TOF_4_TEMP") == 0) return TOF_4_TEMP;
    else if (strcmp(string, "TOF_5_TEMP") == 0) return TOF_5_TEMP;
    else if (strcmp(string, "TOF_6_TEMP") == 0) return TOF_6_TEMP;
    else if (strcmp(string, "TOF_7_TEMP") == 0) return TOF_7_TEMP;
    else if (strcmp(string, "TOF_8_TEMP") == 0) return TOF_8_TEMP;
    else if (strcmp(string, "TRIGGER_BOARD_TEMP") == 0) return TRIGGER_BOARD_TEMP;
    else if (strcmp(string, "TRACKER_1_VOLTAGE") == 0) return TRACKER_1_VOLTAGE;
    else if (strcmp(string, "TRACKER_2_VOLTAGE") == 0) return TRACKER_2_VOLTAGE;
    else if (strcmp(string, "TRACKER_3_VOLTAGE") == 0) return TRACKER_3_VOLTAGE;
    else if (strcmp(string, "TRD_VOLTAGE") == 0) return TRD_VOLTAGE;
    else if (strcmp(string, "CPU_TEMP") == 0) return CPU_TEMP;
    else if (strcmp(string, "PC_TEMP") == 0) return PC_TEMP;
    else if (strcmp(string, "TDC_TEMP") == 0) return TDC_TEMP;
    else if (strcmp(string, "TRIGGER_RATE") == 0) return TRIGGER_RATE;
    else if (strcmp(string, "TRD_UFE_BOTTOM_COLD_TEMP") == 0) return TRD_UFE_BOTTOM_COLD_TEMP;
    else if (strcmp(string, "TRD_UFE_TOP_COLD_TEMP") == 0) return TRD_UFE_TOP_COLD_TEMP;
    else if (strcmp(string, "TRACKER_UFE_BOTTOM_COLD_TEMP") == 0) return TRACKER_UFE_BOTTOM_COLD_TEMP;
    else if (strcmp(string, "TRACKER_UFE_TOP_COLD_TEMP") == 0) return TRACKER_UFE_TOP_COLD_TEMP;
    else if (strcmp(string, "TRACKER_UFE_TOP_HOT_TEMP") == 0) return TRACKER_UFE_TOP_HOT_TEMP;
    else if (strcmp(string, "TRACKER_UFE_BOTTOM_HOT_TEMP") == 0) return TRACKER_UFE_BOTTOM_HOT_TEMP;
    else if (strcmp(string, "TRD_UFE_TOP_HOT_TEMP") == 0) return TRD_UFE_TOP_HOT_TEMP;
    else if (strcmp(string, "TRD_UFE_BOTTOM_HOT_TEMP") == 0) return TRD_UFE_BOTTOM_HOT_TEMP;
    else if (strcmp(string, "POWER_UFE_TEMP") == 0) return POWER_UFE_TEMP;
    else if (strcmp(string, "USB_BOARD_TEMP") == 0) return USB_BOARD_TEMP;
    else if (strcmp(string, "POWER_MID_TEMP") == 0) return POWER_MID_TEMP;
    else if (strcmp(string, "TRD_TUBE_TOP_HOT_TEMP") == 0) return TRD_TUBE_TOP_HOT_TEMP;
    else if (strcmp(string, "TRD_TUBE_TOP_COLD_TEMP") == 0) return TRD_TUBE_TOP_COLD_TEMP;
    else if (strcmp(string, "TRD_GAS_COLD_TEMP") == 0) return TRD_GAS_COLD_TEMP;
    else if (strcmp(string, "TRD_GAS_HOT_TEMP") == 0) return TRD_GAS_HOT_TEMP;
    else if (strcmp(string, "TRD_TUBE_BOTTOM_HOT_TEMP") == 0) return TRD_TUBE_BOTTOM_HOT_TEMP;
    else if (strcmp(string, "TRD_TUBE_BOTTOM_COLD_TEMP") == 0) return TRD_TUBE_BOTTOM_COLD_TEMP;
    else if (strcmp(string, "POWER_GAS_TEMP") == 0) return POWER_GAS_TEMP;
    else if (strcmp(string, "OUTLET_TEMP") == 0) return OUTLET_TEMP;
    else if (strcmp(string, "INLET_TEMP") == 0) return INLET_TEMP;
    else if (strcmp(string, "BAT_BOTTOM_TEMP") == 0) return BAT_BOTTOM_TEMP;
    else if (strcmp(string, "BAT_TOP_TEMP") == 0) return BAT_TOP_TEMP;

    // ATC
    else if (strcmp(string, "ATC_LATITUDE") == 0) return ATC_LATITUDE;
    else if (strcmp(string, "ATC_LONGITUDE") == 0) return ATC_LONGITUDE;
    else if (strcmp(string, "ATC_HEIGHT") == 0) return ATC_HEIGHT;
    else if (strcmp(string, "ATC_HORIZONTAL_SPEED") == 0) return ATC_HORIZONTAL_SPEED;
    else if (strcmp(string, "ATC_HEADING") == 0) return ATC_HEADING;

    // EBASS
    else if (strcmp(string, "EBASS_LATITUDE") == 0) return EBASS_LATITUDE;
    else if (strcmp(string, "EBASS_LONGITUDE") == 0) return EBASS_LONGITUDE;
    else if (strcmp(string, "EBASS_HEIGHT") == 0) return EBASS_HEIGHT;
    else if (strcmp(string, "EBASS_HORIZONTAL_SPEED") == 0) return EBASS_HORIZONTAL_SPEED;
    else if (strcmp(string, "EBASS_HEADING") == 0) return EBASS_HEADING;
    else if (strcmp(string, "EBASS_TEMP_OUT") == 0) return EBASS_TEMP_OUT;
    else if (strcmp(string, "EBASS_TEMP_GAS") == 0) return EBASS_TEMP_GAS;
    else if (strcmp(string, "EBASS_PRESSURE") == 0) return EBASS_PRESSURE;

    // we should never come to this point -> assert
    assert(false);
    return END;
  }
  
  std::string convertToString(Type type)
  {
    switch (type) {
      case TRD_PRESSURE: return "TRD_PRESSURE";
      case TOF_VOLTAGE: return "TOF_VOLTAGE";
      case HPE_0x3000_TEMP: return "HPE_0x3000_TEMP";
      case HPE_0x3001_TEMP: return "HPE_0x3001_TEMP";
      case HPE_0x3100_TEMP: return "HPE_0x3100_TEMP";
      case HPE_0x3101_TEMP: return "HPE_0x3101_TEMP";
      case HPE_0x3300_TEMP: return "HPE_0x3300_TEMP";
      case HPE_0x3301_TEMP: return "HPE_0x3301_TEMP";
      case HPE_0x3700_TEMP: return "HPE_0x3700_TEMP";
      case HPE_0x3701_TEMP: return "HPE_0x3701_TEMP";
      case HPE_0x6000_TEMP: return "HPE_0x6000_TEMP";
      case HPE_0x6001_TEMP: return "HPE_0x6001_TEMP";
      case HPE_0x6100_TEMP: return "HPE_0x6100_TEMP";
      case HPE_0x6101_TEMP: return "HPE_0x6101_TEMP";
      case HPE_0x6200_TEMP: return "HPE_0x6200_TEMP";
      case HPE_0x6201_TEMP: return "HPE_0x6201_TEMP";
      case HPE_0x6300_TEMP: return "HPE_0x6300_TEMP";
      case HPE_0x6301_TEMP: return "HPE_0x6301_TEMP";
      case HPE_0x6400_TEMP: return "HPE_0x6400_TEMP";
      case HPE_0x6401_TEMP: return "HPE_0x6401_TEMP";
      case HPE_0x6500_TEMP: return "HPE_0x6500_TEMP";
      case HPE_0x6501_TEMP: return "HPE_0x6501_TEMP";
      case HPE_0x6600_TEMP: return "HPE_0x6600_TEMP";
      case HPE_0x6601_TEMP: return "HPE_0x6601_TEMP";
      case HPE_0x6700_TEMP: return "HPE_0x6700_TEMP";
      case HPE_0x6701_TEMP: return "HPE_0x6701_TEMP";
      case HPE_0x7800_TEMP: return "HPE_0x7800_TEMP";
      case HPE_0x7801_TEMP: return "HPE_0x7801_TEMP";
      case HPE_0x7900_TEMP: return "HPE_0x7900_TEMP";
      case HPE_0x7901_TEMP: return "HPE_0x7901_TEMP";
      case HPE_0x7a00_TEMP: return "HPE_0x7a00_TEMP";
      case HPE_0x7a01_TEMP: return "HPE_0x7a01_TEMP";
      case HPE_0x7b00_TEMP: return "HPE_0x7b00_TEMP";
      case HPE_0x7b01_TEMP: return "HPE_0x7b01_TEMP";
      case HPE_0x7c00_TEMP: return "HPE_0x7c00_TEMP";
      case HPE_0x7c01_TEMP: return "HPE_0x7c01_TEMP";
      case HPE_0x7d00_TEMP: return "HPE_0x7d00_TEMP";
      case HPE_0x7d01_TEMP: return "HPE_0x7d01_TEMP";
      case HPE_0x7e00_TEMP: return "HPE_0x7e00_TEMP";
      case HPE_0x7e01_TEMP: return "HPE_0x7e01_TEMP";
      case HPE_0x7f00_TEMP: return "HPE_0x7f00_TEMP";
      case HPE_0x7f01_TEMP: return "HPE_0x7f01_TEMP";
      case TOF_1_TEMP: return "TOF_1_TEMP";
      case TOF_2_TEMP: return "TOF_2_TEMP";
      case TOF_3_TEMP: return "TOF_3_TEMP";
      case TOF_4_TEMP: return "TOF_4_TEMP";
      case TOF_5_TEMP: return "TOF_5_TEMP";
      case TOF_6_TEMP: return "TOF_6_TEMP";
      case TOF_7_TEMP: return "TOF_7_TEMP";
      case TOF_8_TEMP: return "TOF_8_TEMP";
      case TRIGGER_BOARD_TEMP: return "TRIGGER_BOARD_TEMP";
      case TRACKER_1_VOLTAGE: return "TRACKER_1_VOLTAGE";
      case TRACKER_2_VOLTAGE: return "TRACKER_2_VOLTAGE";
      case TRACKER_3_VOLTAGE: return "TRACKER_3_VOLTAGE";
      case TRD_VOLTAGE: return "TRD_VOLTAGE";
      case CPU_TEMP: return "CPU_TEMP";
      case PC_TEMP: return "PC_TEMP";
      case TDC_TEMP: return "TDC_TEMP";
      case TRIGGER_RATE: return "TRIGGER_RATE";
      case TRD_UFE_BOTTOM_COLD_TEMP: return "TRD_UFE_BOTTOM_COLD_TEMP";
      case TRD_UFE_TOP_COLD_TEMP: return "TRD_UFE_TOP_COLD_TEMP";
      case TRACKER_UFE_BOTTOM_COLD_TEMP: return "TRACKER_UFE_BOTTOM_COLD_TEMP";
      case TRACKER_UFE_TOP_COLD_TEMP: return "TRACKER_UFE_TOP_COLD_TEMP";
      case TRACKER_UFE_TOP_HOT_TEMP: return "TRACKER_UFE_TOP_HOT_TEMP";
      case TRACKER_UFE_BOTTOM_HOT_TEMP: return "TRACKER_UFE_BOTTOM_HOT_TEMP";
      case TRD_UFE_TOP_HOT_TEMP: return "TRD_UFE_TOP_HOT_TEMP";
      case TRD_UFE_BOTTOM_HOT_TEMP: return "TRD_UFE_BOTTOM_HOT_TEMP";
      case POWER_UFE_TEMP: return "POWER_UFE_TEMP";
      case USB_BOARD_TEMP: return "USB_BOARD_TEMP";
      case POWER_MID_TEMP: return "POWER_MID_TEMP";
      case TRD_TUBE_TOP_HOT_TEMP: return "TRD_TUBE_TOP_HOT_TEMP";
      case TRD_TUBE_TOP_COLD_TEMP: return "TRD_TUBE_TOP_COLD_TEMP";
      case TRD_GAS_COLD_TEMP: return "TRD_GAS_COLD_TEMP";
      case TRD_GAS_HOT_TEMP: return "TRD_GAS_HOT_TEMP";
      case TRD_TUBE_BOTTOM_HOT_TEMP: return "TRD_TUBE_BOTTOM_HOT_TEMP";
      case TRD_TUBE_BOTTOM_COLD_TEMP: return "TRD_TUBE_BOTTOM_COLD_TEMP";
      case POWER_GAS_TEMP: return "POWER_GAS_TEMP";
      case OUTLET_TEMP: return "OUTLET_TEMP";
      case INLET_TEMP: return "INLET_TEMP";
      case BAT_BOTTOM_TEMP: return "BAT_BOTTOM_TEMP";
      case BAT_TOP_TEMP: return "BAT_TOP_TEMP";
      case ATC_LATITUDE: return "ATC_LATITUDE";
      case ATC_LONGITUDE: return "ATC_LONGITUDE";
      case ATC_HEIGHT: return "ATC_HEIGHT";
      case ATC_HORIZONTAL_SPEED: return "ATC_HORIZONTAL_SPEED";
      case ATC_HEADING: return "ATC_HEADING";
      case EBASS_LATITUDE: return "EBASS_LATITUDE";
      case EBASS_LONGITUDE: return "EBASS_LONGITUDE";
      case EBASS_HEIGHT: return "EBASS_HEIGHT";
      case EBASS_HORIZONTAL_SPEED: return "EBASS_HORIZONTAL_SPEED";
      case EBASS_HEADING: return "EBASS_HEADING";
      case EBASS_TEMP_OUT: return "EBASS_TEMP_OUT";
      case EBASS_TEMP_GAS: return "EBASS_TEMP_GAS";
      case EBASS_PRESSURE: return "EBASS_PRESSURE";
      default: return "none";
    }
  }

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
    return ret;
  }

}
