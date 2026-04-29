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
    explicit RenderInfo(const vec2i& screen_size_);
    ~RenderInfo();

    RenderInfo(const RenderInfo&) = delete;
    RenderInfo& operator=(const RenderInfo&) = delete;

    void set_camera(const vec2 camera_position_, const float camera_angle_, const float camera_fov_) {
        camera_position = camera_position_;
        camera_angle = camera_angle_;
        camera_fov = camera_fov_;
    }

    void set_depth_pixel(const int x, const float depth) {
        if(x > 0 && x < screen_size.x)
            depth_buffer[x] = depth;
    }

    void set_ascii_pixel(const int x, const int y, const char c) {
        if (x >= 2 && y >= 2 && x < screen_size.x - 2 && y < screen_size.y - 2)
            ascii_buffer[y * screen_size.x + x] = c;
    }

    void set_label(const std::string& key, const std::string& value) {
        labels[key] = value;
    }

    void clear_console() {
#ifdef _WIN32
        std::system("CLS");
#else
        std::system("clear");
#endif
    }

    void output_labels_and_ascii() {
        clear_console();
        std::cout << ascii_buffer;

        for (std::pair<std::string, std::string> label : labels) {
            std::cout << '\n' << label.first << " : " << label.second;
        }
    }

    [[nodiscard]] vec2 get_camera_position() const { return camera_position; }
    [[nodiscard]] float get_camera_angle() const { return camera_angle; }
    [[nodiscard]] float get_camera_fov() const { return camera_fov; }
    [[nodiscard]] vec2i get_screen_size() const { return screen_size; }

    [[nodiscard]] float get_depth_pixel(const int x) { 
        if (x <= 0 || x >= screen_size.x)
            return 0.0f;
        return depth_buffer[x]; 
    }
};

class Scene {

private:
    std::vector<Entity*> entities;
    RenderInfo* render_info;

public:
    explicit Scene(RenderInfo* render_info_);
    ~Scene();

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    void ready();
    void process();
    void draw();

    [[nodiscard]] RenderInfo* get_render_info() const {
        return render_info;
    }

    void add_entity(Entity* entity) {
        entities.push_back(entity);
    }
};
