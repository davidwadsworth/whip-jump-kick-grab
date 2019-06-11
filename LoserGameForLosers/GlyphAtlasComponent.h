
#pragma once
#include <utility>
#include "ECS.h"
#include "BitmapFont.h"

struct Label
{
	Label(SDL_Rect* pos, std::string text)
	: position(pos), text(std::move(text))
	{}

	SDL_Rect* position;
	std::string text;
};

class GlyphAtlasComponent : public Component
{
	BitmapFont * bitmap_font_;
	std::vector<Label*> labels_;
public:

	GlyphAtlasComponent(): bitmap_font_(nullptr)
	{}

	~GlyphAtlasComponent()
	{}


	void create_labels()
	{
		labels_.clear();
		if (entity->has_component<OptionsComponent>())
		{
			for (auto x_link : entity->get_component<OptionsComponent>().current_links)
			{
				for (auto y_link : x_link)
				{
					labels_.push_back(new Label(y_link->get_link_dimensions(), y_link->get_text()));
				}
			}
		}
	}

	void init() override
	{
		bitmap_font_ = entity->state->palette->get_bitmap_font();
		create_labels();
	}


	void draw() override
	{
		for(auto& label : labels_)
		{
			bitmap_font_->render_text(label->position->x, label->position->y, label->text);
		}	
	}
};
