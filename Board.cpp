#include "Board.h"
#include "Minimax.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <utility>
#include <cstdlib>
#include <ctime>

Board::Board() : squares(8, std::vector<std::shared_ptr<Piece>>(8, nullptr))
{
    // Initialize the board with pieces
    // White pieces
    squares[0][0] = std::make_shared<Rook>(WHITE);
    squares[0][1] = std::make_shared<Knight>(WHITE);
    squares[0][2] = std::make_shared<Bishop>(WHITE);
    squares[0][3] = std::make_shared<Queen>(WHITE);
    squares[0][4] = std::make_shared<King>(WHITE);
    squares[0][5] = std::make_shared<Bishop>(WHITE);
    squares[0][6] = std::make_shared<Knight>(WHITE);
    squares[0][7] = std::make_shared<Rook>(WHITE);
    for (int i = 0; i < 8; ++i)
    {
        squares[1][i] = std::make_shared<Pawn>(WHITE);
    }

    // Black pieces
    squares[7][0] = std::make_shared<Rook>(BLACK);
    squares[7][1] = std::make_shared<Knight>(BLACK);
    squares[7][2] = std::make_shared<Bishop>(BLACK);
    squares[7][3] = std::make_shared<Queen>(BLACK);
    squares[7][4] = std::make_shared<King>(BLACK);
    squares[7][5] = std::make_shared<Bishop>(BLACK);
    squares[7][6] = std::make_shared<Knight>(BLACK);
    squares[7][7] = std::make_shared<Rook>(BLACK);
    for (int i = 0; i < 8; ++i)
    {
        squares[6][i] = std::make_shared<Pawn>(BLACK);
    }

    // Seed the random number generator
    std::srand(std::time(0));
}

void Board::display() const
{
    std::cout << "  ";
    for (int col = 0; col < 8; ++col)
    {
        std::cout << col << ' ';
    }
    std::cout << std::endl;

    for (int row = 7; row >= 0; --row)
    {
        std::cout << row << ' ';
        for (const auto &piece : squares[row])
        {
            if (piece)
            {
                std::cout << piece->getSymbol() << ' ';
            }
            else
            {
                std::cout << ". ";
            }
        }
        std::cout << row;
        std::cout << std::endl;
    }

    std::cout << "  ";
    for (int col = 0; col < 8; ++col)
    {
        std::cout << col << ' ';
    }

    std::cout << std::endl;
}

bool Board::movePiece(int startX, int startY, int endX, int endY)
{
    if (squares[startY][startX])
    {
        auto validMoves = squares[startY][startX]->generateValidMoves(startX, startY, squares);
        for (const auto &move : validMoves)
        {
            if (move.first == endX && move.second == endY)
            {
                squares[endY][endX] = squares[startY][startX];
                squares[startY][startX] = nullptr;
                return true;
            }
        }
    }
    return false;
}

std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> Board::generateAllMoves(Color color) const
{
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> moves;
    for (int startX = 0; startX < 8; ++startX)
    {
        for (int startY = 0; startY < 8; ++startY)
        {
            if (squares[startY][startX] && squares[startY][startX]->getColor() == color)
            {
                auto validMoves = squares[startY][startX]->generateValidMoves(startX, startY, squares);
                for (const auto &move : validMoves)
                {
                    moves.push_back({{startX, startY}, {move.first, move.second}});
                }
            }
        }
    }
    return moves;
}

bool Board::makeAIMove(Color color)
{
    auto bestMove = Minimax::findBestMove(*this, color, 3); // Depth of 3 for simplicity
    return movePiece(bestMove.first.first, bestMove.first.second, bestMove.second.first, bestMove.second.second);
}

std::string Board::toFEN() const
{
    std::string fen;
    for (int row = 7; row >= 0; --row)
    {
        int emptyCount = 0;
        for (int col = 0; col < 8; ++col)
        {
            if (squares[row][col])
            {
                if (emptyCount > 0)
                {
                    fen += std::to_string(emptyCount);
                    emptyCount = 0;
                }
                fen += squares[row][col]->getSymbol();
            }
            else
            {
                ++emptyCount;
            }
        }
        if (emptyCount > 0)
        {
            fen += std::to_string(emptyCount);
        }
        if (row > 0)
        {
            fen += '/';
        }
    }
    // Add additional FEN fields (active color, castling availability, en passant target square, halfmove clock, fullmove number)
    fen += " w KQkq - 0 1"; // Placeholder values
    return fen;
}