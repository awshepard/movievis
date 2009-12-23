/**
 * \file MovieDotSizeProvider.hpp
 * \author Douglas W. Paul and Adam Shepard
 *
 * Declares the DotSizeProvider class
 */

#pragma once

#include "Movie.hpp"
#include <hash_map>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/functional/hash.hpp>

using std::vector;
using stdext::hash_map;
using boost::shared_ptr;
using boost::hash;

typedef hash_map<shared_ptr<Movie>, double, Movie::hashConf> dotSizeMap_t;

/**
 * An movie dot size provider
 */
class MovieDotSizeProvider {
public:

	/** Constructor */
	MovieDotSizeProvider(double minBoxOfficeTotal, double maxBoxOfficeTotal);

	/** Destructor */
	virtual ~MovieDotSizeProvider();
	
	/** Gets the size of the dot for a given movie */
	double getDotSize(const shared_ptr<Movie> &movie);

	/** Sets a new range of box office totals */
	void setBoxOfficeTotalRange(double minBoxOfficeTotal, double maxBoxOfficeTotal);

protected:

	/** The movie dot colors */
	dotSizeMap_t dotSizes;

	/** Min box office total */
	double minBoxOfficeTotal;

	/** Max box office total */
	double maxBoxOfficeTotal;

	/** Calculates the size of a dot */
	double calculateDotSize(double boxOfficeTotal);

};
