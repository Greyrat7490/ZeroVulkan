#ifndef ZSCENE_H_
#define ZSCENE_H_

#include <vector>
#include <functional>
#include <vulkan/vulkan_core.h>
#include "ZObject/ZObject.h"
#include "types.h"
#include "Vulkan/CommandBuffer.h"

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
        
        void postUpdate(float dt);
        
        ZObject& createObject();
        ZObject& createObject(ZShaders& shaders, ZMesh& mesh);
        
        void updateProj();
        void setView(vec3 orgin, vec3 lookAtPos);
        const mat4& getView() const { return view; }

        void bind(VkCommandBuffer& cmdBuffer);
        void addBindFunction(std::function<void(VkCommandBuffer&)> func);
    private:
        std::vector<ZObject> objects;
        std::vector<std::function<void(VkCommandBuffer&)>> binds;

        mat4 proj;
        mat4 view;

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
