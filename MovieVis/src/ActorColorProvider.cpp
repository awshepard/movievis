/**
 * \file ActorColorProvider.cpp
 * \author Douglas W. Paul and Adam Shepard
 *
 * Defines the behavior of the ActorColorProvider class
 */

#include "ActorColorProvider.hpp"

ActorColorProvider::ActorColorProvider() {
	this->nextColorIndex = 0;
	initPalette();
}

ActorColorProvider::~ActorColorProvider() {
}

Color ActorColorProvider::getColor(const shared_ptr<Person> &actor) {
	actorColorMap_t::iterator actorColorMapEntry = this->actorColors.find(actor);

	Color color;

	if (actorColorMapEntry != this->actorColors.end()) {
		color = actorColorMapEntry->second;
	}
	else {
		color = this->palette[getNextColorIndex()];
		this->actorColors[actor] = color;
	}

	return color;
}

unsigned int ActorColorProvider::getNextColorIndex() {
	unsigned int colorIndexToReturn = this->nextColorIndex;
	this->nextColorIndex = (this->nextColorIndex + 1) % this->palette.size();
	return colorIndexToReturn;
}

void ActorColorProvider::initPalette() {
	this->palette.reserve(12);
	this->palette.push_back(Color::orange);
	this->palette.push_back(Color::lime);
	this->palette.push_back(Color::pink);
	this->palette.push_back(Color::teal);
	this->palette.push_back(Color::purple);
	this->palette.push_back(Color::sky);
	this->palette.push_back(Color::cyan);
	this->palette.push_back(Color::magenta);
	this->palette.push_back(Color::red);
	this->palette.push_back(Color::green);
	this->palette.push_back(Color::blue);
}
