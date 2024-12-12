#include "Minimax.h"
#include <limits>
#include <iostream>

std::pair<std::pair<int, int>, std::pair<int, int>> Minimax::findBestMove(Board &board, Color color, int depth)
{
    int bestValue = std::numeric_limits<int>::min();
    std::pair<std::pair<int, int>, std::pair<int, int>> bestMove;

    auto moves = board.generateAllMoves(color);
    for (const auto &move : moves)
    {
        Board tempBoard = board;
        tempBoard.movePiece(move.first.first, move.first.second, move.second.first, move.second.second);
        int moveValue = minimax(tempBoard, depth - 1, false, color);
        if (moveValue > bestValue)
        {
            bestValue = moveValue;
            bestMove = move;
        }
    }
    std::cout << "Best move value: " << bestValue << std::endl;
    return bestMove;
}

int Minimax::minimax(Board &board, int depth, bool isMaximizingPlayer, Color color)
{
    if (depth == 0)
    {
        return evaluateBoard(board, color);
    }

    auto moves = board.generateAllMoves(isMaximizingPlayer ? color : (color == WHITE ? BLACK : WHITE));
    if (moves.empty())
    {
        return evaluateBoard(board, color);
    }

    if (isMaximizingPlayer)
    {
        int bestValue = std::numeric_limits<int>::min();
        for (const auto &move : moves)
        {
            Board tempBoard = board;
            tempBoard.movePiece(move.first.first, move.first.second, move.second.first, move.second.second);
            int moveValue = minimax(tempBoard, depth - 1, false, color);
            bestValue = std::max(bestValue, moveValue);
        }
        return bestValue;
    }
    else
    {
        int bestValue = std::numeric_limits<int>::max();
        for (const auto &move : moves)
        {
            Board tempBoard = board;
            tempBoard.movePiece(move.first.first, move.first.second, move.second.first, move.second.second);
            int moveValue = minimax(tempBoard, depth - 1, true, color);
            bestValue = std::min(bestValue, moveValue);
        }
        return bestValue;
    }
}

int Minimax::evaluateBoard(const Board &board, Color color)
{
    // board.display();
    // Simple evaluation function: count material
    int score = 0;
    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            if (board.squares[row][col])
            {
                int pieceValue = 0;
                switch (board.squares[row][col]->getSymbol())
                {
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
                score += pieceValue; //  * (board.squares[row][col]->getColor() == color ? 1 : -1);
            }
        }
    }
    return score * (color == WHITE ? 1 : -1);
}