#pragma once
#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <cstdint>
#include <glm/glm.hpp>
#include "glad.h"
#include <unordered_map>
#include <string>

enum class BlockType : uint8_t {
    Air,
    NoTexture,
    Dirt,
    Stone,
    Bricks,
    StoneBricks,
    OakPlanks
};

namespace {
    std::unordered_map<std::string, BlockType> JsonKeyToBlockType = {
        {"air", BlockType::Air},
        {"no_texture", BlockType::NoTexture},
        {"dirt", BlockType::Dirt},
        {"stone", BlockType::Stone},
        {"bricks", BlockType::Bricks},
        {"stone_bricks", BlockType::StoneBricks},
        {"oak_planks", BlockType::OakPlanks},
    };
}

class Block{
private:
    enum BlockType blockType;
    
    static std::unordered_map<BlockType, GLsizei> blockTextures;
    static glm::vec3 faceTranslation;
    static glm::mat4 model;
    static GLuint blockVBO;
    static GLuint blockVAO;
    static GLint modelLocation;
    
    static constexpr float vertices[180] = {
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
    
    static void generateBlockBuffers();
    static void loadTextures();
public:
    Block();
    Block(BlockType blockType);
    void setBlockType(BlockType blockType);
    BlockType getBlockType();
    void render(glm::ivec3 pos, char faces);
    static void init();
};

#endif 
