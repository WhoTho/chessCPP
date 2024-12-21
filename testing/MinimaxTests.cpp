#include <limits>
#include "Board.h"
#include "Minimax.h"
#include "Move.h"
#include "catch2/catch_test_macros.hpp"

TEST_CASE("Minimax::evaluateBoard") {
    Board board;
    int score = Minimax::evaluateBoard(board, WHITE);
    REQUIRE(score == 0);  // Initial board should have a score of 0
}

TEST_CASE("Minimax::findBestMove alphaBeta is same as minimax") {
    const std::vector<std::string> FENS = {
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
        "4k3/8/8/8/8/8/8/4K2R w K - 0 1",
        "4k2r/8/8/8/8/8/8/4K3 w k - 0 1",
    };
    // smaller subset for faster testing
    // "4k3/8/8/8/8/8/8/R3K2R w KQ - 0 1",
    // "8/8/8/8/8/8/1k6/R3K3 w Q - 0 1",
    // "r3k3/1K6/8/8/8/8/8/8 w q - 0 1",
    // "r3k2r/8/8/8/8/8/8/1R2K2R w Kkq - 0 1",
    // "r3k2r/8/8/8/8/8/8/2R1K2R w Kkq - 0 1",
    // "1r2k2r/8/8/8/8/8/8/R3K2R w KQk - 0 1",
    // "r3k3/8/8/8/8/8/8/4K3 b q - 0 1",
    // "r3k2r/8/8/8/8/8/8/4K3 b kq - 0 1",
    // "8/8/8/8/8/8/6k1/4K2R b K - 0 1",
    // "8/8/8/8/8/8/1k6/R3K3 b Q - 0 1",
    // "r3k3/1K6/8/8/8/8/8/8 b q - 0 1",
    // "r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1",
    // "1r2k2r/8/8/8/8/8/8/R3K2R b KQk - 0 1",
    // "8/1n4N1/2k5/8/8/5K2/1N4n1/8 w - - 0 1",
    // "8/1k6/8/5N2/8/4n3/8/2K5 w - - 0 1",
    // "8/8/4k3/3Nn3/3nN3/4K3/8/8 w - - 0 1",
    // "K7/8/2n5/1n6/8/8/8/k6N w - - 0 1",
    // "k7/8/2N5/1N6/8/8/8/K6n w - - 0 1",
    // "8/8/3K4/3Nn3/3nN3/4k3/8/8 b - - 0 1",
    // "K7/8/2n5/1n6/8/8/8/k6N b - - 0 1",
    // "B6b/8/8/8/2K5/4k3/8/b6B w - - 0 1",
    // "8/8/1B6/7b/7k/8/2B1b3/7K w - - 0 1",
    // "K7/b7/1b6/1b6/8/8/8/k6B w - - 0 1",
    // "8/8/1B6/7b/7k/8/2B1b3/7K b - - 0 1",
    // "k7/B7/1B6/1B6/8/8/8/K6b b - - 0 1",
    // "7k/RR6/8/8/8/8/rr6/7K w - - 0 1",
    // "R6r/8/8/2K5/5k2/8/8/r6R w - - 0 1",
    // "7k/RR6/8/8/8/8/rr6/7K b - - 0 1",
    // "R6r/8/8/2K5/5k2/8/8/r6R b - - 0 1",
    // "6kq/8/8/8/8/8/8/7K w - - 0 1",
    // "K7/8/8/3Q4/4q3/8/8/7k w - - 0 1",
    // "6KQ/8/8/8/8/8/8/7k b - - 0 1",
    // "K7/8/8/3Q4/4q3/8/8/7k b - - 0 1",
    // "8/8/8/8/8/K7/P7/k7 w - - 0 1",
    // "7K/7p/7k/8/8/8/8/8 w - - 0 1",
    // "8/2k1p3/3pP3/3P2K1/8/8/8/8 w - - 0 1",
    // "8/8/8/8/8/K7/P7/k7 b - - 0 1",
    // "8/8/8/8/8/7K/7P/7k b - - 0 1",
    // "8/2k1p3/3pP3/3P2K1/8/8/8/8 b - - 0 1",
    // "4k3/4p3/4K3/8/8/8/8/8 b - - 0 1",
    // "8/8/7k/7p/7P/7K/8/8 w - - 0 1",
    // "8/8/k7/p7/P7/K7/8/8 w - - 0 1",
    // "8/8/3k4/3p4/3P4/3K4/8/8 w - - 0 1",
    // "8/3k4/3p4/8/3P4/3K4/8/8 w - - 0 1",
    // "8/8/3k4/3p4/3P4/3K4/8/8 b - - 0 1",
    // "8/3k4/3p4/8/3P4/3K4/8/8 b - - 0 1",
    // "k7/8/8/7p/6P1/8/8/K7 w - - 0 1",
    // "k7/8/7p/8/8/6P1/8/K7 w - - 0 1",
    // "k7/8/6p1/8/8/7P/8/K7 w - - 0 1",
    // "7k/3p4/8/8/3P4/8/8/K7 b - - 0 1",
    // "7k/8/8/3p4/8/8/3P4/K7 b - - 0 1",
    // "k7/8/8/6p1/7P/8/8/K7 b - - 0 1",
    // "k7/8/6p1/8/8/7P/8/K7 b - - 0 1",
    // "k7/8/8/3p4/4p3/8/8/7K b - - 0 1",
    // "k7/8/3p4/8/8/4P3/8/7K b - - 0 1",
    // "k7/7p/8/8/8/8/6P1/K7 w - - 0 1",
    // "k7/6p1/8/8/8/8/7P/K7 w - - 0 1",
    // "3k4/3pp3/8/8/8/8/3PP3/3K4 w - - 0 1",
    // "7k/8/8/p7/1P6/8/8/7K b - - 0 1",
    // "7k/8/1p6/8/8/P7/8/7K b - - 0 1",
    // "k7/7p/8/8/8/8/6P1/K7 b - - 0 1",
    // "k7/6p1/8/8/8/8/7P/K7 b - - 0 1",
    // "3k4/3pp3/8/8/8/8/3PP3/3K4 b - - 0 1",
    // "8/PPPk4/8/8/8/8/4Kppp/8 w - - 0 1",
    // "n1n5/PPPk4/8/8/8/8/4Kppp/5N1N w - - 0 1",
    // "8/PPPk4/8/8/8/8/4Kppp/8 b - - 0 1",
    // "n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1"};

    for (const auto& fen : FENS) {
        Board board;
        board.loadFEN(fen);
        Move minimaxMove =
            Minimax::findBestMove(board, board.activeColor, 3, false);
        Move alphaBetaMove =
            Minimax::findBestMove(board, board.activeColor, 3, true);
        REQUIRE(minimaxMove.startX == alphaBetaMove.startX);
        REQUIRE(minimaxMove.startY == alphaBetaMove.startY);
        REQUIRE(minimaxMove.endX == alphaBetaMove.endX);
        REQUIRE(minimaxMove.endY == alphaBetaMove.endY);
    }
}