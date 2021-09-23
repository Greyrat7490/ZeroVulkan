#include "Scene.h"

namespace ZeroVulkan::Scene {
    mat4 view;
 
    const mat4 getView() { return view; }
    
    void setView(vec3 origin, vec3 lookAtPos) {
        vec3 direction = normal(origin - lookAtPos);
        vec3 right = normal( cross( vec3(0.f, 0.f, 1.f), direction ) );
        vec3 up = cross(direction, right);

        view[0] =vec4(right[0], up[0], direction[0], 0);
        view[1] =vec4(right[1], up[1], direction[1], 0);
        view[2] =vec4(right[2], up[2], direction[2], 0);
        view[3] =vec4(-dot(right, origin), -dot(up, origin), -dot(direction, origin), 1);
    }
}
