#ifndef ZLOOK_AT_CAM_H_
#define ZLOOK_AT_CAM_H_

#include "ZCamera/ZCamera.h"

namespace ZeroVulkan 
{
    class ZLookAtCam 
        : public ZCamera
    {
    public:
        ZLookAtCam();
        virtual void updateView() override;

        void lookAt(vec3 pos);
        
    private:
        vec3 m_lookAtPos;
    };
}

#endif // ZLOOK_AT_CAM_H_
