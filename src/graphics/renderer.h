#pragma once
#include "shader.h"
#include "image\image.h"
#include <vector>

namespace LEAF {

struct MeshGL
{
    GLuint VAO;
    uint32_t NumIndices;
};

struct Material
{
    uint32_t shaderId;
    std::vector<uint32_t> uniforms;
    std::vector<uint32_t> textureIds;
};

struct RenderJob
{
    uint32_t meshId;

    // Use a different priority to affect order of rendered jobs but this will
    //  disregard the default state changes optimization sorting. 
    //  State changes optimizations is still performed for RenderJobs with the same priority.
    uint32_t priority = 0;
    std::vector<uint32_t> materialIds;
};

struct Renderer
{
    std::vector<Image> texturesCPU;
    std::vector<ShaderProgram> shaderPrograms;
    std::vector<ShaderUniform> shaderUniforms;
    std::vector<Material> materials;
    std::vector<MeshGL> meshesGL;
    std::vector<RenderJob> renderQueue;
};

uint32_t RegisterImage(Image image, Renderer* renderer);
uint32_t RegisterShaderProgram(ShaderProgram, Renderer* renderer);
uint32_t RegisterShaderUniform(ShaderUniform uniform, Renderer* renderer);
uint32_t RegisterMaterial(Material material, Renderer* renderer);
uint32_t RegisterMeshGL(MeshGL mesh, Renderer* renderer);

void SubmitRenderJob(RenderJob renderJobs, Renderer* renderer, bool sortJobsNow = true);
void SortRenderJobs(Renderer* renderer);
void RenderAllRenderJobs(const Renderer* renderer);





};