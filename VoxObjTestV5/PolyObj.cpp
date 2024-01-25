#include "PolyObj.h"

PreProjectV::PolyObj::PolyObj()
{
}

PreProjectV::PolyObj::PolyObj(shared_ptr<VoxObjSurface8Bit> _voxObj)
{
	//Normal vectors
	normalVectors.push_back({ 1.0, 0.0, 0.0 });
	normalVectors.push_back({ -1.0, 0.0, 0.0 });
	normalVectors.push_back({ 0.0, 1.0, 0.0 });
	normalVectors.push_back({ 0.0, -1.0, 0.0 });
	normalVectors.push_back({ 0.0, 0.0, 1.0 });
	normalVectors.push_back({ 0.0, 0.0, -1.0 });

	//Texture coordinates
	for (float i = 0.0; i <= 1.0; i += 0.00390625)
	{
		textureCoordinates.push_back({ i, 0.0 });
		textureCoordinates.push_back({ i, 1.0 });
	}

	//Vertices
	size_t verticesNumber{ 1 };
	for (const auto& i : _voxObj->surfaces)
	{
		string surfaceName = to_string(i.first);
		vec3 coordinates = SizeTToI16vec3(i.first);
		PolyObjSurface objSurface;

		size_t buffer[CHUNK_SIZE + 1][CHUNK_SIZE + 1][CHUNK_SIZE + 1]{ 0 };

		for (const auto& j : i.second->surface)
		{

			u8 color = j.y >> 8;
			u8 voxelSurface = j.y & 0xFF;
			u16vec3 s{ j.x & 0xF, (j.x >> 4) & 0xF, (j.x >> 8) & 0xF };

			//---------------------------------------------------------------------------------------------------------------

			//oX positive
			if (voxelSurface & 0x4)
			{
				if (buffer[s.x + 1][s.y][s.z] == 0)
				{
					buffer[s.x + 1][s.y][s.z] = verticesNumber;
					verticesNumber++;

					vertices.push_back(
						(coordinates * (float)PHYSICAL_CHUNK_SIZE) +
						(vec3{ s.x + 1, s.y, s.z } *(float)PHYSICAL_VOXEL_SIZE)
					);
				}
				if (buffer[s.x + 1][s.y + 1][s.z] == 0)
				{
					buffer[s.x + 1][s.y + 1][s.z] = verticesNumber;
					verticesNumber++;

					vertices.push_back(
						(coordinates * (float)PHYSICAL_CHUNK_SIZE) +
						(vec3{ s.x + 1, s.y + 1, s.z } *(float)PHYSICAL_VOXEL_SIZE)
					);
				}
				if (buffer[s.x + 1][s.y + 1][s.z + 1] == 0)
				{
					buffer[s.x + 1][s.y + 1][s.z + 1] = verticesNumber;
					verticesNumber++;

					vertices.push_back(
						(coordinates * (float)PHYSICAL_CHUNK_SIZE) +
						(vec3{ s.x + 1, s.y + 1, s.z + 1 } *(float)PHYSICAL_VOXEL_SIZE)
					);
				}
				if (buffer[s.x + 1][s.y][s.z + 1] == 0)
				{
					buffer[s.x + 1][s.y][s.z + 1] = verticesNumber;
					verticesNumber++;

					vertices.push_back(
						(coordinates * (float)PHYSICAL_CHUNK_SIZE) +
						(vec3{ s.x + 1, s.y, s.z + 1 } *(float)PHYSICAL_VOXEL_SIZE)
					);
				}

				Polygon polygon;
				polygon.vertices = { buffer[s.x + 1][s.y][s.z], buffer[s.x + 1][s.y + 1][s.z], buffer[s.x + 1][s.y + 1][s.z + 1], buffer[s.x + 1][s.y][s.z + 1] };
				polygon.vertexNormals = { 1, 1, 1, 1 };
				polygon.textureCoordinates = { (size_t)color * 2 + 1, (size_t)color * 2 + 2, (size_t)color * 2 + 3, (size_t)color * 2 + 4 };

				objSurface.polygons.push_back(polygon);
			}

			//oX negative
			if (voxelSurface & 0x8)
			{
				if (buffer[s.x][s.y][s.z] == 0)
				{
					buffer[s.x][s.y][s.z] = verticesNumber;
					verticesNumber++;

					vertices.push_back(
						(coordinates * (float)PHYSICAL_CHUNK_SIZE) +
						(vec3{ s.x, s.y, s.z } *(float)PHYSICAL_VOXEL_SIZE)
					);
				}
				if (buffer[s.x][s.y + 1][s.z] == 0)
				{
					buffer[s.x][s.y + 1][s.z] = verticesNumber;
					verticesNumber++;

					vertices.push_back(
						(coordinates * (float)PHYSICAL_CHUNK_SIZE) +
						(vec3{ s.x, s.y + 1, s.z } *(float)PHYSICAL_VOXEL_SIZE)
					);
				}
				if (buffer[s.x][s.y + 1][s.z + 1] == 0)
				{
					buffer[s.x][s.y + 1][s.z + 1] = verticesNumber;
					verticesNumber++;

					vertices.push_back(
						(coordinates * (float)PHYSICAL_CHUNK_SIZE) +
						(vec3{ s.x, s.y + 1, s.z + 1 } *(float)PHYSICAL_VOXEL_SIZE)
					);
				}
				if (buffer[s.x][s.y][s.z + 1] == 0)
				{
					buffer[s.x][s.y][s.z + 1] = verticesNumber;
					verticesNumber++;

					vertices.push_back(
						(coordinates * (float)PHYSICAL_CHUNK_SIZE) +
						(vec3{ s.x, s.y, s.z + 1 } *(float)PHYSICAL_VOXEL_SIZE)
					);
				}

				Polygon polygon;
				polygon.vertices = { buffer[s.x][s.y][s.z], buffer[s.x][s.y + 1][s.z], buffer[s.x][s.y + 1][s.z + 1], buffer[s.x][s.y][s.z + 1] };
				polygon.vertexNormals = { 2, 2, 2, 2 };
				polygon.textureCoordinates = { (size_t)color * 2 + 1, (size_t)color * 2 + 2, (size_t)color * 2 + 3, (size_t)color * 2 + 4 };

				objSurface.polygons.push_back(polygon);
			}

			//---------------------------------------------------------------------------------------------------------------

			//oY positive
			if (voxelSurface & 0x10)
			{
				if (buffer[s.x][s.y + 1][s.z] == 0)
				{
					buffer[s.x][s.y + 1][s.z] = verticesNumber;
					verticesNumber++;

					vertices.push_back(
						(coordinates * (float)PHYSICAL_CHUNK_SIZE) +
						(vec3{ s.x, s.y + 1, s.z } *(float)PHYSICAL_VOXEL_SIZE)
					);
				}
				if (buffer[s.x + 1][s.y + 1][s.z] == 0)
				{
					buffer[s.x + 1][s.y + 1][s.z] = verticesNumber;
					verticesNumber++;

					vertices.push_back(
						(coordinates * (float)PHYSICAL_CHUNK_SIZE) +
						(vec3{ s.x + 1, s.y + 1, s.z } *(float)PHYSICAL_VOXEL_SIZE)
					);
				}
				if (buffer[s.x + 1][s.y + 1][s.z + 1] == 0)
				{
					buffer[s.x + 1][s.y + 1][s.z + 1] = verticesNumber;
					verticesNumber++;

					vertices.push_back(
						(coordinates * (float)PHYSICAL_CHUNK_SIZE) +
						(vec3{ s.x + 1, s.y + 1, s.z + 1 } *(float)PHYSICAL_VOXEL_SIZE)
					);
				}
				if (buffer[s.x][s.y + 1][s.z + 1] == 0)
				{
					buffer[s.x][s.y + 1][s.z + 1] = verticesNumber;
					verticesNumber++;

					vertices.push_back(
						(coordinates * (float)PHYSICAL_CHUNK_SIZE) +
						(vec3{ s.x, s.y + 1, s.z + 1 } *(float)PHYSICAL_VOXEL_SIZE)
					);
				}

				Polygon polygon;
				polygon.vertices = { buffer[s.x][s.y + 1][s.z], buffer[s.x + 1][s.y + 1][s.z], buffer[s.x + 1][s.y + 1][s.z + 1], buffer[s.x][s.y + 1][s.z + 1] };
				polygon.vertexNormals = { 3, 3, 3, 3 };
				polygon.textureCoordinates = { (size_t)color * 2 + 1, (size_t)color * 2 + 2, (size_t)color * 2 + 3, (size_t)color * 2 + 4 };

				objSurface.polygons.push_back(polygon);
			}

			//oY negative
			if (voxelSurface & 0x20)
			{
				if (buffer[s.x][s.y][s.z] == 0)
				{
					buffer[s.x][s.y][s.z] = verticesNumber;
					verticesNumber++;

					vertices.push_back(
						(coordinates * (float)PHYSICAL_CHUNK_SIZE) +
						(vec3{ s.x, s.y, s.z } *(float)PHYSICAL_VOXEL_SIZE)
					);
				}
				if (buffer[s.x + 1][s.y][s.z] == 0)
				{
					buffer[s.x + 1][s.y][s.z] = verticesNumber;
					verticesNumber++;

					vertices.push_back(
						(coordinates * (float)PHYSICAL_CHUNK_SIZE) +
						(vec3{ s.x + 1, s.y, s.z } *(float)PHYSICAL_VOXEL_SIZE)
					);
				}
				if (buffer[s.x + 1][s.y][s.z + 1] == 0)
				{
					buffer[s.x + 1][s.y][s.z + 1] = verticesNumber;
					verticesNumber++;

					vertices.push_back(
						(coordinates * (float)PHYSICAL_CHUNK_SIZE) +
						(vec3{ s.x + 1, s.y, s.z + 1 } *(float)PHYSICAL_VOXEL_SIZE)
					);
				}
				if (buffer[s.x][s.y][s.z + 1] == 0)
				{
					buffer[s.x][s.y][s.z + 1] = verticesNumber;
					verticesNumber++;

					vertices.push_back(
						(coordinates * (float)PHYSICAL_CHUNK_SIZE) +
						(vec3{ s.x, s.y, s.z + 1 } *(float)PHYSICAL_VOXEL_SIZE)
					);
				}

				Polygon polygon;
				polygon.vertices = { buffer[s.x][s.y][s.z], buffer[s.x + 1][s.y][s.z], buffer[s.x + 1][s.y][s.z + 1], buffer[s.x][s.y][s.z + 1] };
				polygon.vertexNormals = { 4, 4, 4, 4 };
				polygon.textureCoordinates = { (size_t)color * 2 + 1, (size_t)color * 2 + 2, (size_t)color * 2 + 3, (size_t)color * 2 + 4 };

				objSurface.polygons.push_back(polygon);
			}

			//---------------------------------------------------------------------------------------------------------------

			//oZ positive
			if (voxelSurface & 0x40)
			{
				if (buffer[s.x][s.y][s.z + 1] == 0)
				{
					buffer[s.x][s.y][s.z + 1] = verticesNumber;
					verticesNumber++;

					vertices.push_back(
						(coordinates * (float)PHYSICAL_CHUNK_SIZE) +
						(vec3{ s.x, s.y, s.z + 1 } *(float)PHYSICAL_VOXEL_SIZE)
					);
				}
				if (buffer[s.x + 1][s.y][s.z + 1] == 0)
				{
					buffer[s.x + 1][s.y][s.z + 1] = verticesNumber;
					verticesNumber++;

					vertices.push_back(
						(coordinates * (float)PHYSICAL_CHUNK_SIZE) +
						(vec3{ s.x + 1, s.y, s.z + 1 } *(float)PHYSICAL_VOXEL_SIZE)
					);
				}
				if (buffer[s.x + 1][s.y + 1][s.z + 1] == 0)
				{
					buffer[s.x + 1][s.y + 1][s.z + 1] = verticesNumber;
					verticesNumber++;

					vertices.push_back(
						(coordinates * (float)PHYSICAL_CHUNK_SIZE) +
						(vec3{ s.x + 1, s.y + 1, s.z + 1 } *(float)PHYSICAL_VOXEL_SIZE)
					);
				}
				if (buffer[s.x][s.y + 1][s.z + 1] == 0)
				{
					buffer[s.x][s.y + 1][s.z + 1] = verticesNumber;
					verticesNumber++;

					vertices.push_back(
						(coordinates * (float)PHYSICAL_CHUNK_SIZE) +
						(vec3{ s.x, s.y + 1, s.z + 1 } *(float)PHYSICAL_VOXEL_SIZE)
					);
				}

				Polygon polygon;
				polygon.vertices = { buffer[s.x][s.y][s.z + 1], buffer[s.x + 1][s.y][s.z + 1], buffer[s.x + 1][s.y + 1][s.z + 1], buffer[s.x][s.y + 1][s.z + 1] };
				polygon.vertexNormals = { 5, 5, 5, 5 };
				polygon.textureCoordinates = { (size_t)color * 2 + 1, (size_t)color * 2 + 2, (size_t)color * 2 + 3, (size_t)color * 2 + 4 };

				objSurface.polygons.push_back(polygon);
			}

			//oZ negative
			if (voxelSurface & 0x80)
			{
				if (buffer[s.x][s.y][s.z] == 0)
				{
					buffer[s.x][s.y][s.z] = verticesNumber;
					verticesNumber++;

					vertices.push_back(
						(coordinates * (float)PHYSICAL_CHUNK_SIZE) +
						(vec3{ s.x, s.y, s.z } *(float)PHYSICAL_VOXEL_SIZE)
					);
				}
				if (buffer[s.x + 1][s.y][s.z] == 0)
				{
					buffer[s.x + 1][s.y][s.z] = verticesNumber;
					verticesNumber++;

					vertices.push_back(
						(coordinates * (float)PHYSICAL_CHUNK_SIZE) +
						(vec3{ s.x + 1, s.y, s.z } *(float)PHYSICAL_VOXEL_SIZE)
					);
				}
				if (buffer[s.x + 1][s.y + 1][s.z] == 0)
				{
					buffer[s.x + 1][s.y + 1][s.z] = verticesNumber;
					verticesNumber++;

					vertices.push_back(
						(coordinates * (float)PHYSICAL_CHUNK_SIZE) +
						(vec3{ s.x + 1, s.y + 1, s.z } *(float)PHYSICAL_VOXEL_SIZE)
					);
				}
				if (buffer[s.x][s.y + 1][s.z] == 0)
				{
					buffer[s.x][s.y + 1][s.z] = verticesNumber;
					verticesNumber++;

					vertices.push_back(
						(coordinates * (float)PHYSICAL_CHUNK_SIZE) +
						(vec3{ s.x, s.y + 1, s.z } *(float)PHYSICAL_VOXEL_SIZE)
					);
				}

				Polygon polygon;
				polygon.vertices = { buffer[s.x][s.y][s.z], buffer[s.x + 1][s.y][s.z], buffer[s.x + 1][s.y + 1][s.z], buffer[s.x][s.y + 1][s.z] };
				polygon.vertexNormals = { 6, 6, 6, 6 };
				polygon.textureCoordinates = { (size_t)color * 2 + 1, (size_t)color * 2 + 2, (size_t)color * 2 + 3, (size_t)color * 2 + 4 };

				objSurface.polygons.push_back(polygon);
			}
		}

		surfaces.emplace(surfaceName, objSurface);
	}
}

