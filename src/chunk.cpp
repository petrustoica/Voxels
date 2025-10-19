#include "chunk.hpp"
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include "glad.h"
#include <unordered_map>
#include <string>
#include "stb_image.h"
#include <filesystem>

namespace fs = std::filesystem;

std::unordered_map<std::string, GLsizei> Chunk::blockTextures = {};
GLuint Chunk::blockVBO;
GLuint Chunk::blockVAO;
GLint Chunk::modelLocation;

Shader Chunk::currentBlockShader;

void Chunk::generateBlockBuffers() {
    glGenBuffers(1, &Chunk::blockVBO);
    glGenVertexArrays(1, &Chunk::blockVAO);

    glBindVertexArray(Chunk::blockVAO);
    glBindBuffer(GL_ARRAY_BUFFER, Chunk::blockVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Chunk::vertices), Chunk::vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void Chunk::loadTextures(){
    int imageWidth, imageHeight, nrChannels;
    unsigned char *data;
  
    fs::path assetPath = fs::current_path() / "../assets/textures/block";

    for (const auto& entry : fs::directory_iterator(assetPath)) {
        unsigned char* data = stbi_load(entry.path().string().c_str(), &imageWidth, &imageHeight, &nrChannels, 0);
        if (data) {
            printf("loaded  %s\n", entry.path().string().c_str());
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glGenerateMipmap(GL_TEXTURE_2D);

            Chunk::blockTextures[entry.path().stem().string()] = texture;
        } else {
            printf("texture loader failed loading %s\n", entry.path().string().c_str());
        }
        stbi_image_free(data);
    } 
}

Chunk::Chunk(glm::ivec3 chunkPosition, Shader shader){
    this->chunkPosition = chunkPosition;
    Chunk::generateBlockBuffers();
    Chunk::loadTextures();
    memset(Chunk::blockFaceVisibility, 0, sizeof(blockFaceVisibility));
    Chunk::currentBlockShader = shader;
    Chunk::modelLocation = glGetUniformLocation(Chunk::currentBlockShader.ID, "model");
}

void Chunk::setShaderProgram(Shader shader) {
    Chunk::currentBlockShader = shader;
}

/* blockChunkVisibility bit order:
/       -z  -y  -x  +z  +y  +x
/ 0x00   0   0   0   0   0   0
*/

void Chunk::updateVisibilityOfBlockFaces(glm::ivec3 blockChunkPos) {
    if (blockChunkPos.x < 0 || blockChunkPos.y < 0 || blockChunkPos.z < 0 || blockChunkPos.x > 15 || blockChunkPos.y > 15 || blockChunkPos.z > 15)
        return;
    Chunk::blockFaceVisibility[blockChunkPos.x][blockChunkPos.y][blockChunkPos.z] = 
        0b00111111 -
        ((blockChunkPos.x >= 1  && !(Chunk::blocks[blockChunkPos.x + 1][blockChunkPos.y][blockChunkPos.z].getBlockID() == "air")) << 0) -
        ((blockChunkPos.y >= 1  && !(Chunk::blocks[blockChunkPos.x][blockChunkPos.y + 1][blockChunkPos.z].getBlockID() == "air")) << 1) -
        ((blockChunkPos.z >= 1  && !(Chunk::blocks[blockChunkPos.x][blockChunkPos.y][blockChunkPos.z + 1].getBlockID() == "air")) << 2) -
        ((blockChunkPos.x <= 14 && !(Chunk::blocks[blockChunkPos.x - 1][blockChunkPos.y][blockChunkPos.z].getBlockID() == "air")) << 3) -
        ((blockChunkPos.y <= 14 && !(Chunk::blocks[blockChunkPos.x][blockChunkPos.y - 1][blockChunkPos.z].getBlockID() == "air")) << 4) -
        ((blockChunkPos.z <= 14 && !(Chunk::blocks[blockChunkPos.x][blockChunkPos.y][blockChunkPos.z - 1].getBlockID() == "air")) << 5);
}

void Chunk::setBlock(glm::ivec3 blockChunkPos, Block block) {
    Chunk::blocks[blockChunkPos.x][blockChunkPos.y][blockChunkPos.z] = block;
    updateVisibilityOfBlockFaces(blockChunkPos);
    updateVisibilityOfBlockFaces(blockChunkPos - glm::ivec3(1, 0, 0));
    updateVisibilityOfBlockFaces(blockChunkPos - glm::ivec3(0, 1, 0));
    updateVisibilityOfBlockFaces(blockChunkPos - glm::ivec3(0, 0, 1));
    updateVisibilityOfBlockFaces(blockChunkPos + glm::ivec3(1, 0, 0));
    updateVisibilityOfBlockFaces(blockChunkPos + glm::ivec3(0, 1, 0));
    updateVisibilityOfBlockFaces(blockChunkPos + glm::ivec3(0, 0, 1));
}

void Chunk::renderChunk() {
    Chunk::currentBlockShader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(Chunk::blockVAO);

    glm::mat4 model;

    for (int x = 0; x < 16; x++)
        for (int y = 0; y < 16; y++)
            for (int z = 0; z < 16; z++){
                if (Chunk::blocks[x][y][z].getBlockID() == "air" || Chunk::blockFaceVisibility[x][y][z] == 0) continue;
                
                if (!Chunk::blockTextures[Chunk::blocks[x][y][z].getBlockID().append("")])
                    glBindTexture(GL_TEXTURE_2D, Chunk::blockTextures["notexture"]);
                else
                    glBindTexture(GL_TEXTURE_2D, Chunk::blockTextures[Chunk::blocks[x][y][z].getBlockID().append("")]);
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(chunkPosition.x + x - 7, chunkPosition.y + y - 7, chunkPosition.z + z - 7));
                model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                glUniformMatrix4fv(Chunk::modelLocation, 1, GL_FALSE, glm::value_ptr(model));
                
                for (int face = 0; face < 6; face++){
                    if (Chunk::blockFaceVisibility[x][y][z] & (1 << face)) {
                        glDrawArrays(GL_TRIANGLES, face * 6, 6);
                    }
                }
            }
}