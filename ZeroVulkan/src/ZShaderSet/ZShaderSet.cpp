#include <any>
#include <cmath>
#include <fstream>
#include <string>
#include <vulkan/vulkan_core.h>
#include "Vulkan/ComputeShader.h"
#include "Vulkan/Shader.h"
#include "Vulkan/Uniform.h"
#include "ZShaderSet.h"
#include "utils.h"

namespace ZeroVulkan
{
    void trimFile(std::string& file) {        
        // because of windows style EOL(\n\r)
        size_t pos = file.find('\r');
        while (pos != file.npos) {
            file.erase(pos, 1);
            pos = file.find('\r', pos);
        }
        
        pos = file.find("//");
        while (pos != file.npos) {
            size_t end = file.find('\n', pos) + 1;
            
            file.erase(pos, end - pos);
            pos = file.find("//", pos);
        }

        pos = file.find("/*");
        while (pos != file.npos) {
            size_t end = file.find("*/", pos) + 2;
            
            file.erase(pos, end - pos);
            pos = file.find("/*", pos);
        }
        
        pos = file.find('\n');
        while (pos != file.npos) {
            file.replace(pos, 1, " ");
            pos = file.find('\n', pos);
        }

        pos = file.find("\t");
        while (pos != file.npos) {
            file.erase(pos, 1);
            pos = file.find("\t", pos);
        }
        
        pos = file.find("  ");
        while (pos != file.npos) {
            file.erase(pos, 1);
            pos = file.find("  ", pos);
        }
    }
    
    void trimWord(std::string& word) {
        size_t pos = word.find(' ');
        while (pos != word.npos) {
            word.erase(pos, 1);
            pos = word.find(' ');
        }

        pos = word.find("\t");
        while (pos != word.npos) {
            word.erase(pos, 1);
            pos = word.find("\t", pos);
        }
    }
        
    bool ZShaderSet::addComponentByStr(ZUniform& uniform, const std::string& str) {
        static_assert(ZTYPE_COUNT == 5, "Exhaustive use of ZType (add new else if)");
        
        if      (str == "mat4")
                uniform.addComponent(ZType::MAT4);
        else if (str == "vec4")
                uniform.addComponent(ZType::VEC4);
        else if (str == "vec3")
                uniform.addComponent(ZType::VEC3);
        else if (str == "vec2")
                uniform.addComponent(ZType::VEC2);
        else if (str == "float")
                uniform.addComponent(ZType::FLOAT);
        else {
            printf("ERROR: type '%s' is not supported\n", str.c_str());
            return false;
        }
        
        return true;
    }
    
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
                
