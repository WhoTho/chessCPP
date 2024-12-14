#ifndef PERFTTESTER_H
#define PERFTTESTER_H

#include <string>
#include <vector>
#include "../../Board.h"

std::string readFile(const std::string& filename);
std::vector<std::string> parseFileContents(const std::string& contents);
std::pair<std::string, std::vector<int>> parsePerftTest(
    const std::string& line);
unsigned long long perft(Board* board, int depth, Color color);
bool runPerftTests(const std::vector<std::string>& testCases);
bool runPerftTests();

#endif  // PERFTTESTER_H