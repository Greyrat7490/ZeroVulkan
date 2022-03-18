#ifndef ZSANDBOX_CAM_H_
#define ZSANDBOX_CAM_H_

#include "ZCamera/ZFirstPersonCam/ZFirstPersonCam.h"

namespace ZeroVulkan
{
    class ZSandboxCam
        : public ZFirstPersonCam
    {
    public:
        void update(float dt);

        float moveSens = 2.f;
        float rotSens = 60.f;
        float zoomSens = 60.f;
    };
}

#endif // ZSANDBOX_CAM_H_
