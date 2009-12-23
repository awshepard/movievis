/**
 * \file DataProvider.hpp
 * \author Douglas W. Paul and Adam Shepard
 *
 * Declares the DataProvider class
 */

#pragma once

#include "Movie.hpp"
#include "Person.hpp"
#include "DataProvider.hpp"

/**
 * \interface DataProvider
 * \brief Something that provides movie/actor/director data
 */
class DataProvider {
public:

	/** Gets the movie data */
	virtual const vector<shared_ptr<Movie>> &getMovies() const = 0;

	/** Gets the actor data */
	virtual const vector<shared_ptr<Person>> &getActors() const = 0;

	/** Gets the director data */
	virtual const vector<shared_ptr<Person>> &getDirectors() const = 0;

	/** Gets box office data */
	virtual const double getMaxBoxOffice() const = 0;
	virtual const double getMinBoxOffice() const = 0;


};
