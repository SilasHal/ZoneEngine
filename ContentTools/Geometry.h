// Copyright (c) CedricZ1, 2025
// Distributed under the MIT license. See the LICENSE file in the project root for more information.

#pragma once
#include "ToolsCommon.h"

namespace zone::tools {

struct Mesh
{
	//initial data
	utl::vector<math::Vec3F>					positions;
	utl::vector<math::Vec3F>					normals;
	utl::vector<math::Vec3F>					tangents;
	utl::vector<utl::vector<math::Vec2F>>		uvSets;

	utl::vector<uint32>							rawIndices;
};

struct LodGroup
{
	std::string name;
	std::vector<Mesh> Meshes;
};

struct Scene 
{
	std::string name;
	std::vector<LodGroup> LodGroups;
};

struct GeometryImportSettings
{
	float smoothingAngle;
	uint8 calculateNormals;
	uint8 calculateTangents;
	uint8 reverseHandedness;
	uint8 importEmbededTextures;
	uint8 importAnimations;
};

struct SceneData
{
	uint8*					buffer;
	uint8					bufferSize;
	GeometryImportSettings	settings;
};

void processScene(Scene& scene, const GeometryImportSettings& settings);
void packData(const Scene& scene, SceneData& data);

}