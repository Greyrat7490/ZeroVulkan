#ifndef H_TOOLS
#define H_TOOLS
#include <string>

#define ZASSERT_LINE(condition, msg) ZeroVulkan::zassert(condition, msg, __FILE__, __LINE__);
#define ZASSERT_FUNC(condition, msg) ZeroVulkan::zassert(condition, msg, __FILE__, __PRETTY_FUNCTION__);

namespace ZeroVulkan 
{
    void zassert(bool condition, const char* msg, const char* file, const char* func);
    void zassert(bool condition, const char* msg, const char* file, int line);
    
    std::string getRootDir();
    std::string pathToAbsolue(const std::string& path);

    bool fileExists(const std::string& path);
    bool fileExists(const std::u16string& path);
    
    // create the path it does’t exists
    void createPath(const std::string& path);
    void createPath(const std::u16string& path);

    std::string readFile(const std::string& filepath);

    float rndFloat(float min, float max);    
    float rndFloat();

    int rndInt(int min, int max);    
    int rndInt();
    
    uint32_t getRndSeed();
    void setRndSeed(uint32_t seed);
}

#endif // H_TOOLS
