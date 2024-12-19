// Copyright (c) CedricZ1, 2024
// Distributed under the MIT license. See the LICENSE file in the project root for more information.

#pragma once
#include "Test.h"
#include "..\Engine\Components\Entity.h"
#include "..\Engine\Components\Transform.h"

using namespace zone;

class Engine_Test : public Test
{
public:
	bool initialize() override { return true; }
	void run() override { }
	void shutdown() override { }
};
