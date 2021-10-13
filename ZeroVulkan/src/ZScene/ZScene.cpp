#include <assert.h>
#include "ZScene.h"
#include "ZMesh/ZMesh.h"
#include "Window/window.h"
#include "ZObject/ZObject.h"
#include "ZRenderer/ZRenderer.h"
#include "ZShaderSet/ZShaderSet.h"

namespace ZeroVulkan {
    static std::vector<ZScene*> scenes;
    static size_t currenIdx = 0;

    ZScene& ZScene::current() {
        assert(currenIdx < scenes.size());
        return *scenes[currenIdx];
    }
    
    ZScene::ZScene() {
        setView(ZeroVulkan::vec3(-2.f, 1.f, 0.7f), ZeroVulkan::vec3(0.f, 0.f, 0.f));
        updateProj();
    }

    size_t ZScene::getSceneCount() {
        return scenes.size();
    }
    
    ZScene::~ZScene() {
        objects.clear();
        binds.clear();

        printf("Destroyed a ZScene\n");
    }

    void ZScene::clear() {
        for (ZScene* scene : scenes)
            delete scene;
    }
 
    void ZScene::add() {
        if (!scenes.size()) 
            currenIdx = 0;

        scenes.push_back(this);
        puts("added a scene");
    }
    
    ZObject& ZScene::createObject(ZMesh& mesh) {
        objects.emplace_back(mesh);
        objects.back().addToScene(this);
        ZRenderer::record();
        return objects.back();
    }
    
    ZObject& ZScene::createObject(ZShaderSet& shaders, ZMesh& mesh) {
        objects.emplace_back(shaders, mesh);
        objects.back().addToScene(this);
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

    void ZScene::postUpdate(float dt) {
        for (ZObject& obj : objects)
            obj.update(dt);
    }

    void ZScene::addBindFunction(std::function<void(VkCommandBuffer&)> func) {
        binds.push_back(func);
    }

    
    void ZScene::bind(VkCommandBuffer& cmdBuffer) {
        for (auto bind : binds)
            bind(cmdBuffer);
    }
}
