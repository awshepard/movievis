/**
 * \file ActorCurve.cpp
 * \author Douglas W. Paul and Adam Shepard
 *
 * Defines the behavior of the ActorCurve class
 */

#include "ActorCurve.hpp"

ActorCurve::ActorCurve(const shared_ptr<Person> &actor, BezierCurve curve)
	: actor(actor), curves(curve, optional<BezierCurve>()) {
}

ActorCurve::ActorCurve(const shared_ptr<Person> &actor, BezierCurve curve1, BezierCurve curve2)
	: actor(actor), curves(curve1, optional<BezierCurve>(curve2)) {
}

ActorCurve::~ActorCurve() {
}

void ActorCurve::setCurves(BezierCurve curve) {
	this->curves = pair<BezierCurve, optional<BezierCurve>>(curve, optional<BezierCurve>());
}

void ActorCurve::setCurves(BezierCurve curve1, BezierCurve curve2) {
	this->curves = pair<BezierCurve, optional<BezierCurve>>(curve1, optional<BezierCurve>(curve2));
}

void ActorCurve::draw() {
	this->curves.first.draw();

	if (this->curves.second)
		this->curves.second.get().draw();
}