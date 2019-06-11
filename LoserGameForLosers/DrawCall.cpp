#include "stdafx.h"
#include "DrawCall.h"


DrawCall::DrawCall(AtlasData * data, SDL_Texture * tex, SDL_Rect * dest, const SDL_RendererFlip flip, const int rotation)
	: data_(data), is_rotated_(data->is_rotated), original_w_(data->original_w), original_h_(data->original_h), data_w_(data->w), data_h_(data->h), data_offset_(data->offset_x, data->offset_y), other_offset_(0),
	  height(data->h), width(data->w), rotation_point(nullptr), rotation(rotation), flip(flip), tex(tex), dest(dest)
{
	init();
}

DrawCall::DrawCall(SDL_Texture * tex, const SDL_Rect src, SDL_Rect * dest, const SDL_RendererFlip flip, const int rotation, SDL_Point * point)
	: data_(nullptr), rotation_axis_(), is_rotated_(false), original_w_(0), original_h_(0), data_w_(0), data_h_(0), height(0), width(0),
	  rotation_point(point), rotation(rotation), flip(flip), tex(tex), src(src), dest(dest)
{}


void DrawCall::init()
{
	if (flip)
	{
		if (!is_rotated_)
			data_offset_.x += data_->original_w - data_->w;
		else
			rotation += 180;
	}
	else
	{
		if (is_rotated_)
		{
			data_offset_.x = 0;
			data_offset_.y = 0;
		}
	}

	src = SDL_Rect{ data_->x, data_->y,data_->w, data_->h };
	
	if (is_rotated_)
	{
		rotation -= 90;
		find_rotation_point();
	}
}

/**
 * Uses center of rotation theorem
 * 
 * 
 * Gets two perpendicular bisectors from the lines { C->H } and { D->F } then calculates where it intersects
 * If the 
 * Example Rotation { A,B,C,D }->{ E,F,G,H }
 * -----------------
 * A = (0,0)
 * E = (X_offset, y_offset)
 * 
 * A____B
 * |	|	E______________F
 * |	|	|	   		   |		
 * |	|	|______________|
 * |	|   G			   H
 * |____|
 * C	D
 */

void DrawCall::find_rotation_point()
{

	const float w = data_w_;
	const float h = data_h_;
	const float x_off = data_->offset_x;
	const float y_off = data_->offset_y;

	// find mid points
	const auto m1 = Vector2D{ (w + h + x_off) / 2,  (h + y_off) / 2 };
	const auto m2 = Vector2D{ (h + x_off) / 2, (y_off + w + h) / 2 };

	// find inverse negative slope 
	const auto s1 = (w - x_off - h) / (y_off - h);
	const auto s2 = (-x_off - h) / (y_off + w - h);

	// calculate perpendicular bisectors
	const auto b1 = m1.y - s1 * m1.x;
	const auto b2 = m2.y - s2 * m2.x;

	// find where bisectors intersect
	const auto rx = (b2 - b1) / (s1 - s2);
	const auto ry = (b2 * s1 - b1 * s2) / (s1 - s2);

	rotation_axis_ = SDL_Point{ static_cast<int>(round(rx)), static_cast<int>(round(ry)) };
	rotation_point = new SDL_Point{ rotation_axis_.x , rotation_axis_.y };
}

void DrawCall::update_rotation_point(const float scaling) const
{
	if (rotation_point)
	{
		rotation_point->x = round(rotation_axis_.x * scaling);
		rotation_point->y = round(rotation_axis_.y * scaling);
	}
}

void DrawCall::update_height(const int h)
{
	const auto height_difference = h - original_h_;
	if (is_rotated_)
	{
		const float total_difference = height_difference * data_w_ / original_h_;
		width += static_cast<int>(total_difference);
		other_offset_.x -= total_difference;
	}
	else
		height += height_difference * data_h_ / original_h_;
}

void DrawCall::update_width(const int w)
{
	if (is_rotated_)
		height += (w - original_w_) *  data_h_ / original_w_;
	else
		width += (w - original_w_) * data_w_ / original_w_;
}

Vector2D DrawCall::position_offset()
{
	return data_offset_ + other_offset_;
}
