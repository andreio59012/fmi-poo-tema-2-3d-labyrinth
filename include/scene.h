#pragma once
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "extra_math.h"

class Entity;

class RenderInfo {

private:
    vec2 camera_position;
    float camera_angle;
    float camera_fov;
    const vec2i screen_size;
    
    float* depth_buffer;
    char* ascii_buffer;

    std::map<std::string, std::string> labels;

public:
    RenderInfo(const vec2i& screen_size_);

    void set_camera(const vec2 camera_position_, const float camera_angle_, const float camera_fov_) {
        camera_position = camera_position_;
        camera_angle = camera_angle_;
        camera_fov = camera_fov_;
    }

    void set_depth_pixel(const int x, const float depth) {
        depth_buffer[x] = depth;
    }

    void set_ascii_pixel(const int x, const int y, const char c) {
        if (x >= 2 && y >= 2 && x < screen_size.x - 2 && y < screen_size.y - 2)
            ascii_buffer[y * screen_size.x + x] = c;
    }

    void set_label(const std::string key, const std::string value) {
        labels[key] = value;
    }

    void output_labels_and_ascii() {
#ifdef _WIN32
        std::system("CLS");
#else
        std::system("clear");
#endif

        std::cout << ascii_buffer;

        for (std::pair<std::string, std::string> label : labels) {
            std::cout << '\n' << label.first << " : " << label.second;
        }
    }

    vec2 get_camera_position() const { return camera_position; }
    float get_camera_angle() const { return camera_angle; }
    float get_camera_fov() const { return camera_fov; }
    vec2i get_screen_size() const { return screen_size; }
    float get_depth_pixel(const int x) { return depth_buffer[x]; }
};

class Scene {

private:
    std::vector<Entity*> entities;
    RenderInfo* render_info;

public:
    Scene(RenderInfo* render_info_);
    ~Scene();

    void ready();
    void process();
    void draw();

    RenderInfo* get_render_info() const {
        return render_info;
    }

    void add_entity(Entity* entity) {
        entities.push_back(entity);
    }
};
