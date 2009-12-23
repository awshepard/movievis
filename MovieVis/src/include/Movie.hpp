/**
 * \file Movie.hpp
 * \author Douglas W. Paul and Adam Shepard
 *
 * Declares the Movie class
 */

#pragma once

#include "Person_pre.hpp"
#include <vector>
#include <string>
#include <boost\smart_ptr\shared_ptr.hpp>
#include <boost\smart_ptr\weak_ptr.hpp>
#include <boost\date_time\gregorian\gregorian_types.hpp>
#include <boost\functional\hash.hpp>

using std::string;
using std::vector;
using boost::shared_ptr;
using boost::weak_ptr;
using boost::gregorian::date;
using boost::hash;

/**
 * A movie
 */
class Movie {
public:

	/** Constructor */
	Movie(string title, date releaseDate, double boxOfficeReceipts);

	/** Gets the movie's title */
	string getTitle() const { return this->title; }

	/** Gets the movie's release date */
	date getReleaseDate() const { return this->releaseDate; }

	/** Gets the directors for the movie */
	const weak_ptr<Person> &getDirector() const { return this->director; }

	/** Gets the cast members for the movie */
	const vector<weak_ptr<Person>> &getCastMembers() const { return this->castMembers; }

	/** Gets the USD total of box office receipts */
	double getBoxOfficeReceipts() const { return this->boxOfficeReceipts; }

	/** Sets the director of the movie */
	void setDirector(const shared_ptr<Person> &director);

	/** Adds a cast member to the movie */
	void addCastMember(const shared_ptr<Person> &castMember);

	/** Sets the release date of the movie */
	void setReleaseDate(const boost::gregorian::date d);

	/** Sets the box office results for the movie */
	void setBoxOfficeReceipts(const double boxOffice);

	/** A sort predicate for sorting movies by release date */
	static bool releaseDateSortPredicate(const Movie &m1, const Movie &m2) {
		return m1.getReleaseDate() < m2.getReleaseDate();
	}

	/** A sort predicate for sorting weak movie pointers by release date */
	static bool sharedPtrReleaseDateSortPredicate(const shared_ptr<Movie> &m1, const shared_ptr<Movie> &m2) {
		return m1->getReleaseDate() < m2->getReleaseDate();
	}

	/** A sort predicate for sorting weak movie pointers by release date */
	static bool weakPtrReleaseDateSortPredicate(const weak_ptr<Movie> &m1, const weak_ptr<Movie> &m2) {
		return m1.lock()->getReleaseDate() < m2.lock()->getReleaseDate();
	}

	/** Determines whether two people are the same */
	bool operator==(const Movie &otherMovie) const;

	struct hashConf {
		enum {              
		bucket_size = 4,  // 0 < bucket_size
		min_buckets = 8}; // min_buckets = 2 ^^ N, 0 < N

		size_t operator()(const Movie &movie) const {
			boost::hash<string> hasher;
			return hasher(movie.getTitle());
		}

		size_t operator()(const shared_ptr<Movie> &movie) const {
			boost::hash<string> hasher;
			return hasher(movie->getTitle());
		}

		bool operator()(const Movie &left, const Movie &right) const {
			return Movie::releaseDateSortPredicate(left, right);
		}

		bool operator()(const shared_ptr<Movie> &left, const shared_ptr<Movie> &right) const {
			return Movie::sharedPtrReleaseDateSortPredicate(left, right);
		}
	};

protected:

	/** The movie's title */
	string title;

	/** The movie's release date */
	date releaseDate;

	/** The USD total of box office receipts */
	double boxOfficeReceipts;

	/** The movie's director */
	weak_ptr<Person> director;

	/** The movie's cast members */
	vector<weak_ptr<Person>> castMembers;

	/** The movie's year of release, used for identification */
	double year;

};
