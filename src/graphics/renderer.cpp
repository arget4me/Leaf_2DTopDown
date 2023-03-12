#include "renderer.h"
#include "primitives.h"
#include "shader.h"
#include "image\image.h"
#include <vector>
#include <GL\glew.h>

namespace LEAF
{

uint32_t RegisterImage(Image image, Renderer* renderer) 
{
    if (renderer)
    {
        renderer->texturesCPU.push_back(image);
        return renderer->texturesCPU.size() - 1;
    }

    return -1;
}

uint32_t RegisterShaderProgram(ShaderProgram shader, Renderer* renderer) 
{
    if (renderer)
    {
        renderer->shaderPrograms.push_back(shader);
        return renderer->shaderPrograms.size() - 1;
    }

    return -1;
}

uint32_t RegisterShaderUniform(ShaderUniform uniform, Renderer* renderer) 
{
    if (renderer)
    {
        renderer->shaderUniforms.push_back(uniform);
        return renderer->shaderUniforms.size() - 1;
    }

    return -1;
}
uint32_t RegisterMaterial(Material material, Renderer* renderer)
{
    if (renderer)
    {
        renderer->materials.push_back(material);
        return renderer->materials.size() - 1;
    }

    return -1;
}

uint32_t RegisterMeshGL(MeshGL mesh, Renderer* renderer)
{
    if (renderer)
    {
        renderer->meshesGL.push_back(mesh);
        return renderer->meshesGL.size() - 1;
    }

    return -1;
}

void SubmitRenderJob(RenderJob renderJobs, Renderer* renderer, bool sortJobsNow) 
{
    if (renderer)
    {
        renderer->renderQueue.push_back(renderJobs);
        if (sortJobsNow)
        {
            SortRenderJobs(renderer);
        }
    }
}

void SortRenderJobs(Renderer* renderer) 
{
    if (renderer)
    {
        // TODO sort
        // renderer->renderQueue
    }
}
void RenderAllRenderJobs(const Renderer* renderer) 
{
    // TODO: there should be much more valid checking done here. Leaving as is while prototyping, but this needs fixing.
    if (renderer)
    {
        for (const RenderJob& job : renderer->renderQueue )
        {
            MeshGL mesh = renderer->meshesGL[job.meshId];

            glBindVertexArray(mesh.VAO);

            for (const uint32_t materialId : job.materialIds)
            {
                // TODO: How do you properly support multiple materials? this will just renderer the full mesh several times with different shaders.
                Material material = renderer->materials[materialId];
                ShaderProgram shader = renderer->shaderPrograms[material.shaderId];
                glUseProgram(shader.id);
                
                // TODO: textures
                // TODO: uniforms
                glDrawElements(GL_TRIANGLES, mesh.NumIndices, GL_UNSIGNED_INT, 0);
            }
            
            // glBindBuffer(GL_ARRAY_BUFFER, 0);
            // glBindVertexArray(0);
        }
    }
}

};