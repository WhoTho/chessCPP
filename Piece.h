#ifndef PIECE_H
#define PIECE_H

#include <memory>
#include <vector>
#include "Move.h"

class Board;

enum Color { WHITE, BLACK };

class Piece {
   public:
    Piece(Color color) : color(color) {}
    virtual ~Piece() {}
    virtual char getSymbol() const = 0;
    Color getColor() const { return color; }
    virtual std::vector<Move> generateValidMoves(int startX,
                                                 int startY,
                                                 const Board* board) const = 0;

   private:
    Color color;
};

class King : public Piece {
   public:
    King(Color color) : Piece(color) {}
    char getSymbol() const;
    std::vector<Move> generateValidMoves(int startX,
                                         int startY,
                                         const Board* board) const override;

   private:
    bool isSquareAttacked(int x, int y, const Board* board) const;
};

class Queen : public Piece {
   public:
    Queen(Color color) : Piece(color) {}
    char getSymbol() const;
    std::vector<Move> generateValidMoves(int startX,
                                         int startY,
                                         const Board* board) const override;
};

class Rook : public Piece {
   public:
    Rook(Color color) : Piece(color) {}
    char getSymbol() const;
    std::vector<Move> generateValidMoves(int startX,
                                         int startY,
                                         const Board* board) const override;
};

class Bishop : public Piece {
   public:
    Bishop(Color color) : Piece(color) {}
    char getSymbol() const;
    std::vector<Move> generateValidMoves(int startX,
                                         int startY,
                                         const Board* board) const override;
};

class Knight : public Piece {
   public:
    Knight(Color color) : Piece(color) {}
    char getSymbol() const;
    std::vector<Move> generateValidMoves(int startX,
                                         int startY,
                                         const Board* board) const override;
};

class Pawn : public Piece {
   public:
    Pawn(Color color) : Piece(color) {}
    char getSymbol() const;
    std::vector<Move> generateValidMoves(int startX,
                                         int startY,
                                         const Board* board) const override;
};

#endif  // PIECE_H