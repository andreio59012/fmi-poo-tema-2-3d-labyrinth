#pragma once
#include <cstring>
#include <vector>
#include "exceptions.h"
#include "component.h"
#include "extra_math.h"

enum class TileDir : char { UP = 0, DOWN, LEFT, RIGHT, FRONT, BACK };

class LevelComponent : public Component 
{
private:
	vec2i tile_amount;
	std::vector<bool> tiles;
	std::vector<std::vector<TileDir>> dir;
	std::vector<std::vector<bool>> outline;

	LevelComponent(const vec2i& ta, const std::vector<bool>& t) :
		Component(), tile_amount(ta), tiles(t) {}

public:
	LevelComponent(const vec2i& tile_amount_, const bool* tiles_) :
		Component(),
		tile_amount(tile_amount_)
	{
		if (tiles_ == nullptr)
			throw InvalidLevelException("tile data pointer is null");
		if (tile_amount_.x <= 0 || tile_amount_.y <= 0)
			throw InvalidLevelException("tile dimensions must be positive");

		const std::size_t n = static_cast<size_t>(tile_amount_.x) * static_cast<std::size_t>(tile_amount_.y);
		tiles.assign(tiles_, tiles_ + n);
	}

	[[nodiscard]] Component* clone() const override {
		return new LevelComponent(tile_amount, tiles);
	}

	[[nodiscard]] char get_map_symbol() const override { return '#'; }
	[[nodiscard]] std::string get_type_name() const override { return "Level"; }

	[[nodiscard]] bool tile_exists(const vec2i position) const {
		if (position.x < 0 || position.y < 0 || position.x >= tile_amount.x || position.y >= tile_amount.y)
			return true;
		return tiles[position.y * tile_amount.x + position.x];
	}

	[[nodiscard]] bool tile_exists(const vec2 position) const {
		return tile_exists(vec2i(static_cast<int>(position.x), static_cast<int>(position.y)));
	}

	void ensure_buffers(const RenderInfo& render_info) {
		const int sx = render_info.get_screen_size().x;
		const int sy = render_info.get_screen_size().y;

		if (static_cast<int>(dir.size()) == sx && static_cast<int>(dir[0].size()) == sy && !dir.empty())
			return;

		dir.assign(static_cast<std::size_t>(sx), std::vector<TileDir>(static_cast<std::size_t>(sy), TileDir::UP));
		outline.assign(static_cast<std::size_t>(sx), std::vector<bool>(static_cast<std::size_t>(sy), false));
	}

	void draw(RenderInfo& render_info) override
	{
		ensure_buffers(render_info);

		const int screen_x = render_info.get_screen_size().x;
		const int screen_y = render_info.get_screen_size().y;

		for (int x = 1; x < screen_x; ++x)
		{
			// Raycast
			float ray_angle = (render_info.get_camera_angle() - render_info.get_camera_fov() * 0.5f) + ((float)x / screen_x) * render_info.get_camera_fov();
			float ray_dist = 0.0f; 
			TileDir tile_dir = TileDir::UP;

			const vec2 ray_dir = vec2{ std::sinf(ray_angle), std::cosf(ray_angle) };
			vec2i ray_position = vec2i(), last_ray_position = vec2i();

			do
			{
				if (ray_position.x != last_ray_position.x || ray_position.y != last_ray_position.y)
					last_ray_position = ray_position;

				ray_dist += 0.01f;
				ray_position = vec2i(render_info.get_camera_position() + ray_dir * ray_dist);

			} while (!tile_exists(ray_position));
			
			render_info.set_depth_pixel(x, ray_dist);

			if (ray_position.x > last_ray_position.x)
				tile_dir = TileDir::RIGHT;
			else if (ray_position.x < last_ray_position.x)
				tile_dir = TileDir::LEFT;
			else if (ray_position.y > last_ray_position.y)
				tile_dir = TileDir::FRONT;
			else
				tile_dir = TileDir::BACK;

			// Ceilings / Floors
			// the 0.75f multiplier adds a pseudo-fisheye lens effect which makes depth perception a bit easier
			int ceiling_column = int(screen_y * 0.5f - screen_y / ray_dist * 0.75f);
			int floor_column = screen_y - ceiling_column;

			for (int y = 1; y < screen_y; ++y)
			{
				dir[x][y] = (y < ceiling_column) ? TileDir::DOWN : (y > floor_column ? TileDir::UP : tile_dir);
				outline[x][y] = dir[x - 1][y] != dir[x][y] || dir[x][y - 1] != dir[x][y];
			}
		}

		for (int y = 2; y < render_info.get_screen_size().y - 2; ++y)
		{
			for (int x = 2; x < render_info.get_screen_size().x - 2; ++x)
			{
				if (!outline[x][y])
					render_info.set_ascii_pixel(x, y, ' ');

				else if (
					(outline[x][y - 1] && outline[x + 1][y]) ||
					(outline[x][y + 1] && outline[x - 1][y]) ||
					(outline[x - 1][y - 1] && outline[x + 1][y + 1])
					)
					render_info.set_ascii_pixel(x, y, '\\');

				else if (
					(outline[x][y - 1] && outline[x - 1][y]) ||
					(outline[x][y + 1] && outline[x + 1][y]) ||
					(outline[x + 1][y - 1] && outline[x - 1][y + 1])
					)
					render_info.set_ascii_pixel(x, y, '/');

				else if (outline[x - 1][y] || outline[x + 1][y])
					render_info.set_ascii_pixel(x, y, '-');

				else if (outline[x][y - 1] || outline[x][y + 1])
					render_info.set_ascii_pixel(x, y, '|');

				else
					render_info.set_ascii_pixel(x, y, '.');
			}
		}
	}
};