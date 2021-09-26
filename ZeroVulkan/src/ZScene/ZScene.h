#ifndef SCENE_H_
#define SCENE_H_

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
        ~ZScene();
        
        static ZScene& current();
        static ZScene& create(); 
        static void clear();

        ZObject& createObject();
        
        void updateProj();
        void setView(vec3 orgin, vec3 lookAtPos);
        const mat4 getView() const { return view; }

        void update(float dt);
        void bind(VkCommandBuffer& cmdBuffer);
        void addBindFunction(std::function<void(VkCommandBuffer&)> func);
    private:
        std::vector<ZObject> objects;
        std::vector<std::function<void(VkCommandBuffer&)>> binds;

        mat4 proj;
        mat4 view;
    };
}

#endif // SCENE_H_
