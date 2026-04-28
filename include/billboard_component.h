#pragma once
#include "position_component.h"
#include "entity.h"

class BillboardComponent : public Component
{
private:
	PositionComponent* position_component;
	const vec2i sprite_size;
	char* sprite_ascii;
	const vec2 size_multiplier;

public:
	BillboardComponent(
		const vec2i sprite_size_,
		const char* sprite_ascii_,
		const vec2 size_multiplier_ = vec2(1.0f, 1.0f)
	) :
		Component(),
		position_component(nullptr),
		sprite_size(sprite_size_),
		sprite_ascii(nullptr),
		size_multiplier(size_multiplier_)
	{
		if (sprite_ascii_ == nullptr)
			throw InvalidSpriteException("sprite data pointer is null");
		if (sprite_size_.x <= 0 || sprite_size_.y <= 0)
			throw InvalidSpriteException("sprite dimensions must be positive");

		sprite_ascii = new char[(sprite_size.x * sprite_size.y)];
		memcpy(sprite_ascii, sprite_ascii_, (size_t)sprite_size.x * (size_t)sprite_size.y);
	};

	BillboardComponent(const BillboardComponent& other) :
		Component(),
		position_component(nullptr),
		sprite_size(other.sprite_size),
		sprite_ascii(nullptr),
		size_multiplier(other.size_multiplier)
	{
		sprite_ascii = new char[(sprite_size.x * sprite_size.y)];
		memcpy(sprite_ascii, other.sprite_ascii, (size_t)sprite_size.x * (size_t)sprite_size.y);
	}

	~BillboardComponent() override {
		delete[] sprite_ascii;
	}

	[[nodiscard]] Component* clone() const override {
		return new BillboardComponent(*this);
	}

	[[nodiscard]] std::string get_type_name() const override { return "Billboard"; }

	void ready(const Scene&) override {
		position_component = get_entity()->get_component_of_type<PositionComponent*>();
	}

	void draw(RenderInfo& render_info) override
	{
		const vec2 to = position_component->get_position() - render_info.get_camera_position();
		const float dist = to.get_length();

		if (dist <= EPSILON)
			return;

		// Angle between player's forward and entity
		float angle_to_entity = std::atan2(to.x, to.y);
		float delta = angle_to_entity - render_info.get_camera_angle();

		// Normalize delta to between -pi and pi
		while (delta < -PI) delta += 2.0f * PI;
		while (delta > PI) delta -= 2.0f * PI;

		// Skip if outside fov
		if (std::fabs(delta) > render_info.get_camera_fov() * 0.5f)
			return;

		// Get destination position and size
		const vec2i dst_centered_pos = vec2i(
			(int)(((delta + render_info.get_camera_fov() * 0.5f) / render_info.get_camera_fov()) * render_info.get_screen_size().x),
			render_info.get_screen_size().y / 2
		);

		// Skip if behind wall
		if (render_info.get_depth_pixel(dst_centered_pos.x) < dist)
			return;

		const vec2i dst_sprite_size = vec2i((vec2(sprite_size) * size_multiplier) * (3.0f / dist));

		if (dst_sprite_size.x <= 0 || dst_sprite_size.y <= 0)
			return;

		const vec2i dst_corner = dst_centered_pos - dst_sprite_size * 0.5f;

		// Draw
		for (int x = 0; x < dst_sprite_size.x; x++) 
		{
			// Get source positions
			int src_x = int((x / (float)dst_sprite_size.x) * sprite_size.x);
			src_x = clamp(src_x, 0, sprite_size.x - 1);

			for (int y = 0; y < dst_sprite_size.y; y++)
			{
				int src_y = int((y / (float)dst_sprite_size.y) * sprite_size.y);
				src_y = clamp(src_y, 0, sprite_size.y - 1);

				const char c = sprite_ascii[src_y * sprite_size.x + src_x];

				if (c != ' ') {
					render_info.set_ascii_pixel(dst_corner.x + x, dst_corner.y + y, c);
					render_info.set_depth_pixel(dst_corner.x + x, dist);
				}
			}
		}
	}
};