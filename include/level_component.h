#pragma once
#include <cstring>
#include <vector>
#include "component.h"
#include "extra_math.h"

#define DIR_UP (char)0
#define DIR_DOWN (char)1
#define DIR_LEFT (char)2
#define DIR_RIGHT (char)3
#define DIR_FRONT (char)4
#define DIR_BACK (char)5

class LevelComponent : public Component 
{
private:
	vec2i tile_amount;
	std::vector<bool> tiles;
	std::vector<std::vector<char>> dir;
	std::vector<std::vector<bool>> outline;

public:
	LevelComponent(const vec2i& tile_amount_, const bool* tiles_) :
		tile_amount(tile_amount_)
	{
		size_t n = (size_t)tile_amount.x * (size_t)tile_amount.y;
		tiles.assign(tiles_, tiles_ + n);
	}

	std::string get_type_name() const override { return "Level"; }

	bool tile_exists(const vec2i position) const {
		if (position.x < 0 || position.y < 0 || position.x >= tile_amount.x || position.y >= tile_amount.y)
			return true;
		return tiles[position.y * tile_amount.x + position.x];
	}
	bool tile_exists(const vec2 position) const {
		return tile_exists(vec2i((int)position.x, (int)position.y));
	}

	void ensure_buffers(const RenderInfo& render_info) {
		int sx = render_info.get_screen_size().x, sy = render_info.get_screen_size().y;

		if ((int)dir.size() == sx && (int)dir[0].size() == sy)
			return;

		dir.assign(sx, std::vector<char>(sy, 0));
		outline.assign(sx, std::vector<bool>(sy, false));
	}

	void draw(RenderInfo& render_info) override
	{
		ensure_buffers(render_info);

		for (int x = 1; x < render_info.get_screen_size().x; ++x)
		{
			// Raycast
			float ray_angle = (render_info.get_camera_angle() - render_info.get_camera_fov() * 0.5f) + ((float)x / render_info.get_screen_size().x) * render_info.get_camera_fov();
			float ray_dist = 0.0f; 
			char tile_dir = 0;

			vec2 ray_dir = vec2{ std::sinf(ray_angle), std::cosf(ray_angle) };
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
				tile_dir = DIR_RIGHT;
			else if (ray_position.x < last_ray_position.x)
				tile_dir = DIR_LEFT;
			else if (ray_position.y > last_ray_position.y)
				tile_dir = DIR_FRONT;
			else
				tile_dir = DIR_BACK;

			// Ceilings / Floors
			// the 0.75f multiplier adds a pseudo-fisheye lens effect which makes depth perception a bit easier
			int ceiling_column = int(render_info.get_screen_size().y * 0.5f - render_info.get_screen_size().y / ray_dist * 0.75f);
			int floor_column = render_info.get_screen_size().y - ceiling_column;

			for (int y = 1; y < render_info.get_screen_size().y; ++y)
			{
				dir[x][y] = (y < ceiling_column) ? DIR_DOWN : (y > floor_column ? DIR_UP : tile_dir);
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

#undef DIR_UP
#undef DIR_DOWN
#undef DIR_LEFT
#undef DIR_RIGHT
#undef DIR_FRONT
#undef DIR_BACK