bool PreProjectV::PolyObj::Save(const string& _path, const string& _name)
{
	if (SaveObjFile(_path, _name + ".obj") == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool PreProjectV::PolyObj::SaveObjFile(const string& _path, const string& _name)
{
	ofstream file(_path + _name);
	if (!file.is_open())
	{
		return false;
	}
	else
	{
		file << "mtllib test.mtl" << "\n";
		file << "usemtl palette" << "\n";

		for (const auto& i : vertices)
		{
			file << "v " << i.x << " " << i.y << " " << i.z << "\n";
		}

		for (const auto& i : normalVectors)
		{
			file << "vn " << i.x << " " << i.y << " " << i.z << "\n";
		}

		for (const auto& i : textureCoordinates)
		{
			file << "vt " << i.x << " " << i.y << "\n";
		}

		for (const auto& i : surfaces)
		{
			file << "o " << i.first << "\n";
			for (const auto& j : i.second.polygons)
			{
				file << "f " <<
					j.vertices.x << "/" << j.textureCoordinates.x << "/" << j.vertexNormals.x << " " <<
					j.vertices.z << "/" << j.textureCoordinates.z << "/" << j.vertexNormals.z << " " <<
					j.vertices.y << "/" << j.textureCoordinates.y << "/" << j.vertexNormals.y << "\n";

				file << "f " <<
					j.vertices.x << "/" << j.textureCoordinates.x << "/" << j.vertexNormals.x << " " <<
					j.vertices.w << "/" << j.textureCoordinates.w << "/" << j.vertexNormals.w << " " <<
					j.vertices.z << "/" << j.textureCoordinates.z << "/" << j.vertexNormals.z << "\n";
			}
		}
	}
	file.close();

	return true;
}
