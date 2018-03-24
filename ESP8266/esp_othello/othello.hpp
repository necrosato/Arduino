#ifndef __OTHELLO_HPP
#define __OTHELLO_HPP

#include <string>

class Othello {
public:
    typedef enum {
        OPEN = -1,
        BLACK = 0,
        WHITE = 1
    } Player;

    Othello(int size);
    ~Othello();
    int getSize();
    int **getGrid();
    void setGrid(int i, int j, Player p);
    bool hasTurn(Player p);
    void promptTurn(Player p, int high);
    int checkWin();
    void printBoard();
private:
    int size;
    int **grid;
    void flip(int i, int j);
    bool canFlip(Player p, int i, int j, int d);
    bool checkCanFlip(Player p, int i, int j, int d);
    bool checkTurn(Player p, int i, int j);
};

#endif /* __OTHELLO_HPP */
