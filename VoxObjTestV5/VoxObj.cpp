#include "VoxObj.h"

using namespace PreProjectV;

bool VoxObj8Bit::CreateChunk(i16vec3 _coordinates)
{
	return VoxObj8Bit::CreateChunk(I16vec3ToSizeT(_coordinates));
}

bool VoxObj8Bit::CreateChunk(size_t _key)
{
	if (chunks.find(_key) != chunks.end())
	{
		return false;
	}
	else
	{
		chunks.emplace(_key, make_shared<VoxObjChunk8Bit>());
		return true;
	}
}

shared_ptr<VoxObjChunk8Bit> VoxObj8Bit::GetChunk(i16vec3 _coordinates)
{
	return GetChunk(I16vec3ToSizeT(_coordinates));
}

shared_ptr<VoxObjChunk8Bit> VoxObj8Bit::GetChunk(size_t _key)
{
	auto chunk = chunks.find(_key);
	if (chunk == chunks.end())
	{
		return nullptr;
	}
	else
	{
		return chunk->second;
	}
}

bool VoxObj8Bit::DeleteChunk(i16vec3 _coordinates)
{
	return DeleteChunk(I16vec3ToSizeT(_coordinates));
}

bool VoxObj8Bit::DeleteChunk(size_t _key)
{
	chunks.erase(_key);
	return true;
}

bool VoxObj8Bit::AddChunk(i16vec3 _coordinates, shared_ptr<VoxObjChunk8Bit> _chunk)
{
	return AddChunk(I16vec3ToSizeT(_coordinates), _chunk);
}

bool VoxObj8Bit::AddChunk(size_t _key, shared_ptr<VoxObjChunk8Bit> _chunk)
{
	auto chunk = chunks.find(_key);
	if (chunk != chunks.end())
	{
		return false;
	}
	else
	{
		chunks.emplace(_key, _chunk);
		return true;
	}
}

u8 VoxObj8Bit::GetVoxel(const i32vec3& _coordinates)
{
	i16vec3 externalCoordinates = GetChunkExternalCoordinates(_coordinates);

	auto chunk = chunks.find(I16vec3ToSizeT(externalCoordinates));
	if (chunk == chunks.end())
	{
		return 0;
	}
	else
	{
		u8vec3 internalCoordinates = GetChunkInternalCoordinates(_coordinates);
		return chunk->second->voxels[internalCoordinates.x][internalCoordinates.y][internalCoordinates.z];
	}
}

void VoxObj8Bit::SetVoxel(const i32vec3& _coordinates, u8 _color)
{
	i16vec3 externalCoordinates = GetChunkExternalCoordinates(_coordinates);
	size_t mapKey = I16vec3ToSizeT(externalCoordinates);

	auto chunk = chunks.find(mapKey);
	if (chunk == chunks.end())
	{
		if (_color != 0)
		{
			CreateChunk(externalCoordinates);
			chunk = chunks.find(mapKey);
		}
		else
		{
			return;
		}
	}

	u8vec3 internalCoordinates = GetChunkInternalCoordinates(_coordinates);
	chunk->second->voxels[internalCoordinates.x][internalCoordinates.y][internalCoordinates.z] = _color;
}

void VoxObj8Bit::Extend(shared_ptr<VoxObj8Bit> _voxObj, i32vec3& _coordinates)
{
	for (const auto& i : _voxObj->chunks)
	{
		i16vec3 _chunkCoordinates = SizeTToI16vec3(i.first);
		i32vec3 _shift{ _chunkCoordinates.x << 4, _chunkCoordinates.y << 4, _chunkCoordinates.z << 4 };

		for (size_t x = 0; x < 16; x++)
		{
			for (size_t y = 0; y < 16; y++)
			{
				for (size_t z = 0; z < 16; z++)
				{
					u8 color = i.second->voxels[x][y][z];
					if (color != 0)
					{
						SetVoxel(_shift + i32vec3{x, y, z}, color);
					}
				}
			}
		}
	}
}

size_t VoxObj8Bit::Save(const string& _path, const string& _name)
{
	ofstream file(string{ _path + _name + ".nvox" }, ios::binary);
	if (!file.is_open())
	{
		file.close();
		return 0;
	}
	else
	{
		for (const auto& i : chunks)
		{
			// Save chunk & get a result code
			size_t result = i.second->Save(i.first, file);

			// If there is some error
			if (result == 0)
			{
				return 0;
				file.close();
			}
		}

		file.close();
	}
	return 1;
}

