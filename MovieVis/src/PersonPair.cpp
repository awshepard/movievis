/**
 * \file PersonPair.cpp
 * \author Douglas W. Paul and Adam Shepard
 *
 * Defines the behavior of the PersonPair class
 */

#include "PersonPair.hpp"

PersonPair::PersonPair(const shared_ptr<Person> &firstPerson, const shared_ptr<Person> &secondPerson) {
	this->firstPerson = firstPerson;
	this->secondPerson = secondPerson;
}

PersonPair::~PersonPair() {
}
