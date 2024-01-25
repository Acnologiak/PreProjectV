#pragma once

#include "VoxObj.h"

namespace PreProjectV
{
	class VoxObjChunkSurface8Bit;

	class VoxObjSurface8Bit;

	//--------------------------------------------------------------------------------------------

	class VoxObjChunkSurface8Bit
	{
	public:
		vector<u16vec2> surface;

		VoxObjChunkSurface8Bit();
		VoxObjChunkSurface8Bit(
			shared_ptr<VoxObjChunk8Bit> _centralChunk,
			shared_ptr<VoxObjChunk8Bit> _xpChunk = nullptr,
			shared_ptr<VoxObjChunk8Bit> _xnChunk = nullptr,
			shared_ptr<VoxObjChunk8Bit> _ypChunk = nullptr,
			shared_ptr<VoxObjChunk8Bit> _ynChunk = nullptr,
			shared_ptr<VoxObjChunk8Bit> _zpChunk = nullptr,
			shared_ptr<VoxObjChunk8Bit> _znChunk = nullptr
		);
	};

	class VoxObjSurface8Bit
	{
	public:
		map<size_t, shared_ptr<VoxObjChunkSurface8Bit>> surfaces;

		VoxObjSurface8Bit();
		VoxObjSurface8Bit(shared_ptr<VoxObj8Bit> _voxObj);
	};
}