/**
 * \file ForceDirectedLayoutProvider.hpp
 * \author Douglas W. Paul and Adam Shepard
 *
 * Declares the ForceDirectedLayoutProvider class
 */

#pragma once

#include <vector>
#include <hash_map>
#include <boost/shared_ptr.hpp>
#include "LayoutProvider.hpp"
#include "Model.hpp"
#include "MoviePair.hpp"
#include "PersonPair.hpp"
#include "ActorCurve.hpp"

using std::vector;
using stdext::hash_map;
using boost::shared_ptr;

typedef hash_map<shared_ptr<MoviePair>, actorCurveVectorPtr_t, MoviePair::hashConf> actorCurvesByMoviePairMap_t;
typedef hash_map<shared_ptr<Person>,actorCurveVectorPtr_t, Person::hashConf> actorCurvesByActorMap_t;

/**
 * A ForceDirected layout provider
 */
class ForceDirectedLayoutProvider : public LayoutProvider {
public:

	/** Constructor */
	ForceDirectedLayoutProvider(const shared_ptr<Model> &model);

	/** Gets the X position of the given director */
	virtual double getDirectorXPosition(const shared_ptr<Person> &director);

	/** Gets the Y position of the given date */
	virtual double getDateYPosition(const date &theDate);

	/** Gets the drawables for the given actor */
	virtual actorCurveVectorPtr_t getActorCurves(const shared_ptr<Person> &actor);

protected:

	/** The model */
	shared_ptr<Model> model;

	/** Director X-positions */
	hash_map<shared_ptr<Person>, double, Person::hashConf> directorXPositions;

	hash_map<shared_ptr<Person>, map<std::string,shared_ptr<Person>>> directorWorkedWith;

	/** Actor curves by movie pair */
	actorCurvesByMoviePairMap_t actorCurvesByMoviePair;

	/** Actor curves by actor */
	actorCurvesByActorMap_t actorCurvesByActor;

	/** Initialize the maps from the model */
	void init();

	void initActorCurves(const shared_ptr<Person> &actor);

	void createDirectorProximities();
	unsigned int computeProximity(const shared_ptr<Person> &director1, const shared_ptr<Person> &director2);

	void divergeOverlappingCurves();

	Point3d getMoviePoint(const shared_ptr<Movie> &movie, double z);

	void addActorCurve(const shared_ptr<ActorCurve> &actorCurve, const shared_ptr<MoviePair> &moviePair);
};