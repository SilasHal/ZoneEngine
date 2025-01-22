// Copyright (c) CedricZ1, 2024
// Distributed under the MIT license. See the LICENSE file in the project root for more information.

#pragma once
#include "Test.h"
#include "..\Engine\Components\Entity.h"
#include "..\Engine\Components\Transform.h"

#include <iostream>
#include <ctime>

using namespace zone;

class EngineTest : public Test
{
public:
	bool initialize() override
	{
		srand((uint32)time(nullptr));
		return true;
	}

	void run() override
	{
		do {
			for (uint32 i{ 0 }; i < 10000; ++i)
			{
				create_random();
				remove_random();
				_num_entities = (uint32)_entities.size();
			}
			print_results();
		} while (getchar() != 'q');
	}
	void shutdown() override
	{

	}

private:
	void create_random() {
		uint32 count = rand() % 20;
		if (_entities.empty()) 
		{
			count = 1000;
		}

		transform::init_info transform_info{};
		game_entity::entity_info entity_info
		{
			&transform_info,
		};

		while (count > 0) 
		{
			++_added;
			game_entity::entity entity{ game_entity::create(entity_info) };

			assert(entity.is_valid() && id::is_valid(entity.get_id()));
			_entities.push_back(entity);
			assert(game_entity::is_alive(entity.get_id()));

			--count;
		}
	}


	void remove_random()
	{
		uint32 count = rand() % 20;
		if (_entities.size() < 1000)
		{
			return;
		}
		while (count > 0)
		{
			const uint32 index{ (uint32)rand() % (uint32)_entities.size() };
			const game_entity::entity entity{ _entities[index] };
			assert(entity.is_valid() && id::is_valid(entity.get_id()));
			if (entity.is_valid())
			{
				game_entity::remove(entity.get_id());
				_entities.erase(_entities.begin() + index);
				assert(!game_entity::is_alive(entity.get_id()));
				++_removed;
			}
			--count;
		}
	}

	void print_results()
	{
		std::cout << "Entities created: " << _added << "\n";
		std::cout << "Entities deleted: " << _removed << "\n";
	}

	utl::vector<game_entity::entity> _entities;

	uint32 _added{ 0 };
	uint32 _removed{ 0 };
	uint32 _num_entities{ 0 };
};
