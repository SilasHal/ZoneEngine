#pragma once
#include "CommonHeaders.h"
#include "Window.h"

namespace zone::platform {

	struct WindowInitInfo;

	Window createWindow(const WindowInitInfo* const init_info = nullptr);
	void removeWindow(window_id id);
}