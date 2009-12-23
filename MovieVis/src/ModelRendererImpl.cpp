/**
 * \file ModelRendererImpl.cpp
 * \author Adam Shepard and Douglas W. Paul
 *
 * Defines the behavior of the ModelRendererImpl class
 */

#include "ModelRendererImpl.hpp"
#include <EasyText.hpp>
#include <GlWrappers.hpp>
#include "DefaultLayoutProvider.hpp"
#include "ForceDirectedLayoutProvider.hpp"
#include "EasyBMP.h"
#include "ActorCurve.hpp"
#include <iomanip>
#include <boost/format.hpp>
#include <Numerics.hpp>

using std::sort;
using boost::date_time::neg_infin;
using boost::date_time::pos_infin;
using peek::Color;
using boost::format;
using peek::Point3d;
using peek::Vector3d;
using peek::sign;

const double ModelRendererImpl::DIRECTOR_LINE_LAYER = 0.0;
const double ModelRendererImpl::ACTOR_LINE_LAYER = 0.1;
const double ModelRendererImpl::SELECTED_ACTOR_LINE_LAYER = 0.2;
const double ModelRendererImpl::MOVIE_DOT_LAYER = 0.3;
const double ModelRendererImpl::SELECTED_MOVIE_DOT_LAYER = 0.4;
const double ModelRendererImpl::DIRECTOR_LABEL_LAYER = 0.5;

ModelRendererImpl::ModelRendererImpl(const shared_ptr<Model> &model) {
	this->actorColorProvider = shared_ptr<ActorColorProvider>(new ActorColorProvider());
	this->movieDotSizeProvider = shared_ptr<MovieDotSizeProvider>(new MovieDotSizeProvider(model->getMinBoxOffice(), model->getMaxBoxOffice()));
	setModel(model);
	this->layoutProvider = auto_ptr<LayoutProvider>(new ForceDirectedLayoutProvider(model));
	initTextures();
}

void ModelRendererImpl::setModel(const shared_ptr<Model> &model) {
	this->model = model;
}

void ModelRendererImpl::draw() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	this->model->getCamera()->setProjectionMatrix();
	this->model->getCameraRigging()->initModelviewMatrix();

	ezt::setPadding(2, 2);
	ezt::setScreenHeightUnits(this->model->getCameraRigging()->getOrthoCamera()->getHeight());

	drawDirectors();
	drawActors();
	drawMovies();


	glClear(GL_DEPTH_BUFFER_BIT);
	drawHud();

	glFlush();
}

void ModelRendererImpl::pick(int x, int y) {
	this->model->getCamera()->setPickingMatrix(x, y);
	this->model->getCameraRigging()->initModelviewMatrix();

	glSelectBuffer(SELECT_BUFF_SIZE, this->selectBuff);
	glRenderMode(GL_SELECT);

	glInitNames();

	glPushName(0);
	pickMovies();
	glPopName();
	glPushName(1);
	pickActors();
	glPopName();

	int numHits = glRenderMode(GL_RENDER);
	optional<shared_ptr<Person>> selectedActor;
	optional<shared_ptr<Movie>> selectedMovie;

	if(numHits > 0) {
		GLuint *curHit = this->selectBuff;
		GLuint nearestHitType;
		GLuint nearestHitName;
		GLuint nearestHitDepth = 0xffffffff;

		for(int i = 0; i < numHits; i++) {
			GLuint numNames = curHit[0];
			GLuint hitMinDepth = curHit[1];
			GLuint hitMaxDepth = curHit[2];

			if(numNames > 0) {
				GLuint *names = &curHit[3];

				if(hitMinDepth < nearestHitDepth) {
					nearestHitType = names[0];
					nearestHitName = names[1];
					nearestHitDepth = hitMinDepth;
				}
			}

			curHit += (numNames + 3);
		}
		
		switch (nearestHitType) {
		case 0:
			selectedMovie = this->model->getMovies()[nearestHitName];
			break;
		case 1:
			if (this->model->getBrushedActor()) {
				selectedActor = this->model->getBrushedActor();
			}
			else if (this->model->getBrushedMovie()) {
				selectedActor = shared_ptr<Person>((*this->model->getBrushedMovie())->getCastMembers()[nearestHitName]);
			}
			else {
				selectedActor = this->model->getActors()[nearestHitName];
			}
			break;
		default:
			break;
		}
	}

	this->model->setSelectedActor(selectedActor);
	this->model->setSelectedMovie(selectedMovie);
}

