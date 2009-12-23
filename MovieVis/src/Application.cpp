/**
 * \file Application.cpp
 * \author Douglas W. Paul and Adam Shepard
 *
 * Defines the behavior of the Application class
 */

#include "Application.hpp"
#include <Peek_base.hpp>
#include <Color.hpp>
#include <OrthographicCamera.hpp>
#include <iostream>
#include "UiHandler.hpp"
#include "MockDataProvider.hpp"
#include "FileDataProvider.hpp"
#include "ModelRendererImpl.hpp"

using namespace peek;

Application::Application() {
	this->engine = shared_ptr<Engine>(new Engine(1024, 768, false));
}

Application::~Application() {
}

void Application::run() {
	auto_ptr<DataProvider> dataProvider(new FileDataProvider());

	this->model = shared_ptr<Model>(new Model());
	this->model->reloadData(*dataProvider);

	this->uiHandler = auto_ptr<UiHandler>(new UiHandler(this));
	this->uiHandler->bindKeys();

	this->modelRenderer = shared_ptr<ModelRenderer>(new ModelRendererImpl(this->model));

	this->engine->setDrawable(this->modelRenderer.get());
	this->engine->setCustomEventHandler(this->uiHandler.get());
	this->engine->setMouseButtonEventHandler(this->uiHandler.get());
	this->engine->setMouseMotionEventHandler(this->uiHandler.get());
	this->engine->setResizeEventHandler(this->uiHandler.get());

	this->engine->run();
}

void Application::drawAxes() const {
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);

	pkGlColor(Color::red);
	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(1.0, 0.0, 0.0);

	pkGlColor(Color::green);
	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(0.0, 1.0, 0.0);

	pkGlColor(Color::blue);
	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(0.0, 0.0, 1.0);

	glEnd();
}
