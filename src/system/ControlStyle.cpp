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
#include <cstring>
#include <libintl.h>

#include "ControlStyle.h"
#include "utility/Templates.h"

using namespace violet;





/**
 * @return ControlStyle represented by `serialVersionUid'
 */
enum ControlStyle violet::ControlStyleFromInt(int serialVersionUid) {
	#define EXPAND_CONTROL_STYLE(id, name, serialVersionUid)	\
		case serialVersionUid:					\
			return E_CONTROL_STYLE_##id;

	switch (serialVersionUid) {
		#include "ControlStyles.h"
	}
	#undef EXPAND_CONTROL_STYLE

	return E_CONTROL_STYLE_MODERN;
}



/**
 * @return Static \0-terminated character buffer describing the control style
 */
char* violet::ControlStyleToString(enum ControlStyle style) {
	#define EXPAND_CONTROL_STYLE(id, name, serialVersionUid)	\
		case E_CONTROL_STYLE_##id:				\
			return gettext(name);

	switch (style) {
		#include "ControlStyles.h"
	}
	#undef EXPAND_CONTROL_STYLE

	return gettext("Modern");
}




/**
 * @return The next available control style or the first, if no more are
 *     available
 */
enum ControlStyle violet::GetNextControlStyle(enum ControlStyle style) {
	#define EXPAND_CONTROL_STYLE(id, name, serialVersionUid)	\
		E_CONTROL_STYLE_##id,					\

	static enum ControlStyle styles[] = {
		#include "ControlStyles.h"
	};
	#undef EXPAND_CONTROL_STYLE

	for (size_t i = 0; i < getStructSize(styles); ++i) {
		if (style == styles[i]) {
			if (i + 1 >= getStructSize(styles)) {
				return styles[0];
			} else {
				return styles[i + 1];
			}
		}
	}

	return styles[0];
}