size_t VoxObj8Bit::Load(const string& _path, const string& _name)
{
	ifstream file(string{ _path + _name + ".nvox" }, ios::binary);

	if (!file.is_open())
	{
		file.close();
		return 0;
	}
	else
	{
		file.seekg(0, ios::end);
		size_t fileSize = file.tellg();
		file.seekg(0, ios::beg);

		while (!file.eof()) 
		{
			if (file.tellg() == fileSize) break;

			auto chunk = make_shared<VoxObjChunk8Bit>();
			auto info = chunk->Load(file);

			if (info.first == 1)
			{
				AddChunk(info.second, chunk);
			}
		}
		file.close();
	}

	return 1;
}

size_t VoxObjChunk8Bit::Save(size_t _key, ofstream& _file)
{
	VoxObj8BitOctree octree;

	// Copy all voxels to a temporary space
	memcpy(&(octree.voxelsL4), &voxels, 4096);

	// Check if chunk is empty
	GenOctree(octree);
	if (octree.cStatusL0 == 0x1 && octree.voxelsL0 == 0)
	{
		return 2;
	}

	// Save key (coordinates) and data block size
	_file.write((char*)&(_key), 6);
	_file.write((char*)&(octree.dataBlockSize), 2);

	WriteToFile(_file, octree);

	return 1;
}

pair<size_t, size_t> VoxObjChunk8Bit::Load(ifstream& _file)
{
	VoxObj8BitOctree octree;
	size_t key;
	auto result = pair<size_t, size_t>();

	// Load key (coordinates) and data block size
	_file.read((char*)&(key), 6);
	_file.read((char*)&(octree.dataBlockSize), 2);

	LoadFromFile(_file, octree);

	FillOctree(octree);

	result.first = 1;
	result.second = key;

	return result;
}

