/**
 * \file ActorColorProvider.hpp
 * \author Douglas W. Paul and Adam Shepard
 *
 * Declares the ActorColorProvider class
 */

#pragma once

#include <Color.hpp>
#include "Person.hpp"
#include <hash_map>
#include <vector>
#include <boost/shared_ptr.hpp>

using std::vector;
using stdext::hash_map;
using boost::shared_ptr;
using peek::Color;

typedef hash_map<shared_ptr<Person>, Color, Person::hashConf> actorColorMap_t;

/**
 * An actor color provider
 */
class ActorColorProvider {
public:

	/** Constructor */
	ActorColorProvider();

	/** Destructor */
	virtual ~ActorColorProvider();
	
	/** Gets the color for a given actor */
	Color getColor(const shared_ptr<Person> &actor);

protected:

	/** The actor colors */
	actorColorMap_t actorColors;

	/** The color palette */
	vector<Color> palette;

	/** The index of the next color in the palette to use */
	unsigned int nextColorIndex;

	unsigned int getNextColorIndex();

	/** Initializes the color palette */
	void initPalette();

};
