// Copyright (c) CedricZ1, 2025
// Distributed under the MIT license. See the LICENSE file in the project root for more information.
#include "Platform.h"
#include "PlatformTypes.h"
namespace zone::platform
{
#ifdef _WIN64

namespace {

}// anonymous namespace

Window createWindow(const WindowInitInfo* const initInfo)
{
	window_proc callback{ initInfo ? initInfo->callback : nullptr };
	window_handle callback{ initInfo ? initInfo->parent : nullptr };
}

#elif
#error "Must implement at least one platform"
#endif  

}
