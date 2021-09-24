#ifndef H_TOOLS
#define H_TOOLS

#include <cstdio>
#include <string>
#include <random>
#include <algorithm>
#include <fstream>
#include <limits.h>
#include <unistd.h>

namespace ZeroVulkan 
{
    inline std::string getRootDir()
    {
        #ifdef __linux
            // Linux
            char buf[PATH_MAX];
            if( !getcwd(buf, sizeof(buf)) ) {
                printf("could not get current director\n");
                return std::string(); 
            }

            return std::string(buf);
        #else
            // windows or other OS
            char buf[PATH_MAX];
            if( !_getcwd(buf, sizeof(buf)) ) {
                printf("could not get current director\n");
                return std::string(); 
            }

            return std::string(buf);
        #endif
    }
    
    inline std::string readFile(const std::string& filepath)
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

    class ZRandom
    {
    public:
        ZRandom(const ZRandom&) = delete;

        inline static ZRandom& get()
        {
            static ZRandom s_random;
            return s_random;
        }

        inline static  float rndFloat( float min, float max )
        {
            return get().rndFloatImpl( min, max );
        }

        inline static void setSeed( uint32_t seed )
        {
            get().m_seed = seed;
        }

        static uint32_t getSeed()
        {
            return get().m_seed;
        }

    private:
        ZRandom();
        ~ZRandom();

        float rndFloatImpl( float min, float max );

        std::default_random_engine m_rndEngine;
        uint32_t m_seed;
    };

    inline ZRandom::ZRandom()
    {
        m_seed = static_cast<uint32_t>(time(nullptr));
        m_rndEngine.seed(m_seed);

        printf("created ZRandom with seed: %d\n", m_seed);
    }

    inline ZRandom::~ZRandom()
    {
        printf("destroyed ZRandom\n");
    }

    inline float ZRandom::rndFloatImpl(float min, float max)
    {
        std::uniform_real_distribution<float> rndDist(min, max);
        return rndDist(m_rndEngine);
    }
}

#endif // H_TOOLS
