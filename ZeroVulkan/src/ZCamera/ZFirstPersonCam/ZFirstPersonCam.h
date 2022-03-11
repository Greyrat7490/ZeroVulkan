#ifndef ZFIRST_PERSON_CAM_H_
#define ZFIRST_PERSON_CAM_H_

#include "ZCamera/ZCamera.h"

namespace ZeroVulkan
{
    class ZFirstPersonCam 
        : public ZCamera
    {
    public:
        ZFirstPersonCam();
        
        virtual void updateView() override;
    };
}

#endif // ZFIRST_PERSON_CAM_H_
