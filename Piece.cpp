#include "Piece.h"
#include <iostream>
#include "Board.h"

char King::getSymbol() const {
    return getColor() == WHITE ? 'K' : 'k';
}

std::vector<Move> King::generateValidMoves(int startX,
                                           int startY,
                                           const Board* board) const {
    std::vector<Move> moves;
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0)
                continue;
            int newX = startX + dx;
            int newY = startY + dy;
            if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
                if (!board->squares[newY][newX] ||
                    board->squares[newY][newX]->getColor() != getColor()) {
                    moves.push_back(Move(startX, startY, newX, newY,
                                         std::make_shared<King>(getColor()),
                                         NORMAL, board->squares[newY][newX]));
                }
            }
        }
    }

    // Castling
    if (startY == 0 || startY == 7) {
        if ((getColor() == WHITE && !board->whiteKingMoved &&
             !board->whiteRookMoved[1]) ||
            (getColor() == BLACK && !board->blackKingMoved &&
             !board->blackRookMoved[1])) {
            // King-side castling
            if (board->squares[startY][startX + 1] == nullptr &&
                board->squares[startY][startX + 2] == nullptr &&
                board->squares[startY][startX + 3] &&
                board->squares[startY][startX + 3]->getSymbol() ==
                    (getColor() == WHITE ? 'R' : 'r')) {
                // Check if the king is in check or if any of the squares it
                // passes through or lands on is attacked
                if (!isSquareAttacked(startX, startY, board) &&
                    !isSquareAttacked(startX + 1, startY, board) &&
                    !isSquareAttacked(startX + 2, startY, board)) {
                    moves.push_back(Move(startX, startY, startX + 2, startY,
                                         std::make_shared<King>(getColor()),
                                         CASTLING));
                }
            }
        }
        // Queen-side castling
        if ((getColor() == WHITE && !board->whiteKingMoved &&
             !board->whiteRookMoved[0]) ||
            (getColor() == BLACK && !board->blackKingMoved &&
             !board->blackRookMoved[0])) {
            if (board->squares[startY][startX - 1] == nullptr &&
                board->squares[startY][startX - 2] == nullptr &&
                board->squares[startY][startX - 3] == nullptr &&
                board->squares[startY][startX - 4] &&
                board->squares[startY][startX - 4]->getSymbol() ==
                    (getColor() == WHITE ? 'R' : 'r')) {
                // Check if the king is in check or if any of the squares it
                // passes through or lands on is attacked
                if (!isSquareAttacked(startX, startY, board) &&
                    !isSquareAttacked(startX - 1, startY, board) &&
                    !isSquareAttacked(startX - 2, startY, board)) {
                    moves.push_back(Move(startX, startY, startX - 2, startY,
                                         std::make_shared<King>(getColor()),
                                         CASTLING));
                }
            }
        }
    }

    return moves;
}

bool King::isSquareAttacked(int x, int y, const Board* board) const {
    Color opponentColor = (getColor() == WHITE) ? BLACK : WHITE;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (board->squares[row][col] &&
                board->squares[row][col]->getColor() == opponentColor) {
                if (board->squares[row][col]->getSymbol() ==
                    (opponentColor == WHITE ? 'K' : 'k')) {
                    int dx = std::abs(col - x);
                    int dy = std::abs(row - y);
                    if (dx <= 1 && dy <= 1) {
                        return true;
                    }
                } else {
                    auto opponentMoves =
                        board->squares[row][col]->generateValidMoves(col, row,
                                                                     board);
                    for (const auto& move : opponentMoves) {
                        if (move.endX == x && move.endY == y) {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

char Queen::getSymbol() const {
    return getColor() == WHITE ? 'Q' : 'q';
}

std::vector<Move> Queen::generateValidMoves(int startX,
                                            int startY,
                                            const Board* board) const {
    std::vector<Move> moves;
    // Rook-like moves
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0)
                continue;
            for (int dist = 1; dist < 8; ++dist) {
                int newX = startX + dx * dist;
                int newY = startY + dy * dist;
                if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
                    if (!board->squares[newY][newX]) {
                        moves.push_back(
                            Move(startX, startY, newX, newY,
                                 std::make_shared<Queen>(getColor())));
                    } else {
                        if (board->squares[newY][newX]->getColor() !=
                            getColor()) {
                            moves.push_back(
                                Move(startX, startY, newX, newY,
                                     std::make_shared<Queen>(getColor()),
                                     NORMAL, board->squares[newY][newX]));
                        }
                        break;
                    }
                } else {
                    break;
                }
            }
        }
    }
    return moves;
}

char Rook::getSymbol() const {
    return getColor() == WHITE ? 'R' : 'r';
}

std::vector<Move> Rook::generateValidMoves(int startX,
                                           int startY,
                                           const Board* board) const {
    std::vector<Move> moves;
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx != 0 && dy != 0)
                continue;
            if (dx == 0 && dy == 0)
                continue;
            for (int dist = 1; dist < 8; ++dist) {
                int newX = startX + dx * dist;
                int newY = startY + dy * dist;
                if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
                    if (!board->squares[newY][newX]) {
                        moves.push_back(
                            Move(startX, startY, newX, newY,
                                 std::make_shared<Rook>(getColor())));
                    } else {
                        if (board->squares[newY][newX]->getColor() !=
                            getColor()) {
                            moves.push_back(
                                Move(startX, startY, newX, newY,
                                     std::make_shared<Rook>(getColor()), NORMAL,
                                     board->squares[newY][newX]));
                        }
                        break;
                    }
                } else {
                    break;
                }
            }
        }
    }
    return moves;
}

