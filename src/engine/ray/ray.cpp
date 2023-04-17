#include "ray.h"

using namespace Engine;

Ray::Ray(glm::vec3 pos, glm::vec3 direction, float magnitude)
	: m_pos(pos)
	, m_direction(direction * magnitude)
{}