void ModelRendererImpl::brush(int x, int y) {
	this->model->getCamera()->setPickingMatrix(x, y);
	this->model->getCameraRigging()->initModelviewMatrix();

	glSelectBuffer(SELECT_BUFF_SIZE, this->selectBuff);
	glRenderMode(GL_SELECT);

	glInitNames();

	glPushName(0);
	pickMovies();
	glPopName();
	glPushName(1);
	pickActors();
	glPopName();

	int numHits = glRenderMode(GL_RENDER);
	optional<shared_ptr<Person>> brushedActor;
	optional<shared_ptr<Movie>> brushedMovie;

	if(numHits > 0) {
		GLuint *curHit = this->selectBuff;
		GLuint nearestHitType;
		GLuint nearestHitName;
		GLuint nearestHitDepth = 0xffffffff;

		for(int i = 0; i < numHits; i++) {
			GLuint numNames = curHit[0];
			GLuint hitMinDepth = curHit[1];
			GLuint hitMaxDepth = curHit[2];

			if(numNames > 0) {
				GLuint *names = &curHit[3];

				if(hitMinDepth < nearestHitDepth) {
					nearestHitType = names[0];
					nearestHitName = names[1];
					nearestHitDepth = hitMinDepth;
				}
			}

			curHit += (numNames + 3);
		}
		
		switch (nearestHitType) {
		case 0:
			brushedMovie = this->model->getMovies()[nearestHitName];
			break;
		case 1:
			if (this->model->getBrushedActor()) {
				brushedActor = this->model->getBrushedActor();
			}
			else if (this->model->getBrushedMovie()) {
				brushedActor = shared_ptr<Person>((*this->model->getBrushedMovie())->getCastMembers()[nearestHitName]);
			}
			else {
				brushedActor = this->model->getActors()[nearestHitName];
			}
			break;
		default:
			break;
		}
	}

	this->model->setBrushedActor(brushedActor);
	this->model->setBrushedMovie(brushedMovie);
}

void ModelRendererImpl::drawDirectors() const {
	vector<shared_ptr<Movie>> movies = this->model->getMovies();
	vector<shared_ptr<Person>> directors = this->model->getDirectors();

	// Just return if there's nothing to draw.
	if (movies.size() == 0 || directors.size() == 0) {
		return;
	}

	date earliestMovieDate(pos_infin);
	date latestMovieDate(neg_infin);
	vector<shared_ptr<Movie>>::const_iterator moviePtrIter;
	for (moviePtrIter = movies.begin(); moviePtrIter != movies.end(); ++moviePtrIter) {
		shared_ptr<Movie> moviePtr = *moviePtrIter;
		date releaseDate = moviePtr->getReleaseDate();
		if (releaseDate < earliestMovieDate) {
			earliestMovieDate = releaseDate;
		}
		if (releaseDate > latestMovieDate) {
			latestMovieDate = releaseDate;
		}
	}

	double startY = this->layoutProvider->getDateYPosition(earliestMovieDate) - 0.5;
	double endY = this->layoutProvider->getDateYPosition(latestMovieDate) + 0.5;

	ezt::setCurrentFont(FONT_CALIBRI_14);
	ezt::setTextAnchor(EZT_ANCHOR_TOP_CENTER);
	glDisable(GL_LIGHTING);
	glDisable(GL_LINE_SMOOTH);
	glLineWidth(1.0);
	vector<shared_ptr<Person>>::const_iterator directorPtrIter;

	pkGlColor(Color::white);
	glPushMatrix();
	glTranslated(0.0, 0.0, DIRECTOR_LINE_LAYER);
	glBegin(GL_LINES);
	for (directorPtrIter = directors.begin(); directorPtrIter != directors.end(); ++directorPtrIter) {
		shared_ptr<Person> directorPtr = *directorPtrIter;
		double directorXPosition = this->layoutProvider->getDirectorXPosition(directorPtr);

		glVertex3d(directorXPosition, startY, 0.0);
		glVertex3d(directorXPosition, endY, 0.0);
	}
	glEnd();
	glPopMatrix();

	pkGlColor(Color::white);
	glPushMatrix();
	glTranslated(0.0, 0.0, DIRECTOR_LABEL_LAYER);
	for (directorPtrIter = directors.begin(); directorPtrIter != directors.end(); ++directorPtrIter) {
		shared_ptr<Person> directorPtr = *directorPtrIter;
		double directorXPosition = this->layoutProvider->getDirectorXPosition(directorPtr);

		ezt::renderTextWithScaling(directorPtr->getName(), directorXPosition, startY, 0.0);
	}
	glPopMatrix();
}

