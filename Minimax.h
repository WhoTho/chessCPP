#ifndef MINIMAX_H
#define MINIMAX_H

#include <utility>
#include "Board.h"

class Move;

class Minimax {
   public:
    static Move findBestMove(Board& board,
                             Color color,
                             int depth,
                             bool useAlphaBeta);
    static int evaluateBoard(const Board& board, Color color);

   private:
    static int minimax(Board& board,
                       int depth,
                       bool isMaximizingPlayer,
                       Color color);
    static int minimaxAlphaBeta(Board& board,
                                int depth,
                                int alpha,
                                int beta,
                                bool isMaximizingPlayer,
                                Color color);
};

#endif  // MINIMAX_H