void VoxObjChunk8Bit::GenOctree(VoxObj8BitOctree& _octree)
{
	u8 statusL0;
	u8 statusL1[2][2][2];
	u8 statusL2[4][4][4];
	u8 statusL3[8][8][8];

	//L3 check 
	for (size_t x = 0; x < 8; x++)
	{
		for (size_t y = 0; y < 8; y++)
		{
			for (size_t z = 0; z < 8; z++)
			{
				size_t checkValue =
					(_octree.voxelsL4[x << 1][y << 1][z << 1] == _octree.voxelsL4[(x << 1) + 1][y << 1][z << 1]) &&
					(_octree.voxelsL4[x << 1][(y << 1) + 1][z << 1] == _octree.voxelsL4[(x << 1) + 1][(y << 1) + 1][z << 1]) &&
					(_octree.voxelsL4[x << 1][y << 1][(z << 1) + 1] == _octree.voxelsL4[(x << 1) + 1][y << 1][(z << 1) + 1]) &&
					(_octree.voxelsL4[x << 1][(y << 1) + 1][(z << 1) + 1] == _octree.voxelsL4[(x << 1) + 1][(y << 1) + 1][(z << 1) + 1]) &&
					(_octree.voxelsL4[x << 1][y << 1][z << 1] == _octree.voxelsL4[(x << 1)][(y << 1) + 1][z << 1]) &&
					(_octree.voxelsL4[x << 1][(y << 1) + 1][z << 1] == _octree.voxelsL4[x << 1][y << 1][(z << 1) + 1]) &&
					(_octree.voxelsL4[x << 1][y << 1][(z << 1) + 1] == _octree.voxelsL4[x << 1][(y << 1) + 1][(z << 1) + 1]);

				if (checkValue)
				{
					_octree.voxelsL3[x][y][z] = _octree.voxelsL4[x << 1][y << 1][z << 1];
					statusL3[x][y][z] = 0x1;
				}
				else
				{
					statusL3[x][y][z] = 0x0;
				}
			}
		}
	}

	//L2 check
	for (size_t x = 0; x < 4; x++)
	{
		for (size_t y = 0; y < 4; y++)
		{
			for (size_t z = 0; z < 4; z++)
			{
				if (
					statusL3[x << 1][y << 1][z << 1] && statusL3[(x << 1) + 1][y << 1][z << 1] &&
					statusL3[x << 1][(y << 1) + 1][z << 1] && statusL3[(x << 1) + 1][(y << 1) + 1][z << 1] &&
					statusL3[x << 1][y << 1][(z << 1) + 1] && statusL3[(x << 1) + 1][y << 1][(z << 1) + 1] &&
					statusL3[x << 1][(y << 1) + 1][(z << 1) + 1] && statusL3[(x << 1) + 1][(y << 1) + 1][(z << 1) + 1]
					)
				{
					size_t checkValue =
						(_octree.voxelsL3[x << 1][y << 1][z << 1] == _octree.voxelsL3[(x << 1) + 1][y << 1][z << 1]) &&
						(_octree.voxelsL3[x << 1][(y << 1) + 1][z << 1] == _octree.voxelsL3[(x << 1) + 1][(y << 1) + 1][z << 1]) &&
						(_octree.voxelsL3[x << 1][y << 1][(z << 1) + 1] == _octree.voxelsL3[(x << 1) + 1][y << 1][(z << 1) + 1]) &&
						(_octree.voxelsL3[x << 1][(y << 1) + 1][(z << 1) + 1] == _octree.voxelsL3[(x << 1) + 1][(y << 1) + 1][(z << 1) + 1]) &&
						(_octree.voxelsL3[x << 1][y << 1][z << 1] == _octree.voxelsL3[(x << 1)][(y << 1) + 1][z << 1]) &&
						(_octree.voxelsL3[x << 1][(y << 1) + 1][z << 1] == _octree.voxelsL3[x << 1][y << 1][(z << 1) + 1]) &&
						(_octree.voxelsL3[x << 1][y << 1][(z << 1) + 1] == _octree.voxelsL3[x << 1][(y << 1) + 1][(z << 1) + 1]);

					if (checkValue)
					{
						_octree.voxelsL2[x][y][z] = _octree.voxelsL3[x << 1][y << 1][z << 1];
						statusL2[x][y][z] = 0x1;
					}
					else
					{
						statusL2[x][y][z] = 0x0;
					}
				}
				else
				{
					statusL2[x][y][z] = 0x0;
				}
			}
		}
	}

	//L1 check
	for (size_t x = 0; x < 2; x++)
	{
		for (size_t y = 0; y < 2; y++)
		{
			for (size_t z = 0; z < 2; z++)
			{
				if (
					statusL2[x << 1][y << 1][z << 1] && statusL2[(x << 1) + 1][y << 1][z << 1] &&
					statusL2[x << 1][(y << 1) + 1][z << 1] && statusL2[(x << 1) + 1][(y << 1) + 1][z << 1] &&
					statusL2[x << 1][y << 1][(z << 1) + 1] && statusL2[(x << 1) + 1][y << 1][(z << 1) + 1] &&
					statusL2[x << 1][(y << 1) + 1][(z << 1) + 1] && statusL2[(x << 1) + 1][(y << 1) + 1][(z << 1) + 1]
					)
				{
					size_t checkValue =
						(_octree.voxelsL2[x << 1][y << 1][z << 1] == _octree.voxelsL2[(x << 1) + 1][y << 1][z << 1]) &&
						(_octree.voxelsL2[x << 1][(y << 1) + 1][z << 1] == _octree.voxelsL2[(x << 1) + 1][(y << 1) + 1][z << 1]) &&
						(_octree.voxelsL2[x << 1][y << 1][(z << 1) + 1] == _octree.voxelsL2[(x << 1) + 1][y << 1][(z << 1) + 1]) &&
						(_octree.voxelsL2[x << 1][(y << 1) + 1][(z << 1) + 1] == _octree.voxelsL2[(x << 1) + 1][(y << 1) + 1][(z << 1) + 1]) &&
						(_octree.voxelsL2[x << 1][y << 1][z << 1] == _octree.voxelsL2[(x << 1)][(y << 1) + 1][z << 1]) &&
						(_octree.voxelsL2[x << 1][(y << 1) + 1][z << 1] == _octree.voxelsL2[x << 1][y << 1][(z << 1) + 1]) &&
						(_octree.voxelsL2[x << 1][y << 1][(z << 1) + 1] == _octree.voxelsL2[x << 1][(y << 1) + 1][(z << 1) + 1]);

					if (checkValue)
					{
						_octree.voxelsL1[x][y][z] = _octree.voxelsL2[x << 1][y << 1][z << 1];
						statusL1[x][y][z] = 0x1;
					}
					else
					{
						statusL1[x][y][z] = 0x0;
					}
				}
				else
				{
					statusL1[x][y][z] = 0x0;
				}
			}
		}
	}

	//L0 check
	if (
		statusL1[0][0][0] && statusL1[1][0][0] &&
		statusL1[0][1][0] && statusL1[1][1][0] &&
		statusL1[0][0][1] && statusL1[1][0][1] &&
		statusL1[0][1][1] && statusL1[1][1][1]
		)
	{
		size_t checkValue =
			(_octree.voxelsL1[0][0][0] == _octree.voxelsL1[1][0][0]) &&
			(_octree.voxelsL1[0][1][0] == _octree.voxelsL1[1][1][0]) &&
			(_octree.voxelsL1[0][0][1] == _octree.voxelsL1[1][0][1]) &&
			(_octree.voxelsL1[0][1][1] == _octree.voxelsL1[1][1][1]) &&
			(_octree.voxelsL1[0][0][0] == _octree.voxelsL1[0][1][0]) &&
			(_octree.voxelsL1[0][1][0] == _octree.voxelsL1[0][0][1]) &&
			(_octree.voxelsL1[0][0][1] == _octree.voxelsL1[0][1][1]);

		if (checkValue)
		{
			_octree.voxelsL0 = _octree.voxelsL1[0][0][0];
			statusL0 = 0x1;
		}
		else
		{
			statusL0 = 0x0;
		}
	}
	else
	{
		statusL0 = 0x0;
	}

	//Data block size check and compression
	if (statusL0)
	{
		//L0
		_octree.cStatusL0 = 0x1;
		_octree.dataBlockSize += 2;
	}
	else
	{
		//L0
		_octree.cStatusL0 = 0x0;
		_octree.dataBlockSize += 1;

		//L1
		_octree.cStatusL1 = 0x0;
		_octree.dataBlockSize += 1;

		for (size_t itL1 = 0; itL1 < 8; itL1++)
		{
			size_t xL1{ itL1 & 0x1 };
			size_t yL1{ (itL1 & 0x2) >> 1 };
			size_t zL1{ (itL1 & 0x4) >> 2 };

			if (statusL1[xL1][yL1][zL1])
			{
				_octree.cStatusL1 = _octree.cStatusL1 | (0x1 << itL1);
				_octree.dataBlockSize += 1;
			}
			else
			{
				//L2
				_octree.cStatusL2[xL1][yL1][zL1] = 0x0;
				_octree.dataBlockSize += 1;

				for (size_t itL2 = 0; itL2 < 8; itL2++)
				{
					size_t xL2{ (itL2 & 0x1) + (xL1 << 1) };
					size_t yL2{ ((itL2 & 0x2) >> 1) + (yL1 << 1) };
					size_t zL2{ ((itL2 & 0x4) >> 2) + (zL1 << 1) };

					if (statusL2[xL2][yL2][zL2])
					{
						_octree.cStatusL2[xL1][yL1][zL1] = _octree.cStatusL2[xL1][yL1][zL1] | (0x1 << itL2);
						_octree.dataBlockSize += 1;
					}
					else
					{
						//L3
						_octree.cStatusL3[xL2][yL2][zL2] = 0x0;
						_octree.dataBlockSize += 1;

						for (size_t itL3 = 0; itL3 < 8; itL3++)
						{
							size_t xL3{ (itL3 & 0x1) + (xL2 << 1) };
							size_t yL3{ ((itL3 & 0x2) >> 1) + (yL2 << 1) };
							size_t zL3{ ((itL3 & 0x4) >> 2) + (zL2 << 1) };

							if (statusL3[xL3][yL3][zL3])
							{
								_octree.cStatusL3[xL2][yL2][zL2] = _octree.cStatusL3[xL2][yL2][zL2] | (0x1 << itL3);
								_octree.dataBlockSize += 1;
							}
							else
							{
								_octree.dataBlockSize += 8;
							}
						}
					}
				}
			}
		}
	}
}

