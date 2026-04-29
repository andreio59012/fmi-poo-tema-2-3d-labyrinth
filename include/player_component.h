#pragma once
#include "position_component.h"
#include "rotation_component.h"
#include "level_component.h"
#include "input_utils.h"

enum class PlayerState : int { IDLE = 0, TURN, MOVE, START };

class PlayerComponent : public Component {

private:
	PositionComponent* position_component;
	RotationComponent* rotation_component;
	LevelComponent* level_component;

	vec2 target_position;
	float target_angle;
	float fov;
	PlayerState state;

public:
	PlayerComponent(
		LevelComponent* level_component_,
		const float fov_
	) :
		Component(),
		position_component(nullptr),
		rotation_component(nullptr),
		level_component(level_component_),
		target_position(vec2()),
		target_angle(0.0f),
		fov(fov_),
		state(PlayerState::START)
	{}

	[[nodiscard]] Component* clone() const override {
		return new PlayerComponent(level_component, fov);
	}

	[[nodiscard]] std::string get_type_name() const override { return "Player"; }

	[[nodiscard]] char get_map_symbol() const override { return '@'; }

	void ready(const Scene&) override {
		position_component = get_entity()->get_component_of_type<PositionComponent*>();
		rotation_component = get_entity()->get_component_of_type<RotationComponent*>();
	}

	bool process(Scene& scene) override {
		bool block_thread = false;

		if (state == PlayerState::IDLE) {
			const char input = getch();
			block_thread = true;

			switch (input) {
				case 'a': 
					target_angle = rotation_component->get_angle() - PI * .5f;
					state = PlayerState::TURN; 
					break;

				case 'd': 
					target_angle = rotation_component->get_angle() + PI * .5f;
					state = PlayerState::TURN;
					break;

				case 'w': 
					target_position = position_component->get_position() + rotation_component->get_forward_vector();

					if(!level_component->tile_exists(target_position))
						state = PlayerState::MOVE;
					break;

				case 's':
					target_position = position_component->get_position() - rotation_component->get_forward_vector(); 

					if (!level_component->tile_exists(target_position))
						state = PlayerState::MOVE;
					break;

				case 'x':
					scene.set_game_state(GameState::QUIT);
					return false;

				default: break;
			}
		}
		else if (state == PlayerState::TURN) {
			if (rotation_component->move_toward(target_angle, 0.3f))
				state = PlayerState::IDLE;
		}
		else if (state == PlayerState::MOVE) {
			if (position_component->move_toward(target_position, 0.3f))
				state = PlayerState::IDLE;
		}
		else if (state == PlayerState::START)
			state = PlayerState::IDLE;

		scene.get_render_info()->set_camera(
			position_component->get_position() - rotation_component->get_forward_vector() * 0.4f,
			rotation_component->get_angle(),
			fov
		);

		return block_thread;
	}

	void draw(RenderInfo& render_info) override {
		render_info.set_label("Move/Turn", "WASD");
		render_info.set_label("Exit Game", "Q");
	}
};