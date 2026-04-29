#pragma once
#include "component.h"

class ExitDoorComponent : public Component
{
private:
	const float minimum_distance;
	const PositionComponent* player_position_component;
	PositionComponent* my_position_component;
	CollectibleCounterComponent* counter_component;

public:
	ExitDoorComponent(
		const float minimum_distance_,
		const PositionComponent* player_position_component_,
		CollectibleCounterComponent* counter_component_
	) :
		Component(),
		minimum_distance(minimum_distance_),
		player_position_component(player_position_component_),
		my_position_component(nullptr),
		counter_component(counter_component_)
	{}

	[[nodiscard]] Component* clone() const override {
		return new ExitDoorComponent(minimum_distance, player_position_component, counter_component);
	}

	[[nodiscard]] char get_map_symbol() const override { return 'D'; }
	[[nodiscard]] std::string get_type_name() const override { return "ExitDoor"; }
	[[nodiscard]] bool get_finished() const {
		return my_position_component->get_position().get_dist_to(player_position_component->get_position()) <= minimum_distance && counter_component->get_finished();
	}

	void ready(const Scene&) override {
		my_position_component = get_entity()->get_component_of_type<PositionComponent*>();
	}
};