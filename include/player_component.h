#pragma once
#include "position_component.h"
#include "rotation_component.h"
#include "level_component.h"
#include <conio.h>

#define PLAYER_ENTITY_ID 1
#define PLAYER_STATE_IDLE 0
#define PLAYER_STATE_TURN 1
#define PLAYER_STATE_MOVE 2

class PlayerComponent : public Component {

private:
	PositionComponent* position_component;
	RotationComponent* rotation_component;
	LevelComponent* level_component;

	vec2 target_position;
	float target_angle;
	float fov;
	int state;

public:
	PlayerComponent(
		LevelComponent* level_component_,
		const float fov_
	) :
		position_component(nullptr),
		rotation_component(nullptr),
		level_component(level_component_),
		target_position(vec2()),
		target_angle(0.0f),
		fov(fov_),
		state(0) 
	{}

	std::string get_type_name() const override { return "Player"; }

	void ready(const Scene&) override {
		position_component = get_entity()->get_component_of_type<PositionComponent*>();
		rotation_component = get_entity()->get_component_of_type<RotationComponent*>();
	}

	bool process(const Scene& scene) override {
		if (state == PLAYER_STATE_IDLE) {
			char input = getch();

			switch (input) {
				case 'a': 
					target_angle = rotation_component->get_angle() - PI * .5f;
					state = PLAYER_STATE_TURN; 
					break;

				case 'd': 
					target_angle = rotation_component->get_angle() + PI * .5f;
					state = PLAYER_STATE_TURN;
					break;

				case 'w': 
					target_position = position_component->get_position() + rotation_component->get_forward_vector();

					if(!level_component->tile_exists(target_position))
						state = PLAYER_STATE_MOVE;
					break;

				case 's':
					target_position = position_component->get_position() - rotation_component->get_forward_vector(); 

					if (!level_component->tile_exists(target_position))
						state = PLAYER_STATE_MOVE;
					break;

				default: break;
			}
		}
		else if (state == PLAYER_STATE_TURN) {
			if (rotation_component->move_toward(target_angle, 0.3f))
				state = PLAYER_STATE_IDLE;
		}
		else if (state == PLAYER_STATE_MOVE) {
			if (position_component->move_toward(target_position, 0.3f))
				state = PLAYER_STATE_IDLE;
		}

		scene.get_render_info()->set_camera(
			position_component->get_position() - rotation_component->get_forward_vector() * 0.4f,
			rotation_component->get_angle(),
			fov
		);

		return state == PLAYER_STATE_IDLE;
	}
};

#undef PLAYER_STATE_IDLE
#undef PLAYER_STATE_TURN
#undef PLAYER_STATE_MOVE