#include "Board.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
#include "Minimax.h"
#include "Move.h"

Board::Board()
    : squares(8, std::vector<std::shared_ptr<Piece>>(8, nullptr)),
      enPassantTarget({-1, -1}),
      whiteKingMoved(false),
      blackKingMoved(false),
      whiteRookMoved{false, false},
      blackRookMoved{false, false},
      halfmoveClock(0),
      fullmoveNumber(1),
      activeColor(WHITE) {
    // Initialize the board with pieces
    // Black pieces
    squares[0][0] = std::make_shared<Rook>(WHITE);
    squares[0][1] = std::make_shared<Knight>(WHITE);
    squares[0][2] = std::make_shared<Bishop>(WHITE);
    squares[0][3] = std::make_shared<Queen>(WHITE);
    squares[0][4] = std::make_shared<King>(WHITE);
    squares[0][5] = std::make_shared<Bishop>(WHITE);
    squares[0][6] = std::make_shared<Knight>(WHITE);
    squares[0][7] = std::make_shared<Rook>(WHITE);
    for (int i = 0; i < 8; ++i) {
        squares[1][i] = std::make_shared<Pawn>(WHITE);
    }

    // White pieces
    squares[7][0] = std::make_shared<Rook>(BLACK);
    squares[7][1] = std::make_shared<Knight>(BLACK);
    squares[7][2] = std::make_shared<Bishop>(BLACK);
    squares[7][3] = std::make_shared<Queen>(BLACK);
    squares[7][4] = std::make_shared<King>(BLACK);
    squares[7][5] = std::make_shared<Bishop>(BLACK);
    squares[7][6] = std::make_shared<Knight>(BLACK);
    squares[7][7] = std::make_shared<Rook>(BLACK);
    for (int i = 0; i < 8; ++i) {
        squares[6][i] = std::make_shared<Pawn>(BLACK);
    }

    // Seed the random number generator
    // std::srand(std::time(0));
}

void Board::display() const {
    std::cout << "  ";
    for (int col = 0; col < 8; ++col) {
        std::cout << col << ' ';
    }
    std::cout << std::endl;

    for (int row = 7; row >= 0; --row) {
        std::cout << row << ' ';
        for (const auto& piece : squares[row]) {
            if (piece) {
                std::cout << piece->getSymbol() << ' ';
            } else {
                std::cout << ". ";
            }
        }
        std::cout << row << std::endl;
    }

    std::cout << "  ";
    for (int col = 0; col < 8; ++col) {
        std::cout << col << ' ';
    }
    std::cout << std::endl;
}

bool Board::makeMove(const Move& move) {
    if (!squares[move.startY][move.startX]) {
        return false;
    }

    // add to history
    history.push_back(HistoryItem(
        enPassantTarget, whiteKingMoved, blackKingMoved, whiteRookMoved,
        blackRookMoved, halfmoveClock, fullmoveNumber));

    // Handle en passant
    if (move.type == EN_PASSANT) {
        squares[move.startY][move.endX] = nullptr;
    }

    if (move.type == NORMAL && move.piece->getSymbol() == 'P' &&
        move.startY == 1 && move.endY == 3) {
        enPassantTarget = {move.startX, 2};
    } else if (move.type == NORMAL && move.piece->getSymbol() == 'p' &&
               move.startY == 6 && move.endY == 4) {
        enPassantTarget = {move.startX, 5};
    } else {
        enPassantTarget = {-1, -1};
    }

    // Handle castling
    if (move.type == CASTLING) {
        if (move.endX > move.startX) {
            squares[move.endY][move.endX - 1] =
                squares[move.endY][move.endX + 1];
            squares[move.endY][move.endX + 1] = nullptr;
            if (move.endY == 0) {
                whiteRookMoved[1] = true;
                whiteKingMoved = true;
            } else {
                blackRookMoved[1] = true;
                blackKingMoved = true;
            }
        } else {
            squares[move.endY][move.endX + 1] =
                squares[move.endY][move.endX - 2];
            squares[move.endY][move.endX - 2] = nullptr;
            if (move.endY == 0) {
                whiteRookMoved[0] = true;
                whiteKingMoved = true;
            } else {
                blackRookMoved[0] = true;
                blackKingMoved = true;
            }
        }
    } else if (move.startY == 0 && move.startX == 4) {
        whiteKingMoved = true;
    } else if (move.startY == 7 && move.startX == 4) {
        blackKingMoved = true;
    } else if (move.startY == 0 && move.startX == 0) {
        whiteRookMoved[0] = true;
    } else if (move.startY == 0 && move.startX == 7) {
        whiteRookMoved[1] = true;
    } else if (move.startY == 7 && move.startX == 0) {
        blackRookMoved[0] = true;
    } else if (move.startY == 7 && move.startX == 7) {
        blackRookMoved[1] = true;
    }

    // Handle promotion
    if (move.type == PROMOTION) {
        squares[move.endY][move.endX] = move.promotionPiece;
    } else {
        squares[move.endY][move.endX] = squares[move.startY][move.startX];
    }

    // Update halfmove clock
    if (squares[move.endY][move.endX]->getSymbol() == 'P' ||
        squares[move.endY][move.endX]->getSymbol() == 'p' ||
        squares[move.endY][move.endX]) {
        halfmoveClock = 0;
    } else {
        ++halfmoveClock;
    }

    // Update fullmove number
    if (activeColor == BLACK) {
        ++fullmoveNumber;
    }

    // Switch active color
    activeColor = (activeColor == WHITE) ? BLACK : WHITE;

    squares[move.startY][move.startX] = nullptr;
    return true;
}

