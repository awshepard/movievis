/**
 * \file ModelRenderer.hpp
 * \author Douglas W. Paul and Adam Shepard
 *
 * Declares the ModelRenderer interface
 */

#pragma once

#include "Model.hpp"
#include <Drawable.hpp>

using peek::Drawable;

/**
 * \interface ModelRenderer
 * \brief An interface for something that renders a Model
 */
class ModelRenderer : public Drawable {
public:

	/** Sets the model to render */
	virtual void setModel(const shared_ptr<Model> &model) = 0;

	/** Draws the model */
	virtual void draw() = 0;

	/** Perform a pick at the given mouse location */
	virtual void pick(int x, int y) = 0;

	/** Perform a brush at the given mouse location */
	virtual void brush(int x, int y) = 0;

};