void VoxObjChunk8Bit::FillOctree(VoxObj8BitOctree& _octree)
{
	if (_octree.cStatusL0 == 0x1)
	{
		for (size_t it = 0; it < 4096; it++)
		{
			size_t x{ it & 0xF };
			size_t y{ (it & 0xF0) >> 4 };
			size_t z{ (it & 0xF00) >> 8 };

			voxels[x][y][z] = _octree.voxelsL0;
		}
	}
	else
	{
		for (size_t itL1 = 0; itL1 < 8; itL1++)
		{
			size_t xL1{ itL1 & 0x1 };
			size_t yL1{ (itL1 & 0x2) >> 1 };
			size_t zL1{ (itL1 & 0x4) >> 2 };

			if (_octree.cStatusL1 & (0x1 << itL1))
			{
				for (size_t x = 0; x < 8; x++)
				{
					for (size_t y = 0; y < 8; y++)
					{
						for (size_t z = 0; z < 8; z++)
						{
							size_t local_x = xL1 ? x + 8 : x;
							size_t local_y = yL1 ? y + 8 : y;
							size_t local_z = zL1 ? z + 8 : z;

							voxels[local_x][local_y][local_z] = _octree.voxelsL1[xL1][yL1][zL1];
						}
					}
				}
			}
			else
			{
				for (size_t itL2 = 0; itL2 < 8; itL2++)
				{
					size_t xL2{ (itL2 & 0x1) + (xL1 << 1) };
					size_t yL2{ ((itL2 & 0x2) >> 1) + (yL1 << 1) };
					size_t zL2{ ((itL2 & 0x4) >> 2) + (zL1 << 1) };

					size_t xShift = xL2 << 2;
					size_t yShift = yL2 << 2;
					size_t zShift = zL2 << 2;

					if (_octree.cStatusL2[xL1][yL1][zL1] & (0x1 << itL2))
					{
						for (size_t x = 0; x < 4; x++)
						{
							for (size_t y = 0; y < 4; y++)
							{
								for (size_t z = 0; z < 4; z++)
								{
									size_t local_x = x + xShift;
									size_t local_y = y + yShift;
									size_t local_z = z + zShift;

									voxels[local_x][local_y][local_z] = _octree.voxelsL2[xL2][yL2][zL2];
								}
							}
						}
					}
					else
					{
						for (size_t itL3 = 0; itL3 < 8; itL3++)
						{
							size_t xL3{ (itL3 & 0x1) + (xL2 << 1) };
							size_t yL3{ ((itL3 & 0x2) >> 1) + (yL2 << 1) };
							size_t zL3{ ((itL3 & 0x4) >> 2) + (zL2 << 1) };

							size_t xShift = xL3 << 1;
							size_t yShift = yL3 << 1;
							size_t zShift = zL3 << 1;

							if (_octree.cStatusL3[xL2][yL2][zL2] & (0x1 << itL3))
							{
								for (size_t x = 0; x < 2; x++)
								{
									for (size_t y = 0; y < 2; y++)
									{
										for (size_t z = 0; z < 2; z++)
										{
											size_t local_x = x + xShift;
											size_t local_y = y + yShift;
											size_t local_z = z + zShift;

											voxels[local_x][local_y][local_z] = _octree.voxelsL3[xL3][yL3][zL3];
										}
									}
								}
							}
							else
							{
								for (size_t x = 0; x < 2; x++)
								{
									for (size_t y = 0; y < 2; y++)
									{
										for (size_t z = 0; z < 2; z++)
										{
											size_t local_x = x + xShift;
											size_t local_y = y + yShift;
											size_t local_z = z + zShift;

											voxels[local_x][local_y][local_z] = _octree.voxelsL4[local_x][local_y][local_z];
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

size_t VoxObjChunk8Bit::WriteToFile(ofstream& _file, VoxObj8BitOctree& _octree)
{
	//L0
	_file.write((char*)&(_octree.cStatusL0), 1);

	if (_octree.cStatusL0 == 0x1)
	{
		_file.write((char*)&(_octree.voxelsL0), 1);
	}
	else
	{
		//L1
		_file.write((char*)&(_octree.cStatusL1), 1);

		for (size_t itL1 = 0; itL1 < 8; itL1++)
		{
			size_t xL1{ itL1 & 0x1 };
			size_t yL1{ (itL1 & 0x2) >> 1 };
			size_t zL1{ (itL1 & 0x4) >> 2 };

			if (_octree.cStatusL1 & (0x1 << itL1))
			{
				_file.write((char*)&(_octree.voxelsL1[xL1][yL1][zL1]), 1);
			}
			else
			{
				//L2
				_file.write((char*)&(_octree.cStatusL2[xL1][yL1][zL1]), 1);

				for (size_t itL2 = 0; itL2 < 8; itL2++)
				{
					size_t xL2{ (itL2 & 0x1) + (xL1 << 1) };
					size_t yL2{ ((itL2 & 0x2) >> 1) + (yL1 << 1) };
					size_t zL2{ ((itL2 & 0x4) >> 2) + (zL1 << 1) };

					if (_octree.cStatusL2[xL1][yL1][zL1] & (0x1 << itL2))
					{
						_file.write((char*)&(_octree.voxelsL2[xL2][yL2][zL2]), 1);
					}
					else
					{
						//L3
						_file.write((char*)&(_octree.cStatusL3[xL2][yL2][zL2]), 1);

						for (size_t itL3 = 0; itL3 < 8; itL3++)
						{
							size_t xL3{ (itL3 & 0x1) + (xL2 << 1) };
							size_t yL3{ ((itL3 & 0x2) >> 1) + (yL2 << 1) };
							size_t zL3{ ((itL3 & 0x4) >> 2) + (zL2 << 1) };

							if (_octree.cStatusL3[xL2][yL2][zL2] & (0x1 << itL3))
							{
								_file.write((char*)&(_octree.voxelsL3[xL3][yL3][zL3]), 1);
							}
							else
							{
								for (size_t itL4 = 0; itL4 < 8; itL4++)
								{
									size_t xL4{ (itL4 & 0x1) + (xL3 << 1) };
									size_t yL4{ ((itL4 & 0x2) >> 1) + (yL3 << 1) };
									size_t zL4{ ((itL4 & 0x4) >> 2) + (zL3 << 1) };

									_file.write((char*)&(_octree.voxelsL4[xL4][yL4][zL4]), 1);
								}
							}
						}
					}
				}
			}
		}
	}

	return 1;
}

size_t VoxObjChunk8Bit::LoadFromFile(ifstream& _file, VoxObj8BitOctree& _octree)
{
	// L0
	_file.read((char*)&(_octree.cStatusL0), 1);

	if (_octree.cStatusL0 == 0x1)
	{
		_file.read((char*)&(_octree.voxelsL0), 1);
	}
	else
	{
		// L1
		_file.read((char*)&(_octree.cStatusL1), 1);

		for (size_t itL1 = 0; itL1 < 8; itL1++)
		{
			size_t xL1{ itL1 & 0x1 };
			size_t yL1{ (itL1 & 0x2) >> 1 };
			size_t zL1{ (itL1 & 0x4) >> 2 };

			if (_octree.cStatusL1 & (0x1 << itL1))
			{
				_file.read((char*)&(_octree.voxelsL1[xL1][yL1][zL1]), 1);
			}
			else
			{
				// L2
				_file.read((char*)&(_octree.cStatusL2[xL1][yL1][zL1]), 1);

				for (size_t itL2 = 0; itL2 < 8; itL2++)
				{
					size_t xL2{ (itL2 & 0x1) + (xL1 << 1) };
					size_t yL2{ ((itL2 & 0x2) >> 1) + (yL1 << 1) };
					size_t zL2{ ((itL2 & 0x4) >> 2) + (zL1 << 1) };

					if (_octree.cStatusL2[xL1][yL1][zL1] & (0x1 << itL2))
					{
						_file.read((char*)&(_octree.voxelsL2[xL2][yL2][zL2]), 1);
					}
					else
					{
						// L3
						_file.read((char*)&(_octree.cStatusL3[xL2][yL2][zL2]), 1);

						for (size_t itL3 = 0; itL3 < 8; itL3++)
						{
							size_t xL3{ (itL3 & 0x1) + (xL2 << 1) };
							size_t yL3{ ((itL3 & 0x2) >> 1) + (yL2 << 1) };
							size_t zL3{ ((itL3 & 0x4) >> 2) + (zL2 << 1) };

							if (_octree.cStatusL3[xL2][yL2][zL2] & (0x1 << itL3))
							{
								_file.read((char*)&(_octree.voxelsL3[xL3][yL3][zL3]), 1);
							}
							else
							{
								for (size_t itL4 = 0; itL4 < 8; itL4++)
								{
									size_t xL4{ (itL4 & 0x1) + (xL3 << 1) };
									size_t yL4{ ((itL4 & 0x2) >> 1) + (yL3 << 1) };
									size_t zL4{ ((itL4 & 0x4) >> 2) + (zL3 << 1) };

									_file.read((char*)&(_octree.voxelsL4[xL4][yL4][zL4]), 1);
								}
							}
						}
					}
				}
			}
		}
	}

	return 1;
}