void Board::unmakeMove(const Move& move) {
    squares[move.startY][move.startX] = squares[move.endY][move.endX];
    squares[move.endY][move.endX] = move.capturedPiece;

    // Handle en passant
    if (move.type == EN_PASSANT) {
        squares[move.startY][move.endX] = std::make_shared<Pawn>(activeColor);
    }

    // Handle castling
    if (move.type == CASTLING) {
        if (move.endX > move.startX) {
            squares[move.endY][move.endX + 1] =
                squares[move.endY][move.endX - 1];
            squares[move.endY][move.endX - 1] = nullptr;
        } else {
            squares[move.endY][move.endX - 2] =
                squares[move.endY][move.endX + 1];
            squares[move.endY][move.endX + 1] = nullptr;
        }
    }

    // Handle promotion
    if (move.type == PROMOTION) {
        squares[move.startY][move.startX] =
            std::make_shared<Pawn>(activeColor == WHITE ? BLACK : WHITE);
    }

    // Switch active color back
    activeColor = (activeColor == WHITE) ? BLACK : WHITE;

    // Restore history
    enPassantTarget = history.back().enPassantTarget;
    whiteKingMoved = history.back().whiteKingMoved;
    blackKingMoved = history.back().blackKingMoved;
    whiteRookMoved[0] = history.back().whiteRookMoved[0];
    whiteRookMoved[1] = history.back().whiteRookMoved[1];
    blackRookMoved[0] = history.back().blackRookMoved[0];
    blackRookMoved[1] = history.back().blackRookMoved[1];
    halfmoveClock = history.back().halfmoveClock;
    fullmoveNumber = history.back().fullmoveNumber;
    history.pop_back();
}

std::vector<Move> Board::generateAllMoves(Color color, bool legal) {
    std::vector<Move> moves;
    for (int startX = 0; startX < 8; ++startX) {
        for (int startY = 0; startY < 8; ++startY) {
            if (squares[startY][startX] &&
                squares[startY][startX]->getColor() == color) {
                auto validMoves = squares[startY][startX]->generateValidMoves(
                    startX, startY, this);
                for (const auto& move : validMoves) {
                    if (legal) {
                        makeMove(move);
                        // display();
                        if (!isKingInCheck(color)) {
                            moves.push_back(move);
                        }
                        unmakeMove(move);
                    } else {
                        moves.push_back(move);
                    }
                }
            }
        }
    }
    return moves;
}