void ModelRendererImpl::drawActors() const {
	vector<shared_ptr<Person>> actors = this->model->getActors();

	// Just return if there's nothing to draw.
	if (actors.size() == 0) {
		return;
	}

	glDisable(GL_LIGHTING);
	glEnable(GL_LINE_SMOOTH);

	glLineWidth(2.0);

	glPushMatrix();
	glTranslated(0.0, 0.0, ACTOR_LINE_LAYER);

	if (this->model->getBrushedActor()) {
		drawActor(*(this->model->getBrushedActor()));
	}
	else if (this->model->getBrushedMovie()) {
		vector<weak_ptr<Person>> actors = (*this->model->getBrushedMovie())->getCastMembers();
		vector<weak_ptr<Person>>::const_iterator actorPtrIter;
		for (actorPtrIter = actors.begin(); actorPtrIter != actors.end(); ++actorPtrIter) {
			shared_ptr<Person> actorPtr = actorPtrIter->lock();

			if (this->model->getSelectedActor() && actorPtr == *(this->model->getSelectedActor())) {
				continue;
			}

			pkGlColor(this->actorColorProvider->getColor(actorPtr));
			drawActor(actorPtr);
		}
	}
	else {
		vector<shared_ptr<Person>>::const_iterator actorPtrIter;
		for (actorPtrIter = actors.begin(); actorPtrIter != actors.end(); ++actorPtrIter) {
			shared_ptr<Person> actorPtr = *actorPtrIter;

			if (this->model->getSelectedActor() && actorPtr == *(this->model->getSelectedActor())) {
				continue;
			}

			pkGlColor(this->actorColorProvider->getColor(actorPtr));
			drawActor(actorPtr);
		}
	}
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, 0.0, SELECTED_ACTOR_LINE_LAYER);
	if (this->model->getSelectedActor()) {
		shared_ptr<Person> actorPtr = *(this->model->getSelectedActor());
		glLineWidth(3.0);
		pkGlColor(Color::yellow);
		drawActor(actorPtr);
	}
	glPopMatrix();
}

void ModelRendererImpl::drawActor(const shared_ptr<Person> &actorPtr) const {
	actorCurveVectorPtr_t actorCurvesVectorPtr = this->layoutProvider->getActorCurves(actorPtr);
	actorCurveVector_t actorCurvesVector = *actorCurvesVectorPtr;

	actorCurveVector_t::iterator actorCurveIter;
	for (actorCurveIter = actorCurvesVector.begin(); actorCurveIter != actorCurvesVector.end(); ++actorCurveIter) {
		shared_ptr<ActorCurve> actorCurvePtr = *actorCurveIter;
		actorCurvePtr->draw();
	}
}

