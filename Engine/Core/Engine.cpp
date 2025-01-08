// Copyright (c) CedricZ1, 2025
// Distributed under the MIT license. See the LICENSE file in the project root for more information.
#if !defined(SHIPPING)
#include "..\Content\ContentLoader.h"
#include "..\Components\Script.h"
#include <thread>

bool engine_initialize()
{
	bool result{ zone::content::load_game() };
	return true;
}

void engine_update()
{
	zone::script::update(10.f);
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void engine_shutdown()
{
	zone::content::unload_game();
}
#endif // !defined(SHIPPING)