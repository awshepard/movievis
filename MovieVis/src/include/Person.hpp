/**
 * \file Person.hpp
 * \author Douglas W. Paul and Adam Shepard
 *
 * Declares the Person class
 */

#pragma once

#include "Movie_pre.hpp"
#include <vector>
#include <boost\smart_ptr\shared_ptr.hpp>
#include <boost\smart_ptr\weak_ptr.hpp>
#include <boost\functional\hash.hpp>

using std::string;
using std::vector;
using boost::shared_ptr;
using boost::weak_ptr;
using boost::hash;

/**
 * A person involved with movies
 */
class Person {
public:

	/** Constructor */
	Person(string name);

	/** Get's the person's name */
	string getName() const { return this->name; }

	/** Gets the movies this person directed */
	const vector<weak_ptr<Movie>> &getMoviesDirected() const;

	/** Gets the movies this person starred in */
	const vector<weak_ptr<Movie>> &getMoviesStarredIn() const;

	/** Adds a movie this person directed */
	void addMovieDirected(const shared_ptr<Movie> &movie);

	/** Adds a movies this person starred in */
	void addMovieStarredIn(const shared_ptr<Movie> &movie);

	/** Determines whether two people are the same */
	bool operator==(const Person &otherPerson) const;

	struct hashConf {
		enum {              
		bucket_size = 4,  // 0 < bucket_size
		min_buckets = 8}; // min_buckets = 2 ^^ N, 0 < N

		size_t operator()(const Person &person) const {
			boost::hash<string> hasher;
			return hasher(person.getName());
		}

		size_t operator()(const shared_ptr<Person> &person) const {
			boost::hash<string> hasher;
			return hasher(person->getName());
		}

		bool operator()(const Person &left, const Person &right) const {
			boost::hash<string> hasher;
			return hasher(left.getName()) < hasher(right.getName());
		}

		bool operator()(const shared_ptr<Person> &left, const shared_ptr<Person> &right) const {
			boost::hash<string> hasher;
			return hasher(left->getName()) < hasher(right->getName());
		}
	};

protected:

	/** The person's name */
	string name;

	/** The movies this person directed */
	vector<weak_ptr<Movie>> moviesDirected;

	/** The movies this person starred in */
	vector<weak_ptr<Movie>> moviesStarredIn;

};
