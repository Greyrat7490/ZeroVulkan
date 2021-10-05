#ifndef ZOBJECT_H_
#define ZOBJECT_H_

#include "ZShaders/ZShaders.h"
#include "ZMesh/ZMesh.h"

namespace ZeroVulkan {
    class ZScene;
    
    class ZObject {
    public:
        ZObject();
        ZObject(ZShaders& shaders, ZMesh& mesh);
 
        void update(float dt);

        void addBindsToScene(ZScene* scene);
        
        void setShaders(const ZShaders& shaders); 
        void setMesh(const ZMesh& mesh); 

        // TODO only temporary public to private later needs better update function for uniform
        ZShaders shaders;
    private:
        ZMesh mesh;
    };
}

#endif // ZOBJECT_H_
