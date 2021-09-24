#ifndef ZOBJECT_H_
#define ZOBJECT_H_

#include "ZShaders/ZShaders.h"
#include "ZMesh/ZMesh.h"

namespace ZeroVulkan {
    class ZObject {
    public:
        ZObject();
        ZObject(const ZShaders& shaders, const ZMesh& mesh);
 
        void update(float dt);

        void setShaders(const ZShaders& shaders); 
        void setMesh(const ZMesh& mesh); 
        void bind();
        
        // TODO only temporary public to private later needs better update function for uniform
        ZShaders shaders;
    private:
        ZMesh mesh;
        bool binded = false;
    };
}

#endif // ZOBJECT_H_
