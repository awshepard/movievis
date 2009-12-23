/**
 * \file ForceDirectedLayoutProvider.cpp
 * \author Douglas W. Paul and Adam Shepard
 *
 * Defines the behavior of the ForceDirectedLayoutProvider class
 */

#include <hash_set>
#include <Numerics.hpp>
#include "ForceDirectedLayoutProvider.hpp"

using stdext::hash_set;
using peek::sign;

ForceDirectedLayoutProvider::ForceDirectedLayoutProvider(const shared_ptr<Model> &model) {
	this->model = model;
	actorCurvesByMoviePairMap_t actorCurvesByMoviePair;
	actorCurvesByActorMap_t actorCurvesByActor;
	directorXPositions;
	init();
}

double ForceDirectedLayoutProvider::getDateYPosition(const boost::gregorian::date &theDate) {
	static const unsigned int EARLIEST_YEAR = 1985;
	static const unsigned int LATEST_YEAR = 2009;
	static const double UNITS_PER_YEAR = 5.0 / (double) (LATEST_YEAR - EARLIEST_YEAR);
	double yearSpan = (LATEST_YEAR - EARLIEST_YEAR) * UNITS_PER_YEAR;

	double dateAsYear = (double) theDate.year() + (double) theDate.day_of_year() / 365.0;
	double yearsAfterEarliestYear = dateAsYear - (double) EARLIEST_YEAR;
	return yearsAfterEarliestYear * UNITS_PER_YEAR - (yearSpan / 2);
}

double ForceDirectedLayoutProvider::getDirectorXPosition(const shared_ptr<Person> &director) {
	static const double DISTANCE = 4.0;

	hash_map<shared_ptr<Person>, double, Person::hashConf>::iterator iter = this->directorXPositions.find(director);

	if (iter == this->directorXPositions.end()) {
		double xPosition = DISTANCE * this->directorXPositions.size();
		this->directorXPositions[director] = xPosition;
		return xPosition;
	}
	else {
		return iter->second;
	}
}

actorCurveVectorPtr_t ForceDirectedLayoutProvider::getActorCurves(const shared_ptr<Person> &actor) {
	actorCurvesByActorMap_t::iterator actorCurvesByActorMapEntry = this->actorCurvesByActor.find(actor);

	if (actorCurvesByActorMapEntry == this->actorCurvesByActor.end()) {
		return actorCurveVectorPtr_t (new vector<shared_ptr<ActorCurve>>());
	}

	return actorCurvesByActorMapEntry->second;
}

void ForceDirectedLayoutProvider::init() {
	createDirectorProximities();
	vector<shared_ptr<Person>> actors = this->model->getActors();
	vector<shared_ptr<Person>>::iterator actorsIter;

	for (actorsIter = actors.begin(); actorsIter != actors.end(); ++actorsIter) {
		shared_ptr<Person> actor = *actorsIter;
		initActorCurves(actor);
	}

	divergeOverlappingCurves();
	
}

void ForceDirectedLayoutProvider::createDirectorProximities()
{
	static const double DISTANCE = 4.0;
	const int NUM_LOOPS = 212;
	int count = 0;
	vector<shared_ptr<Person>> directors = this->model->getDirectors();
	vector<shared_ptr<Person>>::iterator directorIter = directors.begin();

	std::cout << "There are " << directors.size() << " directors." << std::endl;

	const int MAX_NEIGHBORHOOD_SIZE = directors.size();

	//reimplementation - reorder on vector, then set up map
	for(int loopCounter = 0; loopCounter < NUM_LOOPS; loopCounter++) {
		float fractionRemaining = (float)(NUM_LOOPS - loopCounter)/(float)NUM_LOOPS;
		int neighborhoodSize = (int) ceil(fractionRemaining * (float) MAX_NEIGHBORHOOD_SIZE);

		std::cout << "Loop: " << loopCounter+1 << " of " << NUM_LOOPS << std::endl;

		vector<shared_ptr<Person>>::iterator directorIter = directors.begin();
		for(directorIter;directorIter!=directors.end();++directorIter) {
			float leftWeight = 0.0;
			float rightWeight = 0.0;

			int leftSteps = 0;
			int rightSteps = 0;

			vector<shared_ptr<Person>>::iterator leftDirectorIter = directorIter;
			if (leftDirectorIter != directors.begin()) {
				--leftDirectorIter;
				for(leftDirectorIter;leftDirectorIter!=directors.begin() && leftSteps<neighborhoodSize; --leftDirectorIter) {
					leftWeight += computeProximity(*directorIter,*leftDirectorIter);
					leftSteps++;
				}
				leftWeight /= (float) leftSteps;
			}

			vector<shared_ptr<Person>>::iterator rightDirectorIter = directorIter;
			if (rightDirectorIter != directors.end()) {
				++rightDirectorIter;
				for(rightDirectorIter;rightDirectorIter!=directors.end() && rightSteps<neighborhoodSize; ++rightDirectorIter) {
					rightWeight += computeProximity(*directorIter,*rightDirectorIter);
					rightSteps++;
				}
				rightWeight /= (float) rightSteps;
			}

			if (leftWeight>rightWeight) {
				if(directorIter != directors.begin()) {
					vector<shared_ptr<Person>>::iterator prevDirectorIter = directorIter + (-1);
					//prevDirectorIter--;
					shared_ptr<Person> temp = *prevDirectorIter;
					*prevDirectorIter = *directorIter;
					*directorIter = temp;
				}
			}
			if (leftWeight<rightWeight) {
				if((directorIter)!=directors.end()) {
					vector<shared_ptr<Person>>::iterator nextDirectorIter = directorIter + 1;
					//nextDirectorIter++;
					shared_ptr<Person> temp = *nextDirectorIter;
					*nextDirectorIter = *directorIter;
					*directorIter = temp;
				}
			}
		}
	}

	directorIter = directors.begin();
	//initialize director x positions in increments of DISTANCE
	for(directorIter;directorIter!=directors.end();++directorIter,++count) {
		this->directorXPositions[*directorIter] = DISTANCE * (double)count;
	}
}