bool Board::isKingInCheck(Color color) const {
    int kingX = -1, kingY = -1;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (squares[row][col] && squares[row][col]->getSymbol() ==
                                         (color == WHITE ? 'K' : 'k')) {
                kingX = col;
                kingY = row;
                break;
            }
        }
        if (kingX != -1)
            break;
    }

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (squares[row][col] && squares[row][col]->getColor() != color) {
                auto opponentMoves =
                    squares[row][col]->generateValidMoves(col, row, this);
                for (const auto& move : opponentMoves) {
                    if (move.endX == kingX && move.endY == kingY) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool Board::makeAIMove(Color color) {
    Move bestMove =
        Minimax::findBestMove(*this, color, 3);  // Depth of 3 for simplicity
    return makeMove(bestMove);
}

std::string Board::toFEN() const {
    std::stringstream fen;
    for (int row = 7; row >= 0; --row) {
        int emptyCount = 0;
        for (int col = 0; col < 8; ++col) {
            if (squares[row][col]) {
                if (emptyCount > 0) {
                    fen << emptyCount;
                    emptyCount = 0;
                }
                fen << squares[row][col]->getSymbol();
            } else {
                ++emptyCount;
            }
        }
        if (emptyCount > 0) {
            fen << emptyCount;
        }
        if (row > 0) {
            fen << '/';
        }
    }

    // Active color
    fen << " " << (activeColor == WHITE ? 'w' : 'b') << " ";

    // Castling availability
    std::string castling = "";
    if (!whiteKingMoved) {
        if (!whiteRookMoved[1])
            castling += "K";
        if (!whiteRookMoved[0])
            castling += "Q";
    }
    if (!blackKingMoved) {
        if (!blackRookMoved[1])
            castling += "k";
        if (!blackRookMoved[0])
            castling += "q";
    }
    if (castling.empty())
        castling = "-";
    fen << castling << " ";

    // En passant target square
    if (enPassantTarget.first != -1 && enPassantTarget.second != -1) {
        fen << static_cast<char>('a' + enPassantTarget.first)
            << (enPassantTarget.second + 1);
    } else {
        fen << "-";
    }

    // Halfmove clock and fullmove number
    fen << " " << halfmoveClock << " " << fullmoveNumber;

    return fen.str();
}

void Board::displayFEN() const {
    std::string fen = toFEN();
    std::cout << "FEN: " << fen << std::endl;
    std::cout << "  ";
    for (int col = 0; col < 8; ++col) {
        std::cout << col << ' ';
    }
    std::cout << std::endl;

    int row = 7;
    int col = 0;
    for (char c : fen) {
        if (c == ' ')
            break;
        if (c == '/') {
            std::cout << std::endl;
            --row;
            col = 0;
            std::cout << row << ' ';
        } else if (isdigit(c)) {
            int emptyCount = c - '0';
            for (int i = 0; i < emptyCount; ++i) {
                std::cout << ". ";
                ++col;
            }
        } else {
            std::cout << c << ' ';
            ++col;
        }
    }
    std::cout << std::endl;
    std::cout << "  ";
    for (int col = 0; col < 8; ++col) {
        std::cout << col << ' ';
    }
    std::cout << std::endl;
}

void Board::loadFEN(const std::string& fen) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(fen);
    while (std::getline(tokenStream, token, ' ')) {
        tokens.push_back(token);
    }

    // Load pieces
    int row = 7;
    int col = 0;
    for (char c : tokens[0]) {
        if (c == '/') {
            --row;
            col = 0;
        } else if (isdigit(c)) {
            int emptyCount = c - '0';
            for (int i = 0; i < emptyCount; ++i) {
                squares[row][col] = nullptr;
                ++col;
            }
        } else {
            Color color = isupper(c) ? WHITE : BLACK;
            switch (std::tolower(c)) {
                case 'p':
                    squares[row][col] = std::make_shared<Pawn>(color);
                    break;
                case 'r':
                    squares[row][col] = std::make_shared<Rook>(color);
                    break;
                case 'n':
                    squares[row][col] = std::make_shared<Knight>(color);
                    break;
                case 'b':
                    squares[row][col] = std::make_shared<Bishop>(color);
                    break;
                case 'q':
                    squares[row][col] = std::make_shared<Queen>(color);
                    break;
                case 'k':
                    squares[row][col] = std::make_shared<King>(color);
                    break;
                default:
                    break;
            }
            ++col;
        }
    }

    // Load active color
    activeColor = (tokens[1] == "w") ? WHITE : BLACK;

    // Load castling availability
    whiteKingMoved = false;
    whiteRookMoved[0] = true;
    whiteRookMoved[1] = true;
    blackKingMoved = false;
    blackRookMoved[0] = true;
    blackRookMoved[1] = true;
    for (char c : tokens[2]) {
        switch (c) {
            case 'K':
                whiteRookMoved[1] = false;
                break;
            case 'Q':
                whiteRookMoved[0] = false;
                break;
            case 'k':
                blackRookMoved[1] = false;
                break;
            case 'q':
                blackRookMoved[0] = false;
                break;
            default:
                break;
        }
    }

    // Load en passant target square
    if (tokens[3] != "-") {
        enPassantTarget.first = tokens[3][0] - 'a';
        enPassantTarget.second = tokens[3][1] - '1';
    } else {
        enPassantTarget.first = -1;
        enPassantTarget.second = -1;
    }

    // Load halfmove clock and fullmove number
    halfmoveClock = std::stoi(tokens[4]);
    fullmoveNumber = std::stoi(tokens[5]);
}