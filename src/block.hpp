#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <string>

class Block{
    std::string blockID;
public:
    Block();
    Block(std::string blockID);
    std::string getBlockID();
};

#endif 
