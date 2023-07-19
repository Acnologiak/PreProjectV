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
