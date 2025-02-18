#include "PrimitiveMesh.h"
#include "Geometry.h"
namespace zone::tools {
	namespace {

		using namespace math;
		using MeshCreator = void(*)(Scene&, const MeshInitInfo& mesh);

		void CreatePlane(Scene& scene, const MeshInitInfo& mesh);
		void CreateCube(Scene& scene, const MeshInitInfo& mesh);
		void CreateUVSphere(Scene& scene, const MeshInitInfo& mesh);
		void CreateICOSphere(Scene& scene, const MeshInitInfo& mesh);
		void CreateCylinder(Scene& scene, const MeshInitInfo& mesh);
		void CreateCapsule(Scene& scene, const MeshInitInfo& mesh);

		MeshCreator meshCreators[]
		{
			CreatePlane,
			CreateCube,
			CreateUVSphere,
			CreateICOSphere,
			CreateCylinder,
			CreateCapsule
		};

		static_assert(_countof(meshCreators) == MeshType::Count);

		struct Axis
		{
			enum : uint32 {
				X = 0,
				Y = 1,
				Z = 2
			};
		};

		Mesh CreatePlane(const MeshInitInfo* mesh,
						 uint32 horizontalIndex = Axis::X, uint32 verticalIndex = Axis::Z, bool flipWinding = false,
						 Vec3F offset = { -0.5f, 0.f, -0.5f }, Vec2F uRange = { 0.f, 1.f }, Vec2F vRange = { 0.f, 1.f })
		{
			assert(horizontalIndex < 3 && verticalIndex < 3);
			assert(horizontalIndex != verticalIndex);

			const uint32 horizontalCount{ clamp(mesh->segments[horizontalIndex], 1u, 10u) };
			const uint32 verticalCount{ clamp(mesh->segments[verticalIndex], 1u, 10u) };
			const float horizontalStep{ 1.f / horizontalCount };
			const float verticalStep{ 1.f / verticalCount };
			const float uStep{ (uRange.y - uRange.x) / horizontalCount };
			const float vStep{ (vRange.y - vRange.x) / verticalCount };

			Mesh m{};
			utl::vector<Vec2F> uvs{};
			
			for (uint32 j{ 0 }; j <= verticalCount; ++j)
			{
				for (uint32 i{ 0 }; i <= horizontalCount; ++i)
				{
					Vec3F position{ offset };
					float* const asArray{ &position.x };
					asArray[horizontalIndex] += i * horizontalStep;
					asArray[verticalIndex] += j * verticalStep;
					m.positions.emplace_back(position.x * mesh->size.x, position.y * mesh->size.y, position.z * mesh->size.z);

					Vec2F uv{ uRange.x, 1.f - vRange.x };
					uv.x += i * uStep;
					uv.y -= j * vStep;
					uvs.emplace_back(uv);
				}
			}

			assert(m.positions.size() == (((uint64)horizontalCount + 1) * ((uint64)verticalCount + 1)));

			const uint32 rowLength{ horizontalCount + 1 }; // number of vertices in a row
			for (uint32 j{ 0 }; j < verticalCount; ++j)
			{
				uint32 k{ 0 };
				for (uint32 i{ k }; i < horizontalCount; ++i)
				{
					const uint32 index[4]
					{
						i + j * rowLength,
						i + (j + 1) * rowLength,
						i + 1 + j * rowLength,
						i + 1 + (j + 1) * rowLength
					};

					m.rawIndices.emplace_back(index[0]);
					m.rawIndices.emplace_back(index[flipWinding ? 2 : 1]);
					m.rawIndices.emplace_back(index[flipWinding ? 1 : 2]);

					m.rawIndices.emplace_back(index[2]);
					m.rawIndices.emplace_back(index[flipWinding ? 3 : 1]);
					m.rawIndices.emplace_back(index[flipWinding ? 1 : 3]);
				}
				++k;
			}

			const uint32 numIndices{ 3 * 2 * horizontalCount * verticalCount };
			assert(m.rawIndices.size() == numIndices);
			
			m.uvSets.resize(1);

			for (uint32 i{ 0 }; i < numIndices; ++i)
			{
				m.uvSets[0].emplace_back(uvs[m.rawIndices[i]]);
			}

			return m;
		}

		void CreatePlane(Scene& scene, const MeshInitInfo& mesh)
		{
			// Create a plane mesh
			// 0----1
			// |    |
			// 3----2
			// 0, 1, 2, 3 are the indices of the vertices
			
			LodGroup lod{};
			lod.name = "plane";
			lod.meshes.emplace_back(CreatePlane(&mesh));
			scene.lodGroups.emplace_back(lod);
		}

		void CreateCube(Scene& scene, const MeshInitInfo& mesh)
		{}

		void CreateUVSphere(Scene& scene, const MeshInitInfo& mesh)
		{ }

		void CreateICOSphere(Scene& scene, const MeshInitInfo& mesh)
		{ }

		void CreateCylinder(Scene& scene, const MeshInitInfo& mesh)
		{

		}
		void CreateCapsule(Scene& scene, const MeshInitInfo& mesh)
		{
		}

	} // anonymous namespace

	EDITOR_INTERFACE void CreateMesh(SceneData* data, MeshInitInfo* mesh)
	{
		assert(data && mesh);
		assert(mesh->type < MeshType::Count);
		Scene scene{};
		meshCreators[mesh->type](scene, *mesh);

		data->settings.calculateNormals = 1;
		ProcessScene(scene, data->settings);
		PackData(scene, *data);
	}
}