float getRandom(const float minv, const float maxv) {
  return minv + ((maxv - minv) * (float)rand() / (float)RAND_MAX);
} 

unsigned int ForceDirectedLayoutProvider::computeProximity(const shared_ptr<Person> &director1, const shared_ptr<Person> &director2) {
	vector<weak_ptr<Movie>> moviesDirectedOne = director1->getMoviesDirected();
	vector<weak_ptr<Movie>> moviesDirectedTwo = director2->getMoviesDirected();
	
	hash_set<string> whoIveWorkedWithOne;
	hash_set<string> whoIveWorkedWithTwo;

	//for each movie
	vector<weak_ptr<Movie>>::iterator iterOne = moviesDirectedOne.begin();
	for(iterOne;iterOne!=moviesDirectedOne.end();++iterOne)
	{
		//get list of actors
		vector<weak_ptr<Person>> cast = iterOne->lock()->getCastMembers();

		//add them to the overall list
		vector<weak_ptr<Person>>::iterator castIter = cast.begin();
		for (castIter; castIter != cast.end(); ++castIter) {
			whoIveWorkedWithOne.insert(castIter->lock()->getName());
		}
	}
	vector<weak_ptr<Movie>>::iterator iterTwo = moviesDirectedTwo.begin();	
	for(iterTwo;iterTwo!=moviesDirectedTwo.end();++iterTwo)
	{
		//get list of actors
		vector<weak_ptr<Person>> cast = iterTwo->lock()->getCastMembers();

		//add them to the overall list
		vector<weak_ptr<Person>>::iterator castIter = cast.begin();
		for (castIter; castIter != cast.end(); ++castIter) {
			whoIveWorkedWithTwo.insert(castIter->lock()->getName());
		}
	}

	unsigned int proximity = 0;
	
	//count overlap betwen two whoIveWorkedWith vectors
	hash_set<string>::iterator workedWithIter = whoIveWorkedWithOne.begin();
	for (workedWithIter; workedWithIter != whoIveWorkedWithOne.end(); ++workedWithIter) {
		hash_set<string>::const_iterator it = whoIveWorkedWithTwo.find(*workedWithIter);
		if (it != whoIveWorkedWithTwo.end())
			proximity++;
	}

	return proximity;
}

void ForceDirectedLayoutProvider::initActorCurves(const shared_ptr<Person> &actor) {
	// Feel free to play with this number until things look good
	static const double MAX_DELTA = 5.0;

	vector<weak_ptr<Movie>> moviesStarredIn = actor->getMoviesStarredIn();

	if (moviesStarredIn.size() == 0) {
		// Don't render the actor's line if we're not looking at any
		// movies he's starred in.
		return;
	}

	double z = 0.0;

	sort(moviesStarredIn.begin(), moviesStarredIn.end(), Movie::weakPtrReleaseDateSortPredicate);

	vector<weak_ptr<Movie>>::const_iterator moviePtrIter = moviesStarredIn.begin();
	shared_ptr<Movie> lastMoviePtr = moviePtrIter->lock();
	Point3d lastAnchor = getMoviePoint(lastMoviePtr, z);
	for (++moviePtrIter; moviePtrIter != moviesStarredIn.end(); ++moviePtrIter) {
		shared_ptr<Movie> moviePtr = moviePtrIter->lock();
		Point3d anchor = getMoviePoint(moviePtr, z);

		double nominalDelta = (anchor.y - lastAnchor.y);

		/// \todo Not checking if we can lock or not is risky
		if (lastMoviePtr->getDirector().lock() == moviePtr->getDirector().lock()) {
			nominalDelta /= 3.0;
		}

		double delta = (fabs(nominalDelta) <= MAX_DELTA ? nominalDelta : MAX_DELTA * sign(nominalDelta));

		Point3d handle1(lastAnchor.x, lastAnchor.y + delta, lastAnchor.z);
		Point3d handle2(anchor.x, anchor.y - delta, anchor.z);

		shared_ptr<MoviePair> moviePair(new MoviePair(lastMoviePtr, moviePtr));
		shared_ptr<ActorCurve> actorCurve(new ActorCurve(actor, BezierCurve(lastAnchor, handle1, handle2, anchor)));

		addActorCurve(actorCurve, moviePair);

		lastAnchor = anchor;
	}

}

