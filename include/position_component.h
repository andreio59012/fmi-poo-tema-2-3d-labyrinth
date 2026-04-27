#pragma once
#include "component.h"
#include "extra_math.h"

class PositionComponent : public Component
{
private:
	vec2 position;

public:
	PositionComponent(const vec2& position_ = vec2()) : position(position_) {}

	std::string get_type_name() const override { return "Position"; }

	float get_x() const {
		return position.x;
	}

	void set_x(const float x) {
		position.x = x;
	}

	float get_y() const {
		return position.y;
	}

	void set_y(const float y) {
		position.y = y;
	}

	vec2 get_position() const {
		return position;
	}

	vec2i get_position_tile() const {
		return vec2i((int)position.x, (int)position.y);
	}

	void set_position(const vec2 position_)
	{
		position = position_;
	}

	void set_position_tile(const vec2i position_)
	{
		position = vec2(position_.x, position_.y);
	}

	bool move_toward(const vec2& target, const float& step) {
		float vx = target.x - position.x;
		float vy = target.y - position.y;
		float dist = sqrtf(vx * vx + vy * vy);

		if (dist <= step) { 
			position = target; 
			return true;
		}

		float inv = step / dist;
		position.x += vx * inv;
		position.y += vy * inv;

		return false;
	}
};