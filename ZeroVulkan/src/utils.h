#ifndef H_TOOLS
#define H_TOOLS
#include <string>
#include <random>

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

    // TODO: no singleton
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
}

#endif // H_TOOLS
