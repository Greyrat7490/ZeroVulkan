#include "utils.h"

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <filesystem>
#include <limits.h>
#include <string>
#include <unistd.h>
#include <random>

namespace ZeroVulkan 
{
    static uint32_t s_seed = static_cast<uint32_t>(time(nullptr));
    static std::mt19937 s_gen(s_seed);
    
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


    float rndFloat() { return s_gen(); }
    float rndFloat(float min, float max) {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(s_gen);
    }

    int rndInt() { return s_gen(); }
    int rndInt(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(s_gen);
    }

    uint32_t getRndSeed() { return s_seed; }
    void setRndSeed(uint32_t newSeed) { s_seed = newSeed; }
}