void ForceDirectedLayoutProvider::divergeOverlappingCurves() {
	actorCurvesByMoviePairMap_t::iterator pairIter;
	for (pairIter = this->actorCurvesByMoviePair.begin(); pairIter != this->actorCurvesByMoviePair.end(); ++pairIter) {
		shared_ptr<MoviePair> moviePairPtr = pairIter->first;
		actorCurveVectorPtr_t actorCurveVectorPtr = pairIter->second;

		if (actorCurveVectorPtr->size() <= 1) {
			continue;
		}
		
		Point3d m1 = getMoviePoint(moviePairPtr->getFirstMovie(), 0.0);
		Point3d m2 = getMoviePoint(moviePairPtr->getSecondMovie(), 0.0);
		
		Vector3d v = m2 - m1;
		Vector3d offsetDirection(-v.y, v.x, v.z);
		offsetDirection.normalize();

		static const double offsetIncrement = 0.05;
		unsigned int actorIndex = 0;

		actorCurveVector_t::iterator actorIter;
		for (actorIter = actorCurveVectorPtr->begin(); actorIter != actorCurveVectorPtr->end(); ++actorIter) {
			shared_ptr<ActorCurve> actorCurvePtr = *actorIter;

			double offsetAmount = (actorIndex == 0 ? 0.0 : ((actorIndex+1)/2) * offsetIncrement * (actorIndex%2==0 ? -1.0 : 1.0));
			Vector3d offsetVector = offsetAmount * offsetDirection;
			
			// This assumes the curve isn't a pair already

			pair<BezierCurve, BezierCurve> bp = actorCurvePtr->getFirstCurve().subdivideAt(0.5);
			BezierCurve b1 = bp.first;
			BezierCurve b2 = bp.second;

			BezierCurve newB1(b1.getP0(), b1.getP1(), b1.getP2() + offsetVector, b1.getP3() + offsetVector);
			BezierCurve newB2(b2.getP0() + offsetVector, b2.getP1() + offsetVector, b2.getP2(), b2.getP3());

			actorCurvePtr->setCurves(newB1, newB2);

			actorIndex++;
		}
	}
}

Point3d ForceDirectedLayoutProvider::getMoviePoint(const shared_ptr<Movie> &movie, double z) {
	double x = getDirectorXPosition(movie->getDirector().lock());
	double y = getDateYPosition(movie->getReleaseDate());
	return Point3d(x, y, z);
}

void ForceDirectedLayoutProvider::addActorCurve(const shared_ptr<ActorCurve> &actorCurve, const shared_ptr<MoviePair> &moviePair) {
	actorCurveVectorPtr_t actorCurveVectorPtr;

	// Add the curve to the list of curves by actor ...
	shared_ptr<Person> actor = actorCurve->getActor();

	actorCurvesByActorMap_t::iterator actorCurvesByActorMapEntry = this->actorCurvesByActor.find(actor);

	if (actorCurvesByActorMapEntry == this->actorCurvesByActor.end()) {
		actorCurveVectorPtr = actorCurveVectorPtr_t(new vector<shared_ptr<ActorCurve>>());
		this->actorCurvesByActor[actor] = actorCurveVectorPtr;
	}
	else {
		actorCurveVectorPtr = actorCurvesByActorMapEntry->second;
	}

	actorCurveVectorPtr->push_back(actorCurve);

	// Add the curve to the list of curves by movie pair ...

	actorCurvesByMoviePairMap_t::iterator actorCurvesByMoviePairMapEntry = this->actorCurvesByMoviePair.find(moviePair);

	if (actorCurvesByMoviePairMapEntry == this->actorCurvesByMoviePair.end()) {
		actorCurveVectorPtr = actorCurveVectorPtr_t(new vector<shared_ptr<ActorCurve>>());
		this->actorCurvesByMoviePair[moviePair] = actorCurveVectorPtr;
	}
	else {
		actorCurveVectorPtr = actorCurvesByMoviePairMapEntry->second;
	}

	actorCurveVectorPtr->push_back(actorCurve);
}
