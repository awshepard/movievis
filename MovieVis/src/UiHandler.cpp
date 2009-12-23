/**
 * \file UiHandler.cpp
 * \author Douglas W. Paul and Adam Shepard
 *
 * Defines the behavior for the UiHandler class
 */

#include "UiHandler.hpp"

/**
 * \param app A pointer to the application to control
 */
UiHandler::UiHandler(Application *app) {
	this->app = app;
	this->leftMouseButtonDown = false;
	this->rightMouseButtonDown = false;
	this->minDistanceStep = 0.0001;
	this->distanceRatio = 0.05;
	this->mouseSensitivity = 0.0625;
}

UiHandler::~UiHandler() {}

void UiHandler::bindKeys() {
	shared_ptr<Engine> engine(this->app->getEngine());

	engine->bindKey(SDLK_ESCAPE, MV_EXIT);
	engine->bindKey(SDLK_UP, MV_MOVE_NORTH);
	engine->bindKey(SDLK_e, MV_MOVE_NORTH);
	engine->bindKey(SDLK_DOWN, MV_MOVE_SOUTH);
	engine->bindKey(SDLK_d, MV_MOVE_SOUTH);
	engine->bindKey(SDLK_LEFT, MV_MOVE_WEST);
	engine->bindKey(SDLK_s, MV_MOVE_WEST);
	engine->bindKey(SDLK_RIGHT, MV_MOVE_EAST);
	engine->bindKey(SDLK_f, MV_MOVE_EAST);
	engine->bindKey(SDLK_PAGEUP, MV_MOVE_DOWN);
	engine->bindKey(SDLK_PAGEDOWN, MV_MOVE_UP);
	engine->bindKey(SDLK_SPACE, MV_UNBRUSH);
}

/**
* @param action The action to take
*/
void UiHandler::handleCustomEvent(int customEvent) {
	switch (customEvent) {
		case MV_EXIT: // Quit (ESC)
			exit(0);
		case MV_MOVE_NORTH:   // Increase camera's latitude
			this->app->getModel()->getCameraRigging()->changeY(getXYStep());
			this->app->getEngine()->invalidate();
			break;
		case MV_MOVE_SOUTH:   // Decrease camera's latitude
			this->app->getModel()->getCameraRigging()->changeY(-getXYStep());
			this->app->getEngine()->invalidate();
			break;
		case MV_MOVE_WEST:   // Decrease camera's longitude
			this->app->getModel()->getCameraRigging()->changeX(-getXYStep());
			this->app->getEngine()->invalidate();
			break;
		case MV_MOVE_EAST:   // Increase camera's longitude
			this->app->getModel()->getCameraRigging()->changeX(getXYStep());
			this->app->getEngine()->invalidate();
			break;
		case MV_MOVE_UP:   // Increase camera's height
			this->app->getModel()->getCameraRigging()->changeHeight(getHeightStep());
			this->app->getEngine()->invalidate();
			break;
		case MV_MOVE_DOWN:   // Decrease camera's height
			this->app->getModel()->getCameraRigging()->changeHeight(-getHeightStep());
			this->app->getEngine()->invalidate();
			break;
		case MV_UNBRUSH:
			this->app->getModel()->setBrushedActor(optional<shared_ptr<Person>>());
			this->app->getModel()->setBrushedMovie(optional<shared_ptr<Movie>>());
			break;
		default: break;
	}
}

/*!
* @param buttonEvent The button event
*/
void UiHandler::handleMouseButtonEvent(const SDL_MouseButtonEvent &buttonEvent) {
	const int button = buttonEvent.button;
	const int state = buttonEvent.state;
	const int x = buttonEvent.x;
	const int y = buttonEvent.y;
	
	if (button == SDL_BUTTON_LEFT) {
		if (state == SDL_PRESSED) {
			this->app->getModelRenderer()->brush(x, y);
		}
	} else if (button == SDL_BUTTON_RIGHT) {
		if (state == SDL_PRESSED) {
			// Begin dragging
			this->rightMouseButtonDown = true;
			SDL_ShowCursor(SDL_DISABLE);
			SDL_WM_GrabInput(SDL_GRAB_ON);
			this->isFirstDelta = true;
		} else {
			// End dragging
			this->rightMouseButtonDown = false;
			SDL_ShowCursor(SDL_ENABLE);
			SDL_WM_GrabInput(SDL_GRAB_OFF);
		}
	} else if (button == SDL_BUTTON_WHEELUP) {
		// Move camera down ("zoom in")
		this->app->getModel()->getCameraRigging()->changeHeight(-getHeightStep());
		this->app->getEngine()->invalidate();
	} else if (button == SDL_BUTTON_WHEELDOWN) {
		// Move camera up ("zoom out")
		this->app->getModel()->getCameraRigging()->changeHeight(getHeightStep());
		this->app->getEngine()->invalidate();
	}
}

/*!
* @param motionEvent The motion event
*/
void UiHandler::handleMouseMotionEvent(const SDL_MouseMotionEvent &motionEvent) {
	unsigned int mouseX = motionEvent.x;
	unsigned int mouseY = motionEvent.y;
	int mouseDX = motionEvent.xrel;
	int mouseDY = motionEvent.yrel;
	double step = getXYStep();

	if (this->rightMouseButtonDown == true) {
		if (this->isFirstDelta == true) {
			this->isFirstDelta = false;
			return;
		}

		double dX = (double) mouseDX * (-step) * this->mouseSensitivity;
		double dY = (double) mouseDY * step * this->mouseSensitivity;

		this->app->getModel()->getCameraRigging()->changeGroundPoint(dX, dY);
		this->app->getEngine()->invalidate();

		this->app->getModel()->setSelectedActor(optional<shared_ptr<Person>>());
		this->app->getModel()->setSelectedMovie(optional<shared_ptr<Movie>>());
	}
	else {
		this->app->getModelRenderer()->pick(mouseX, mouseY);
		this->app->getEngine()->invalidate();
	}

	this->app->getModel()->setMouseXY(mouseX, mouseY);
}

/*!
* @param sizeX The new x-size of the window
* @param sizeY The new y-size of the window
*/
void UiHandler::handleResizeEvent(int sizeX, int sizeY) {
	this->app->getModel()->getCamera()->setAspectRatio((double)sizeX/(double)sizeY);
	this->app->getEngine()->invalidate();
}

/**
 * We calculate the change in height based on the current height.
 * 
 * \return The increment to move up or down
 */
double UiHandler::getXYStep() {
	double distanceStep = this->app->getModel()->getCameraRigging()->getHeight() * this->distanceRatio;

	return max(distanceStep, this->minDistanceStep);
}

/**
 * We calculate the change in height based on the current height.
 * 
 * \return The increment to move up or down
 */
double UiHandler::getHeightStep() {
	double distanceStep = this->app->getModel()->getCameraRigging()->getHeight() * this->distanceRatio;

	return max(distanceStep, this->minDistanceStep);
}
