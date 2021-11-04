#ifndef UNIFORM_BUFFER_H_
#define UNIFORM_BUFFER_H_

#include <initializer_list>

#include "Device.h"
#include "types.h"
#include "utils.h"

namespace ZeroVulkan
{
    class ZUniform
    {
    public:
        ZUniform() = default;
        ZUniform(ZUniform&& source);
        ZUniform& operator=(ZUniform&& source);
        ~ZUniform();

        inline const VkDescriptorBufferInfo* getBufferInfo() { return &m_bufferInfo; }
        inline VkDeviceMemory getMemory() const { return m_memory; }
        inline VkDeviceSize getBufferSize() const { return m_size; }
        inline VkDeviceSize getDynamicAlignment() const { return m_dynamicAlignment; }
    
        void addComponent(ZType type);
        bool addComponentByStr(const std::string& str);
        
        template<typename T>
        T* getComponent(size_t index);

        /*
         * WARNING make sure the layout you provided is correct
         * the method is type safe as long as the provided layout is correct
         * TODO: automatically get the layout (maybe via parsing the source code)
         */
        template<typename Struct>
        inline Struct* getStruct(const std::initializer_list<ZType>& fields) const;

        template<typename T>
        void dynamicUpdate(T* ubos, uint32_t objectCount);

        void create(uint32_t objectCount = 1);
    private:
        void* m_mappedData = nullptr;
        VkBuffer m_buffer = nullptr;
        VkDeviceMemory m_memory = nullptr;
        VkDeviceSize m_size = 0;
        VkDescriptorBufferInfo m_bufferInfo;
        VkDeviceSize m_dynamicAlignment = 0;

        std::vector<std::pair<ZType, size_t>> m_components;
        
        bool ready = false;
        
        void setDynamicAlignments();

        template<typename Struct>
        void checkStruct(const std::initializer_list<ZType>& fields) const;
    };

    template<typename Struct>
    void ZUniform::checkStruct(const std::initializer_list<ZType>& fields) const {
        // check size of the provided struct
        if (m_size == sizeof(Struct))
        {
            // check if the provided struct has the correct amount of fields
            if (m_components.size() == fields.size())
            {
                // check if the type of every field is correct
                bool correct = true;
                size_t i = 0;
                for (ZType field : fields) {
                    if (field != m_components[i].first) {
                        printf("ERROR: the struct defined in the shader has not the type %s at index %zu\n", zTypeToStr(field), i);
                        correct = false;
                    }

                    i++;
                }
                
                if (correct) {
                    puts("the provided struct seems to be correct (but only if the layout you provided is correct)");
                    return;
                }
            }
            else
                printf("ERROR: your provided layout has '%zu' fields, but the actual struct should have '%zu'\n"
                        "\tmaybe you forgot some\n", fields.size(), m_components.size());
        }
        else
            printf("ERROR: Struct size is: '%zu', but buffer size is: '%zu'\n", sizeof(Struct), m_size);

        exit(1);
    }

    template<typename Struct>
    Struct* ZUniform::getStruct(const std::initializer_list<ZType>& fields) const {
        checkStruct<Struct>(fields);
        return (Struct*)m_mappedData;
    }
    
    template<typename T>
    inline T* ZUniform::getComponent(size_t index) {
        if (index >= m_components.size()) {
            printf("ERROR: uniform has only %zu components", m_components.size());
            exit(1);
            return nullptr;
        }
            
        size_t typeID = typeid(T).hash_code(); 
        if (typeID != zTypeToID(m_components[index].first)) {
            printf("ERROR: couldn't get uniform component at index %zu\n"
                    "\tthe actual type defined in the shader is '%s', but you asked for '%s'\n", index, zTypeToStr(m_components[index].first), zTypeToStr(typeIDToZType(typeID)));
            exit(1);
            return nullptr;
        }

        return (T*)((char*)m_mappedData + m_components[index].second);
    }


    template<typename T>//TODO: only update parts which actually changed
    inline void ZUniform::dynamicUpdate(T* ubos, uint32_t objectCount)
    {
        VkMappedMemoryRange memoryRange = {};
        memoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        memoryRange.offset = 0;
        memoryRange.size = sizeof(T) * objectCount;
        memoryRange.memory = m_memory;
        vkFlushMappedMemoryRanges(ZDevice::getDevice(), 1, &memoryRange);

        char* mappedDataArray = (char*)m_mappedData;

        for(uint32_t i = 0; i < objectCount; i++)
            memcpy(&mappedDataArray[i * m_dynamicAlignment], &ubos[i], sizeof(T));
    }
}

#endif // UNIFORM_BUFFER_H_
