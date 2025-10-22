#pragma once
#ifndef WORLD_HPP
#define WORLD_HPP

#include <unordered_map>
#include <glm/glm.hpp>
#include "chunk.hpp"

namespace std {
    template <>
    struct hash<glm::ivec3> {
        size_t operator()(const glm::ivec3& ivec3) const {
            return hash<uint64_t>{}((ivec3.x << (sizeof(ivec3.x) * 2)) | (ivec3.y << sizeof(ivec3.x)) | (ivec3.z));
        }
    };
}

class World{
private:
    std::unordered_map<glm::ivec3, Chunk> chunks;
    // Chunk getChunkAtPos(glm::ivec3);

public:
    World();
    void setBlock(glm::ivec3 blockWorldPos, BlockType blockType);
    void render();
    void loadChunk(glm::ivec3 chunkPos);
    void unloadChunk(glm::ivec3 chunkPos);
};

#endif