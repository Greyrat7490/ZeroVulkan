#ifndef ZSCENE_H_
#define ZSCENE_H_

#include <vector>
#include <functional>
#include "types.h"
#include "ZObject/ZObject.h"
#include "ZPrimitive/ZPrimitvie.h"

namespace ZeroVulkan {
    class ZScene {
    public:
        ZScene();
        virtual ~ZScene();
 
        template<typename ZSceneClass>
        static ZScene* create();
        static ZScene& current();
        static void clear();
        static size_t getSceneCount();

        virtual void start() = 0;
        virtual void update(float dt) = 0;
        virtual void end() = 0;
        
        void postUpdate();
        
        ZObject& createObject(ZMesh& mesh);
        ZObject& createObject(ZShaderSet& shaders, ZMesh& mesh);
        
        ZRect& createRect(vec2 pos, float width, float height, vec4 color);
        
        void updateProj();
        void setView(vec3 origin, vec3 lookAtPos);

        const mat4& getView() const { return view; }
        const mat4& getProjection() const { return proj; }

        void bind(VkCommandBuffer& cmdBuffer);
    protected:
        mat4 proj;
        mat4 view;
    private:
        std::vector<ZObject> objects;
        std::vector<ZRect> rects;
        void add();
    };

    template<typename ZSceneClass>
    ZScene* ZScene::create() {
        ZSceneClass* scene = new ZSceneClass;
        scene->add();
        return scene;
    }
}

#endif // ZSCENE_H_
