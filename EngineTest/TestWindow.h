#pragma once
#include "Test.h"
#include"..\Platform\PlatformTypes.h"
#include "..\Platform\Platform.h"

using namespace zone;

platform::Window _windows[4];

LRESULT winProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
	{
		bool allClosed{ true };
		for (uint32 i{ 0 }; i < _countof(_windows); ++i)
		{
			if (!_windows[i].isClosed())
			{
				allClosed = false;
			}
		}
		if (allClosed)
		{
			PostQuitMessage(0);
			return 0;
		}
	}
	break;
	case WM_SYSCHAR:
		if (wparam == VK_RETURN && (HIWORD(lparam) & KF_ALTDOWN))
		{
			platform::Window win{ platform::window_id{(id::id_type)GetWindowLongPtr(hwnd,GWLP_USERDATA)} };
			win.setFullscreen(!win.isFullscreen());
			return 0;
		}
		break;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

class EngineTest : public Test
{
public:
	bool initialize() override
	{
		platform::WindowInitInfo info[]
		{
			{&winProc,nullptr,L"Test window 1",100,100,400,800},
			{&winProc,nullptr,L"Test window 2",350,150,400,800},
			{&winProc,nullptr,L"Test window 3",600,200,400,800},
			{&winProc,nullptr,L"Test window 4",950,250,400,800},
		};
		static_assert(_countof(info) == _countof(_windows));

		for (uint32 i{ 0 }; i < _countof(_windows); ++i)
		{
			_windows[i] = platform::createWindow(&info[i]);
		}
		return true;
	}

	void run() override
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	void shutdown() override
	{
		for (uint32 i{ 0 }; i < _countof(_windows); ++i)
		{
			platform::removeWindow(_windows[i].getID());
		}
	}
};