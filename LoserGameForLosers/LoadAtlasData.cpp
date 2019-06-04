#include "stdafx.h"
#include <rapidxml.hpp>
#include <fstream>
#include <vector>
#include "LoadAtlasData.h"

AtlasCollection LoadAtlasData::load() const
{
	std::ifstream character_path(path);
	rapidxml::xml_document<> data;

	std::vector<char> buffer((std::istreambuf_iterator<char>(character_path)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');

	data.parse<0>(&buffer[0]);

	auto texture_atlas_node = data.first_node("TextureAtlas");


	auto atlas_data = AtlasCollection();

	atlas_data.id = texture_atlas_node->first_attribute("id")->value();
	atlas_data.path = texture_atlas_node->first_attribute("imagePath")->value();
	atlas_data.image_width = atoi(texture_atlas_node->first_attribute("width")->value());
	atlas_data.image_height = atoi(texture_atlas_node->first_attribute("height")->value());
	atlas_data.texture_id = atoi(texture_atlas_node->first_attribute("textureID")->value());

	for (auto sprite_node = texture_atlas_node->first_node("sprite"); sprite_node; sprite_node = sprite_node->next_sibling())
	{
		auto sprite_data = AtlasData();

		sprite_data.texture_id = atlas_data.texture_id;
		sprite_data.n = sprite_node->first_attribute("n")->value();
		sprite_data.x = atoi(sprite_node->first_attribute("x")->value());
		sprite_data.y = atoi(sprite_node->first_attribute("y")->value());
		sprite_data.w = sprite_data.original_w = atoi(sprite_node->first_attribute("w")->value());
		sprite_data.h = sprite_data.original_h = atoi(sprite_node->first_attribute("h")->value());

		if (sprite_node->first_attribute("oX"))
		{
			sprite_data.offset_x = atoi(sprite_node->first_attribute("oX")->value());
			sprite_data.offset_y = atoi(sprite_node->first_attribute("oY")->value());
			sprite_data.original_w = atoi(sprite_node->first_attribute("oW")->value());
			sprite_data.original_h = atoi(sprite_node->first_attribute("oH")->value());
		}

		if (sprite_node->first_attribute("r"))
		{
			sprite_data.is_rotated = true;
		}

		atlas_data.data.push_back(sprite_data);
	}

	return atlas_data;
}

const char * LoadAtlasData::image_source() const
{
	std::ifstream character_path(path);
	rapidxml::xml_document<> data;

	std::vector<char> buffer((std::istreambuf_iterator<char>(character_path)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');

	data.parse<0>(&buffer[0]);

	return data.first_node("TextureAtlas")->first_attribute("imagePath")->value();
}
