#ifndef BOARD_H
#define BOARD_H

#include "Piece.h"
#include <vector>
#include <memory>
#include <utility>

class Minimax;

class Board
{
public:
    Board();
    void display() const;
    bool movePiece(int startX, int startY, int endX, int endY);
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> generateAllMoves(Color color) const;
    bool makeAIMove(Color color);
    std::string toFEN() const;

    std::vector<std::vector<std::shared_ptr<Piece>>> squares;
};

#endif // BOARD_H