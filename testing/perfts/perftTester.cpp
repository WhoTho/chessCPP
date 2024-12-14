#include "perftTester.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

const int MAX_NODES_PER_TEST = 10000;

// Function to read the contents of a file
std::string readFile(const std::string& filename) {
    std::ifstream file("testing/perfts/" + filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Function to parse the contents of a file into a vector of strings
std::vector<std::string> parseFileContents(const std::string& contents) {
    std::vector<std::string> lines;
    std::stringstream ss(contents);
    std::string line;
    while (std::getline(ss, line)) {
        if (!line.empty() && line[0] == '#') {
            continue;  // Ignore lines that start with '#'
        }
        lines.push_back(line);
    }
    return lines;
}

std::pair<std::string, std::vector<int>> parsePerftTest(
    const std::string& line) {
    // std::cout << "Parsing perft test: " << line << std::endl;
    std::stringstream ss(line);
    std::string fen;
    std::vector<int> nodesAtDepth;
    std::getline(ss, fen, ';');
    std::string depthStr;
    while (std::getline(ss, depthStr, ';')) {
        // std::cout << "Parsing depth: " << depthStr << std::endl;
        std::stringstream depthStream(depthStr);
        std::string depthLabel;
        int nodes;
        depthStream >> depthLabel >> nodes;
        // std::cout << "Parsed depth: " << depthLabel << " " << nodes
        // << std::endl;
        if (nodes > MAX_NODES_PER_TEST) {
            break;
        }
        nodesAtDepth.push_back(nodes);
    }
    // std::cout << "Parsed perft test: " << fen << std::endl;
    return {fen, nodesAtDepth};
}

// Function to run perft tests on the board
bool runPerftTests(const std::vector<std::string>& testCases) {
    bool failed = false;
    std::ofstream failedFile("testing/perfts/failedPerfts.txt",
                             std::ios::out | std::ios::app);
    if (!failedFile.is_open()) {
        std::cerr << "Error: Could not open failedPerfts.txt for writing."
                  << std::endl;
        return false;
    }
    std::cout << "Running perft tests... in thingy" << std::endl;
    for (const auto& testCase : testCases) {
        std::cout << "Running perft test: " << testCase << std::endl;
        auto [fen, nodesAtDepth] = parsePerftTest(testCase);
        // std::cout << "Running perft tests... " << fen << std::endl;
        for (int depth = 1; depth <= nodesAtDepth.size(); ++depth) {
            // std::cout << "Running test for depth " << depth << std::endl;
            Board board;
            board.loadFEN(fen);
            // std::cout << "Loaded FEN" << std::endl;
            unsigned long long nodes = perft(&board, depth, board.activeColor);
            // std::cout << "Got nodes: " << nodes << std::endl;
            if (nodes != nodesAtDepth[depth - 1]) {
                std::cerr << "Perft test failed for FEN: " << fen
                          << " at depth " << depth << std::endl;
                std::cerr << "Expected: " << nodesAtDepth[depth - 1]
                          << " nodes, got: " << nodes << " nodes" << std::endl;
                failedFile << testCase << std::endl;
                failed = true;
                break;
            }
        }
    }

    if (!failed) {
        std::cout << "All perft tests passed!" << std::endl;
    }

    // failedFile.close();
    return !failed;
}

unsigned long long perft(Board* board, int depth, Color color) {
    if (depth == 0) {
        return 1;
    }

    unsigned long long nodes = 0;
    // std::cout << "Generating moves for depth " << depth << " and color "
    //           << (color == WHITE ? "WHITE" : "BLACK") << std::endl;
    // board->display();
    auto moves = board->generateAllMoves(color, true);
    // std::cout << "Generated moves for depth " << depth << std::endl;
    for (const auto& move : moves) {
        // std::cout << "Making move for depth " << depth << std::endl;
        // if (depth == 4) {
        //     board->display();
        //     std::cout << "before move: " << board->toFEN() << std::endl;
        // }
        board->makeMove(move);
        // std::cout << "Made move for depth " << depth << std::endl;
        // if (depth == 4) {
        //     board->display();
        //     std::cout << "after move: " << board->toFEN() << std::endl;
        // }
        nodes += perft(board, depth - 1, color == WHITE ? BLACK : WHITE);
        // std::cout << "Unmaking move for depth " << depth << std::endl;
        board->unmakeMove(move);
        // std::cout << "Unmade move for depth " << depth << std::endl;
    }
    return nodes;
}

bool runPerftTests() {
    // return runPerftTests(
    //     {"r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1 ;D1 26 ;D2 568 ;D3 13744 ;D4 "
    //      "314346 ;D5 7594526 ;D6 179862938"});
    // Read the contents of the three text files
    // std::vector<std::string> filesToRead = {"customPerfts.txt"};
    std::vector<std::string> filesToRead = {
        "basicPerfts.txt", "complexPerfts.txt", "specialMovesPerfts.txt",
        "customPerfts.txt"};
    std::vector<std::string> allTestCases;

    for (const auto& file : filesToRead) {
        std::string fileContents = readFile(file);
        std::vector<std::string> testCases = parseFileContents(fileContents);
        allTestCases.insert(allTestCases.end(), testCases.begin(),
                            testCases.end());
    }

    return runPerftTests(allTestCases);
}