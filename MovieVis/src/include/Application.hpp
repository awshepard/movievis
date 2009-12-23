/**
 * \file Application.hpp
 * \author Douglas W. Paul and Adam Shepard
 *
 * Declares the Application class
 */

#pragma once

#include <boost\smart_ptr\shared_ptr.hpp>
#include <Geometry.hpp>
#include <Drawable.hpp>
#include <Engine.hpp>
#include <GlWrappers.hpp>
#include <EasyText.hpp>
#include "UiHandler_pre.hpp"
#include "Model.hpp"
#include "ModelRenderer.hpp"

using boost::shared_ptr;
using namespace peek;

/**
 * The application
 */
class Application {
public:

	/** Constructor */
	Application();

	/** Destructor */
	~Application();

	void run();

	/** Provides access to the engine */
	inline shared_ptr<Engine> getEngine() { return this->engine; }

	/** Provides access to the model */
	inline shared_ptr<Model> getModel() { return this->model; }

	/** Provides access to the model renderer */
	inline shared_ptr<ModelRenderer> getModelRenderer() { return this->modelRenderer; }

protected:

	/** The engine */
	shared_ptr<Engine> engine;

	/** The data model */
	shared_ptr<Model> model;

	/** The UI handler */
	auto_ptr<UiHandler> uiHandler;

	/** The model renderer */
	shared_ptr<ModelRenderer> modelRenderer;

	/** Draws a set of axes at the origin */
	void drawAxes() const;

};

// Custom MovieVis events...
static const int MV_NONE = 0;
static const int MV_EXIT = 1;
static const int MV_MOVE_NORTH = 2;
static const int MV_MOVE_SOUTH = 3;
static const int MV_MOVE_WEST = 4;
static const int MV_MOVE_EAST = 5;
static const int MV_MOVE_UP = 6;
static const int MV_MOVE_DOWN = 7;
static const int MV_UNBRUSH = 8;
