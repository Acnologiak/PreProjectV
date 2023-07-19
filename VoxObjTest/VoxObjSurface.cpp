#include "VoxObjSurface.h"


MiniVoxChunkSpace::MiniVoxChunkSpace(shared_ptr<VoxObjChunk> _centralChunk, shared_ptr<VoxObjChunk> _xpChunk, shared_ptr<VoxObjChunk> _xnChunk, shared_ptr<VoxObjChunk> _ypChunk, shared_ptr<VoxObjChunk> _ynChunk, shared_ptr<VoxObjChunk> _zpChunk, shared_ptr<VoxObjChunk> _znChunk)
{
    centralChunk = _centralChunk.get();

    if (_xpChunk != nullptr)
    {
        xpChunk = _xpChunk.get();
    }
    if (_xnChunk != nullptr)
    {
        xnChunk = _xnChunk.get();
    }
    if (_ypChunk != nullptr)
    {
        ypChunk = _ypChunk.get();
    }
    if (_ynChunk != nullptr)
    {
        ynChunk = _ynChunk.get();
    }
    if (_zpChunk != nullptr)
    {
        zpChunk = _zpChunk.get();
    }
    if (_znChunk != nullptr)
    {
        znChunk = _znChunk.get();
    }
}

u16 MiniVoxChunkSpace::GetVoxel(const i16vec3& _coordinates)
{
    i16vec3 coordinates;
    u8 surface{ 0 };
    u8 color{ 0 };

    color = centralChunk->GetVoxel(_coordinates);

    if (color == 0)
    {
        return 0x1;
    }
    else
    {
        //oX positive
        coordinates = _coordinates;
        coordinates.x += 1;
        if (coordinates.x == CHUNK_SIZE)
        {
            if (xpChunk != nullptr)
            {
                coordinates.x = 0;
                if (xpChunk->GetVoxel(coordinates) == 0)
                {
                    surface |= 0x4;
                }
            }
        }
        else
        {
            if (centralChunk->GetVoxel(coordinates) == 0)
            {
                surface |= 0x4;
            }
        }

        //oX negative
        coordinates = _coordinates;
        coordinates.x -= 1;
        if (coordinates.x == -1)
        {
            if (xnChunk != nullptr)
            {
                coordinates.x = CHUNK_SIZE - 1;
                if (xnChunk->GetVoxel(coordinates) == 0)
                {
                    surface |= 0x8;
                }
            }
        }
        else
        {
            if (centralChunk->GetVoxel(coordinates) == 0)
            {
                surface |= 0x8;
            }
        }

        //oY positive
        coordinates = _coordinates;
        coordinates.y += 1;
        if (coordinates.y == CHUNK_SIZE)
        {
            if (ypChunk != nullptr)
            {
                coordinates.y = 0;
                if (ypChunk->GetVoxel(coordinates) == 0)
                {
                    surface |= 0x10;
                }
            }
        }
        else
        {
            if (centralChunk->GetVoxel(coordinates) == 0)
            {
                surface |= 0x10;
            }
        }

        //oY negative
        coordinates = _coordinates;
        coordinates.y -= 1;
        if (coordinates.y == -1)
        {
            if (ynChunk != nullptr)
            {
                coordinates.y = CHUNK_SIZE - 1;
                if (ynChunk->GetVoxel(coordinates) == 0)
                {
                    surface |= 0x20;
                }
            }
        }
        else
        {
            if (centralChunk->GetVoxel(coordinates) == 0)
            {
                surface |= 0x20;
            }
        }

        //oZ positive
        coordinates = _coordinates;
        coordinates.z += 1;
        if (coordinates.z == CHUNK_SIZE)
        {
            if (zpChunk != nullptr)
            {
                coordinates.z = 0;
                if (zpChunk->GetVoxel(coordinates) == 0)
                {
                    surface |= 0x40;
                }
            }
        }
        else
        {
            if (centralChunk->GetVoxel(coordinates) == 0)
            {
                surface |= 0x40;
            }
        }

        //oZ negative
        coordinates = _coordinates;
        coordinates.z -= 1;
        if (coordinates.z == -1)
        {
            if (znChunk != nullptr)
            {
                coordinates.z = CHUNK_SIZE - 1;
                if (znChunk->GetVoxel(coordinates) == 0)
                {
                    surface |= 0x80;
                }
            }
        }
        else
        {
            if (centralChunk->GetVoxel(coordinates) == 0)
            {
                surface |= 0x80;
            }
        }

        if ((surface & 0xFC) != 0)
        {
            return surface + (color << 8);
        }
        else
        {
            return 0x1;
        }
    }
}

