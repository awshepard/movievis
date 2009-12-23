/**
 * \file MockDataProvider.cpp
 * \author Douglas W. Paul and Adam Shepard
 *
 * Defines the behavior of the MockDataProvider class
 */

#include "MockDataProvider.hpp"

MockDataProvider::MockDataProvider() {
	init();
}

const vector<shared_ptr<Movie>> &MockDataProvider::getMovies() const {
	return this->movies;
}

const vector<shared_ptr<Person>> &MockDataProvider::getActors() const {
	return this->actors;
}

const vector<shared_ptr<Person>> &MockDataProvider::getDirectors() const {
	return this->directors;
}

const double MockDataProvider::getMaxBoxOffice() const
{
	return this->maxBoxOffice;
}
const double MockDataProvider::getMinBoxOffice() const
{
	return this->minBoxOffice;
}
void MockDataProvider::init() {
	//initialize our max and min
	this->minBoxOffice = 5000;
	this->maxBoxOffice = 1000000000;

	shared_ptr<Movie> missionImpossible3(new Movie("Mission: Impossible III",date(2006,5,5),133382309));
	shared_ptr<Movie> warOfTheWorlds(new Movie("War of the Worlds",date(2005,6,29),234280354));
	shared_ptr<Movie> piratesOfTheCaribbean(new Movie("Pirates of the Caribbean",date(2003,7,9),305388685));
	shared_ptr<Movie> insideMan(new Movie("Inside Man",date(2006,3,24),88504640));
	shared_ptr<Movie> domino(new Movie("Domino",date(2005,10,14),10169202));
	shared_ptr<Movie> manOnFire(new Movie("Man On Fire",date(2004,4,23),77862546));
	shared_ptr<Movie> topGun(new Movie("Top Gun", date(1986, 5, 6),176650237));
	shared_ptr<Movie> boogieNights(new Movie("Boogie Nights", date(1997, 10, 10),26384919));
	shared_ptr<Movie> magnolia(new Movie("Magnolia", date(2000, 1, 7),22450975));
	shared_ptr<Movie> talladegaNights(new Movie("Talladega Nights", date(2006, 8, 4),148213377));
	
	shared_ptr<Person> jjAbrams(new Person("J.J. Abrams"));
	shared_ptr<Person> stevenSpielberg(new Person("Steven Spielberg"));
	shared_ptr<Person> goreVerbinski(new Person("Gore Verbinski"));
	shared_ptr<Person> spikeLee(new Person("Spike Lee"));
	shared_ptr<Person> tonyScott(new Person("Tony Scott"));
	shared_ptr<Person> paulAnderson(new Person("Paul Thomas Anderson"));
	shared_ptr<Person> adamMcKay(new Person("Adam McKay"));

	shared_ptr<Person> dakotaFanning(new Person("Dakota Fanning"));
	shared_ptr<Person> keiraKnightley(new Person("Keira Knightley"));
	shared_ptr<Person> denzelWashington(new Person("Denzel Washington"));
	shared_ptr<Person> williamMacy(new Person("William H. Macy"));
	shared_ptr<Person> johnReilly(new Person("John C. Reilly"));
	shared_ptr<Person> tomCruise(new Person("Tom Cruise"));
	shared_ptr<Person> philipHoffman(new Person("Philip Seymour Hoffman"));

	missionImpossible3->setDirector(jjAbrams);
	jjAbrams->addMovieDirected(missionImpossible3);
	missionImpossible3->addCastMember(philipHoffman);
	missionImpossible3->addCastMember(tomCruise);
	philipHoffman->addMovieStarredIn(missionImpossible3);
	tomCruise->addMovieStarredIn(missionImpossible3);

	warOfTheWorlds->setDirector(stevenSpielberg);
	stevenSpielberg->addMovieDirected(warOfTheWorlds);
	warOfTheWorlds->addCastMember(tomCruise);
	warOfTheWorlds->addCastMember(dakotaFanning);
	tomCruise->addMovieStarredIn(warOfTheWorlds);
	dakotaFanning->addMovieStarredIn(warOfTheWorlds);

	piratesOfTheCaribbean->setDirector(goreVerbinski);
	goreVerbinski->addMovieDirected(piratesOfTheCaribbean);
	piratesOfTheCaribbean->addCastMember(keiraKnightley);
	keiraKnightley->addMovieStarredIn(piratesOfTheCaribbean);

	insideMan->setDirector(spikeLee);
	spikeLee->addMovieDirected(insideMan);
	insideMan->addCastMember(denzelWashington);
	denzelWashington->addMovieStarredIn(insideMan);

	domino->setDirector(tonyScott);
	tonyScott->addMovieDirected(domino);
	domino->addCastMember(keiraKnightley);
	keiraKnightley->addMovieStarredIn(domino);

	manOnFire->setDirector(tonyScott);
	tonyScott->addMovieDirected(manOnFire);
	manOnFire->addCastMember(denzelWashington);
	denzelWashington->addMovieStarredIn(manOnFire);
	manOnFire->addCastMember(dakotaFanning);
	dakotaFanning->addMovieStarredIn(manOnFire);

	topGun->setDirector(tonyScott);
	tonyScott->addMovieDirected(topGun);
	topGun->addCastMember(tomCruise);
	tomCruise->addMovieStarredIn(topGun);

	boogieNights->setDirector(paulAnderson);
	paulAnderson->addMovieDirected(boogieNights);
	boogieNights->addCastMember(williamMacy);
	williamMacy->addMovieStarredIn(boogieNights);
	boogieNights->addCastMember(johnReilly);
	johnReilly->addMovieStarredIn(boogieNights);
	boogieNights->addCastMember(philipHoffman);
	philipHoffman->addMovieStarredIn(boogieNights);

	magnolia->setDirector(paulAnderson);
	paulAnderson->addMovieDirected(magnolia);
	magnolia->addCastMember(williamMacy);
	williamMacy->addMovieStarredIn(magnolia);
	magnolia->addCastMember(johnReilly);
	johnReilly->addMovieStarredIn(magnolia);
	magnolia->addCastMember(tomCruise);
	tomCruise->addMovieStarredIn(magnolia);
	magnolia->addCastMember(philipHoffman);
	philipHoffman->addMovieStarredIn(magnolia);

	talladegaNights->setDirector(adamMcKay);
	adamMcKay->addMovieDirected(talladegaNights);
	talladegaNights->addCastMember(johnReilly);
	johnReilly->addMovieStarredIn(talladegaNights);

	this->movies.reserve(10);
	this->movies.push_back(missionImpossible3);
	this->movies.push_back(warOfTheWorlds);
	this->movies.push_back(piratesOfTheCaribbean);
	this->movies.push_back(insideMan);
	this->movies.push_back(domino);
	this->movies.push_back(manOnFire);
	this->movies.push_back(topGun);
	this->movies.push_back(boogieNights);
	this->movies.push_back(magnolia);
	this->movies.push_back(talladegaNights);

	this->directors.reserve(7);
	this->directors.push_back(jjAbrams);
	this->directors.push_back(stevenSpielberg);
	this->directors.push_back(goreVerbinski);
	this->directors.push_back(spikeLee);
	this->directors.push_back(tonyScott);
	this->directors.push_back(paulAnderson);
	this->directors.push_back(adamMcKay);

	this->actors.reserve(7);
	this->actors.push_back(dakotaFanning);
	this->actors.push_back(keiraKnightley);
	this->actors.push_back(denzelWashington);
	this->actors.push_back(williamMacy);
	this->actors.push_back(johnReilly);
	this->actors.push_back(tomCruise);
	this->actors.push_back(philipHoffman);
}
