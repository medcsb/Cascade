#ifndef VCR_DESCRIPTOR_HPP
#define VCR_DESCRIPTOR_HPP

#include "vcr_device.hpp"

#include <memory>
#include <unordered_map>
#include <vector>

namespace vcr {

class DescriptorSetLayout {
private:
    Device& device;
    VkDescriptorSetLayout descriptorSetLayout;
    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

    friend class DescriptorWriter;

public:
    class Builder {
    private:
        Device& device;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};

    public:
        Builder(Device& device) : device{device} {}

        Builder& addBinding(uint32_t binding,
                            VkDescriptorType descriptorType,
                            VkShaderStageFlags stageFlags,
                            uint32_t count = 1);
        std::unique_ptr<DescriptorSetLayout> build() const;
    };

    DescriptorSetLayout(Device& device,
                        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
    ~DescriptorSetLayout();
    DescriptorSetLayout(const DescriptorSetLayout&) = delete;
    DescriptorSetLayout& operator=(const DescriptorSetLayout&) = delete;

    VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }
};

class DescriptorPool {
private:
    Device& device;
    VkDescriptorPool descriptorPool;

    friend class DescriptorWriter;

public:
    class Builder {
    private:
        Device& device;
        std::vector<VkDescriptorPoolSize> poolSizes{};
        uint32_t maxSets = 1000;
        VkDescriptorPoolCreateFlags poolFlags = 0;

    public:
        Builder(Device& device) : device{device} {}

        Builder& addPoolSize(VkDescriptorType descriptorType, uint32_t count);
        Builder& setPoolFlags(VkDescriptorPoolCreateFlags flags);
        Builder& setMaxSets(uint32_t count);
        std::unique_ptr<DescriptorPool> build() const;
    };

    DescriptorPool(Device& device,
                   uint32_t maxSets,
                   VkDescriptorPoolCreateFlags poolFlags,
                   const std::vector<VkDescriptorPoolSize>& poolSizes);
    ~DescriptorPool();
    DescriptorPool(const DescriptorPool&) = delete;
    DescriptorPool& operator=(const DescriptorPool&) = delete;

    bool allocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout,
                            VkDescriptorSet& descriptor) const;

    void freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;

    void resetPool();
};

class DescriptorWriter {
private:
    DescriptorSetLayout& setLayout;
    DescriptorPool& pool;
    std::vector<VkWriteDescriptorSet> writes;

public:
    DescriptorWriter(DescriptorSetLayout& setLayout, DescriptorPool& pool);

    DescriptorWriter& writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
    DescriptorWriter& writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

    bool build(VkDescriptorSet& set);
    void overwrite(VkDescriptorSet& set);
};

} // namespace vcr

#endif // VCR_DESCRIPTOR_HPP