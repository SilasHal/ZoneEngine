// Copyright (c) CedricZ1, 2024
// Distributed under the MIT license. See the LICENSE file in the project root for more information.
#pragma once
#include<thread>

class Test
{
public:
	virtual bool initialize() = 0;
	virtual void run() = 0;
	virtual void shutdown() = 0;
};