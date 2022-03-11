#ifndef ZSCENE_H_
#define ZSCENE_H_

#include <cstdint>
#include <vector>
#include "types.h"

#include "ZCamera/ZCamera.h"
#include "ZCamera/ZLookAtCam/ZLookAtCam.h"
#include "ZCamera/ZFirstPersonCam/ZFirstPersonCam.h"

#include "ZObject/ZObject.h"
#include "ZPrimitive/ZPrimitvie.h"
#include "ZComputeShader/ZComputeShader.h"

namespace ZeroVulkan {
    class ZScene {
    public:
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

        ZFirstPersonCam* createFirstPersonCam();
        ZLookAtCam* createLookAtCam();

        ZObject& createObject(ZMesh& mesh);
        ZObject& createObject(ZShaderSet& shaders, ZMesh& mesh);

        ZRect& createRect(vec2 pos, float width, float height, vec4 color);

        ZShaderSet& createShaderSet(const std::string& vertShaderPath, const std::string& fragShaderPath);

        void setAspect();
        void updateProj();

        const mat4& getView() const { return cams[m_mainCam]->getView(); }
        const mat4& getProjection() const { return cams[m_mainCam]->getProj(); }

        bool hasCam() const { return !cams.empty(); }


        void bind(VkCommandBuffer& cmdBuffer);
        void buildComputeShaders();
        void submitComputeShaders();
    private:
        std::vector<ZCamera*> cams;
        std::vector<ZShaderSet> shaders;
        std::vector<ZObject> objects;
        std::vector<ZRect> rects;

        uint32_t m_mainCam = 0;
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
