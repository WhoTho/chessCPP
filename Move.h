#ifndef MOVE_H
#define MOVE_H

#include <memory>
#include <string>

enum MoveType { NORMAL, PROMOTION, CASTLING, EN_PASSANT };

class Piece;

class Move {
   public:
    Move(int startX,
         int startY,
         int endX,
         int endY,
         std::shared_ptr<Piece> piece,
         MoveType type = NORMAL,
         std::shared_ptr<Piece> capturedPiece = nullptr,
         std::shared_ptr<Piece> promotionPiece = nullptr)
        : startX(startX),
          startY(startY),
          endX(endX),
          endY(endY),
          piece(piece),
          type(type),
          capturedPiece(capturedPiece),
          promotionPiece(promotionPiece) {}

    int startX, startY, endX, endY;
    std::shared_ptr<Piece> piece;
    MoveType type;
    std::shared_ptr<Piece> promotionPiece;
    std::shared_ptr<Piece> capturedPiece;
    std::string toString() const {
        std::string typeToString = "";
        switch (type) {
            case NORMAL:
                typeToString = "NORMAL";
                break;
            case PROMOTION:
                typeToString = "PROMOTION";
                break;
            case CASTLING:
                typeToString = "CASTLING";
                break;
            case EN_PASSANT:
                typeToString = "EN_PASSANT";
                break;
        }
        return "(" + std::to_string(startX) + ", " + std::to_string(startY) +
               ") (" + std::to_string(endX) + ", " + std::to_string(endY) +
               ") " + " " + typeToString;
    };
};

#endif  // MOVE_H