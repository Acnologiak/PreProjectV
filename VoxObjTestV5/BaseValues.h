#pragma once

#define CHUNK_SIZE 16
#define PHYSICAL_VOXEL_SIZE 0.03125
#define PHYSICAL_CHUNK_SIZE 0.5


#include <map>
#include <memory>
#include <vector>
#include <string>
#include <fstream>

#include "glm/glm.hpp"

using std::shared_ptr;
using std::make_shared;
using std::string;
using std::map;
using std::ifstream;
using std::ofstream;
using std::ios;
using std::pair;
using std::vector;
using std::to_string;

using glm::i16vec3;
using glm::u8;
using glm::u16;
using glm::u8vec3;
using glm::i32vec3;
using glm::i32vec4;
using glm::f32vec3;
using glm::f32vec2;
using glm::u16vec2;
using glm::u16vec3;
using glm::vec3;