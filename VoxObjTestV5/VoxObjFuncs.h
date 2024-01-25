#pragma once

#include "VoxObj.h"

namespace PreProjectV
{
	bool AddSphereToVoxObj8Bit(
		shared_ptr<VoxObj8Bit> _voxObj,
		const i32vec3& _position,
		size_t _radius,
		u8 _color
	);

	bool AddCubeToVoxObj8Bit(
		shared_ptr<VoxObj8Bit> _voxObj,
		const i32vec3& _position,
		size_t _size,
		u8 _color
	);
}
