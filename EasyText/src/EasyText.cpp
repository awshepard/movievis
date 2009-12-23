/**
 * \file EasyText.cpp
 * \author Douglas W. Paul
 *
 * Defines the behavior of the EasyText functions
 */

//STL headers
#include <string>
#include <utility>
#include <iostream>
#include <fstream>
using namespace std;

//OpenGL headers
#ifdef _WIN32
#  include <windows.h>
#  include <gl/Gl.h>
#else
#  include <GL/gl.h>
#endif

#include "EasyText.hpp"

using std::pair;

namespace ezt {

	GLFont __registeredFonts[EZT_MAX_FONTS];
	unsigned short __anchor = EZT_ANCHOR_TOP_LEFT;
	float __hPadding = 0.0f;
	float __vPadding = 0.0f;
	double __screenHeightUnits = 1.0;
	unsigned int __currentFont = 0;

	void registerFont(unsigned int handle, char fontPath[], int textureNumber) {
		if (handle >= EZT_MAX_FONTS) throw 13;
		if (!__registeredFonts[handle].Create(fontPath, textureNumber)) throw 14;
	}

	void unregisterFont(unsigned int handle) {
		__registeredFonts[handle].Destroy();
	}

	void setTextAnchor(unsigned short anchor) {
		__anchor = anchor;
	}

	void setPadding(float hPadding, float vPadding) {
		__hPadding = hPadding;
		__vPadding = vPadding;
	}

	void setScreenHeightUnits(double screenHeightUnits) {
		__screenHeightUnits = screenHeightUnits;
	}

	void setCurrentFont(unsigned int handle) {
		if (handle >= EZT_MAX_FONTS) throw 13;
		__currentFont = handle;
	}

	void renderTextWithScaling(const char text[], double x, double y, double z) {
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		double screenHeightPixels = (double) viewport[3];
		double scale = __screenHeightUnits / screenHeightPixels;

		glPushMatrix();
		glTranslated(x, y, z);
		glScaled(scale, scale, scale);
		_renderText(text);
		glPopMatrix();
	}

	void renderText(const char text[], double x, double y, double z) {
		glPushMatrix();
		glTranslated(x, y, z);
		_renderText(text);
		glPopMatrix();
	}

	void renderTextWithScaling(string text, double x, double y, double z) {
		const char *cStrText = text.c_str();
		renderTextWithScaling(cStrText, x, y, z);
	}

	void renderText(string text, double x, double y, double z) {
		const char *cStrText = text.c_str();
		renderText(cStrText, x, y, z);
	}

	void getTextSize(const char text[], pair<int, int> &textSize) {
		GLFont *font = &__registeredFonts[__currentFont];
		font->GetStringSize(text, &textSize);
	}

	void getTextSize(string text, pair<int, int> &textSize) {
		const char *cStrText = text.c_str();
		getTextSize(cStrText, textSize);
	}

	void _renderText(const char text[]) {
		GLFont *font = &__registeredFonts[__currentFont];
		pair<int, int> textSize;
		font->GetStringSize(text, &textSize);
		float textWidth = (float) textSize.first;
		float textHeight = (float) textSize.second;

		float xOffset = 0.0f;
		switch (__anchor & __EZT_ANCHOR_HMASK) {
			case __EZT_ANCHOR_LEFT:
				xOffset += __hPadding;
				break;
			case __EZT_ANCHOR_CENTER:
				xOffset -= textWidth / 2.0f;
				break;
			case __EZT_ANCHOR_RIGHT:
				xOffset -= textWidth + __hPadding;
				break;
		}

		float yOffset = 0.0f;
		switch (__anchor & __EZT_ANCHOR_VMASK) {
			case __EZT_ANCHOR_TOP:
				yOffset -= __vPadding;
				break;
			case __EZT_ANCHOR_MIDDLE:
				yOffset += textHeight / 2.0f;
				break;
			case __EZT_ANCHOR_BOTTOM:
				yOffset += textHeight + __vPadding;
				break;
		}

		GLint glBlendSrc;
		GLint glBlendDst;

		glPushAttrib(GL_ENABLE_BIT);
		glGetIntegerv(GL_BLEND_SRC, &glBlendSrc);
		glGetIntegerv(GL_BLEND_DST, &glBlendDst);

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		font->Begin();
		font->DrawString(text, xOffset, yOffset);

		glBlendFunc(glBlendSrc, glBlendDst);
		glPopAttrib();
	}

}