char Bishop::getSymbol() const {
    return getColor() == WHITE ? 'B' : 'b';
}

std::vector<Move> Bishop::generateValidMoves(int startX,
                                             int startY,
                                             const Board* board) const {
    std::vector<Move> moves;
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 || dy == 0)
                continue;
            for (int dist = 1; dist < 8; ++dist) {
                int newX = startX + dx * dist;
                int newY = startY + dy * dist;
                if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
                    if (!board->squares[newY][newX]) {
                        moves.push_back(
                            Move(startX, startY, newX, newY,
                                 std::make_shared<Bishop>(getColor())));
                    } else {
                        if (board->squares[newY][newX]->getColor() !=
                            getColor()) {
                            moves.push_back(
                                Move(startX, startY, newX, newY,
                                     std::make_shared<Bishop>(getColor()),
                                     NORMAL, board->squares[newY][newX]));
                        }
                        break;
                    }
                } else {
                    break;
                }
            }
        }
    }
    return moves;
}

char Knight::getSymbol() const {
    return getColor() == WHITE ? 'N' : 'n';
}

std::vector<Move> Knight::generateValidMoves(int startX,
                                             int startY,
                                             const Board* board) const {
    std::vector<Move> moves;
    int dx[] = {1, 1, 2, 2, -1, -1, -2, -2};
    int dy[] = {2, -2, 1, -1, 2, -2, 1, -1};
    for (int i = 0; i < 8; ++i) {
        int newX = startX + dx[i];
        int newY = startY + dy[i];
        if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
            if (!board->squares[newY][newX] ||
                board->squares[newY][newX]->getColor() != getColor()) {
                moves.push_back(Move(startX, startY, newX, newY,
                                     std::make_shared<Knight>(getColor()),
                                     NORMAL, board->squares[newY][newX]));
            }
        }
    }
    return moves;
}

char Pawn::getSymbol() const {
    return getColor() == WHITE ? 'P' : 'p';
}

std::vector<Move> Pawn::generateValidMoves(int startX,
                                           int startY,
                                           const Board* board) const {
    std::vector<Move> moves;
    int direction = (getColor() == WHITE) ? 1 : -1;
    int startRow = (getColor() == WHITE) ? 1 : 6;
    int promotionRow = (getColor() == WHITE) ? 7 : 0;

    // Move forward
    int newY = startY + direction;
    if (newY >= 0 && newY < 8 && !board->squares[newY][startX]) {
        if (newY == promotionRow) {
            // Promotion moves
            moves.push_back(Move(startX, startY, startX, newY,
                                 std::make_shared<Pawn>(getColor()), PROMOTION,
                                 nullptr, std::make_shared<Queen>(getColor())));
            moves.push_back(Move(startX, startY, startX, newY,
                                 std::make_shared<Pawn>(getColor()), PROMOTION,
                                 nullptr, std::make_shared<Rook>(getColor())));
            moves.push_back(Move(startX, startY, startX, newY,
                                 std::make_shared<Pawn>(getColor()), PROMOTION,
                                 nullptr,
                                 std::make_shared<Bishop>(getColor())));
            moves.push_back(Move(startX, startY, startX, newY,
                                 std::make_shared<Pawn>(getColor()), PROMOTION,
                                 nullptr,
                                 std::make_shared<Knight>(getColor())));
        } else {
            moves.push_back(Move(startX, startY, startX, newY,

                                 std::make_shared<Pawn>(getColor())));
            // Move two squares forward from starting position
            if (startY == startRow &&
                !board->squares[newY + direction][startX]) {
                moves.push_back(Move(startX, startY, startX, newY + direction,
                                     std::make_shared<Pawn>(getColor())));
            }
        }
    }

    // Capture diagonally
    for (int dx = -1; dx <= 1; dx += 2) {
        int newX = startX + dx;
        if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8 &&
            board->squares[newY][newX] &&
            board->squares[newY][newX]->getColor() != getColor()) {
            if (newY == promotionRow) {
                // Promotion capture moves
                moves.push_back(Move(startX, startY, newX, newY,
                                     std::make_shared<Pawn>(getColor()),
                                     PROMOTION, board->squares[newY][newX],
                                     std::make_shared<Queen>(getColor())));
                moves.push_back(Move(startX, startY, newX, newY,
                                     std::make_shared<Pawn>(getColor()),
                                     PROMOTION, board->squares[newY][newX],
                                     std::make_shared<Rook>(getColor())));
                moves.push_back(Move(startX, startY, newX, newY,
                                     std::make_shared<Pawn>(getColor()),
                                     PROMOTION, board->squares[newY][newX],
                                     std::make_shared<Bishop>(getColor())));
                moves.push_back(Move(startX, startY, newX, newY,
                                     std::make_shared<Pawn>(getColor()),
                                     PROMOTION, board->squares[newY][newX],
                                     std::make_shared<Knight>(getColor())));
            } else {
                moves.push_back(Move(startX, startY, newX, newY,
                                     std::make_shared<Pawn>(getColor()), NORMAL,
                                     board->squares[newY][newX]));
            }
        }
    }

    // En passant
    if (board->enPassantTarget.first != -1 &&
        board->enPassantTarget.second != -1) {
        if (std::abs(board->enPassantTarget.first - startX) == 1 &&
            board->enPassantTarget.second == newY) {
            moves.push_back(Move(startX, startY, board->enPassantTarget.first,
                                 board->enPassantTarget.second,
                                 std::make_shared<Pawn>(getColor()),
                                 EN_PASSANT));
        }
    }

    return moves;
}