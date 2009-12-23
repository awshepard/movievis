/**
 * \file MockDataProvider.hpp
 * \author Douglas W. Paul and Adam Shepard
 *
 * Declares the MockDataProvider class
 */

#pragma once

#include "DataProvider.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <list>
#include <string>
#include <hash_map>
#include <boost\date_time\gregorian\gregorian.hpp>
#include <boost\optional.hpp>

using std::vector;
using std::list;
using stdext::hash_map;
using boost::gregorian::from_string;
using boost::optional;

/**
 * Provides mock movie/actor/director data
 */
class FileDataProvider : public DataProvider {
public:

	/** Constructor */
	FileDataProvider();

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

	/** The movies, mapped by name|year */
	hash_map<string, shared_ptr<Movie>> moviesByTitleAndYear;

	double maxBoxOffice;
	double minBoxOffice;

	/** The actors (in a list, for working) */
	list<shared_ptr<Person>> actors;

	/** The actors (in a vector, for return) */
	vector<shared_ptr<Person>> actorVector;

	/** The actors, mapped by name */
	hash_map<string, shared_ptr<Person>> actorsByName;

	/** The directors */
	vector<shared_ptr<Person>> directors;

	/** The directors, mapped by name */
	hash_map<string, shared_ptr<Person>> directorsByName;

	/** Initializes the mock data */
	void init();

	void readMovies(std::string file);

	void readActors(std::string file);

	void eliminateNoNameActors();

	shared_ptr<Person> findOrCreateDirector(string directorName);

	shared_ptr<Person> findOrCreateActor(string directorName);

	optional<shared_ptr<Movie>> findMovie(string movieTitleAndYear);
};
