#pragma once
#include "component.h"
#include "extra_math.h"

class RotationComponent : public Component
{
private:
	float angle;

public:
	RotationComponent(const float angle_ = 0.0f) : angle(angle_) {}

	std::string get_type_name() const override { return "Rotation"; }

	float get_angle() const {
		return angle;
	}

	void set_angle(const float angle_) {
		angle = angle_;
	}

	vec2 get_forward_vector() const {
		return vec2(
			std::sinf(angle),
			std::cosf(angle)
		);
	}

	bool move_toward(const float& target, const float& step) {
		float dif = target - angle;

		if (dif < 0.0f)
			dif = -dif;

		if (dif <= step) {
			angle = target;
			return true;
		}

		if (angle < target)
			angle += step;
		else
			angle -= step;

		return false;
	}
};