#include "Parser.h"
#include "utils.h"

#include "Vulkan/Vertex.h"
#include "Vulkan/Pipeline.h"
#include "Vulkan/Uniform.h"
#include "Vulkan/StorageBuffer.h"

#include "ZComputeShader/ZComputeShader.h"


namespace ZeroVulkan {
    void parseVertexAttr(ZVertexLayout* vertexLayout, const std::string& file) {
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
                vertexLayout->addLocation(l.first, ZType::MAT4);
            else if (l.second == "vec4")
                vertexLayout->addLocation(l.first, ZType::VEC4);
            else if (l.second == "vec3")
                vertexLayout->addLocation(l.first, ZType::VEC3);
            else if (l.second == "vec2")
                vertexLayout->addLocation(l.first, ZType::VEC2);
            else if (l.second == "float")
                vertexLayout->addLocation(l.first, ZType::FLOAT);
            else
                printf("ERROR: %s isn't a supported type\n", l.second.c_str());
        }
        
        vertexLayout->createBinding();
    }

    void parseStructDef(ZStorageBuffer* storageBuffer, const std::string& structDef, const std::string& file, size_t defaultArrLen) {
        size_t pos = 0;
        size_t pos2 = structDef.find(' ', pos);
        while (pos2 != structDef.npos) {
            std::string type = structDef.substr(pos, pos2 - pos);

            pos = structDef.find(';', pos) + 1;

            // check if array
            size_t arrStart = structDef.find('[');
            size_t arrLen = defaultArrLen;
            if (arrStart < pos)
            {
                std::string arr = structDef.substr(arrStart);
                trimWord(arr);

                if (arr.find("[]") == arr.npos)
                    arrLen = std::stoll(arr.substr(1));
                
                printf("arr length: %zu\n", arrLen);
            }
                
            // skip whitespace at the beginning
            if (structDef.at(pos) == ' ')
                pos++;

            pos2 = structDef.find(' ', pos);

            if (storageBuffer->addComponentByStr(type, arrLen))
                printf("  %s\n", type.c_str());
            else {                                          // custom type
                puts("custom type:");
                pos = file.find(type);
                if (pos == file.npos) {
                    printf("ERROR: type '%s' is not supported\n", type.c_str());
                    exit(1);
                }

                pos = file.find('{', pos + 7) + 1;
                
                if (file.at(pos) == ' ')
                    pos++;

                parseStructDef(storageBuffer, file.substr(pos, file.find('}', pos) - pos), file, arrLen);
            }
        }
    }
    
    void parseStructDef(ZUniform* uniform, const std::string& structDef, const std::string& file) {
        size_t pos = 0;
        size_t pos2 = structDef.find(' ', pos);
        while (pos2 != structDef.npos) {
            std::string type = structDef.substr(pos, pos2 - pos);

            pos = structDef.find(';', pos) + 1;

            // check if array
            size_t arrStart = structDef.find('[');
            // size_t arrLen = 1;
            if (arrStart < pos)
                ZASSERT_FUNC(false, "arrays are not yet supported for uniforms")
                
            // skip whitespace at the beginning
            if (structDef.at(pos) == ' ')
                pos++;

            pos2 = structDef.find(' ', pos);

            if (uniform->addComponentByStr(type))
                printf("  %s\n", type.c_str());
            else {                                          // custom type
                puts("custom type:");
                pos = file.find(type);
                if (pos == file.npos) {
                    printf("ERROR: type '%s' is not supported\n", type.c_str());
                    exit(1);
                }

                pos = file.find('{', pos + 7) + 1;
                
                if (file.at(pos) == ' ')
                    pos++;

                parseStructDef(uniform, file.substr(pos, file.find('}', pos) - pos), file);
            }
        }
    }
    
    void parseBindings(ZComputeShader* compShader, size_t defaultArrLen, const std::string& file) {
        size_t bpos = file.find("binding");
        while (bpos != file.npos) {
            // get binding type -------------------------
            size_t pos = file.find(")", bpos);
            if (pos == file.npos) {
                puts("the file is not correct");
                exit(1);
            }
            if (file.at(pos + 1) == ' ')
                pos += 2;
            std::string btype = file.substr(pos, file.find(' ', pos) - pos);
            // ------------------------------------------
    
            ZASSERT_FUNC(btype == "uniform" || btype == "buffer", "only 'uniform' and 'buffer' are supported in compute shader yet");
            
            // get binding ------------------------------
            std::string bindingStr = file.substr(file.find('=', bpos) + 1, file.find(')', bpos) - bpos);
            trimWord(bindingStr);
            size_t binding = std::stoll(bindingStr);
            // ------------------------------------------

            // get struct definition --------------------
            pos = file.find('{', pos + 7) + 1;
            
            if (file.at(pos) == ' ')
                pos++;
            
            std::string structDef = file.substr(pos, file.find('}', pos) - pos);
            // ------------------------------------------

            // go to next binding
            bpos = file.find("binding", bpos + 7);


            if (btype == "uniform") 
            {
                parseStructDef(&compShader->uniformBuffer, structDef, file);
                compShader->uniformBuffer.create();
                compShader->descriptorSetLayout.addBinding(binding, compShader->uniformBuffer.getBufferInfo(), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT);
                printf("(binding = %zu) uniform\n", binding);
            }
            else
            {
                parseStructDef(&compShader->storageBuffer, structDef, file, defaultArrLen);
                compShader->storageBuffer.create();
                compShader->descriptorSetLayout.addBinding(binding, compShader->storageBuffer.getBufferInfo(), VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT);
                printf("(binding = %zu) buffer\n", binding);
            }
        }
    }
    
    void parseBindings(std::vector<ZUniform>& uniforms, ZPipeline* pipeline, const std::string& file) {
        size_t bpos = file.find("binding");
        while (bpos != file.npos) {
            // get binding type -------------------------
            size_t pos = file.find(")", bpos);
            if (pos == file.npos) {
                puts("the file is not correct");
                exit(1);
            }
            if (file.at(pos + 1) == ' ')
                pos += 2;
            std::string btype = file.substr(pos, file.find(' ', pos) - pos);
            // ------------------------------------------
    
            ZASSERT_FUNC(btype == "uniform" || btype == "buffer", "only 'uniform' and 'buffer' are supported in compute shader yet");
            
            // get binding ------------------------------
            std::string bindingStr = file.substr(file.find('=', bpos) + 1, file.find(')', bpos) - bpos);
            trimWord(bindingStr);
            size_t binding = std::stoll(bindingStr);
            // ------------------------------------------

            // get struct definition --------------------
            pos = file.find('{', pos + 7) + 1;
            
            if (file.at(pos) == ' ')
                pos++;
            
            std::string structDef = file.substr(pos, file.find('}', pos) - pos);
            // ------------------------------------------

            // go to next binding
            bpos = file.find("binding", bpos + 7);

            if (btype == "uniform") 
            {
                uniforms.emplace_back();
                ZUniform& uniform = uniforms.back();
                
                parseStructDef(&uniform, structDef, file);
                uniform.create();
                pipeline->addBinding(binding, uniform.getBufferInfo(), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
                printf("(binding = %zu) uniform\n", binding);
            }
            else
                ZASSERT_FUNC(false, "only uniforms are supported for vertex/frament shaders");
        }
    }
}
