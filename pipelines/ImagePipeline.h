#include <webgpu/webgpu.hpp>
#include "ResourceManager.h"
#include <vector>

class ImagePipeline
{
public:
    wgpu::ShaderModule shaderModule = nullptr;
    wgpu::RenderPipeline pipeline = nullptr;
    bool init(wgpu::Device &device_, wgpu::TextureFormat &swapChainFormat_, wgpu::Queue &queue_);
    void draw(wgpu::RenderPassEncoder &renderPass);
    void updateImages(std::vector<ResourceManager::ImageAttributes> &images, std::vector<float> &data);
    void terminate();

private:
    std::vector<ResourceManager::ImageAttributes> images;
    std::vector<ResourceManager::ImageAttributes> prevImages;

    std::vector<float> data;

    wgpu::Queue queue = nullptr;

    wgpu::Device device = nullptr;

    wgpu::Buffer imageBuffer = nullptr;

    wgpu::Sampler sampler = nullptr;

    std::vector<wgpu::Texture> textures;
    std::vector<wgpu::TextureView> textureViews;

    wgpu::BindGroupLayout bindGroupLayout = nullptr;
    std::vector<wgpu::BindGroup> bindGroups;

    void createLayout();
    void createBindGroups();
    bool isPrevLayout();

    wgpu::Texture createTexture(ResourceManager::ImageAttributes &image);
    void createTextures();
    wgpu::TextureView createTextureView(wgpu::Texture &texture);
    void createTextureViews();

    void copyDataToTextures();
    void copyDataToTexture(ResourceManager::ImageAttributes &image, std::vector<float> &data, wgpu::Texture &texture);
    void ImagePipeline::reallocateBuffer(wgpu::Buffer &buffer, size_t count);
};