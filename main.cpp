#include <cassert>
#include <iostream>
#include <limits>
#include "Board.h"
#include "Minimax.h"
// #include "testing/perfts/perftTester.h"

void testToFEN() {
    Board board;
    std::string expectedFEN =
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    // std::cout << board.toFEN() << std::endl;
    assert(board.toFEN() == expectedFEN);
    std::cout << "testToFEN passed!" << std::endl;
}

void testUnmakeMove() {
    Board board;
    Board originalBoard = board;
    Move move = Move(1, 1, 1, 3, std::make_shared<Pawn>(WHITE));
    board.makeMove(move);
    board.display();
    board.unmakeMove(move);
    board.display();

    std::cout << "Original FEN: " << originalBoard.toFEN() << std::endl;
    std::cout << "Current FEN: " << board.toFEN() << std::endl;
    assert(board.toFEN() == originalBoard.toFEN());
    std::cout << "testUnmakeMove passed!" << std::endl;
}

void testMoveGeneration() {
    Board board;
    board.loadFEN("kn6/nn6/8/5Pp1/8/8/8/K7 w - g6 0 1");
    /*
    kn......
    n.......
    ........
    .....Pp.
    ........
    ........
    ........
    K.......
    */
    board.display();

    for (const auto& move : board.generateAllMoves(WHITE, true)) {
        board.makeMove(move);
        board.display();
        board.unmakeMove(move);
    }
    board.display();
    std::cout << "testMoveGeneration passed!" << std::endl;
}

void printAllMovesForFEN(const std::string& fen) {
    Board board;
    board.loadFEN(fen);
    std::cout << board.whiteKingMoved << " " << board.blackKingMoved << " "
              << board.whiteRookMoved[0] << " " << board.whiteRookMoved[1]
              << " " << board.blackRookMoved[0] << " "
              << board.blackRookMoved[1] << std::endl;
    board.display();
    int totalMoves = 0;
    for (const auto& move : board.generateAllMoves(board.activeColor, true)) {
        board.makeMove(move);
        board.display();
        board.unmakeMove(move);
        std::cout << std::endl;
        ++totalMoves;
    }
    std::cout << "Total moves: " << totalMoves << std::endl;
}

void testCastling() {
    Board board;
    // board.loadFEN("4k3/8/8/8/8/8/8/4K2R w K - 0 1");
    board.loadFEN("4k2r/8/8/8/8/8/8/4K3 b k - 0 1");

    // int totalMoves = 15;
    int totalMoves = 15;

    assert(board.generateAllMoves(board.activeColor, true).size() ==
           totalMoves);
}

void testEvaluateBoard() {
    Board board;
    int score = Minimax::evaluateBoard(board, WHITE);
    std::cout << "Score: " << score << std::endl;
    assert(score == 0);  // Initial board should have a score of 0
    std::cout << "testEvaluateBoard passed!" << std::endl;
}

void runTests() {
    testCastling();
    // testMoveGeneration();
    testToFEN();
    testEvaluateBoard();
    testUnmakeMove();
    std::cout << "All tests passed!" << std::endl;
}

int main() {
    // Run tests
    // testMoveGeneration();
    // return 0;
    // runTests();
    // testCastling();
    // printAllMovesForFEN(
    //     "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    // return 0;

    // if (!runPerftTests()) {
    //     std::cout << "Perft tests failed!" << std::endl;
    //     return 1;
    // }

    // Start the game
    Board board;
    board.display();

    int startX, startY, endX, endY;
    while (true) {
        std::cout << "Enter move (startX startY endX endY): ";
        std::cin >> startX >> startY >> endX >> endY;

        // Check if the input is valid
        if (std::cin.fail()) {
            std::cin.clear();  // Clear the error state
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                            '\n');  // Ignore the invalid input
            std::cout << "Invalid input! Please enter four integers."
                      << std::endl;
            continue;
        }

        // Check if the input is within the bounds of the board
        if (startX < 0 || startX >= 8 || startY < 0 || startY >= 8 ||
            endX < 0 || endX >= 8 || endY < 0 || endY >= 8) {
            std::cout << "Invalid input! Coordinates must be between 0 and 7."
                      << std::endl;
            continue;
        }

        if (board.makeMove(Move(startX, startY, endX, endY,
                                board.squares[startY][startX]))) {
            board.display();
            std::cout << "AI is making a move..." << std::endl;
            if (board.makeAIMove(BLACK)) {
                board.display();
            } else {
                std::cout << "AI has no valid moves!" << std::endl;
            }
        } else {
            std::cout << "Invalid move!" << std::endl;
        }
    }

    return 0;
}
