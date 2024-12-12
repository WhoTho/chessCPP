#ifndef MINIMAX_H
#define MINIMAX_H

#include "Board.h"
#include <utility>

class Minimax
{
public:
    static std::pair<std::pair<int, int>, std::pair<int, int>> findBestMove(Board &board, Color color, int depth);
    static int evaluateBoard(const Board &board, Color color);

private:
    static int minimax(Board &board, int depth, bool isMaximizingPlayer, Color color);
};

#endif // MINIMAX_H