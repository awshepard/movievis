/**
 * \file LayoutProvider.hpp
 * \author Douglas W. Paul and Adam Shepard
 *
 * Declares the LayoutProvider interface
 */

#pragma once

#include <Drawable.hpp>
#include <boost\date_time\gregorian\gregorian_types.hpp>
#include "Person.hpp"
#include "ActorCurve.hpp"

using boost::gregorian::date;

typedef vector<shared_ptr<ActorCurve>> actorCurveVector_t;
typedef shared_ptr<actorCurveVector_t> actorCurveVectorPtr_t;

/**
 * \interface LayoutProvider
 * \brief An interface for something that provides a layout
 */
class LayoutProvider  {
public:
	
	/** Gets the X position of the given director */
	virtual double getDirectorXPosition(const shared_ptr<Person> &director) = 0;

	/** Gets the Y position of the given date */
	virtual double getDateYPosition(const date &theDate) = 0;

	/** Gets the drawables for the given actor */
	virtual actorCurveVectorPtr_t getActorCurves(const shared_ptr<Person> &actor) = 0;

};
