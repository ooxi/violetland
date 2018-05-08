#include "ControlPreset.h"
#include <sstream>

using namespace violet;

std::string violet::ControlPresetToString(int preset) {
    switch(preset){
    case 2:
	return "2";
    case 3:
	return "3";
    case 4:
	return "4";
    default:
	break;
    }
    return "1";
}

int violet::ControlPresetFromString(std::string preset) {
    int i;
    std::stringstream(preset) >> i;
    return i;
}
