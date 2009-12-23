/**
 * \file UiHandler.hpp
 * \author Douglas W. Paul and Adam Shepard
 *
 * Declares the UiHandler class
 */

#pragma once

#include "Application.hpp"

/**
 * Handles UI events for the application
 */
class UiHandler : public CustomEventHandler, public MouseButtonEventHandler, public MouseMotionEventHandler, public ResizeEventHandler {
public:

	/** Constructor */
	UiHandler(Application *app);

	/** Destructor */
	~UiHandler();

	/** Handles a custom event */
	void handleCustomEvent(int customEvent);

	/** Handles a mouse button event */
	virtual void handleMouseButtonEvent(const SDL_MouseButtonEvent &buttonEvent);

	/** Handles a mouse motion event */
	virtual void handleMouseMotionEvent(const SDL_MouseMotionEvent &motionEvent);

	/** Handles a window-resize event */
	virtual void handleResizeEvent(int sizeX, int sizeY);

	void bindKeys();

protected:

	/** The application */
	Application *app;

	/** Whether the left mouse button is depressed */
	bool leftMouseButtonDown;

	/** Whether the right mouse button is depressed */
	bool rightMouseButtonDown;

	/** Whether or not we've seen a delta since we started dragging */
	bool isFirstDelta;

	/** Minimum amount to change the distance of the camera from its target */
	double minDistanceStep;

	/** The fraction of the current distance by which the distance should change */
	double distanceRatio;

	/** Sensitivity of the mouse for mouselook */
	double mouseSensitivity;

	/** Calculates the step in the x or y direction */
	double getXYStep();

	/** Calculates the height step */
	double getHeightStep();

};
