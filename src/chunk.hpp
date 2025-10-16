#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <unordered_map>
#include <glm/glm.hpp>
#include "block.hpp"
#include "shader.h"

class Chunk{
private:
    glm::ivec3 chunkPosition;
    glm::vec3 faceTranslation;
    Block blocks[16][16][16];
    static std::unordered_map<std::string, GLsizei> blockTextures;
    static GLuint blockVBO;
    static GLuint blockVAO;
    static Shader currentBlockShader;
    static GLint modelLocation;

    float vertices[180] = {
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f, //+x right
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,

            0.5f, 0.5f, -0.5f, 1.0f, 1.0f, //+y top
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 1.0f, //+z front
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,

            -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, //-x left
            -0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            
            -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, //-y bottom
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 1.0f,

            -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, //-z rear
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 1.0f, 0.0f
        };

    void draw();
    void generateBlockBuffers();
    void loadTextures();

public:
    Chunk(glm::ivec3 chunkPosition, Shader shader);
    void setBlock(glm::ivec3 blockChunkPos, Block block);
    void setShaderProgram(Shader shader);
    void renderChunk();
};

#endif