void ModelRendererImpl::drawMovies() const {
	vector<shared_ptr<Movie>> movies = this->model->getMovies();

	// Just return if there's nothing to draw.
	if (movies.size() == 0) {
		return;
	}

	glDisable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, this->dotTextureName);
	glEnable(GL_TEXTURE_2D);
	pkGlColor(Color::white);

	vector<shared_ptr<Movie>>::const_iterator moviePtrIter;

	glPushMatrix();
	glTranslated(0.0, 0.0, MOVIE_DOT_LAYER);
	for (moviePtrIter = movies.begin(); moviePtrIter != movies.end(); ++moviePtrIter) {
		shared_ptr<Movie> moviePtr = *moviePtrIter;

		if (this->model->getSelectedMovie() && moviePtr == *(this->model->getSelectedMovie())) {
			continue;
		}

		drawMovie(moviePtr);
	}
	glPopMatrix();

	if (this->model->getSelectedMovie()) {
		shared_ptr<Movie> moviePtr = *(this->model->getSelectedMovie());
		glPushMatrix();
		glTranslated(0.0, 0.0, SELECTED_MOVIE_DOT_LAYER);
		pkGlColor(Color::yellow);
		drawMovie(moviePtr);
		glPopMatrix();
	}

	glDisable(GL_TEXTURE_2D);	
}

void ModelRendererImpl::drawMovie(const shared_ptr<Movie> &moviePtr) const {
	double y = this->layoutProvider->getDateYPosition(moviePtr->getReleaseDate());
	float pointSize = (float) this->movieDotSizeProvider->getDotSize(moviePtr);

	shared_ptr<Person> directorPtr = moviePtr->getDirector().lock();
	double x = this->layoutProvider->getDirectorXPosition(directorPtr);
	
	glBegin(GL_QUADS);
		glTexCoord2f(1.0, 0.0);
		glVertex3d(x+pointSize, y+pointSize, 0.0);
		glTexCoord2f(0.0, 0.0);
		glVertex3d(x-pointSize, y+pointSize, 0.0);
		glTexCoord2f(0.0, 1.0);
		glVertex3d(x-pointSize, y-pointSize, 0.0);
		glTexCoord2f(1.0, 1.0);
		glVertex3d(x+pointSize, y-pointSize, 0.0);
	glEnd();
}

void ModelRendererImpl::drawHud() const {
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, viewport[2], 0, viewport[3], 1.0, -1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	pkGlColor(Color::red);
	drawTooltips();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void ModelRendererImpl::drawTooltips() const {
	if (this->model->getSelectedActor() || this->model->getSelectedMovie()) {
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);

		unsigned int mouseX = this->model->getMouseX();
		unsigned int mouseY = viewport[3] - this->model->getMouseY();
		string text;

		if (this->model->getSelectedActor()) {
			shared_ptr<Person> actorPtr = *(this->model->getSelectedActor());
			text = actorPtr->getName();
		}

		if (this->model->getSelectedMovie()) {
			shared_ptr<Movie> moviePtr = *(this->model->getSelectedMovie());
			string boTotal = doubleToCurrency(moviePtr->getBoxOfficeReceipts());
			text = moviePtr->getTitle() + " (" + boTotal + ")";
		}

		pair<int, int> textSize;
		ezt::getTextSize(text, textSize);

		const int paddingX = 4;
		const int paddingY = 2;
		int boxX = mouseX;
		int boxY = mouseY - 20;
		int boxWidth = textSize.first + 2 * paddingX;
		int boxHeight = textSize.second + 2 * paddingY;

		pkGlColor(Color::white);
		glBegin(GL_QUADS);
		glVertex3d(boxX, boxY, 0.0);
		glVertex3d(boxX, boxY-boxHeight, 0.0);
		glVertex3d(boxX+boxWidth, boxY-boxHeight, 0.0);
		glVertex3d(boxX+boxWidth, boxY, 0.0);
		glEnd();

		pkGlColor(Color::black);
		ezt::setTextAnchor(EZT_ANCHOR_TOP_LEFT);
		ezt::setPadding((float) paddingX, (float) paddingY);
		ezt::renderText(text, boxX, boxY, 0.5);
	}
}

