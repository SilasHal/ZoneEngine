#pragma once
#include "ToolsCommon.h"

namespace zone::tools {

	namespace packedVertex {
		struct VertexStatic
		{
			math::Vec3F                                position{};
			uint8                                      reserved[3];
			uint8                                      tSign; //bit 0; tangent handedness * (tangent.z sign), bit 1: normal.z sign (0 means -1, 1 means +1)
			uint16                                     normal[2];
			uint16                                     tangent[2];
			math::Vec2F								   uv;
		};

	} // namespace packedVertex

	struct Vertex
	{
		math::Vec4F                                    tangent{};
		math::Vec3F                                    position{};
		math::Vec3F                                    normal{};
		math::Vec2F									   uv{};
	};

	struct Mesh
	{
		// initial data
		utl::vector<math::Vec3F>                       positions;
		utl::vector<math::Vec3F>                       normals;
		utl::vector<math::Vec4F>                       tangents;
		utl::vector<utl::vector<math::Vec2F>>          uvSets;

		utl::vector<uint32>                            rawIndices;

		// Intermediate data
		utl::vector<Vertex>   				           vertices;
		utl::vector<uint32>                            indices;
		// Output data
		std::string                                    name;
		utl::vector<packedVertex::VertexStatic>        packedVerticesStatic;
		float                                          lodThreshold{ -1.f };
		uint32                                         lodId{ uint32_invalid_id };
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

	void ProcessScene(Scene& scene, const GeometryImportSettings& settings);
	void PackData(const Scene& scene, SceneData& data);
}