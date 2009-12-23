/**
 * \file Movie.cpp
 * \author Douglas W. Paul and Adam Shepard
 *
 * Defines the behavior of the Movie class
 */

#include "Movie.hpp"
#include "Person.hpp"

Movie::Movie(string title, date releaseDate, double boxOfficeReceipts) {
	this->title = title;
	this->releaseDate = releaseDate;
	this->boxOfficeReceipts = boxOfficeReceipts;
}

bool Movie::operator==(const Movie &otherMovie) const {
	return (this->title.compare(otherMovie.getTitle()) == 0);
}

void Movie::setDirector(const shared_ptr<Person> &director) {
	this->director = weak_ptr<Person>(director);
}

void Movie::addCastMember(const shared_ptr<Person> &castMember) {
	this->castMembers.push_back(weak_ptr<Person>(castMember));
}

void Movie::setReleaseDate(const boost::gregorian::date d)
{
	this->releaseDate = d;
}

void Movie::setBoxOfficeReceipts(const double boxOffice)
{
	this->boxOfficeReceipts = boxOffice;
}