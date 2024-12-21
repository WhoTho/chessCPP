#include "Minimax.h"
#include <limits>
#include "Board.h"
#include "Move.h"

Move Minimax::findBestMove(Board& board,
                           Color color,
                           int depth,
                           bool useAlphaBeta) {
    int bestValue = std::numeric_limits<int>::min();
    Move bestMove = Move(-1, -1, -1, -1, nullptr);

    auto moves = board.generateAllMoves(color, (depth <= 1));
    for (const auto& move : moves) {
        board.makeMove(move);
        int moveValue = useAlphaBeta
                            ? minimaxAlphaBeta(board, depth - 1,
                                               std::numeric_limits<int>::min(),
                                               std::numeric_limits<int>::max(),
                                               false, color)
                            : minimax(board, depth - 1, false, color);
        if (moveValue > bestValue) {
            bestValue = moveValue;
            bestMove = move;
        }
        board.unmakeMove(move);
    }
    return bestMove;
}

int Minimax::minimax(Board& board,
                     int depth,
                     bool isMaximizingPlayer,
                     Color color) {
    if (depth == 0) {
        return evaluateBoard(board, color);
    }

    auto moves = board.generateAllMoves(
        isMaximizingPlayer ? color : (color == WHITE ? BLACK : WHITE),
        (depth <= 1));
    if (moves.empty()) {
        return evaluateBoard(board, color);
    }

    if (isMaximizingPlayer) {
        int bestValue = std::numeric_limits<int>::min();
        for (const auto& move : moves) {
            board.makeMove(move);
            int moveValue = minimax(board, depth - 1, false, color);
            bestValue = std::max(bestValue, moveValue);
            board.unmakeMove(move);
        }
        return bestValue;
    } else {
        int bestValue = std::numeric_limits<int>::max();
        for (const auto& move : moves) {
            board.makeMove(move);
            int moveValue = minimax(board, depth - 1, true, color);
            bestValue = std::min(bestValue, moveValue);
            board.unmakeMove(move);
        }
        return bestValue;
    }
}

int Minimax::minimaxAlphaBeta(Board& board,
                              int depth,
                              int alpha,
                              int beta,
                              bool isMaximizingPlayer,
                              Color color) {
    if (depth == 0) {
        return evaluateBoard(board, color);
    }

    auto moves = board.generateAllMoves(
        isMaximizingPlayer ? color : (color == WHITE ? BLACK : WHITE),
        (depth <= 1));
    if (moves.empty()) {
        return evaluateBoard(board, color);
    }

    if (isMaximizingPlayer) {
        int bestValue = std::numeric_limits<int>::min();
        for (const auto& move : moves) {
            board.makeMove(move);
            int moveValue =
                minimaxAlphaBeta(board, depth - 1, alpha, beta, false, color);
            bestValue = std::max(bestValue, moveValue);
            alpha = std::max(alpha, bestValue);
            board.unmakeMove(move);
            if (beta <= alpha) {
                break;
            }
        }
        return bestValue;
    } else {
        int bestValue = std::numeric_limits<int>::max();
        for (const auto& move : moves) {
            board.makeMove(move);
            int moveValue =
                minimaxAlphaBeta(board, depth - 1, alpha, beta, true, color);
            bestValue = std::min(bestValue, moveValue);
            beta = std::min(beta, bestValue);
            board.unmakeMove(move);
            if (beta <= alpha) {
                break;
            }
        }
        return bestValue;
    }
}

int Minimax::evaluateBoard(const Board& board, Color color) {
    // Simple evaluation function: count material
    int score = 0;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (board.squares[row][col]) {
                int pieceValue = 0;
                switch (board.squares[row][col]->getSymbol()) {
                    case 'P':
                        pieceValue = 1;
                        break;
                    case 'N':
                    case 'B':
                        pieceValue = 3;
                        break;
                    case 'R':
                        pieceValue = 5;
                        break;
                    case 'Q':
                        pieceValue = 9;
                        break;
                    case 'K':
                        pieceValue = 100;
                        break;
                    case 'p':
                        pieceValue = -1;
                        break;
                    case 'n':
                    case 'b':
                        pieceValue = -3;
                        break;
                    case 'r':
                        pieceValue = -5;
                        break;
                    case 'q':
                        pieceValue = -9;
                        break;
                    case 'k':
                        pieceValue = -100;
                        break;
                }
                score += pieceValue;
            }
        }
    }
    return score * (color == WHITE ? 1 : -1);
}