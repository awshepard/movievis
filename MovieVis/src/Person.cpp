/**
 * \file Person.cpp
 * \author Douglas W. Paul and Adam Shepard
 *
 * Defines the behavior of the Person class
 */

#include "Person.hpp"
#include "Movie.hpp"

using boost::hash;

Person::Person(string name) {
	this->name = name;
}

bool Person::operator==(const Person &otherPerson) const {
	return (this->name.compare(otherPerson.getName()) == 0);
}

const vector<weak_ptr<Movie>> &Person::getMoviesDirected() const {
	return this->moviesDirected;
}

const vector<weak_ptr<Movie>> &Person::getMoviesStarredIn() const {
	return this->moviesStarredIn;
}

void Person::addMovieDirected(const shared_ptr<Movie> &movie) {
	this->moviesDirected.push_back(weak_ptr<Movie>(movie));
}

void Person::addMovieStarredIn(const shared_ptr<Movie> &movie) {
	this->moviesStarredIn.push_back(weak_ptr<Movie>(movie));
}
