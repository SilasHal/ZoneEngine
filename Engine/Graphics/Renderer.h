#pragma once
#include "CommonHeaders.h"
#include "..\Platform\Window.h"

namespace zone::graphics
{
	class Surface
	{
	};

	struct RenderSurface
	{
		platform::Window window{};
		Surface surface{};
	};

}