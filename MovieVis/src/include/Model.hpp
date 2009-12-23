/**
 * \file Model.hpp
 * \author Douglas W. Paul and Adam Shepard
 *
 * Declares the Model class
 */

#pragma once

#include <boost\ptr_container\ptr_vector.hpp>
#include <boost\optional\optional.hpp>
#include <OrthoBirdsEyeCameraRigging.hpp>
#include "Movie.hpp"
#include "Person.hpp"
#include "DataProvider.hpp"

#define FONT_CALIBRI_12_PATH  "..\\resources\\fonts\\calibri_12.glf"
#define FONT_CALIBRI_14_PATH  "..\\resources\\fonts\\calibri_14.glf"
#define FONT_CALIBRI_22_PATH  "..\\resources\\fonts\\calibri_22.glf"

#define FONT_CALIBRI_12  0
#define FONT_CALIBRI_14  1
#define FONT_CALIBRI_22  2

using boost::ptr_vector;
using boost::optional;
using peek::Camera;
using peek::OrthoBirdsEyeCameraRigging;

/**
 * The data model for the application
 */
class Model {
public:

	/** Constructor */
	Model();

	/** Destructor */
	virtual ~Model();
	
	/** Gets the movies in the model */
	const vector<shared_ptr<Movie>> &getMovies() const;

	/** Gets the actors in the model */
	const vector<shared_ptr<Person>> &getActors() const;

	/** Gets the directors in the model */
	const vector<shared_ptr<Person>> &getDirectors() const;

	/** Gets the selected movie */
	const optional<shared_ptr<Movie>> &getSelectedMovie() const;

	/** Sets the selected movie */
	void setSelectedMovie(const optional<shared_ptr<Movie>> &selectedMovie);

	/** Gets the brushed movie */
	const optional<shared_ptr<Movie>> &getBrushedMovie() const;

	/** Sets the brushed movie */
	void setBrushedMovie(const optional<shared_ptr<Movie>> &brushedMovie);

	/** Gets the selected actor */
	const optional<shared_ptr<Person>> &getSelectedActor() const;

	/** Sets the selected actor */
	void setSelectedActor(const optional<shared_ptr<Person>> &selectedActor);

	/** Gets the brushed actor */
	const optional<shared_ptr<Person>> &getBrushedActor() const;

	/** Sets the brushed actor */
	void setBrushedActor(const optional<shared_ptr<Person>> &brushedActor);

	/** Gets the maximum money earned at teh box office for any movie */
	const double getMaxBoxOffice() const;

	/** Gets the minimum money earned at teh box office for any movie */
	const double getMinBoxOffice() const;

	/** Gets the x-coordinate of the mouse */
	unsigned int getMouseX() const { return this->mouseX; }

	/** Gets the y-coordinate of the mouse */
	unsigned int getMouseY() const { return this->mouseY; }

	/** Sets the x- and y-coordinates of the mouse */
	void setMouseXY(unsigned int mouseX, unsigned int mouseY) { this->mouseX = mouseX; this->mouseY = mouseY; }

	/** Provides access to the camera */
	inline Camera *getCamera() { return this->cameraRigging->getCamera(); }

	/** Provides access to the camera rigging */
	inline OrthoBirdsEyeCameraRigging *getCameraRigging() { return this->cameraRigging.get(); }

	/** Reloads the model's data */
	void reloadData(const DataProvider &dataProvider);

protected:

	/** The movies */
	vector<shared_ptr<Movie>> movies;

	/** Actors */
	vector<shared_ptr<Person>> actors;

	/** Directors */
	vector<shared_ptr<Person>> directors;

	/** The selected movie */
	optional<shared_ptr<Movie>> selectedMovie;

	/** The brushed movie */
	optional<shared_ptr<Movie>> brushedMovie;

	/** The selected actor */
	optional<shared_ptr<Person>> selectedActor;

	/** The brushed actor */
	optional<shared_ptr<Person>> brushedActor;

	/** Maximum money earned at the box office for any movie */
	double maxBoxOffice;

	/** Minimum money earned at the box office for any movie */
	double minBoxOffice;

	/** The x-coordinate of the mouse */
	unsigned int mouseX;

	/** The y-coordinate of the mouse */
	unsigned int mouseY;

	/** The camera rigging */
	auto_ptr<OrthoBirdsEyeCameraRigging> cameraRigging;

};
