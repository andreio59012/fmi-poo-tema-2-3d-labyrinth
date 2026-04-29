#include <iostream>
#include "scene.h"
#include "entity.h"
#include "level_component.h"
#include "player_component.h"
#include "billboard_component.h"
#include "collectible_component.h"
#include "exit_door_component.h"

int main()
{
	const int collectible_amount = 10;

	std::cout << "You are trapped in a Labyrinth.\nThere is an exit door, but it is locked.\nYou must find and collect " << collectible_amount << " keys to unlock it.\n\nEnter (0) to start the game, or (1) to quit.\n";
	int x; std::cin >> x;

	if (x != 0)
		return 0;

	try {
		Scene* scene = new Scene(new RenderInfo(vec2i(200, 70)));

		// Level
		const bool level_tiles[] = {
			1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
			1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,
			1,0,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,
			1,0,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,
			1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,1,
			1,0,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,
			1,0,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,
			1,0,1,1,0,0,0,1,1,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,1,
			1,0,1,1,0,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,
			1,0,1,1,0,1,1,1,1,0,1,1,0,1,0,0,0,0,0,1,1,0,1,1,0,1,
			1,0,1,1,0,0,0,1,1,0,0,0,0,1,0,0,0,0,0,1,1,0,1,1,0,1,
			1,0,1,1,1,1,0,1,1,0,1,1,0,1,0,0,0,0,0,1,1,0,1,1,0,1,
			1,0,1,1,1,1,0,1,1,0,1,1,0,1,1,1,0,1,1,1,1,0,1,1,0,1,
			1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,1,
			1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,
			1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,
			1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,
			1,0,1,1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,
			1,0,1,1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,
			1,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,1,
			1,0,1,1,1,1,0,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,
			1,0,1,1,1,1,0,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,
			1,0,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
			1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		};

		Entity* level_entity = new Entity();
		level_entity->add_component(new LevelComponent(vec2i(26, 24), level_tiles));
		scene->add_entity(level_entity);

		// Player
		Entity* player_entity = new Entity();
		player_entity->add_component(new PositionComponent(vec2(1.5f, 1.5f)));
		player_entity->add_component(new RotationComponent());
		player_entity->add_component(new PlayerComponent(static_cast<LevelComponent*>(level_entity->get_component_of_type_name("Level")), 1.5f));
		scene->add_entity(player_entity);

		// Counter
		Entity* counter_entity = new Entity();
		counter_entity->add_component(new CollectibleCounterComponent(collectible_amount));
		scene->add_entity(counter_entity);

		// Collectibles
		const char collectible_sprite[] = {
			'/','-','\\',' ',' ','.',' ','.',
			'|',' ','|','-','-','-','-','-',
			'\\','-','/',' ',' ',' ',' ',' ',
		};

		const vec2 collectible_pos[collectible_amount] = {
			vec2(24.5f, 22.5f),
			vec2(12.5f, 22.5f),
			vec2(9.5f, 19.5f),
			vec2(6.5f, 22.5f),
			vec2(1.5f, 22.5f),
			vec2(12.5f, 10.5f),
			vec2(9.5f, 1.5f),
			vec2(12.5f, 7.5f),
			vec2(15.5f, 16.5f),
			vec2(21.5f, 13.5f)
		};

		for (int i = 0; i < collectible_amount; i++) {
			Entity* collectible_entity = new Entity();
			collectible_entity->add_component(new PositionComponent(collectible_pos[i]));
			collectible_entity->add_component(new BillboardComponent(vec2i(8, 3), collectible_sprite, vec2(2.0f, 1.0f)));
			collectible_entity->add_component(new CollectibleComponent(0.1f, player_entity->get_component_of_type<PositionComponent*>(), counter_entity->get_component_of_type<CollectibleCounterComponent*>()));
			scene->add_entity(collectible_entity);
		}

		// Door
		const char door_sprite[] = {
			'|','-','-','-','-','-','-','|',
			'|',' ','E','X','I','T',' ','|',
			'|',' ',' ',' ',' ',' ',' ','|',
			'|',' ',' ',' ',' ',' ','-','|',
			'|',' ',' ',' ',' ',' ',' ','|',
			'|','-','-','-','-','-','-','|',
		};

		Entity* door_entity = new Entity();
		ExitDoorComponent* door_component = new ExitDoorComponent(0.1f, player_entity->get_component_of_type<PositionComponent*>(), counter_entity->get_component_of_type<CollectibleCounterComponent*>());
		door_entity->add_component(new PositionComponent(vec2(16.5f, 10.5f)));
		door_entity->add_component(new BillboardComponent(vec2i(8, 6), door_sprite, vec2(4.0f, 4.5f)));
		door_entity->add_component(door_component);
		scene->add_entity(door_entity);
		
		// Game Loop
		scene->ready();
		scene->draw();

		while (scene->get_game_state() == GameState::RUNNING)
		{
			scene->process();
			scene->draw();
		}

		scene->get_render_info()->clear_console();

		if (scene->get_game_state() == GameState::WON) 
		{
			std::cout << "Congratulations! You escaped the Labyrinth!\n\nPress the [Enter] key to exit the game.\n";
			std::cin.get();
		}

		delete scene;
	}
	catch (const LabyrinthException& e) {
		std::cout << "[Labyrinth error] " << e.what() << '\n';
		std::cout << "Extra info: Component count is " << Component::get_instance_count() << '\n';
		return 1;
	}
	catch (const std::exception& e) {
		std::cout << "[Error] " << e.what() << '\n';
		return 1;
	}

	return 0;
}