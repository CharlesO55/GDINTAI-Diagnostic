#pragma once

#include <deque>
class Node;


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