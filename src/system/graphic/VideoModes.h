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



/**
 * According to the original implementation of `VideoManager::GetAvailableModes'
 * this way is better than SDL_ListModes because of SDL_ListModes returns not
 * all possible modes...
 *
 * ...except, that you have to manually take care of it and it's not
 * configurable at runtime -.-
 *
 * This list was extracted from http://en.wikipedia.org/w/index.php?title=File:Vector_Video_Standards4.svg
 */
EXPAND_VIDEO_MODE(CGA,		320, 200)
EXPAND_VIDEO_MODE(QVGA,		320, 240)
EXPAND_VIDEO_MODE(CIF,		352, 288)
EXPAND_VIDEO_MODE(SIF,		384, 288)
EXPAND_VIDEO_MODE(HVGA,		480, 320)
EXPAND_VIDEO_MODE(VGA,		640, 480)
EXPAND_VIDEO_MODE(PAL,		768, 576)
EXPAND_VIDEO_MODE(WVGA,		800, 480)
EXPAND_VIDEO_MODE(SVGA,		800, 600)
EXPAND_VIDEO_MODE(WVGA,		854, 480)
EXPAND_VIDEO_MODE(PAL,		1024, 576)
EXPAND_VIDEO_MODE(WSVGA,	1024, 600)
EXPAND_VIDEO_MODE(XGA,		1024, 768)
EXPAND_VIDEO_MODE(UNNAMED,	1152, 768)
EXPAND_VIDEO_MODE(XGA_PLUS,	1152, 864)
EXPAND_VIDEO_MODE(HD,		1280, 720)
EXPAND_VIDEO_MODE(WXGA,		1280, 768)
EXPAND_VIDEO_MODE(WXGA,		1280, 800)
EXPAND_VIDEO_MODE(UNNAMED,	1280, 854)
EXPAND_VIDEO_MODE(UNNAMED,	1280, 960)
EXPAND_VIDEO_MODE(SXGA,		1280, 1024)
EXPAND_VIDEO_MODE(UNNAMED,	1366, 768)
EXPAND_VIDEO_MODE(SXGA_PLUS,	1400, 1050)
EXPAND_VIDEO_MODE(UNNAMED,	1440, 900)
EXPAND_VIDEO_MODE(UNNAMED,	1440, 960)
EXPAND_VIDEO_MODE(UNNAMED,	1440, 1080)
EXPAND_VIDEO_MODE(UXGA,		1600, 1200)
EXPAND_VIDEO_MODE(WSXGA_PLUS,	1680, 1050)
EXPAND_VIDEO_MODE(FULL_HD,	1920, 1080)
EXPAND_VIDEO_MODE(WUXGA,	1920, 1200)
EXPAND_VIDEO_MODE(TWO_K,	2048, 1080)
EXPAND_VIDEO_MODE(QXGA,		2048, 1536)
EXPAND_VIDEO_MODE(WQHD,		2560, 1440)
EXPAND_VIDEO_MODE(WQXGA,	2560, 1600)
EXPAND_VIDEO_MODE(QSXGA,	2560, 2048)

