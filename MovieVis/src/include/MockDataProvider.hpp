/**
 * \file MockDataProvider.hpp
 * \author Douglas W. Paul and Adam Shepard
 *
 * Declares the MockDataProvider class
 */

#pragma once

#include "DataProvider.hpp"

/**
 * Provides mock movie/actor/director data
 */
class MockDataProvider : public DataProvider {
public:

	/** Constructor */
	MockDataProvider();

	/** Gets the movie data */
	virtual const vector<shared_ptr<Movie>> &getMovies() const;

	/** Gets the actor data */
	virtual const vector<shared_ptr<Person>> &getActors() const;

	/** Gets the director data */
	virtual const vector<shared_ptr<Person>> &getDirectors() const;

	/** Gets box office data */
	virtual const double getMaxBoxOffice() const;
	virtual const double getMinBoxOffice() const;

protected:

	/** The movies */
	vector<shared_ptr<Movie>> movies;
	double maxBoxOffice;
	double minBoxOffice;
	/** The actors */
	vector<shared_ptr<Person>> actors;

	/** The directors */
	vector<shared_ptr<Person>> directors;

	/** Initializes the mock data */
	void init();

};