                descSetLayout.addBinding(binding, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
                uniforms.emplace_back();
                
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

                    if (addComponentByStr(uniforms.back(), type))
                        printf("  %s\n", type.c_str());
                }
                // ------------------------------------------
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

    
    ZShaderSet::ZShaderSet(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
    {
        printf("vertexShader: %s\n", vertexShaderPath.c_str());
        std::string absPath = pathToAbsolue(vertexShaderPath); 
        createShaderModule(absPath, &shaderModuleVert);
        parseVertShader(absPath);
        
        printf("fragmentShader: %s\n", fragmentShaderPath.c_str());
        absPath = pathToAbsolue(fragmentShaderPath); 
        createShaderModule(absPath, &shaderModuleFrag);
        parseFragShader(absPath);

        for (ZUniform& uniform : uniforms)
            uniform.create();
    }

    ZShaderSet::~ZShaderSet()
    {
        if (stencilBuffer)
            delete stencilBuffer;

        if (ready) {
            vkDestroyPipeline(ZDevice::getDevice(), pipeline, nullptr);
            vkDestroyPipelineLayout(ZDevice::getDevice(), pipelineLayout, nullptr);
        }

        vkDestroyShaderModule(ZDevice::getDevice(), shaderModuleVert, nullptr);
        vkDestroyShaderModule(ZDevice::getDevice(), shaderModuleFrag, nullptr);

        printf("destroyed ZShaders\n");
    }

    ZShaderSet::ZShaderSet(ZShaderSet&& source) {
        uniforms.swap(source.uniforms);
        stencilBuffer = source.stencilBuffer;
        pipeline = source.pipeline;
        pipelineLayout = source.pipelineLayout;
        descPool = source.descPool;
        descSetLayout = source.descSetLayout;
        descriptorSet = source.descriptorSet;
        vertexLayout = source.vertexLayout;
        shaderModuleVert = source.shaderModuleVert;
        shaderModuleFrag = source.shaderModuleFrag;

        ready = false;

        source.stencilBuffer = nullptr;
        source.pipeline = nullptr;
        source.pipelineLayout = nullptr;
        source.shaderModuleVert = nullptr;
        source.shaderModuleFrag = nullptr;

        puts("moved shaders");
    }

    ZShaderSet& ZShaderSet::operator=(ZShaderSet&& source) {
        uniforms.swap(source.uniforms);
        stencilBuffer = source.stencilBuffer;
        pipeline = source.pipeline;
        pipelineLayout = source.pipelineLayout;
        descPool = source.descPool;
        descSetLayout = source.descSetLayout;
        descriptorSet = source.descriptorSet;
        vertexLayout = source.vertexLayout;
        shaderModuleVert = source.shaderModuleVert;
        shaderModuleFrag = source.shaderModuleFrag;

        ready = false;

        source.stencilBuffer = nullptr;
        source.pipeline = nullptr;
        source.pipelineLayout = nullptr;
        source.shaderModuleVert = nullptr;
        source.shaderModuleFrag = nullptr;

        puts("moved shaders (=)");
        return *this;
    }


    ZUniform& ZShaderSet::getUniform(size_t index) {
        ZASSERT_FUNC(index < uniforms.size(), "ShaderSet has not enough uniforms");
        
        return uniforms[index];
    }       

    void ZShaderSet::create(bool debug, bool triangleTopology)
    {
        if (debug)
        {
            stencilBuffer = new ZStencilBuffer();
            descPool.addDescriptorLayout(stencilBuffer->getOutlineDescSetLayout());
            stencilBuffer->createDescSet(descPool.descriptorPool);
            stencilBuffer->createPipelines(&vertexLayout, shaderModuleVert, shaderModuleFrag, &descSetLayout.layout, 1);
        }


        if (!descSetLayout.getBindings().empty())
        {
            descSetLayout.create();
            descPool.addDescriptorLayout(&descSetLayout);

            if (debug)
                descPool.create(2);
            else
                descPool.create();

            descriptorSet.setLayout(&descSetLayout);

            // TODO: move in setLayout
            for (uint32_t i = 0; i < descSetLayout.getBindings().size(); i++)
            {
                switch(descSetLayout.getBindings()[i].descriptorType)
                {
                case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
                case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
                    descriptorSet.addDescriptorInfo(i, uniforms[i].getBufferInfo());
                    break;
                default:
                    printf("DescriptorTyp is not yet supported\n");
                    break;
                }
            }
            // -----------------------

            // TODO: move setLayout into create
            descriptorSet.create(descPool.descriptorPool);
            createPipelineLayout(pipelineLayout, &descSetLayout.layout, 1);
        }
        else 
        {
            if (debug)
            {
                stencilBuffer = new ZStencilBuffer();
                descPool.addDescriptorLayout(stencilBuffer->getOutlineDescSetLayout());
                descPool.create();
                createPipelineLayout(pipelineLayout, &descSetLayout.layout, 1);
            }
            else 
                createPipelineLayout(pipelineLayout, nullptr, 0);
        }


        if(triangleTopology)
        {
            createGraphicsPipeline(
                pipeline,
                pipelineLayout,
                shaderModuleVert,
                shaderModuleFrag,
                &vertexLayout
            );
        }
        else
        {
            createParticleGraphicsPipeline(
                pipeline,
                pipelineLayout,
                shaderModuleVert,
                shaderModuleFrag,
                &vertexLayout
            );
        }
        
        ready = true;
        puts("created shaders");
    }

    void ZShaderSet::setShader(const std::string& path, ZShaderType type)
    {
        static_assert(SHADER_TYPE_COUNT == 3, "Exhaustive use of ZShaderType (add cases)");
        
        switch(type)
        {
        case ZShaderType::VERTEX: {
            printf("vertexShader: %s\n", path.c_str());
            std::string absPath = pathToAbsolue(path); 
            createShaderModule(absPath, &shaderModuleVert);
            parseVertShader(absPath);
        }
        break;
        case ZShaderType::FRAGMENT: {
            printf("fragmentShader: %s\n", path.c_str());
            std::string absPath = pathToAbsolue(path); 
            createShaderModule(absPath, &shaderModuleFrag);
            parseVertShader(absPath);
        }
        break;
        // TODO: Compute Shader support
        case ZShaderType::COMPUTE:
            ZASSERT_FUNC(false, "Computer Shaders are not yet supported");
            break;
        default:
            puts("unknown ShaderType");
            break;
        }
    }
 
    void ZShaderSet::bind(VkCommandBuffer& cmdBuffer) {
        if (!ready)
            create(false);
            
        if (!descSetLayout.getBindings().empty())
            vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSet.descSet, 0, 0);

        vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
    }
}
