#ifndef BOARD_H
#define BOARD_H

#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "HistoryItem.h"
#include "Move.h"
#include "Piece.h"

class Minimax;

class Board {
   public:
    Board();
    void display() const;
    bool makeMove(const Move& move);
    void unmakeMove(const Move& move);
    std::vector<Move> generateAllMoves(Color color, bool legal);
    bool makeAIMove(Color color);
    std::string toFEN() const;
    void displayFEN() const;
    void loadFEN(const std::string& fen);

    std::vector<std::vector<std::shared_ptr<Piece>>> squares;
    std::pair<int, int> enPassantTarget;
    bool whiteKingMoved;
    bool blackKingMoved;
    bool whiteRookMoved[2];
    bool blackRookMoved[2];
    int halfmoveClock;
    int fullmoveNumber;
    Color activeColor;
    std::vector<HistoryItem> history;

   private:
    bool isKingInCheck(Color color) const;
};

#endif  // BOARD_H