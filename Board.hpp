#pragma once

#include "Node.hpp"
#include <cmath>
#include <vector>
#include <string>
#include <fstream>

class Board{
    private:
        std::vector <Node*> vecNodes;
        int nRows = 0, nCols = 0;
        
    public:
        Board();
        Node* getEnemyNode();

    private:
        void createBoard();
        void processLine(std::string strLine);
        void linkNodeConnections();
};

Board::Board(){
    this->createBoard();
    this->linkNodeConnections();
}


void Board::createBoard(){
    std::ifstream fp("agent.txt");

    std::string strLine; 

    if(fp.is_open()){
        //KEEP READING EACH LINE
        while (!fp.fail()){
            strLine.clear();
            std::getline(fp, strLine);
            this->processLine(strLine);

            //NOTHING WAS READ
            if (strLine.empty()){
                // std::cout << "EMPTY";
                break;
            }
            this->nRows++;
            this->nCols = (strLine[strLine.size()-1] == ' ') ? strLine.size()-1 : strLine.size();
        };
        
        

        fp.close();
        std::cout << "\nCREATED BOARD OF SIZE" << this->nRows <<"x" << this->nCols<<std::endl;
        std::cout << "TOTAL TILES: " << this->vecNodes.size() << std::endl;
    }
    else{
        std::cout << "[ERROR]: TXT FILE NOT FOUND";
    }
}

void Board::processLine(std::string strLine){
    // std::cout << "\nRECEIVED: " << strLine << strLine.size(); 

    for (int i = 0; i < (int)strLine.size(); i++){
        if (strLine[i] != ' '){
            EnumNode EType;
            switch(strLine[i]){
                //IGNORE SPACES
                case '*':
                    EType = EnumNode::IMPASSABLE;
                    break;
                case '.':
                    EType = EnumNode::PASSABLE;
                    break;
                case 'P':
                    EType = EnumNode::PLAYER;
                    break;
                case 'E':
                    EType = EnumNode::ENEMY;
                    break;
                default: 
                    EType = EnumNode::PASSABLE;
                    std::cout << "\n[ERROR] Char symbol not recognized";
            }
            this->vecNodes.push_back(new Node(EType, strLine[i]));
        }
    }
}

void Board::linkNodeConnections(){
    int nCurrRow = 0;
    int nCurrCol = -1;

    for (Node* pCurr : this->vecNodes){
        nCurrCol++;
        if (nCurrCol >= this->nCols){
            nCurrRow++;
            nCurrCol = 0;
            std::cout << std::endl;
        }

        pCurr->verifyType();
        //ONLY CONNECT PASSABLE TILES
        if (pCurr->EType != EnumNode::IMPASSABLE){
            //CONNECT TOP
            if (nCurrRow != 0 && (this->vecNodes[(nCurrRow-1)*this->nCols + nCurrCol]->EType != EnumNode::IMPASSABLE)){                
                pCurr->aNodeConnections[0] = this->vecNodes[(nCurrRow-1)*this->nCols + nCurrCol];
                // std::cout << "[T]";
            }

            //CONNECT LEFT
            if (nCurrCol != 0 && this->vecNodes[nCurrRow*this->nCols + nCurrCol-1]->EType != EnumNode::IMPASSABLE){
                pCurr->aNodeConnections[3] = this->vecNodes[nCurrRow*this->nCols + nCurrCol-1];
                // std::cout << "[L]";
            }

            //CONNECT RIGHT
            if (nCurrCol < this->nCols-1 && this->vecNodes[nCurrRow*this->nCols + nCurrCol + 1]->EType != EnumNode::IMPASSABLE){
                pCurr->aNodeConnections[1] = this->vecNodes[nCurrRow*this->nCols + nCurrCol + 1];
                // std::cout << "[R]";
            }

            //CONNECT DOWN
            if (nCurrRow < this->nRows-1 && this->vecNodes[(nCurrRow+1)*this->nCols + nCurrCol]->EType != EnumNode::IMPASSABLE){
                pCurr->aNodeConnections[2] = this->vecNodes[(nCurrRow+1)*this->nCols + nCurrCol];
                // std::cout << "[D]";
            }
        }
    }
}

Node* Board::getEnemyNode(){
    for (Node* pNode : this->vecNodes){
        if (pNode->EType == EnumNode::ENEMY){
            return pNode;
        }
    }
    std::cout << "\n[ERROR] ENEMY NODE NOT FOUND";
    return NULL;
}