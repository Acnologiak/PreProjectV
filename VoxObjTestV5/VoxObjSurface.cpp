#include "VoxObjSurface.h"


PreProjectV::VoxObjSurface8Bit::VoxObjSurface8Bit()
{
}

PreProjectV::VoxObjSurface8Bit::VoxObjSurface8Bit(shared_ptr<VoxObj8Bit> _voxObj)
{
	for (const auto& i : _voxObj->chunks)
	{
		i16vec3 coordinates = SizeTToI16vec3(i.first);

		shared_ptr<VoxObjChunk8Bit> centralChunk = i.second;
		shared_ptr<VoxObjChunk8Bit> xpChunk = _voxObj->GetChunk(coordinates + i16vec3{1, 0, 0});
		shared_ptr<VoxObjChunk8Bit> xnChunk = _voxObj->GetChunk(coordinates + i16vec3{-1, 0, 0});
		shared_ptr<VoxObjChunk8Bit> ypChunk = _voxObj->GetChunk(coordinates + i16vec3{0, 1, 0});
		shared_ptr<VoxObjChunk8Bit> ynChunk = _voxObj->GetChunk(coordinates + i16vec3{0, -1, 0});
		shared_ptr<VoxObjChunk8Bit> zpChunk = _voxObj->GetChunk(coordinates + i16vec3{0, 0, 1});
		shared_ptr<VoxObjChunk8Bit> znChunk = _voxObj->GetChunk(coordinates + i16vec3{0, 0, -1});

		shared_ptr<VoxObjChunkSurface8Bit> surface = make_shared<VoxObjChunkSurface8Bit>(
			centralChunk,
			xpChunk,
			xnChunk,
			ypChunk,
			ynChunk,
			zpChunk,
			znChunk
		);

		if (surface->surface.size() != 0)
		{
			surfaces.emplace(i.first, surface);
		}
	}
}

PreProjectV::VoxObjChunkSurface8Bit::VoxObjChunkSurface8Bit()
{
}

PreProjectV::VoxObjChunkSurface8Bit::VoxObjChunkSurface8Bit(shared_ptr<VoxObjChunk8Bit> _centralChunk, shared_ptr<VoxObjChunk8Bit> _xpChunk, shared_ptr<VoxObjChunk8Bit> _xnChunk, shared_ptr<VoxObjChunk8Bit> _ypChunk, shared_ptr<VoxObjChunk8Bit> _ynChunk, shared_ptr<VoxObjChunk8Bit> _zpChunk, shared_ptr<VoxObjChunk8Bit> _znChunk)
{
	VoxObjChunk8Bit& centralChunk{ *_centralChunk };
	VoxObjChunk8Bit& xpChunk{ *_xpChunk };
	VoxObjChunk8Bit& xnChunk{ *_xnChunk };
	VoxObjChunk8Bit& ypChunk{ *_ypChunk };
	VoxObjChunk8Bit& ynChunk{ *_ynChunk };
	VoxObjChunk8Bit& zpChunk{ *_zpChunk };
	VoxObjChunk8Bit& znChunk{ *_znChunk };

	for (size_t i = 0; i < CHUNK_SIZE; i++)
	{
		for (size_t j = 0; j < CHUNK_SIZE; j++)
		{
			for (size_t k = 0; k < CHUNK_SIZE; k++)
			{
				i32vec3 coordinates;
				u8 voxelColor = centralChunk.voxels[i][j][k];
				u8 voxelSurface{ 0 };

				if (voxelColor == 0)
				{
					continue;
				}
				else
				{
					//oX positive
					coordinates = { i + 1, j, k };
					if (coordinates.x == CHUNK_SIZE)
					{
						if (_xpChunk != nullptr)
						{
							coordinates.x = 0;
							if (xpChunk.voxels[coordinates.x][coordinates.y][coordinates.z] == 0)
							{
								voxelSurface |= 0x4;
							}
						}
						else
						{
							voxelSurface |= 0x4;
						}
					}
					else
					{
						if (centralChunk.voxels[coordinates.x][coordinates.y][coordinates.z] == 0)
						{
							voxelSurface |= 0x4;
						}
					}

					//oX negative
					coordinates = { i - 1, j, k };
					if (coordinates.x == -1)
					{
						if (_xnChunk != nullptr)
						{
							coordinates.x = CHUNK_SIZE - 1;
							if (xnChunk.voxels[coordinates.x][coordinates.y][coordinates.z] == 0)
							{
								voxelSurface |= 0x8;
							}
						}
						else
						{
							voxelSurface |= 0x8;
						}
					}
					else
					{
						if (centralChunk.voxels[coordinates.x][coordinates.y][coordinates.z] == 0)
						{
							voxelSurface |= 0x8;
						}
					}

					//oY positive
					coordinates = { i, j + 1, k };
					if (coordinates.y == CHUNK_SIZE)
					{
						if (_ypChunk != nullptr)
						{
							coordinates.y = 0;
							if (ypChunk.voxels[coordinates.x][coordinates.y][coordinates.z] == 0)
							{
								voxelSurface |= 0x10;
							}
						}
						else
						{
							voxelSurface |= 0x10;
						}
					}
					else
					{
						if (centralChunk.voxels[coordinates.x][coordinates.y][coordinates.z] == 0)
						{
							voxelSurface |= 0x10;
						}
					}

					//oY negative
					coordinates = { i, j - 1, k };
					if (coordinates.y == -1)
					{
						if (_ynChunk != nullptr)
						{
							coordinates.y = CHUNK_SIZE - 1;
							if (ynChunk.voxels[coordinates.x][coordinates.y][coordinates.z] == 0)
							{
								voxelSurface |= 0x20;
							}
						}
						else
						{
							voxelSurface |= 0x20;
						}
					}
					else
					{
						if (centralChunk.voxels[coordinates.x][coordinates.y][coordinates.z] == 0)
						{
							voxelSurface |= 0x20;
						}
					}

					//oZ positive
					coordinates = { i, j, k + 1 };
					if (coordinates.z == CHUNK_SIZE)
					{
						if (_zpChunk != nullptr)
						{
							coordinates.z = 0;
							if (zpChunk.voxels[coordinates.x][coordinates.y][coordinates.z] == 0)
							{
								voxelSurface |= 0x40;
							}
						}
						else
						{
							voxelSurface |= 0x40;
						}
					}
					else
					{
						if (centralChunk.voxels[coordinates.x][coordinates.y][coordinates.z] == 0)
						{
							voxelSurface |= 0x40;
						}
					}

					//oZ negative
					coordinates = { i, j, k - 1 };
					if (coordinates.z == -1)
					{
						if (_znChunk != nullptr)
						{
							coordinates.z = CHUNK_SIZE - 1;
							if (znChunk.voxels[coordinates.x][coordinates.y][coordinates.z] == 0)
							{
								voxelSurface |= 0x80;
							}
						}
						else
						{
							voxelSurface |= 0x80;
						}
					}
					else
					{
						if (centralChunk.voxels[coordinates.x][coordinates.y][coordinates.z] == 0)
						{
							voxelSurface |= 0x80;
						}
					}

					//adding voxel surface
					if ((voxelSurface & 0xFC) != 0)
					{
						surface.push_back({ i + (j << 4) + (k << 8), voxelSurface + (voxelColor << 8) });
					}
				}
			}
		}
	}
}