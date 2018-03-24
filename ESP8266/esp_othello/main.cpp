#include <iostream>

#include "reversi.hpp"

using std::cin;
using std::cout;

int main() {
    int size;
    cout << "Enter board dimension (8 - 50): ";
    size = reversi::getInt(50, 8);
    
    Player p = BLACK;
    reversi::Board b(size);
    
    while (b.checkWin() == -1) {
        if (!b.hasTurn(p)) {
            if (p == WHITE)
                p = BLACK;
            else p = WHITE;
        }
        b.promptTurn(p, size);
        if (p == WHITE)
            p = BLACK;
        else p = WHITE;
    }
    b.printBoard();
}
