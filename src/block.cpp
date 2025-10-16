#include "block.hpp"
#include <string>

Block::Block(std::string blockID){
    Block::blockID = blockID;
}

Block::Block(){
    Block::blockID = "air";
}

std::string Block::getBlockID(){
    return Block::blockID;
}
