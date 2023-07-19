#pragma once

#include <glm/glm.hpp>


using glm::i16vec3;
using glm::u8vec3;
using glm::i16vec3;
using glm::i32vec3;


size_t I16vec3ToSizeT(i16vec3 _coordinates);

i16vec3 SizeTToI16vec3(size_t _key);

u8vec3 GetChunkInternalCoordinates(const i32vec3& _coordinates);

i16vec3 GetChunkExternalCoordinates(const i32vec3& _coordinates);
