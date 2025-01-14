// Copyright (c) CedricZ1, 2025
// Distributed under the MIT license. See the LICENSE file in the project root for more information.
#pragma once
#include "CommonHeaders.h"
#include "Window.h"

namespace zone::platform {

struct WindowInitInfo;

Window createWindow(const WindowInitInfo* const init_info = nullptr);
void removeWindow(window_id id);
}