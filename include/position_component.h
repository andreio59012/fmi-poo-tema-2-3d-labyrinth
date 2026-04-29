#pragma once
#include "component.h"
#include "extra_math.h"

class PositionComponent : public Component
{
private:
	vec2 position;

public:
	explicit PositionComponent(const vec2& position_ = vec2()) : Component(), position(position_) {}

	[[nodiscard]] Component* clone() const override {
		return new PositionComponent(position);
	}

	[[nodiscard]] std::string get_type_name() const override { return "Position"; }
	[[nodiscard]] vec2 get_position() const { return position; }
	[[nodiscard]] vec2i get_position_tile() const {
		return vec2i(static_cast<int>(position.x), static_cast<int>(position.y));
	}

	void set_position(const vec2 position_) { position = position_; }

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