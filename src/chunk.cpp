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
            printf("texture loader crapped pants");
        }
        stbi_image_free(data);
    } 
}

Chunk::Chunk(glm::ivec3 chunkPosition, Shader shader){
    this->chunkPosition = chunkPosition;
    Chunk::generateBlockBuffers();
    Chunk::loadTextures();
    Chunk::currentBlockShader = shader;
    Chunk::modelLocation = glGetUniformLocation(Chunk::currentBlockShader.ID, "model");
}

void Chunk::setShaderProgram(Shader shader) {
    Chunk::currentBlockShader = shader;
}

void Chunk::setBlock(glm::ivec3 blockChunkPos, Block block){
    Chunk::blocks[blockChunkPos.x][blockChunkPos.y][blockChunkPos.z] = block;
}

void Chunk::renderChunk() {
    Chunk::currentBlockShader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(Chunk::blockVAO);

    glm::mat4 model;

    for (int x = 0; x < 16; x++)
        for (int y = 0; y < 16; y++)
            for (int z = 0; z < 16; z++){
                if (Chunk::blocks[x][y][z].getBlockID() == "air") continue;
                for (int face = 0; face < 6; face++)
                    switch(face) {
                        case 0:
                            if (!Chunk::blockTextures[Chunk::blocks[x][y][z].getBlockID().append("")])
                                glBindTexture(GL_TEXTURE_2D, Chunk::blockTextures["notexture"]);
                            else
                                glBindTexture(GL_TEXTURE_2D, Chunk::blockTextures[Chunk::blocks[x][y][z].getBlockID().append("")]);
                            model = glm::mat4(1.0f);
                            model = glm::translate(model, glm::vec3(chunkPosition.x + x - 7, chunkPosition.y + y - 7, chunkPosition.z + z - 7));
                            model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                            glUniformMatrix4fv(Chunk::modelLocation, 1, GL_FALSE, glm::value_ptr(model));
                            glDrawArrays(GL_TRIANGLES, 0, 36);
                            break;
                    }
                }
}