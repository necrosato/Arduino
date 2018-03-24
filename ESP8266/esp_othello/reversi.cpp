#include <iostream>
#include <iomanip>
#include <limits>

#include "reversi.hpp"

using std::cin;
using std::cout;

namespace reversi {

int getInt(int high, int low) {
    int num = -1;
    while (num < low || num > high) {
        while(!(cin >> num)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    return num;
}

Board::Board(int size) {
    this->size = size;
    this->grid = new int *[size];
    for (int i = 0; i < size; i++) {
        this->grid[i] = new int[size];
        for (int j = 0; j < size; j++)
            this->grid[i][j] = OPEN;
    }
    int startIndex = (this->size - 1) / 2;
    this->grid[startIndex][startIndex + 1] = BLACK;
    this->grid[startIndex][startIndex] = WHITE;
    this->grid[startIndex + 1][startIndex] = BLACK;
    this->grid[startIndex + 1][startIndex + 1] = WHITE;
}

Board::~Board() {
    for (int i = 0; i < this->size; i++)
        delete[] this->grid[i];
    delete[] this->grid;
}

int Board::getSize() {
    return this->size;
}

int** Board::getGrid() {
    return this->grid;
}

void Board::setGrid(int i, int j, Player p) {
    this->grid[i][j] = p;
}

bool Board::hasTurn(Player p) {
    for (int i = 0; i < this->size; i++)
        for (int j = 0; j < this->size; j++)
            for (int k = 0; k < 8; k++)
                if (this->grid[i][j] == OPEN)
                    if (checkCanFlip(p, i, j, k))
                        return true;
    return false;
}

void Board::promptTurn(Player p, int high) {
    int row, col;
    printBoard();
    cout << "Player " << p + 1 << " turn, input 2 numbers (0 - " << high - 1 << ") for row and col: ";
    row = getInt(high - 1, 0);
    col = getInt(high - 1, 0);
    if (!checkTurn(p, row, col)) {
        cout << "Invalid input: Try Again\n";
        promptTurn(p, high);
    }
}

int Board::checkWin() {
    int numWhite = 0, numBlack = 0, winner = -2;
    for (int i = 0; i < this->size; i++)
        for (int j = 0; j < this->size; j++) {
            switch (this->grid[i][j]) {
                case OPEN: winner = OPEN; break;
                case BLACK: numBlack++; break;
                case WHITE: numWhite++; break;
            }
        }
    cout << "Player 1: " << numBlack << "   Player 2: " << numWhite << "\n";
    
    if (numBlack == 0)
        winner = WHITE;
    else if (numWhite == 0)
        winner = BLACK;
    
    if (winner == -1)
        return winner;
    else if (numBlack > numWhite)
        winner = BLACK;
    else if (numWhite > numBlack)
        winner = WHITE;
    cout << "Player " << winner + 1 << " won!\n";
    return winner;
}

void Board::printBoard() {
    cout << "  ";
    for (int i = 0; i < this->size; i++)
        cout << std::setw(2) << i;
    cout << "\n";
    for (int i = 0; i < this->size; i++) {
        cout << std::setw(2) << i;
        for (int j = 0; j < this->size; j++) {
            if (this->grid[i][j] == BLACK) cout << " ■";
            else if (this->grid[i][j] == WHITE) cout << " □";
            else cout << " .";
        }
        cout << "\n";
    }
}

void Board::flip(int i, int j) {
    this->grid[i][j] = !(this->grid[i][j]);
}

bool Board::canFlip(Player p, int i, int j, int d) {
    int iOffset = 0, jOffset = 0, nextI, nextJ;
    switch (d) {
        case 0:
            iOffset = -1;
            jOffset = -1;
            break;
        case 1:
            iOffset = -1;
            break;
        case 2:
            iOffset = -1;
            jOffset = 1;
            break;
        case 3:
            jOffset = -1;
            break;
        case 4:
            jOffset = 1;
            break;
        case 5:
            iOffset = 1;
            jOffset = -1;
            break;
        case 6:
            iOffset = 1;
            break;
        case 7:
            iOffset = 1;
            jOffset = 1;
            break;
    }

    nextI = i + iOffset;
    nextJ = j + jOffset;
    
    if (nextI < 0 || nextI >= this->size || nextJ < 0 || nextJ >= this->size)
    return false;
    else if (this->grid[nextI][nextJ] == (!p) && this->grid[i][j] == p)
        return canFlip(p, nextI, nextJ, d);
    else if (this->grid[nextI][nextJ] == (!p) && this->grid[i][j] == OPEN) {
        bool nextCanFlip = canFlip(p, nextI, nextJ, d);
        if (nextCanFlip)
            this->grid[i][j] = p;
        return nextCanFlip;
    }
    else if (this->grid[nextI][nextJ] == (!p) && this->grid[i][j] == (!p)) {
        bool nextCanFlip = canFlip(p, nextI, nextJ, d);
        if (nextCanFlip)
            flip(i, j);
        return nextCanFlip;
    }
    else if (this->grid[nextI][nextJ] == p && this->grid[i][j] == (!p)) {
        flip(i, j);
        return true;
    }
    return false;
}

bool Board::checkCanFlip(Player p, int i, int j, int d) {
    int iOffset = 0, jOffset = 0, nextI, nextJ;
    switch (d) {
        case 0:
            iOffset = -1;
            jOffset = -1;
            break;
        case 1:
            iOffset = -1;
            break;
        case 2:
            iOffset = -1;
            jOffset = 1;
            break;
        case 3:
            jOffset = -1;
            break;
        case 4:
            jOffset = 1;
            break;
        case 5:
            iOffset = 1;
            jOffset = -1;
            break;
        case 6:
            iOffset = 1;
            break;
        case 7:
            iOffset = 1;
            jOffset = 1;
            break;
    }

    nextI = i + iOffset;
    nextJ = j + jOffset;

    if (nextI < 0 || nextI >= this->size || nextJ < 0 || nextJ >= this->size) {
        return false;
    }
    
    if (this->grid[nextI][nextJ] == OPEN)
        return false;
    else if (this->grid[nextI][nextJ] == p && this->grid[i][j] == OPEN) {
        return false;
    }
    else if (this->grid[nextI][nextJ] == p && this->grid[i][j] == (!p)) {
        return true;
    }
    return checkCanFlip(p, nextI, nextJ, d);
}

bool Board::checkTurn(Player p, int i, int j) {
    bool valid = false;
    if (this->grid[i][j] != OPEN)
        return false;
    for (int k = 0; k < 8; k++)
        if (canFlip(p, i, j, k))
            valid = true;
    return valid;
}

}
