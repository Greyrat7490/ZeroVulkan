#ifndef SCENE_H_
#define SCENE_H_

#include "types.h"

namespace ZeroVulkan::Scene {
    void setView(vec3 orgin, vec3 lookAtPos);
    const mat4 getView();
}

#endif // SCENE_H_
