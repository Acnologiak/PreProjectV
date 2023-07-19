#pragma once

#include <glm/glm.hpp>

#include "BaseValues.h"

using glm::u8;
using glm::u16;
using glm::u32;


struct VoxObjChunk8Bit
{
	u32 chunkVersion;
	u8 voxels[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
};