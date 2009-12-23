/**
 * \file Model.cpp
 * \author Douglas W. Paul and Adam Shepard
 *
 * Defines the behavior of the Model class
 */

#include "Model.hpp"
#include <Peek_base.hpp>
#include <Color.hpp>
#include <OrthographicCamera.hpp>
#include <iostream>
#include "UiHandler.hpp"

using namespace peek;
using std::sort;
using boost::date_time::neg_infin;
using boost::date_time::pos_infin;
using peek::Color;

Model::Model() {
	OrthographicCamera::handle camera = OrthographicCamera::handle(new OrthographicCamera(10.0, 4.0/3.0, 0.0001, 30.0));
	this->cameraRigging = auto_ptr<OrthoBirdsEyeCameraRigging>(new OrthoBirdsEyeCameraRigging(camera, Point2d(0.0, 0.0), 25.0, 45.0));

	ezt::registerFont(FONT_CALIBRI_12, FONT_CALIBRI_12_PATH, 0);
	ezt::registerFont(FONT_CALIBRI_14, FONT_CALIBRI_14_PATH, 1);
	ezt::registerFont(FONT_CALIBRI_22, FONT_CALIBRI_22_PATH, 2);
}

Model::~Model() {
	ezt::unregisterFont(FONT_CALIBRI_12);
	ezt::unregisterFont(FONT_CALIBRI_14);
	ezt::unregisterFont(FONT_CALIBRI_22);
}

const vector<shared_ptr<Movie>> &Model::getMovies() const {
	return this->movies;
}

const vector<shared_ptr<Person>> &Model::getActors() const {
	return this->actors;
}

const vector<shared_ptr<Person>> &Model::getDirectors() const {
	return this->directors;
}

const optional<shared_ptr<Movie>> &Model::getSelectedMovie() const {
	return this->selectedMovie;
}

void Model::setSelectedMovie(const optional<shared_ptr<Movie>> &selectedMovie) {
	this->selectedMovie = selectedMovie;
}

const optional<shared_ptr<Movie>> &Model::getBrushedMovie() const {
	return this->brushedMovie;
}

void Model::setBrushedMovie(const optional<shared_ptr<Movie>> &brushedMovie) {
	this->brushedMovie = brushedMovie;
}

const optional<shared_ptr<Person>> &Model::getSelectedActor() const {
	return this->selectedActor;
}

void Model::setSelectedActor(const optional<shared_ptr<Person>> &selectedActor) {
	this->selectedActor = selectedActor;
}

const optional<shared_ptr<Person>> &Model::getBrushedActor() const {
	return this->brushedActor;
}

void Model::setBrushedActor(const optional<shared_ptr<Person>> &brushedActor) {
	this->brushedActor = brushedActor;
}

const double Model::getMaxBoxOffice() const {
	return this->maxBoxOffice;
}
const double Model::getMinBoxOffice() const {
	return this->minBoxOffice;
}

void Model::reloadData(const DataProvider &dataProvider) {
	this->movies = dataProvider.getMovies();
	this->actors = dataProvider.getActors();
	this->directors = dataProvider.getDirectors();
	this->maxBoxOffice = dataProvider.getMaxBoxOffice();
	this->minBoxOffice = dataProvider.getMinBoxOffice();
}
