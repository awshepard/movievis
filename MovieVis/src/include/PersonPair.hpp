/**
 * \file PersonPair.hpp
 * \author Douglas W. Paul and Adam Shepard
 *
 * Declares the PersonPair class
 */

#pragma once

#include <boost/shared_ptr.hpp>
#include <boost\functional\hash.hpp>
#include <boost\date_time\gregorian\gregorian_types.hpp>
#include "Person.hpp"

using boost::shared_ptr;
using boost::hash;
using boost::gregorian::date;

/**
 * A pair of Persons
 */
class PersonPair {
public:

	/** Constructor */
	PersonPair(const shared_ptr<Person> &firstPerson, const shared_ptr<Person> &secondPerson);

	/** Destructor */
	virtual ~PersonPair();

	const shared_ptr<Person> &getFirstPerson() const { return this->firstPerson; }

	const shared_ptr<Person> &getSecondPerson() const { return this->secondPerson; }

	/** Determines whether two Person pairs are the same */
	bool operator==(const PersonPair &otherPersonPair) const;

	struct hashConf {
		enum {              
		bucket_size = 4,  // 0 < bucket_size
		min_buckets = 8}; // min_buckets = 2 ^^ N, 0 < N

		size_t operator()(const PersonPair &PersonPair) const {
			static const unsigned int PRIME = 31;
			static const Person::hashConf PERSON_HASH_CONF;
			size_t firstPersonHash = PERSON_HASH_CONF(PersonPair.getFirstPerson());
			size_t secondPersonHash = PERSON_HASH_CONF(PersonPair.getSecondPerson());
			return firstPersonHash * PRIME + secondPersonHash;
		}

		size_t operator()(const shared_ptr<PersonPair> &PersonPair) const {
			static const unsigned int PRIME = 31;
			static const Person::hashConf PERSON_HASH_CONF;
			size_t firstPersonHash = PERSON_HASH_CONF(PersonPair->getFirstPerson());
			size_t secondPersonHash = PERSON_HASH_CONF(PersonPair->getSecondPerson());
			return firstPersonHash * PRIME + secondPersonHash;
		}

		/*bool operator()(const PersonPair &left, const PersonPair &right) const {
			return Person::releaseDateSortPredicate(*left.getFirstPerson(), *right.getFirstPerson());
		}

		bool operator()(const shared_ptr<PersonPair> &left, const shared_ptr<PersonPair> &right) const {
			return Person::sharedPtrReleaseDateSortPredicate(left->getFirstPerson(), right->getFirstPerson());
		}*/
	};

protected:

	/** The first Person */
	shared_ptr<Person> firstPerson;

	/** The second Person */
	shared_ptr<Person> secondPerson;

};
