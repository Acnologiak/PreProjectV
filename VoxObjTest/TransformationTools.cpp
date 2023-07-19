#include "TransformationTools.h"


size_t I16vec3ToSizeT(i16vec3 _coordinates)
{
	size_t result;
	memcpy(&result, &_coordinates, 6);
	return result;
}

i16vec3 SizeTToI16vec3(size_t _key)
{
	i16vec3 result;
	memcpy(&result, &_key, 6);
	return result;
}

u8vec3 GetChunkInternalCoordinates(const i32vec3& _coordinates)
{
	return i16vec3{ _coordinates.x & 0xF, _coordinates.y & 0xF, _coordinates.z & 0xF };
}

i16vec3 GetChunkExternalCoordinates(const i32vec3& _coordinates)
{
	return i16vec3{ _coordinates.x >> 4, _coordinates.y >> 4, _coordinates.z >> 4 };
}
