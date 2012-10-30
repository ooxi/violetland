/**
 * Copyright (c) 2012 ooxi/violetland
 *     https://github.com/ooxi/violetland
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the
 * use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software in a
 *     product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 * 
 *  2. Altered source versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any source distribution.
 */
#ifndef CONTROL_STYLE_H_
#define CONTROL_STYLE_H_
namespace violetland {





/**
 * Enumerates all implemented control styles
 */
enum ControlStyle {
#define EXPAND_CONTROL_STYLE(id, name, serialVersionUid)		\
	E_CONTROL_STYLE_##id = serialVersionUid,
#include "ControlStyles.h"
#undef EXPAND_CONTROL_STYLE

	_E_CONTROL_STYLE
};



/**
 * serialVersionUid -> ControlStyle
 */
enum ControlStyle ControlStyleFromInt(int serialVersionUid);

/**
 * ControlStyle -> Human readable description
 */
char* ControlStyleToString(enum ControlStyle style);

/**
 * Switches through the available control styles
 */
enum ControlStyle GetNextControlStyle(enum ControlStyle);





}
#endif