shared_ptr<VoxObjChunkSurface> VoxObjChunkToVoxObjChunkSurface(MiniVoxChunkSpace& _space)
{
    shared_ptr<VoxObjChunkSurface> result = make_shared<VoxObjChunkSurface>();

    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int y = 0; y < CHUNK_SIZE; y++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                //std::cout << x << " " << y << " " << z << std::endl;
                u16 voxelSurface = _space.GetVoxel({ x, y, z });
                if (voxelSurface != 0x1)
                {
                    result->surface.push_back(make_pair(i16vec3{ x, y, z }, voxelSurface));
                }
            }
        }
    }

    return result;
}

shared_ptr<ObjFile> GetObj(shared_ptr<VoxObjSurface> _voxObjSurface)
{
    shared_ptr<ObjFile> objFile = make_shared<ObjFile>();

    size_t poinNumber{ 0 };
    for (const auto& i : _voxObjSurface->surfaces)
    {
        for (const auto& j : i.second->surface)
        {
            bool boolCube[2][2][2];
            
            u8 color = j.second >> 8;
            std::cout << (size_t)color << std::endl;
        }
    }
    return objFile;
}

VoxObjSurface::VoxObjSurface()
{
}

VoxObjSurface::VoxObjSurface(const VoxObj& _voxObj)
{
    for (const auto& i : _voxObj.voxObjChunks)
    {
        shared_ptr<VoxObjChunk> _xpChunk;
        shared_ptr<VoxObjChunk> _xnChunk;
        shared_ptr<VoxObjChunk> _ypChunk;
        shared_ptr<VoxObjChunk> _ynChunk;
        shared_ptr<VoxObjChunk> _zpChunk;
        shared_ptr<VoxObjChunk> _znChunk;

        i16vec3 chunkCoordinates = SizeTToI16vec3(i.first);

        //std::cout << "Chunk " << chunkCoordinates.x << " " << chunkCoordinates.y << " " << chunkCoordinates.z << std::endl;

        auto iterator = _voxObj.voxObjChunks.find(I16vec3ToSizeT(chunkCoordinates + i16vec3{1, 0, 0}));
        if (iterator == _voxObj.voxObjChunks.end())
        {
            _xpChunk = nullptr;
        }
        else
        {
            _xpChunk = iterator->second;
        }

        iterator = _voxObj.voxObjChunks.find(I16vec3ToSizeT(chunkCoordinates - i16vec3{1, 0, 0}));
        if (iterator == _voxObj.voxObjChunks.end())
        {
            _xnChunk = nullptr;
        }
        else
        {
            _xnChunk = iterator->second;
        }

        iterator = _voxObj.voxObjChunks.find(I16vec3ToSizeT(chunkCoordinates + i16vec3{0, 1, 0}));
        if (iterator == _voxObj.voxObjChunks.end())
        {
            _ypChunk = nullptr;
        }
        else
        {
            _ypChunk = iterator->second;
        }

        iterator = _voxObj.voxObjChunks.find(I16vec3ToSizeT(chunkCoordinates - i16vec3{0, 1, 0}));
        if (iterator == _voxObj.voxObjChunks.end())
        {
            _ynChunk = nullptr;
        }
        else
        {
            _ynChunk = iterator->second;
        }

        iterator = _voxObj.voxObjChunks.find(I16vec3ToSizeT(chunkCoordinates + i16vec3{0, 0, 1}));
        if (iterator == _voxObj.voxObjChunks.end())
        {
            _zpChunk = nullptr;
        }
        else
        {
            _zpChunk = iterator->second;
        }

        iterator = _voxObj.voxObjChunks.find(I16vec3ToSizeT(chunkCoordinates - i16vec3{0, 0, 1}));
        if (iterator == _voxObj.voxObjChunks.end())
        {
            _znChunk = nullptr;
        }
        else
        {
            _znChunk = iterator->second;
        }

        MiniVoxChunkSpace miniVoxChunkSpace{
            i.second, _xpChunk, _xnChunk, _ypChunk, _ynChunk, _zpChunk, _znChunk
        };
        shared_ptr<VoxObjChunkSurface> chunkSurface = VoxObjChunkToVoxObjChunkSurface(miniVoxChunkSpace);

        if (chunkSurface->surface.size() != 0)
        {
            surfaces.emplace(i.first, chunkSurface);
        }
    }
}
