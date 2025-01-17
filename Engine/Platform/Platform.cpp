// Copyright (c) CedricZ1, 2025
// Distributed under the MIT license. See the LICENSE file in the project root for more information.
#include "Platform.h"
#include "PlatformTypes.h"
namespace zone::platform
{
#ifdef _WIN64

namespace {

struct WindowInfo 
{
	HWND	hwnd{ nullptr };
	RECT	clientArea{ 0, 0, 1920, 1080 };
	RECT	fullScreenArea{};
	POINT	topLeft{ 0, 0 };
	DWORD	style{ WS_VISIBLE };
	bool	isFullScreen{ false };
	bool	isClosed{ false };
};

utl::vector<WindowInfo> windows;
//////////////////////////////////////////////////////////////
// TODO: this part will be handled by a free-list container later
utl::vector<uint32> avaliableSlots;

uint32 addToWindows(WindowInfo info) 
{
	uint32 id{ uint32_invalid_id };
	if (avaliableSlots.empty())
	{
		id = (uint32)windows.size();
		windows.emplace_back(info);
	}
	else 
	{
		id = avaliableSlots.back();
		avaliableSlots.pop_back();
		assert(id != uint32_invalid_id);
		windows[id] = info;
	}
	return id;
}

void removeFromWindows(uint32 id) 
{
	assert(id < windows.size());
	avaliableSlots.emplace_back(id);
}
//////////////////////////////////////////////////////////////

WindowInfo& getFromID(window_id id)
{
	assert(id < windows.size());
	assert(windows[id].hwnd);
	return windows[id];
}

LRESULT CALLBACK internal_window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	LONG_PTR long_ptr{ GetWindowLongPtr(hwnd, 0) };
	return long_ptr ? ((window_proc)long_ptr)(hwnd, msg, wparam, lparam) : DefWindowProc(hwnd, msg, wparam, lparam);
}
}// anonymous namespace

Window createWindow(const WindowInitInfo* const initInfo)
{
	window_proc callback{ initInfo ? initInfo->callback : nullptr };
	window_handle parent{ initInfo ? initInfo->parent : nullptr };

	// Setup a window class
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = internal_window_proc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = callback ? sizeof(callback) : 0;
	wc.hInstance = 0;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = CreateSolidBrush(RGB(26, 48, 76));
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"ZoneWindow";
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// Register the window class
	RegisterClassEx(&wc);
	WindowInfo info{};
	RECT rc{ info.clientArea };

	// adjust the window size for correct device size
	AdjustWindowRect(&rc, info.style, FALSE);

	const wchar_t* caption{ (initInfo && initInfo->caption) ? initInfo->caption : L"Zone Game" };
	const int32 left{ (initInfo && initInfo->left) ? initInfo->left : info.clientArea.left };
	const int32 top{ (initInfo && initInfo->top) ? initInfo->top : info.clientArea.top };
	const int32 width{ (initInfo && initInfo->width) ? initInfo->width : rc.right - rc.left };
	const int32 height{ (initInfo && initInfo->height) ? initInfo->height : rc.bottom - rc.top };

	info.style |= parent ? WS_CHILD : WS_OVERLAPPEDWINDOW;
	// Create an instance of the window class
	 info.hwnd = CreateWindowEx(
		0,					// extended style
		wc.lpszClassName,	// window class name
		caption,			// instance title
		info.style,			// window style
		left, top,			// initial window position
		width, height,		// initial window dimensions
		parent,				// handle to parent window
		NULL,				// handle to menu
		NULL,				// instance of this application
		NULL				// extra creation parameters
	);

	 if (info.hwnd)
	 {
		 window_id id{ addToWindows(info) };

		 if (callback)
		 {
			 SetWindowLongPtr(info.hwnd, 0, (LONG_PTR)callback);
		 }
		 assert(GetLastError() == 0);
		 ShowWindow(info.hwnd, SW_SHOWNORMAL);
		 UpdateWindow(info.hwnd);
		 return Window{ id };
	 }
	 return {};
}

void removeWindow(window_id id)
{
	WindowInfo& info{ getFromID(id) };
	DestroyWindow(info.hwnd);
	removeFromWindows(id);
}

#elif
#error "Must implement at least one platform"
#endif  

}
