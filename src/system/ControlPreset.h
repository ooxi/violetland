#ifndef VIOLET_CONTROL_PRESET_H_
#define VIOLET_CONTROL_PRESET_H_

#include <string>
namespace violet {

    std::string ControlPresetToString(int preset);
    int ControlPresetFromString(std::string preset);
    
}
#endif /* VIOLET_CONTROL_PRESET_H_ */
