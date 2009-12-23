/**
 * \file BezierCurve.hpp
 * \author Douglas W. Paul and Adam Shepard
 *
 * Declares the BezierCurve class
 */

#pragma once

#include <utility>
#include <Drawable.hpp>
#include <Geometry.hpp>

using std::pair;
using peek::Drawable;
using peek::Point3d;
using peek::Vector3d;

static const unsigned int NUM_BEZIER_SEGMENTS = 16;

/**
 * A Bezier curve
 */
class BezierCurve : public Drawable {
public:

	/** Constructor */
	BezierCurve(const Point3d &anchor1, const Point3d &handle1, const Point3d &handle2, const Point3d &anchor2);

	/** Destructor */
	virtual ~BezierCurve();

	/** Draws the bezier curve */
	virtual void draw();

	/** Evaluate the curve at the given value of t */
	Point3d evaluateAt(double t);

	/** Evaluate the curve at the given value of t */
	pair<BezierCurve, BezierCurve> subdivideAt(double t);

	Point3d getP0() const { return this->p0; }
	Point3d getP1() const { return this->p1; }
	Point3d getP2() const { return this->p2; }
	Point3d getP3() const { return this->p3; }

	static Point3d evaluateCubic(const Vector3d &p0, const Vector3d &p1, const Vector3d &p2, const Vector3d &p3, double t);
	static Point3d evaluateQuadratic(const Vector3d &p0, const Vector3d &p1, const Vector3d &p2, double t);
	static Point3d evaluateLinear(const Vector3d &p0, const Vector3d &p1, double t);
	
protected:

	/** The first anchor */
	Vector3d p0;

	/** The first handle */
	Vector3d p1;

	/** The second handle */
	Vector3d p2;

	/** The second anchor */
	Vector3d p3;

	/** Cached samples */
	Point3d points[NUM_BEZIER_SEGMENTS+1];

};
