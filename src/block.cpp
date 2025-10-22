#include "block.hpp"
#include "glad.h"
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <unordered_map>
#include <filesystem>
#include "stb_image.h"
#include "camera.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::unordered_map<BlockType, GLsizei> Block::blockTextures = {};
glm::vec3 Block::faceTranslation;
glm::mat4 Block::model;
GLuint Block::blockVBO;
GLuint Block::blockVAO;
GLint Block::modelLocation;

namespace fs = std::filesystem;

Block::Block(BlockType blockType) {
    Block::blockType = blockType;
}

void Block::render(glm::ivec3 pos, char faces) {
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(Block::blockVAO);

    if (!Block::blockTextures[Block::blockType])
        glBindTexture(GL_TEXTURE_2D, Block::blockTextures[BlockType::NoTexture]);
    else
        glBindTexture(GL_TEXTURE_2D, Block::blockTextures[Block::blockType]);
    model = glm::mat4(1.0f);
    model = glm::translate(model, (glm::vec3)pos);
    model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(Block::modelLocation, 1, GL_FALSE, glm::value_ptr(model));

    for (int face = 0; face < 6; face++){
        if (faces & (1 << face)) {
            glDrawArrays(GL_TRIANGLES, face * 6, 6);
        }
    }
}

void Block::init() {
    glGenBuffers(1, &Block::blockVBO);
    glGenVertexArrays(1, &Block::blockVAO);

    glBindVertexArray(Block::blockVAO);
    glBindBuffer(GL_ARRAY_BUFFER, Block::blockVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Block::vertices), Block::vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

    fs::path assetFolderPath = "../assets"; //hardcode for now

    std::ifstream blockJsonStream((assetFolderPath/"textures/block.json").string().c_str());
    json blockJson = json::parse(blockJsonStream);

    int imageWidth, imageHeight, nrChannels;
    unsigned char *data;
  
    fs::path assetPath = assetFolderPath / "textures";

    for (const auto& [key, entry] : blockJson.items()) {
        if (!entry.contains("texture")) continue;
        unsigned char* data = stbi_load((assetPath/entry["texture"].template get<std::string>()).string().append(".png").c_str(), &imageWidth, &imageHeight, &nrChannels, 0);
        if (data) {
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glGenerateMipmap(GL_TEXTURE_2D);
            Block::blockTextures[JsonKeyToBlockType[key.c_str()]] = texture;
            printf("loaded  %s\n", (assetPath/entry["texture"].template get<std::string>()).string().append(".png").c_str());
        } else {
            printf("texture loader failed loading %s\n", (assetPath/entry["texture"].template get<std::string>()).string().append(".png").c_str());
        }
        stbi_image_free(data);
    }

    modelLocation = glGetUniformLocation(Camera::getCurrentShader().ID, "model");
}

Block::Block(){
    Block::blockType = BlockType::Air;
}

BlockType Block::getBlockType() {
    return Block::blockType;
}

void Block::setBlockType(BlockType blockType) {
    Block::blockType = blockType;
}