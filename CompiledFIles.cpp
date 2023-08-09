#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <deque>

// g++ -Wall -std=c++17 -o demo CompiledFiles.cpp && demo
// agent.txt doesn't need size specified. Just enter the values immediatley
/*
.....P.
.**..**
**...**
**....*
****.**
*E.....
*/


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

class PathFinder{
    private:
        Node *pToken;
        std::deque <Node*> WaitingQueue;
        std::deque <Node*> VisitedQueue;

    private:
        bool isInQueue(Node* pNeighbor, std::deque <Node*> Queue);
        void printQueue(const std::deque <Node*> Queue);
        void removeFromQueue(std::deque <Node*> Queue);

    public:
        bool findPath(Node* pStartNode);
        void findPathDirections();
};




int main(){
    Board CBoard;
    PathFinder CPathFinder;

    if (CPathFinder.findPath(CBoard.getEnemyNode())){
        std::cout << "\nPath Found: ";
        CPathFinder.findPathDirections();
    }
    else{
        std::cout << "\nNo Path Exists";
    }
}

//[1]NODE CPP
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

//[2]BOARD CPP
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



//[3] PATHFINDER CPP
bool PathFinder::isInQueue(Node *pNeighbor, std::deque <Node*> Queue){
    if (pNeighbor == NULL){
        std::cout << "\n[WARNING] NULL WAS PASSED";
        return false;
    }
    
    for (Node* pCurr : Queue){
        if(pCurr == pNeighbor){
            return true;
        }
    }
    return false;
}


void PathFinder::removeFromQueue(std::deque <Node*> Queue){
    int i;
    for (i = 0; i < (int)Queue.size(); i++){
        if (Queue[i] == this->pToken){
            break;
        }
    }
    if (i < (int)Queue.size()){
        std::cout << "[REMOVED] "; Queue[i]->verifyType();
        Queue.erase(Queue.begin() + i);
    }
    else {
        std::cout << "[FAILED TO REMOVE] ";
    }

    std::cout << " [REMAINING] "; 
    printQueue(Queue);
}

bool PathFinder::findPath(Node* pStartNode){
    //ADD THE ENEMY NODE TO THE QUEUE
    this->WaitingQueue.push_back(pStartNode);
    
    while(!WaitingQueue.empty()){
        //GET THE FRONT TOKEN FROM QUEUE
        this->pToken = this->WaitingQueue.front();
        if (!isInQueue(pToken, this->VisitedQueue)){
            this->VisitedQueue.push_back(pToken);
        }
        this->WaitingQueue.pop_front();

        // std::cout << "\n[TOKEN] "; pToken->verifyType(); 
        
        //SCAN THROUGH ITS CHILDREN AND ADD TO WAITING
        int nDirection = 0;
        for (Node* pNeighbor : this->pToken->aNodeConnections){
            if(pNeighbor && !isInQueue(pNeighbor, this->VisitedQueue)){
                this->WaitingQueue.push_back(pNeighbor);
                
                //SET THE NEIGHBOR'S ATTRIBUTES/CONNECTIONS
                pNeighbor->pParent = this->pToken;
                pNeighbor->EDirectionFromParent = static_cast<EnumDirection> (nDirection);
                
                //TERMINAL CHECK
                // pNeighbor->verifyType();
                // std::cout << "[DIR]" << pNeighbor->verifyDirection();


                //TRUE WHEN PLAYER IS FOUND
                if (pNeighbor->EType == EnumNode::PLAYER){
                    this->pToken = pNeighbor;
                    return true;
                }
            }
            nDirection++;

        }

    }
    //FALSE WHEN NOT FOUND
    return false;
}

void PathFinder::printQueue(const std::deque <Node*> Queue){
    for (Node *pNode : Queue){
        pNode->verifyType();
    }
}

void PathFinder::findPathDirections(){
    std::cout << "\nRETRACING PATH FROM VISITED: ";
    printQueue(this->VisitedQueue);


    std::string strDirections;
    while(this->pToken->EType != EnumNode::ENEMY){        
        std::cout << "\n[TOKEN] "; this->pToken->verifyType();
        std::cout << " <- " << pToken->verifyDirection();
        std::cout << " [PARENT] "; this->pToken->pParent->verifyType();    

        //INSERT THE NEWEST DIRECTION (CLOSER TO ENEMY) AT THE START
        strDirections.insert(0, pToken->verifyDirection());
        
        //NEXT TOKEN IS THE PARENT OF THE CURRENT
        this->pToken = pToken->pParent;

    }        
   std::cout << "\n[DIRECTIONS FROM ENEMY TO PLAYER] " << strDirections;
}