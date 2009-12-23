/**
 * \file ActorCurve.hpp
 * \author Douglas W. Paul and Adam Shepard
 *
 * Declares the ActorCurve class
 */

#pragma once

#include <utility>
#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>
#include <Drawable.hpp>
#include "Person.hpp"
#include "BezierCurve.hpp"

using std::pair;
using boost::shared_ptr;
using boost::optional;
using peek::Drawable;

/**
 * A curve representing an actor
 */
class ActorCurve : public Drawable {
public:

	/** Single-curve constructor */
	ActorCurve(const shared_ptr<Person> &actor, BezierCurve curve);

	/** Dual-curve constructor */
	ActorCurve(const shared_ptr<Person> &actor, BezierCurve curve1, BezierCurve curve2);

	/** Destructor */
	virtual ~ActorCurve();

	/** Sets the curve pair to a single curve */
	void setCurves(BezierCurve curve);

	/** Sets the curve pair */
	void setCurves(BezierCurve curve1, BezierCurve curve2);

	/** Draws the curve */
	virtual void draw();

	/** Gets the actor this curve represents */
	const shared_ptr<Person> &getActor() const { return this->actor; }

	/** Gets the first curve */
	BezierCurve getFirstCurve() const { return this->curves.first; }
	
	/** Gets the first curve */
	optional<BezierCurve> getSecondCurve() const { return this->curves.second; }

	bool isSplit() const { return this->curves.second; }
	
protected:

	/** The actor this curve represents */
	shared_ptr<Person> actor;

	/** The curve */
	pair<BezierCurve, optional<BezierCurve>> curves;

};
