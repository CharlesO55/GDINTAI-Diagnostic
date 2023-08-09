#pragma once

#include <iostream>

enum class EnumNode{
    IMPASSABLE = 0,
    PASSABLE,
    PLAYER,
    ENEMY
};

enum class EnumDirection{
    EMPTY = -1,
    NORTH = 0,
    EAST,
    SOUTH,
    WEST
};

class Node{
    public:
        //N, E, S, W
        Node* aNodeConnections[4] = {NULL, NULL, NULL, NULL};
        Node* pParent = NULL;
        EnumNode EType;
        EnumDirection EDirectionFromParent;
        char cSymbol;
        

        Node(EnumNode EType, char cSymbol); 
        void verifyType();
        std::string verifyDirection();
        
}; 

Node::Node(EnumNode EType, char cSymbol){
    this->EType = EType;
    this->cSymbol = cSymbol;
}

void Node::verifyType(){
    std::cout << this->cSymbol;
}

std::string Node::verifyDirection(){
    switch (this->EDirectionFromParent){
        case EnumDirection::NORTH:
            return "N";
        case EnumDirection::EAST:
            return "E";
        case EnumDirection::SOUTH:
            return "S";
        case EnumDirection::WEST:
            return "W";
        default:
            throw("Direction Unset");
    }
}