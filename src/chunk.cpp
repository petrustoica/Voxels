#include "chunk.hpp"
#include <glm/glm.hpp>
#include <string.h>


Chunk::Chunk(){
    memset(Chunk::blockFaceVisibility, 0, sizeof(blockFaceVisibility));
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
        ((blockChunkPos.x <= 14  && !(Chunk::blocks[blockChunkPos.x + 1][blockChunkPos.y][blockChunkPos.z].getBlockType() == BlockType::Air)) << 0) -
        ((blockChunkPos.y <= 14  && !(Chunk::blocks[blockChunkPos.x][blockChunkPos.y + 1][blockChunkPos.z].getBlockType() == BlockType::Air)) << 1) -
        ((blockChunkPos.z <= 14  && !(Chunk::blocks[blockChunkPos.x][blockChunkPos.y][blockChunkPos.z + 1].getBlockType() == BlockType::Air)) << 2) -
        ((blockChunkPos.x >= 1 && !(Chunk::blocks[blockChunkPos.x - 1][blockChunkPos.y][blockChunkPos.z].getBlockType() == BlockType::Air)) << 3) -
        ((blockChunkPos.y >= 1 && !(Chunk::blocks[blockChunkPos.x][blockChunkPos.y - 1][blockChunkPos.z].getBlockType() == BlockType::Air)) << 4) -
        ((blockChunkPos.z >= 1 && !(Chunk::blocks[blockChunkPos.x][blockChunkPos.y][blockChunkPos.z - 1].getBlockType() == BlockType::Air)) << 5);
}

void Chunk::setBlock(glm::ivec3 blockChunkPos, BlockType blockType) {
    Chunk::blocks[blockChunkPos.x][blockChunkPos.y][blockChunkPos.z].setBlockType(blockType);
    updateVisibilityOfBlockFaces(blockChunkPos);
    updateVisibilityOfBlockFaces(blockChunkPos - glm::ivec3(1, 0, 0));
    updateVisibilityOfBlockFaces(blockChunkPos - glm::ivec3(0, 1, 0));
    updateVisibilityOfBlockFaces(blockChunkPos - glm::ivec3(0, 0, 1));
    updateVisibilityOfBlockFaces(blockChunkPos + glm::ivec3(1, 0, 0));
    updateVisibilityOfBlockFaces(blockChunkPos + glm::ivec3(0, 1, 0));
    updateVisibilityOfBlockFaces(blockChunkPos + glm::ivec3(0, 0, 1));
}

void Chunk::render(glm::ivec3 chunkPos) {
    for (int x = 0; x < 16; x++)
        for (int y = 0; y < 16; y++)
            for (int z = 0; z < 16; z++){
                if (Chunk::blocks[x][y][z].getBlockType() == BlockType::Air || Chunk::blockFaceVisibility[x][y][z] == 0) continue;
                Chunk::blocks[x][y][z].render(chunkPos*16 + glm::ivec3(x, y, z), Chunk::blockFaceVisibility[x][y][z]); 
            }
}