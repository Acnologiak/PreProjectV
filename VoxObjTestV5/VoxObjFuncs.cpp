#include "VoxObjFuncs.h"

bool PreProjectV::AddSphereToVoxObj8Bit(shared_ptr<VoxObj8Bit> _voxObj, const i32vec3& _position, size_t _radius, u8 _color)
{
	if (_voxObj != nullptr)
	{
		i32vec3 max{ _position + i32vec3{ _radius, _radius, _radius} }, min{ _position - i32vec3{ _radius, _radius,_radius } };
		int mRadius = _radius * _radius;

		for (int x = min.x; x <= max.x; x++)
		{
			for (int y = min.y; y <= max.y; y++)
			{
				for (int z = min.z; z <= max.z; z++)
				{
					if (pow(x - _position.x, 2) + pow(y - _position.y, 2) + pow(z - _position.z, 2) <= mRadius)
					{
						_voxObj->SetVoxel({ x, y, z }, _color);
					}
				}
			}
		}

		return true;
	}
	else
	{
		return false;
	}
}

bool PreProjectV::AddCubeToVoxObj8Bit(shared_ptr<VoxObj8Bit> _voxObj, const i32vec3& _position, size_t _size, u8 _color)
{
	if (_voxObj != nullptr)
	{
		size_t halfSize = _size / 2;
		i32vec3 max{ _position + i32vec3{ halfSize, halfSize, halfSize} }, min{ _position - i32vec3{ halfSize, halfSize, halfSize } };

		for (int x = min.x; x <= max.x; x++)
		{
			for (int y = min.y; y <= max.y; y++)
			{
				for (int z = min.z; z <= max.z; z++)
				{
					_voxObj->SetVoxel({ x, y, z }, _color);
				}
			}
		}

		return true;
	}
	else
	{
		return false;
	}
}
