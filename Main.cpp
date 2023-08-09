#include "Main.h"


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