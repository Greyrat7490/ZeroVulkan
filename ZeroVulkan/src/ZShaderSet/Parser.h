#ifndef PARSER_H_
#define PARSER_H_

#include <string>
#include <vector>

namespace ZeroVulkan {
    class ZVertexLayout;
    class ZComputeShader;
    class ZStorageBuffer;
    class ZPipeline;
    class ZUniform;
    
    void parseVertexAttr(ZVertexLayout* vertexLayout, const std::string& file);
    
    void parseStructDef(ZStorageBuffer* storageBuffer, const std::string& structDef, const std::string& file, size_t defaultArrLen);
    void parseStructDef(ZUniform* uniform, const std::string& structDef, const std::string& file);
    
    void parseBindings(ZComputeShader* compShader, size_t defaultArrLen, const std::string& file);
    void parseBindings(std::vector<ZUniform>& uniforms, ZPipeline* pipeline, const std::string& file);
}

#endif // PARSER_H_
