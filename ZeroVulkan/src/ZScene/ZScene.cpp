#include "ZScene.h"

#include "utils.h"

#include "Window/window.h"
#include "ZRenderer/ZRenderer.h"

#include "ZMesh/ZMesh.h"
#include "ZObject/ZObject.h"
#include "ZShaderSet/ZShaderSet.h"


namespace ZeroVulkan {
    static std::vector<ZScene*> scenes;
    static size_t currenIdx = 0;

    ZScene& ZScene::current() {
        ZASSERT_FUNC(currenIdx < scenes.size(), "could not get current scene");
        return *scenes[currenIdx];
    }

    size_t ZScene::getSceneCount() {
        return scenes.size();
    }

    ZScene::~ZScene() {
        objects.clear();
        rects.clear();
        shaders.clear();

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

    void ZScene::updateProj() {
        ZASSERT_FUNC(hasCam(), "current scene has no cameras");
        cams[m_mainCam]->updateProj();
    }

    void ZScene::setAspect() {
        ZASSERT_FUNC(hasCam(), "current scene has no cameras");
        cams[m_mainCam]->setAspect();
    }

    ZLookAtCam* ZScene::createLookAtCam() {
        ZLookAtCam* cam = new ZLookAtCam();
        cams.push_back(cam);
        return cam;
    }
    ZFirstPersonCam* ZScene::createFirstPersonCam() {
        ZFirstPersonCam* cam = new ZFirstPersonCam();
        cams.push_back(cam);
        return cam;
    }

    ZObject& ZScene::createObject(ZMesh& mesh) {
        objects.emplace_back(mesh);
        ZRenderer::record();
        return objects.back();
    }
    ZObject& ZScene::createObject(ZShaderSet& shaders, ZMesh& mesh) {
        objects.emplace_back(shaders, mesh);
        ZRenderer::record();
        return objects.back();
    }

    ZRect& ZScene::createRect(vec2 pos, float width, float height, vec4 color) {
        rects.emplace_back(pos, width, height, color);
        ZRenderer::record();
        return rects.back();
    }

    ZShaderSet& ZScene::createShaderSet(const std::string& vertShaderPath, const std::string& fragShaderPath) {
        shaders.emplace_back(vertShaderPath, fragShaderPath);
        ZRenderer::record();
        return shaders.back();
    }

    void ZScene::postUpdate() {
        for (ZObject& obj : objects)
            obj.update(this);
    }

    void ZScene::bind(VkCommandBuffer& cmdBuffer) {
        for (ZRect& rect : rects)
            rect.bind(cmdBuffer);

        for (ZObject& obj : objects)
            obj.bind(cmdBuffer);

        for (ZShaderSet& shader : shaders)
            shader.bind(cmdBuffer);
    }


    void ZScene::buildComputeShaders() {
        for (ZShaderSet& shader : shaders)
            shader.buildComputeShader();
    }

    void ZScene::submitComputeShaders() {
        for (ZShaderSet& shader : shaders)
            shader.submitComputeShader();
    }
}
