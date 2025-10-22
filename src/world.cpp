#include "world.hpp"
#include "block.hpp"
#include <stdio.h>

void World::loadChunk(glm::ivec3 chunkPos) {
    Chunk chunk;
    chunks[chunkPos] = chunk;
}

World::World() {
}

void World::unloadChunk(glm::ivec3 chunkPos) {
    // chunks;
}

void World::setBlock(glm::ivec3 blockWorldPos, BlockType blockType) {
    glm::ivec3 chunkPos = blockWorldPos / 16 - glm::ivec3(blockWorldPos.x < 0, blockWorldPos.y < 0, blockWorldPos.z < 0);
    if (!chunks.count(chunkPos)) {
        World::loadChunk(chunkPos);
    }
    chunks[chunkPos].setBlock(
        glm::ivec3(blockWorldPos.x % 16, blockWorldPos.y % 16, blockWorldPos.z % 16) - 
        glm::ivec3(blockWorldPos.x < 0, blockWorldPos.y < 0, blockWorldPos.z < 0) * 16,
        blockType);
}

void World::render() {
    for (auto & [chunkPos, chunk] : chunks) {
        chunk.render(chunkPos);
    }
}