/**
 * \file MoviePair.hpp
 * \author Douglas W. Paul and Adam Shepard
 *
 * Declares the MoviePair class
 */

#pragma once

#include <boost/shared_ptr.hpp>
#include <boost\functional\hash.hpp>
#include <boost\date_time\gregorian\gregorian_types.hpp>
#include "Movie.hpp"

using boost::shared_ptr;
using boost::hash;
using boost::gregorian::date;

/**
 * A pair of movies
 */
class MoviePair {
public:

	/** Constructor */
	MoviePair(const shared_ptr<Movie> &firstMovie, const shared_ptr<Movie> &secondMovie);

	/** Destructor */
	virtual ~MoviePair();

	const shared_ptr<Movie> &getFirstMovie() const { return this->firstMovie; }

	const shared_ptr<Movie> &getSecondMovie() const { return this->secondMovie; }

	/** Determines whether two movie pairs are the same */
	bool operator==(const MoviePair &otherMoviePair) const;

	struct hashConf {
		enum {              
		bucket_size = 4,  // 0 < bucket_size
		min_buckets = 8}; // min_buckets = 2 ^^ N, 0 < N

		size_t operator()(const MoviePair &moviePair) const {
			static const unsigned int PRIME = 31;
			static const Movie::hashConf MOVIE_HASH_CONF;
			size_t firstMovieHash = MOVIE_HASH_CONF(moviePair.getFirstMovie());
			size_t secondMovieHash = MOVIE_HASH_CONF(moviePair.getSecondMovie());
			return firstMovieHash * PRIME + secondMovieHash;
		}

		size_t operator()(const shared_ptr<MoviePair> &moviePair) const {
			static const unsigned int PRIME = 31;
			static const Movie::hashConf MOVIE_HASH_CONF;
			size_t firstMovieHash = MOVIE_HASH_CONF(moviePair->getFirstMovie());
			size_t secondMovieHash = MOVIE_HASH_CONF(moviePair->getSecondMovie());
			return firstMovieHash * PRIME + secondMovieHash;
		}

		bool operator()(const MoviePair &left, const MoviePair &right) const {
			return Movie::releaseDateSortPredicate(*left.getFirstMovie(), *right.getFirstMovie());
		}

		bool operator()(const shared_ptr<MoviePair> &left, const shared_ptr<MoviePair> &right) const {
			return Movie::sharedPtrReleaseDateSortPredicate(left->getFirstMovie(), right->getFirstMovie());
		}
	};

protected:

	/** The first movie */
	shared_ptr<Movie> firstMovie;

	/** The second movie */
	shared_ptr<Movie> secondMovie;

};
