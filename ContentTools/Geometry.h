// Copyright (c) CedricZ1, 2025
// Distributed under the MIT license. See the LICENSE file in the project root for more information.

#pragma once
#include "ToolsCommon.h"

namespace zone::tools {

struct GeometryImportSettings
{
	float smoothingAngle;
};

struct SceneData
{
	uint8*					buffer;
	uint8					bufferSize;
	GeometryImportSettings	settings;
};

}