void ModelRendererImpl::pickActors() {
	vector<shared_ptr<Person>> actors = this->model->getActors();
	glLineWidth(2.0);
	glPushMatrix();
	glTranslated(0.0, 0.0, ACTOR_LINE_LAYER);

	if (this->model->getBrushedActor()) {
		glPushName(0);
		drawActor(*(this->model->getBrushedActor()));
		glPopName();
	}
	else if (this->model->getBrushedMovie()) {
		vector<weak_ptr<Person>> actors = (*this->model->getBrushedMovie())->getCastMembers();
		for (unsigned int i = 0; i < actors.size(); ++i) {
			glPushName(i);
			drawActor(actors[i].lock());
			glPopName();
		}
	}
	else {
		for (unsigned int i = 0; i < actors.size(); ++i) {
			glPushName(i);
			drawActor(actors[i]);
			glPopName();
		}
	}
	glPopMatrix();
}

void ModelRendererImpl::pickMovies() {
	vector<shared_ptr<Movie>> movies = this->model->getMovies();
	glPushMatrix();
	glTranslated(0.0, 0.0, MOVIE_DOT_LAYER);
	for (unsigned int i = 0; i < movies.size(); ++i) {
		glPushName(i);
		drawMovie(movies[i]);
		glPopName();
	}
	glPopMatrix();
}

void ModelRendererImpl::initTextures() {
	glGenTextures(1, &this->dotTextureName);
	glBindTexture(GL_TEXTURE_2D, this->dotTextureName);

	BMP test;

	test.ReadFromFile("..\\resources\\textures\\dot.bmp");
	GLsizei textureWidth = test.TellWidth();
	GLsizei textureHeight = test.TellHeight();
	const unsigned int numChannels = 1;
	unsigned char *textureData = new unsigned char[textureWidth * textureHeight * numChannels];

	for (int i = 0; i < textureWidth; i++) {
		for (int j = 0; j < textureHeight; j++) {
			RGBApixel *pixel = test(i, j);
			unsigned int pixelByteOffset = ((j * textureWidth) + i) * numChannels;
			textureData[pixelByteOffset + 0] = pixel->Red;
		}
	}

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_ALPHA8, textureWidth,textureHeight, GL_ALPHA, GL_UNSIGNED_BYTE, textureData);

	delete textureData;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

/**
 * Note: This function only works properly on positive numbers.
 */
string ModelRendererImpl::doubleToCurrency(double input) const {

	// Determine the appropriate abbreviation for the order of magnitude, and divide
	// by the appropriate power of 1000.
	string magnitudeAbbreviation;
	if (input / pow(1000.0, 4) > 1.0) {
		input /= pow(1000.0, 4);
		magnitudeAbbreviation = "T";
	}
	else if (input / pow(1000.0, 3) > 1.0) {
		input /= pow(1000.0, 3);
		magnitudeAbbreviation = "B";
	}
	else if (input / pow(1000.0, 2) > 1.0) {
		input /= pow(1000.0, 2);
		magnitudeAbbreviation = "M";
	}
	else if (input / pow(1000.0, 1) > 1.0) {
		input /= pow(1000.0, 1);
		magnitudeAbbreviation = "K";
	}
	else {
		magnitudeAbbreviation = "";
	}

	// We want to show only 3 significant digits, so choose the format string with the
	// appropriate precision.
	string formatString;
	if (input / 100.0 > 1.0) {
		formatString = "%.0f";
	}
	else if (input / 10.0 > 1.0) {
		formatString = "%.1f";
	}
	else {
		formatString = "%.2f";
	}

	return "$" + (format(formatString) % input).str() + magnitudeAbbreviation;
}
