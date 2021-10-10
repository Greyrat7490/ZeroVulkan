#include "utils.h"

#include <algorithm>
#include <fstream>
#include <filesystem>
#include <limits.h>
#include <string>
#include <unistd.h>

namespace ZeroVulkan 
{
    void zassert(bool condition, const char* msg, const char* file, const char* func) {
        if (!condition) {
            printf("ASSERT FAILED\n  file: %s\n  func: %s\n  msg: %s\n", file, func, msg);
            exit(1);
        }
    }
    
    void zassert(bool condition, const char* msg, const char* file, int line) {
        if (!condition) {
            printf("ASSERT FAILED\n  file: %s\n  line: %s\n  msg: %s\n", file, std::to_string(line).c_str(), msg);
            exit(1);
        }
    }
    
    std::string getRootDir()
    {
        #ifdef __linux
            // Linux
            char buf[PATH_MAX];
            if (!getcwd(buf, sizeof(buf))) {
                puts("could not get current director");
                return std::string(); 
            }

            return std::string(buf) + '/';
        #else
            // windows or other OS
            char buf[PATH_MAX];
            if (!_getcwd(buf, sizeof(buf))) {
                puts("could not get current director");
                return std::string(); 
            }

            return std::string(buf) + '/';
        #endif
    }
 
    std::string pathToAbsolue(const std::string& path) {
        return std::filesystem::absolute(path);
    }
    
    bool fileExists(const std::string& path) {
        return std::filesystem::exists(path);
    }

    bool fileExists(const std::u16string& path) {
        return std::filesystem::exists(path);
    }
    
    // create the path it does’t exists
    void createPath(const std::string& path) {
        if (!std::filesystem::exists(path))
            std::filesystem::create_directories(path);
    }
 
    void createPath(const std::u16string& path) {
        if (!std::filesystem::exists(path))
            std::filesystem::create_directories(path);
    }
    
    std::string readFile(const std::string& filepath)
    {
        std::ifstream file(filepath, std::ios::binary | std::ios::ate);
        if (file)
        {
            size_t size = (size_t)file.tellg();
            std::string fileBuffer;
            fileBuffer.resize(size);

            file.seekg(0);
            file.read(fileBuffer.data(), size);
            file.close();

            return fileBuffer;
        }

        printf("ERROR: file (%s) not found or unable to open\n", filepath.c_str());
        return "\0";
    }


    ZRandom::ZRandom()
    {
        m_seed = static_cast<uint32_t>(time(nullptr));
        m_rndEngine.seed(m_seed);

        printf("created ZRandom with seed: %d\n", m_seed);
    }

    ZRandom::~ZRandom()
    {
        printf("destroyed ZRandom\n");
    }

    float ZRandom::rndFloatImpl(float min, float max)
    {
        std::uniform_real_distribution<float> rndDist(min, max);
        return rndDist(m_rndEngine);
    }
}
