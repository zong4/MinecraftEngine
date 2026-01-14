#pragma once

#include "../../Physics/BoundingBox.hpp"

namespace Engine
{

struct ColliderComponent
{
    bool IsTrigger = false;
    BoundingBox BBox;
    BoundingBox WorldBBox;
};

} // namespace Engine