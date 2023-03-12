#include "game.h"
#include <GL/glew.h>
#include "graphics\renderer.h"
#include "graphics\primitives.h"
#include "graphics\shader.h"
#include "image\image.h"

namespace LEAF {

    void InitializeGame(GameState& gameState)
    {
        constexpr uint32_t TOTALSIZE = 128 * MEGABYTE;
        constexpr uint32_t TEMP_TOTALSIZE = 8 * MEGABYTE;
        constexpr uint32_t BLOCKSIZE = 64;
        constexpr uint32_t NUMBLOCKS = TOTALSIZE / BLOCKSIZE;
        constexpr uint32_t TEMP_NUMBLOCKS = TEMP_TOTALSIZE / BLOCKSIZE;
        gameState.persistentMemory = MemoryManager(BLOCKSIZE, NUMBLOCKS);
        gameState.temporaryMemory = MemoryManager(BLOCKSIZE, TEMP_NUMBLOCKS, true);
        Image debug_image = LoadImageFile("data/images/debug_image.png", gameState.persistentMemory);
        gameState.running = true;
        ShaderSource vertex;
        vertex.ShaderCode = "#version 330 core\n"
                            "layout (location = 0) in vec4 aPos;\n"
                            "layout (location = 1) in vec2 iUV;\n"
                            "out vec2 UV;\n"
                            "void main()\n"
                            "{\n"
                            "   UV = iUV;\n"
                            "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                            "}\0";
        
        ShaderSource fragment;
        fragment.ShaderCode = "#version 330 core\n"
                            "in vec2 UV;\n"
                            "out vec4 FragColor;\n"
                            "uniform sampler2D sampler1;\n"
                            "void main()\n"
                            "{\n"
                            "   FragColor = texture(sampler1, UV);\n" 
                            "}\n\0";
                            //"   FragColor = vec4(UV.x, UV.y, 0.0f, 1.0f);\n" 

        ShaderProgramSource programSource = CreateShaderProgramSource(vertex, fragment);
        ShaderProgram program = CreateShaderProgram(&programSource);

        uint32_t shaderProgramID = RegisterShaderProgram(program, &gameState.renderer);

        Material material;
        material.shaderId = shaderProgramID;
        uint32_t materialID = RegisterMaterial(material, &gameState.renderer);
        
        Quad quad;
        for (int i = 0; i < sizeof(quad.position) / sizeof(decltype(quad.position[0])); i++ )
        {
            quad.position[i].vec2 = quad.position[i].vec2 * 0.5f;
        }

        {
            GLuint VAO, VBO, EBO;
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);
            // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            // glBufferData(GL_ARRAY_BUFFER, sizeof(quad.position), quad.position, GL_STATIC_DRAW);

            //Allocate size of the Buffer skip upload 
            glBufferData(GL_ARRAY_BUFFER, sizeof(quad.position)+sizeof(quad.uvs), NULL, GL_STATIC_DRAW);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quad.position), quad.position);
            glBufferSubData(GL_ARRAY_BUFFER, sizeof(quad.position), sizeof(quad.uvs), quad.uvs);

            //upload indices
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad.indices), quad.indices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vec4), (void*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vec2), (void*)sizeof(quad.position));
            glEnableVertexAttribArray(1);

            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            MeshGL mesh;
            mesh.VAO = VAO;
            mesh.VBO = VBO;
            mesh.EBO = EBO;
            mesh.NumIndices = sizeof(quad.indices) / sizeof(decltype(quad.indices[0]));
            uint32_t meshId = RegisterMeshGL(mesh, &gameState.renderer);

            RenderJob job;
            job.meshId = meshId;
            job.materialIds = {materialID};
            SubmitRenderJob(job, &gameState.renderer);
        }


        {
            GLuint texture_handle;
            glGenTextures(1, &texture_handle);

            glActiveTexture(GL_TEXTURE0 + 0);
            glBindTexture(GL_TEXTURE_2D, texture_handle);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, debug_image.width, debug_image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, debug_image.buffer.byteBuffer);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }

    void UpdateAndRenderGame(GameState& gameState)
    {
        RenderAllRenderJobs(&gameState.renderer);
    }
};