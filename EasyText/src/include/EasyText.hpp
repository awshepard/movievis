/**
 * \file EasyText.hpp
 * \author Douglas W. Paul
 *
 * Declares EasyText constants, variables, and functions
 */

#pragma once

#define __EZT_ANCHOR_VMASK    0x03
#define __EZT_ANCHOR_TOP      0x00
#define __EZT_ANCHOR_MIDDLE   0x01
#define __EZT_ANCHOR_BOTTOM   0x02
#define __EZT_ANCHOR_HMASK    0x0C
#define __EZT_ANCHOR_LEFT     0x00
#define __EZT_ANCHOR_CENTER   0x04
#define __EZT_ANCHOR_RIGHT    0x08

#define EZT_MAX_FONTS 256

#define EZT_ANCHOR_TOP_LEFT        __EZT_ANCHOR_TOP     |  __EZT_ANCHOR_LEFT
#define EZT_ANCHOR_TOP_CENTER      __EZT_ANCHOR_TOP     |  __EZT_ANCHOR_CENTER
#define EZT_ANCHOR_TOP_RIGHT       __EZT_ANCHOR_TOP     |  __EZT_ANCHOR_RIGHT
#define EZT_ANCHOR_MIDDLE_LEFT     __EZT_ANCHOR_MIDDLE  |  __EZT_ANCHOR_LEFT
#define EZT_ANCHOR_MIDDLE_CENTER   __EZT_ANCHOR_MIDDLE  |  __EZT_ANCHOR_CENTER
#define EZT_ANCHOR_MIDDLE_RIGHT    __EZT_ANCHOR_MIDDLE  |  __EZT_ANCHOR_RIGHT
#define EZT_ANCHOR_BOTTOM_LEFT     __EZT_ANCHOR_BOTTOM  |  __EZT_ANCHOR_LEFT
#define EZT_ANCHOR_BOTTOM_CENTER   __EZT_ANCHOR_BOTTOM  |  __EZT_ANCHOR_CENTER
#define EZT_ANCHOR_BOTTOM_RIGHT    __EZT_ANCHOR_BOTTOM  |  __EZT_ANCHOR_RIGHT

#include "glfont2.h"

using glfont::GLFont;
using std::string;

namespace ezt {

	void registerFont(unsigned int handle, char fontPath[], int textureNumber);

	void unregisterFont(unsigned int handle);

	void setTextAnchor(unsigned short anchor);

	void setPadding(float hPadding, float vPadding);

	void setScreenHeightPixels(int screenHeightPixels);

	void setScreenHeightUnits(double screenHeightUnits);

	void setCurrentFont(unsigned int handle);

	void renderTextWithScaling(const char text[], double x, double y, double z);

	void renderText(const char text[], double x, double y, double z);

	void renderTextWithScaling(string text, double x, double y, double z);

	void renderText(string text, double x, double y, double z);

	void getTextSize(const char text[], pair<int, int> &textSize);

	void getTextSize(string text, pair<int, int> &textSize);

	void _renderText(const char text[]);

}