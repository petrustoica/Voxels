#pragma once
#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <glm/glm.hpp>
#include "block.hpp"

class Chunk{
private:
    Block blocks[16][16][16];
    char blockFaceVisibility[16][16][16];
    void updateVisibilityOfBlockFaces(glm::ivec3 blockChunkPos);

public:
    Chunk();
    void setBlock(glm::ivec3 blockChunkPos, BlockType blockType);
    void render(glm::ivec3 chunkPos);
};

#endif