#pragma once
#include "ToolsCommon.h"

namespace zone::tools {

	struct Mesh
	{
		// initial data
		utl::vector<math::Vec3F>                       positions;
		utl::vector<math::Vec3F>                       normals;
		utl::vector<math::Vec3F>                       tangents;
		utl::vector<math::Vec4F>                       uvSets;

		utl::vector<uint32>                            rawIndices;

		// Intermediate data

		// Output data
	};

	struct LodGroup
	{
		std::string             name;
		utl::vector<Mesh>       meshes;
	};

	struct Scene
	{
		std::string             name;
		utl::vector<LodGroup>   lodGroups;
	};

	struct GeometryImportSettings
	{
		float smoothingAngle;
		uint8 calculateNormals;
		uint8 calculateTangents;
		uint8 reverseHandedness;
		uint8 importEmebeddedTextures;
		uint8 importAnimations;
	};

	struct SceneData
	{
		uint8*                  buffer;
		uint32					bufferSize;
		GeometryImportSettings	settings;
	};

}