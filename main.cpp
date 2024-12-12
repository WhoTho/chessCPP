#include "Board.h"
#include "Minimax.h"
#include <iostream>
#include <limits>
#include <cassert>

// Test functions
void testMovePiece()
{
    Board board;
    assert(board.movePiece(0, 1, 0, 2));  // Move white pawn
    assert(!board.movePiece(0, 0, 0, 2)); // Invalid move for rook
    std::cout << "testMovePiece passed!" << std::endl;
}

void testToFEN()
{
    Board board;
    std::string expectedFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    // std::cout << board.toFEN() << std::endl;
    assert(board.toFEN() == expectedFEN);
    std::cout << "testToFEN passed!" << std::endl;
}

void testEvaluateBoard()
{
    Board board;
    int score = Minimax::evaluateBoard(board, WHITE);
    std::cout << "Score: " << score << std::endl;
    assert(score == 0); // Initial board should have a score of 0
    std::cout << "testEvaluateBoard passed!" << std::endl;
}

void testFindBestMove()
{
    Board board;
    auto bestMove = Minimax::findBestMove(board, WHITE, 1);
    assert(board.movePiece(bestMove.first.first, bestMove.first.second, bestMove.second.first, bestMove.second.second));
    std::cout << "testFindBestMove passed!" << std::endl;
}

void runTests()
{
    testMovePiece();
    testToFEN();
    testEvaluateBoard();
    testFindBestMove();
    std::cout << "All tests passed!" << std::endl;
}

// Perft function
unsigned long long perft(Board &board, int depth, Color color = WHITE)
{
    if (depth == 0)
    {
        return 1;
    }

    unsigned long long nodes = 0;
    auto moves = board.generateAllMoves(color); // Assuming WHITE to start, you can parameterize this
    for (const auto &move : moves)
    {
        Board tempBoard = board;
        if (tempBoard.movePiece(move.first.first, move.first.second, move.second.first, move.second.second))
        {
            // tempBoard.display();
            // std::cout << std::endl;
            nodes += perft(tempBoard, depth - 1, color == WHITE ? BLACK : WHITE);
        }
    }
    return nodes;
}

void runPerftTests()
{
    Board board;
    for (int depth = 1; depth <= 4; ++depth)
    {
        unsigned long long nodes = perft(board, depth);
        std::cout << "Depth " << depth << ": " << nodes << " nodes" << std::endl;
    }
}

int main()
{
    // Run tests
    runTests();
    runPerftTests();

    // Start the game
    Board board;
    board.display();

    int startX, startY, endX, endY;
    while (true)
    {
        std::cout << "Enter move (startX startY endX endY): ";
        std::cin >> startX >> startY >> endX >> endY;

        // Check if the input is valid
        if (std::cin.fail())
        {
            std::cin.clear();                                                   // Clear the error state
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore the invalid input
            std::cout << "Invalid input! Please enter four integers." << std::endl;
            continue;
        }

        // Check if the input is within the bounds of the board
        if (startX < 0 || startX >= 8 || startY < 0 || startY >= 8 ||
            endX < 0 || endX >= 8 || endY < 0 || endY >= 8)
        {
            std::cout << "Invalid input! Coordinates must be between 0 and 7." << std::endl;
            continue;
        }

        if (board.movePiece(startX, startY, endX, endY))
        {
            board.display();
            std::cout << "AI is making a move..." << std::endl;
            if (board.makeAIMove(BLACK))
            {
                board.display();
            }
            else
            {
                std::cout << "AI has no valid moves!" << std::endl;
            }
        }
        else
        {
            std::cout << "Invalid move!" << std::endl;
        }
    }

    return 0;
}