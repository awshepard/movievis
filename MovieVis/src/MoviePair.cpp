/**
 * \file MoviePair.cpp
 * \author Douglas W. Paul and Adam Shepard
 *
 * Defines the behavior of the MoviePair class
 */

#include "MoviePair.hpp"

MoviePair::MoviePair(const shared_ptr<Movie> &firstMovie, const shared_ptr<Movie> &secondMovie) {
	this->firstMovie = firstMovie;
	this->secondMovie = secondMovie;
}

MoviePair::~MoviePair() {
}
