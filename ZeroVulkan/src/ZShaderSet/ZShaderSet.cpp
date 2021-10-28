#include <any>
#include <cmath>
#include <fstream>
#include <string>
#include <vulkan/vulkan_core.h>
#include "Vulkan/ComputeShader.h"
#include "Vulkan/Device.h"
#include "Vulkan/Shader.h"
#include "Vulkan/Uniform.h"
#include "ZRenderer/ZRenderer.h"
#include "ZShaderSet.h"
#include "utils.h"

namespace ZeroVulkan
{
    void ZShaderSet::parseUniforms(const std::string& file) {
        size_t bpos = file.find("binding");
        while (bpos != file.npos) {

            // only support for uniform bindings so far
            // find uniform binding
            size_t pos = file.find("uniform ", bpos);
            if (pos != file.npos) {

                // add binding ------------------------------
                std::string bindingStr = file.substr(file.find('=', bpos) + 1, file.find(')', bpos) - bpos);
                trimWord(bindingStr);
                size_t binding = std::stoll(bindingStr);
                
                uniforms.emplace_back();
                pipeline.addBinding(binding, uniforms.back().getBufferInfo(), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
                
                printf("(binding = %zu) uniform\n", binding);
                // ------------------------------------------

                // add components ---------------------------
                pos = file.find('{', pos + 7) + 1;
                
                // skip whitespace at the beginning
                if (file.at(pos) == ' ')
                    pos++;
                
                std::string structDef = file.substr(pos, file.find('}', pos) - pos);
                
                pos = 0;
                size_t pos2 = structDef.find(' ', pos);
                while (pos2 != structDef.npos) {
                    std::string type = structDef.substr(pos, pos2 - pos);
                    
                    pos = structDef.find(';', pos) + 1;
                    if (structDef.at(pos) == ' ')
                        pos++;
                    pos2 = structDef.find(' ', pos);

                    if (uniforms.back().addComponentByStr(type))
                        printf("  %s\n", type.c_str());
                }
                // ------------------------------------------
                
                uniforms.back().create();
            }

            bpos = file.find("binding", bpos + 7);
        }
    }

    void ZShaderSet::parseVertexAttr(const std::string& file) {
        std::vector<std::pair<size_t, std::string>> locations;

        size_t lpos = file.find("location");
        while (lpos != file.npos) {

            size_t pos = file.find("in ", lpos);
            if (pos != file.npos) {
                pos += 3;
                std::string type = file.substr(pos, file.find(" ", pos) - pos);
                
                std::string location = file.substr(file.find('=', lpos) + 1, file.find(')', lpos) - lpos);
                trimWord(location);
                locations.push_back(std::make_pair(std::stoll(location), type));

                printf("(location = %zu) '%s'\n", locations.back().first, locations.back().second.c_str());
            }

            lpos = file.find("location", lpos + 8);
        }
        

        static_assert(ZTYPE_COUNT == 5, "Exhaustive use of ZType (add new else if)");
        
        for (auto& l : locations) {
            if      (l.second == "mat4")
                vertexLayout.addLocation(l.first, ZType::MAT4);
            else if (l.second == "vec4")
                vertexLayout.addLocation(l.first, ZType::VEC4);
            else if (l.second == "vec3")
                vertexLayout.addLocation(l.first, ZType::VEC3);
            else if (l.second == "vec2")
                vertexLayout.addLocation(l.first, ZType::VEC2);
            else if (l.second == "float")
                vertexLayout.addLocation(l.first, ZType::FLOAT);
            else
                printf("ERROR: %s isn't a supported type\n", l.second.c_str());
        }
        
        vertexLayout.createBinding();
    }
    
    void ZShaderSet::parseVertShader(const std::string& path) {
        std::string file = readFile(path);
        trimFile(file);

        parseVertexAttr(file);
        parseUniforms(file);
    }

    void ZShaderSet::parseFragShader(const std::string& path) {
        std::string file = readFile(path);
        trimFile(file);

        parseUniforms(file);
    }


    void ZShaderSet::parseCompShader(const std::string& path) {
        std::string file = readFile(path);
        trimFile(file);
        
        parseUniforms(file);
    }

    
    ZShaderSet::ZShaderSet(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
    {
        std::string absPath = pathToAbsolue(vertexShaderPath); 
        createShaderModule(absPath, &m_vertShader);
        parseVertShader(absPath);
        
        absPath = pathToAbsolue(fragmentShaderPath); 
        createShaderModule(absPath, &m_fragShader);
        parseFragShader(absPath);
    }

    ZShaderSet::~ZShaderSet()
    {
        if (m_computeShader)
            delete m_computeShader;
            
        vkDestroyShaderModule(ZDevice::getDevice(), m_vertShader, nullptr);
        vkDestroyShaderModule(ZDevice::getDevice(), m_fragShader, nullptr);

        uniforms.clear();
        
        printf("destroyed ZShaders\n");
    }


    void ZShaderSet::setComputeShader(ZComputeShader* computeShader) {
        m_computeShader = computeShader; 
        ZRenderer::record();
    }

    ZShaderSet::ZShaderSet(ZShaderSet&& source) {
        uniforms.swap(source.uniforms);
        // stencilBuffer = source.stencilBuffer;
        m_computeShader = source.m_computeShader;
        pipeline = source.pipeline;
        descPool = source.descPool;
        vertexLayout = source.vertexLayout;
        m_vertShader = source.m_vertShader;
        m_fragShader = source.m_fragShader;

        ready = false;

        // source.stencilBuffer = nullptr;
        source.m_computeShader = nullptr;
        source.m_vertShader = nullptr;
        source.m_fragShader = nullptr;

        puts("moved shaders");
    }

    ZShaderSet& ZShaderSet::operator=(ZShaderSet&& source) {
        uniforms.swap(source.uniforms);
        // stencilBuffer = source.stencilBuffer;
        m_computeShader = source.m_computeShader;
        pipeline = source.pipeline;
        descPool = source.descPool;
        vertexLayout = source.vertexLayout;
        m_vertShader = source.m_vertShader;
        m_fragShader = source.m_fragShader;

        ready = false;

        // source.stencilBuffer = nullptr;
        source.m_computeShader = nullptr;
        source.m_vertShader = nullptr;
        source.m_fragShader = nullptr;

        puts("moved shaders (=)");
        return *this;
    }


    ZUniform& ZShaderSet::getUniform(size_t index) {
        ZASSERT_FUNC(index < uniforms.size(), "ShaderSet has not enough uniforms");
        
        return uniforms[index];
    }       

    void ZShaderSet::create()
    {
        descPool.addDescriptorLayout(pipeline.getDescLayout());
        descPool.create();

        pipeline.setShaders(m_vertShader, m_fragShader);
        pipeline.setVertexLayout(&vertexLayout);
        pipeline.create();
        pipeline.createDescSet(descPool);
        
        ready = true;
        puts("created shaders");
    }

    void ZShaderSet::setShader(const std::string& path, ZShaderType type)
    {
        static_assert(SHADER_TYPE_COUNT == 3, "Exhaustive use of ZShaderType (add cases)");
        
        switch(type)
        {
        case ZShaderType::VERTEX: {
            std::string absPath = pathToAbsolue(path); 
            createShaderModule(absPath, &m_vertShader);
            parseVertShader(absPath);
        }
        break;
        case ZShaderType::FRAGMENT: {
            std::string absPath = pathToAbsolue(path); 
            createShaderModule(absPath, &m_fragShader);
            parseVertShader(absPath);
        }
        break;
        // TODO: Compute Shader support
        case ZShaderType::COMPUTE:
            ZASSERT_FUNC(false, "Computer Shaders are not yet supported");
            break;
        default:
            ZASSERT_FUNC(false, "unknown ShaderType");
            break;
        }
    }

    void ZShaderSet::bind(VkCommandBuffer& cmdBuffer) {
        if (!ready)
            create();
            
        pipeline.bind(cmdBuffer);

        if (m_computeShader)
            m_computeShader->bind(cmdBuffer);
    }


    void ZShaderSet::buildComputeShader() {
        if (m_computeShader)
            m_computeShader->buildCommandBuffer();
    }

    void ZShaderSet::submitComputeShader() {
        if (m_computeShader)
            m_computeShader->submit(ZDevice::getQueue());
    }
}
