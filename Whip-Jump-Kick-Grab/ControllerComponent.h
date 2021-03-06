#pragma once

#include "ECS.h"
#include <SDL.h>
#include "Controller.h"
#include "KeyboardHandler.h"
#include "LoadControllerData.h"

/**
 * @author David Wadsworth
 * 
 * in charge of linking the keyboard to its loaded in command
 */

class ControllerComponent : public Component
{
	KeyboardHandler * keyboard_ = nullptr;

	std::vector<std::vector<SDL_Scancode>> player_keys_ =
	{
		{SDL_SCANCODE_W, SDL_SCANCODE_S, SDL_SCANCODE_A, SDL_SCANCODE_D, SDL_SCANCODE_Z, SDL_SCANCODE_X, SDL_SCANCODE_C, SDL_SCANCODE_ESCAPE, SDL_SCANCODE_1, SDL_SCANCODE_2},
		{SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT, SDL_SCANCODE_COMMA, SDL_SCANCODE_PERIOD, SDL_SCANCODE_SLASH, SDL_SCANCODE_KP_9, SDL_SCANCODE_KP_7, SDL_SCANCODE_KP_8},
		{SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT, SDL_SCANCODE_Z, SDL_SCANCODE_X, SDL_SCANCODE_C, SDL_SCANCODE_ESCAPE, SDL_SCANCODE_1, SDL_SCANCODE_2}
	};
	std::vector<SDL_Scancode> keys_;

	int controller_id_;
	int keys_owner_id_;

	ControllerData * data_;
	Controller controller_;
	bool keys_are_active_;
public:

	ControllerComponent(const int controller_id, const int keys_owner_id)
		: controller_id_(controller_id), keys_owner_id_(keys_owner_id), data_(nullptr), keys_are_active_(true)
	{}

	~ControllerComponent()
	= default;

	void change_controller(const int new_controller)
	{
		data_ = &GameState::get_controller_data()->data[new_controller];
		controller_ = Controller(data_);
	}

	void init() override
	{
		data_ = &GameState::get_controller_data()->data[controller_id_];
		keys_ = player_keys_[keys_owner_id_];

		keyboard_ = new KeyboardHandler;
		controller_ = Controller(data_);
	}

	void deactivate()
	{
		keys_are_active_ = false;
	}

	void activate()
	{
		keys_are_active_ = true;
	}

	bool keyboard_check(const int key)
	{
		auto is_pressed = true;

		const auto first_key = key % 10;

		if (first_key < 0)
			is_pressed &= !keyboard_->check_key_state(keys_[std::abs(first_key)]);		
		else
			is_pressed &= keyboard_->check_key_state(keys_[first_key]);
		if (std::abs(key) > 9)
			is_pressed &= keyboard_check(key / 10);

		return is_pressed;
	}

	void update() override
	{
		if (keys_are_active_)
		{
			for (auto input : controller_.inputs)
			{
				if (keyboard_check(input.keys))
					input.i_command->execute(entity);
				else
					input.i_command->idle(entity);
			}
		}
	}

};

