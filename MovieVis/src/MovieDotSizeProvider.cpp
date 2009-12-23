/**
 * \file MovieDotSizeProvider.cpp
 * \author Douglas W. Paul and Adam Shepard
 *
 * Defines the behavior of the MovieDotSizeProvider class
 */

#include "MovieDotSizeProvider.hpp"

MovieDotSizeProvider::MovieDotSizeProvider(double minBoxOfficeTotal, double maxBoxOfficeTotal) {
	this->minBoxOfficeTotal = minBoxOfficeTotal;
	this->maxBoxOfficeTotal = maxBoxOfficeTotal;
}

MovieDotSizeProvider::~MovieDotSizeProvider() {
}

double MovieDotSizeProvider::getDotSize(const shared_ptr<Movie> &movie) {
	dotSizeMap_t::iterator dotSizeMapEntry = this->dotSizes.find(movie);

	double dotSize;

	if (dotSizeMapEntry != this->dotSizes.end()) {
		dotSize = dotSizeMapEntry->second;
	}
	else {
		dotSize = calculateDotSize(movie->getBoxOfficeReceipts());
		this->dotSizes[movie] = dotSize;
	}

	return dotSize;
}

void MovieDotSizeProvider::setBoxOfficeTotalRange(double minBoxOfficeTotal, double maxBoxOfficeTotal) {
	this->minBoxOfficeTotal = minBoxOfficeTotal;
	this->maxBoxOfficeTotal = maxBoxOfficeTotal;

	// We need to reset the map to force all of the dot sizes to be recalculated
	// based on the new min and max.
	this->dotSizes.clear();
}

double MovieDotSizeProvider::calculateDotSize(double boxOfficeTotal) {
	//scale factors
	static const double SCALE_MULT = 10.0;
	static const double SCALE_ADD = 3.0;
	static const double SCALE_DIV = 50.0;

	double dotSize = 1.0f;

	//uses the triangle distribution
	double a = this->minBoxOfficeTotal;
	double b = this->maxBoxOfficeTotal;

	double c = (b - a) / 3; //some modal value, currently just a mid point

	if (boxOfficeTotal >= a && boxOfficeTotal <= c) {
		dotSize = pow(boxOfficeTotal - a, 2) / ((b - a) * (c - a));
		dotSize = (dotSize * SCALE_MULT + SCALE_ADD) / SCALE_DIV;
	}
	else if (boxOfficeTotal > c && boxOfficeTotal <= b) {
		dotSize = 1.0f - pow(b - boxOfficeTotal, 2) / ((b - a) * (b - c));
		dotSize = (dotSize * SCALE_MULT + SCALE_ADD) / SCALE_DIV;
	}

	return dotSize;
}
