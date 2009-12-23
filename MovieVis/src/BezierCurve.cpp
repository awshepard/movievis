/**
 * \file BezierCurve.cpp
 * \author Douglas W. Paul and Adam Shepard
 *
 * Defines the behavior of the BezierCurve class
 */

#include "BezierCurve.hpp"

BezierCurve::BezierCurve(const Point3d &anchor1, const Point3d &handle1, const Point3d &handle2, const Point3d &anchor2) {
	this->p0 = anchor1;
	this->p1 = handle1;
	this->p2 = handle2;
	this->p3 = anchor2;

	for (int i = 0; i <= NUM_BEZIER_SEGMENTS; i++) {
		double t = (double) i / (double) NUM_BEZIER_SEGMENTS;
		this->points[i] = evaluateAt(t);
	}
}

BezierCurve::~BezierCurve() {
}

void BezierCurve::draw() {
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= NUM_BEZIER_SEGMENTS; i++) {
		glVertex3d(this->points[i].x, this->points[i].y, this->points[i].z);
	}
	glEnd();
}

/**
 * See http://en.wikipedia.org/wiki/B%C3%A9zier_curve#Cubic_B.C3.A9zier_curves
 * \param t A t-value in the range [0, 1]
 */
Point3d BezierCurve::evaluateAt(double t) {
	return evaluateCubic(this->p0, this->p1, this->p2, this->p3, t);
}

/**
 * \param t A t-value in the range [0, 1]
 */
pair<BezierCurve, BezierCurve> BezierCurve::subdivideAt(double t) {
	Point3d p0 = this->p0;
	Point3d p1 = evaluateLinear(this->p0, this->p1, t);
	Point3d p2 = evaluateQuadratic(this->p0, this->p1, this->p2, t);
	Point3d p3 = evaluateCubic(this->p0, this->p1, this->p2, this->p3, t);
	Point3d p4 = evaluateQuadratic(this->p1, this->p2, this->p3, t);
	Point3d p5 = evaluateLinear(this->p2, this->p3, t);
	Point3d p6 = this->p3;

	BezierCurve bezier1(p0, p1, p2, p3);
	BezierCurve bezier2(p3, p4, p5, p6);

	return pair<BezierCurve, BezierCurve>(bezier1, bezier2);
}

/**
 * See http://en.wikipedia.org/wiki/B%C3%A9zier_curve#Cubic_B.C3.A9zier_curves
 * \param t A t-value in the range [0, 1]
 */
Point3d BezierCurve::evaluateCubic(const Vector3d &p0, const Vector3d &p1, const Vector3d &p2, const Vector3d &p3, double t) {
	double coef0 = pow(1.0 - t, 3);
	double coef1 = 3.0 * pow(1.0 - t, 2) * t;
	double coef2 = 3.0 * (1.0 - t) * pow(t, 2);
	double coef3 = pow(t, 3);

	return
		(coef0 * p0) +
		(coef1 * p1) +
		(coef2 * p2) +
		(coef3 * p3);
}

/**
 * See http://en.wikipedia.org/wiki/B%C3%A9zier_curve#Quadratic_B.C3.A9zier_curves
 * \param t A t-value in the range [0, 1]
 */
Point3d BezierCurve::evaluateQuadratic(const Vector3d &p0, const Vector3d &p1, const Vector3d &p2, double t) {
	double coef0 = pow(1.0 - t, 2);
	double coef1 = 2.0 * (1.0 - t) * t;
	double coef2 = pow(t, 2);

	return
		(coef0 * p0) +
		(coef1 * p1) +
		(coef2 * p2);
}

/**
 * See http://en.wikipedia.org/wiki/B%C3%A9zier_curve#Linear_B.C3.A9zier_curves
 * \param t A t-value in the range [0, 1]
 */
Point3d BezierCurve::evaluateLinear(const Vector3d &p0, const Vector3d &p1, double t) {
	double coef0 = (1.0 - t);
	double coef1 = t;

	return
		(coef0 * p0) +
		(coef1 * p1);
}