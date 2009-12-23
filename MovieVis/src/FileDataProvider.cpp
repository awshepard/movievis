/**
 * \file FileDataProvider.cpp
 * \author Douglas W. Paul and Adam Shepard
 *
 * Defines the behavior of the FileDataProvider class
 */

#include "FileDataProvider.hpp"
#include <fstream>
#include <iostream>

FileDataProvider::FileDataProvider() {
	init();
}

const vector<shared_ptr<Movie>> &FileDataProvider::getMovies() const {
	return this->movies;
}

const vector<shared_ptr<Person>> &FileDataProvider::getActors() const {
	return this->actorVector;
}

const vector<shared_ptr<Person>> &FileDataProvider::getDirectors() const {
	return this->directors;
}

const double FileDataProvider::getMaxBoxOffice() const
{
	return this->maxBoxOffice;
}
const double FileDataProvider::getMinBoxOffice() const
{
	return this->minBoxOffice;
}
void FileDataProvider::init() {
	//initialize our max and min
	this->minBoxOffice = 1000000000;
	this->maxBoxOffice = 0;

	readMovies("../Data/all_but_actors_317.dat");
	readActors("../Data/actors.massaged");
	eliminateNoNameActors();

	list<shared_ptr<Person>>::const_iterator actorIter;
	for (actorIter = this->actors.begin(); actorIter != this->actors.end(); ++actorIter) {
		this->actorVector.push_back(*actorIter);
	}
}

void FileDataProvider::readMovies(std::string file) {
	std::ifstream inFile;
	inFile.open(&file[0],std::ios::in);
	if(inFile.fail()) {
		std::cerr << "Crap, we failed to read the movies file." << std::endl;
	}

	string line;
	int lineNum = 0;

	while (getline(inFile, line)) {
		lineNum++;

		if (lineNum == 1) continue;

		std::istringstream linestream(line);

		string movieTitle;
		string movieYearText;
		string directorName;
		string movieGrossText;
		string releaseDateText;

		getline(linestream, movieTitle, '|');
		getline(linestream, movieYearText, '|');
		getline(linestream, directorName, '|');
		getline(linestream, movieGrossText, '|');
		getline(linestream, releaseDateText, '|');

		long movieGross = atol(movieGrossText.c_str());
		date releaseDate(from_string(releaseDateText));

		shared_ptr<Person> director = findOrCreateDirector(directorName);
		shared_ptr<Movie> theMovie(new Movie(movieTitle, releaseDate, movieGross));

		theMovie->setDirector(director);
		director->addMovieDirected(theMovie);

		this->movies.push_back(theMovie);
		this->moviesByTitleAndYear[movieTitle + "|" + movieYearText] = theMovie;

		if (movieGross < this->minBoxOffice) {
			this->minBoxOffice = movieGross;
		}

		if (movieGross > this->maxBoxOffice) {
			this->maxBoxOffice = movieGross;
		}
	}

}

void FileDataProvider::readActors(std::string file) {
	std::ifstream inFile;
	inFile.open(&file[0],std::ios::in);
	if(inFile.fail()) {
		std::cerr << "Crap, we failed to read the actors file." << std::endl;
	}

	string line;
	int lineNum = 0;

	while (getline(inFile, line)) {
		lineNum++;

		std::istringstream linestream(line);

		string movieTitle;
		string movieYearText;
		string actorName;

		getline(linestream, movieTitle, '|');
		getline(linestream, movieYearText, '|');
		getline(linestream, actorName, '|');

		optional<shared_ptr<Movie>> maybeTheMovie = findMovie(movieTitle + "|" + movieYearText);

		if (!maybeTheMovie) continue;

		shared_ptr<Person> actor = findOrCreateActor(actorName);

		shared_ptr<Movie> theMovie = *maybeTheMovie;

		theMovie->addCastMember(actor);
		actor->addMovieStarredIn(theMovie);
	}

}

void FileDataProvider::eliminateNoNameActors() {
	list<shared_ptr<Person>>::iterator actorIter;
	list<shared_ptr<Person>>::iterator actorToDeleteIter;

	for (actorIter = this->actors.begin(); actorIter != this->actors.end();) {
		shared_ptr<Person> actor = *actorIter;

		if (actor->getMoviesStarredIn().size() <= 2) {
			actorToDeleteIter = actorIter++;
			this->actors.erase(actorToDeleteIter);
		}
		else {
			actorIter++;
		}
	}
}

shared_ptr<Person> FileDataProvider::findOrCreateDirector(string directorName) {
	hash_map<string, shared_ptr<Person>>::iterator directorIter = this->directorsByName.find(directorName);

	if (directorIter == this->directorsByName.end()) {
		shared_ptr<Person> director(new Person(directorName));
		this->directorsByName[directorName] = director;
		this->directors.push_back(director);
		return director;
	}
	else {
		return directorIter->second;
	}
}

shared_ptr<Person> FileDataProvider::findOrCreateActor(string actorName) {
	hash_map<string, shared_ptr<Person>>::iterator actorIter = this->actorsByName.find(actorName);

	if (actorIter == this->actorsByName.end()) {
		shared_ptr<Person> actor(new Person(actorName));
		this->actorsByName[actorName] = actor;
		this->actors.push_back(actor);
		return actor;
	}
	else {
		return actorIter->second;
	}
}

optional<shared_ptr<Movie>> FileDataProvider::findMovie(string movieTitleAndYear) {
	hash_map<string, shared_ptr<Movie>>::iterator movieIter = this->moviesByTitleAndYear.find(movieTitleAndYear);

	if (movieIter == this->moviesByTitleAndYear.end()) {
		return optional<shared_ptr<Movie>>();
	}
	else {
		return optional<shared_ptr<Movie>>(movieIter->second);
	}
}
