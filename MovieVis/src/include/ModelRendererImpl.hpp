/**
 * \file ModelRendererImpl.hpp
 * \author Douglas W. Paul and Adam Shepard
 *
 * Declares the ModelRendererImpl class
 */

#pragma once

#include "ModelRenderer.hpp"
#include <Color.hpp>
#include "Model.hpp"
#include "LayoutProvider.hpp"
#include "ActorColorProvider.hpp"
#include "MovieDotSizeProvider.hpp"
#include <Geometry.hpp>

using peek::Color;
using std::auto_ptr;
using peek::Point3d;

/**
 * A simple ModelRenderer implementation
 */
class ModelRendererImpl : public ModelRenderer {
public:

	/** Constructor */
	ModelRendererImpl(const shared_ptr<Model> &model);

	/** Sets the model to render */
	virtual void setModel(const shared_ptr<Model> &model);

	/** Draws the model */
	virtual void draw();

	/** Perform a pick at the given mouse location */
	virtual void pick(int x, int y);

	/** Perform a brush at the given mouse location */
	virtual void brush(int x, int y);

protected:

	/** The data model */
	shared_ptr<Model> model;

	/** The layout provider */
	auto_ptr<LayoutProvider> layoutProvider;

	/** Draws the directors */
	void drawDirectors() const;

	/** Draws the actors */
	void drawActors() const;

	/** Draws an actor */
	void drawActor(const shared_ptr<Person> &actorPtr) const;

	/** Draws the movies */
	void drawMovies() const;

	/** Draws a movie */
	void drawMovie(const shared_ptr<Movie> &moviePtr) const;

	/** Draws the heads-up display (HUD) */
	void drawHud() const;

	/** Draws the tooltips */
	void drawTooltips() const;

	/** Perform a pick across the actor geometry */
	void pickActors();

	/** Perform a pick across the actor geometry */
	void pickMovies();

	/** Initializes the textures*/
	void initTextures();

	/** convert a double to a currency **/
	std::string doubleToCurrency(double input) const;

	/** The actor color provider */
	shared_ptr<ActorColorProvider> actorColorProvider;

	/** The movie dot size provider */
	shared_ptr<MovieDotSizeProvider> movieDotSizeProvider;

	static const int SELECT_BUFF_SIZE = 512;
	GLuint selectBuff[SELECT_BUFF_SIZE];

	GLuint dotTextureName;

	static const double DIRECTOR_LINE_LAYER;
	static const double ACTOR_LINE_LAYER;
	static const double SELECTED_ACTOR_LINE_LAYER;
	static const double MOVIE_DOT_LAYER;
	static const double SELECTED_MOVIE_DOT_LAYER;
	static const double DIRECTOR_LABEL_LAYER;
};
