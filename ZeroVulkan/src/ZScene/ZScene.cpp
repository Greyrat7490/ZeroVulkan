#include "ZScene.h"
#include "Window/window.h"
#include "ZRenderer/ZRenderer.h"

namespace ZeroVulkan {
    static std::vector<ZScene> scenes;
    static size_t currenIdx;

    ZScene& ZScene::current() {
        return scenes[currenIdx];
    }
    
    ZScene::ZScene() {
        setView(ZeroVulkan::vec3(-2.f, 1.f, 0.7f), ZeroVulkan::vec3(0.f, 0.f, 0.f));
        updateProj();
    }

    ZScene::~ZScene() {
        objects.clear();
        binds.clear();

        printf("Destroyed a ZScene");
    }

    void ZScene::clear() {
        scenes.clear();       
    }
    
    ZScene& ZScene::create() {
        if (!scenes.size()) 
            currenIdx = 0;
            
        scenes.emplace_back();
        return scenes.back();
    }
    
    ZObject& ZScene::createObject() {
        objects.emplace_back(this);
        ZRenderer::record();
        return objects.back();
    }
    
    void ZScene::updateProj() {
        vec2 ws = ZWindow::getSize();
        
        float aspect = ws[0] / ws[1];
        float fovY = 70.f;
        float farValue = 30.f;
        float nearValue = 0.001f;
        
        proj[0][0] = 1.f/ (aspect * std::tan(radian(fovY / 2.f)));
        proj[1][1] = -1.f / std::tan(radian(fovY / 2.f));// -1.f to flip y_axis
        proj[2][2] = farValue / (nearValue - farValue);
        proj[2][3] = -1.f;
        proj[3][2] = (farValue * nearValue) / (nearValue - farValue);
        proj[3][3] = 0.f;
    }
    
    void ZScene::setView(vec3 origin, vec3 lookAtPos) {
        vec3 direction = normal(origin - lookAtPos);
        vec3 right = normal( cross( vec3(0.f, 0.f, 1.f), direction ) );
        vec3 up = cross(direction, right);

        view[0] =vec4(right[0], up[0], direction[0], 0);
        view[1] =vec4(right[1], up[1], direction[1], 0);
        view[2] =vec4(right[2], up[2], direction[2], 0);
        view[3] =vec4(-dot(right, origin), -dot(up, origin), -dot(direction, origin), 1);
    }

    void ZScene::update(float dt) {
        for (ZObject& obj : objects) {
            obj.update(dt);
            obj.shaders.uniform.update(0, proj, view, ZeroVulkan::mat4(1.f), ZeroVulkan::vec3(1.f, 1.f, 3.f));
        }
    }

    void ZScene::addBindFunction(std::function<void(VkCommandBuffer&)> func) {
        binds.push_back(func);
    }

    
    void ZScene::bind(VkCommandBuffer& cmdBuffer) {
        for (auto bind : binds)
            bind(cmdBuffer);
    }
}
