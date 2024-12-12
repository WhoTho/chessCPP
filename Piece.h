#ifndef PIECE_H
#define PIECE_H

#include <cmath> // Include cmath for abs function
#include <vector>
#include <memory>

enum Color
{
    WHITE,
    BLACK
};

class Piece
{
public:
    Piece(Color color) : color(color) {}
    virtual ~Piece() {}
    virtual char getSymbol() const = 0;
    Color getColor() const { return color; }
    virtual std::vector<std::pair<int, int>> generateValidMoves(int startX, int startY, const std::vector<std::vector<std::shared_ptr<Piece>>> &board) const = 0;

private:
    Color color;
};

class King : public Piece
{
public:
    King(Color color) : Piece(color) {}
    char getSymbol() const override { return getColor() == WHITE ? 'K' : 'k'; }
    std::vector<std::pair<int, int>> generateValidMoves(int startX, int startY, const std::vector<std::vector<std::shared_ptr<Piece>>> &board) const override
    {
        std::vector<std::pair<int, int>> moves;
        for (int dx = -1; dx <= 1; ++dx)
        {
            for (int dy = -1; dy <= 1; ++dy)
            {
                if (dx == 0 && dy == 0)
                    continue;
                int newX = startX + dx;
                int newY = startY + dy;
                if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8)
                {
                    if (!board[newY][newX] || board[newY][newX]->getColor() != getColor())
                    {
                        moves.push_back({newX, newY});
                    }
                }
            }
        }
        return moves;
    }
};

class Queen : public Piece
{
public:
    Queen(Color color) : Piece(color) {}
    char getSymbol() const override { return getColor() == WHITE ? 'Q' : 'q'; }
    std::vector<std::pair<int, int>> generateValidMoves(int startX, int startY, const std::vector<std::vector<std::shared_ptr<Piece>>> &board) const override
    {
        std::vector<std::pair<int, int>> moves;
        // Rook-like moves
        for (int dx = -1; dx <= 1; ++dx)
        {
            for (int dy = -1; dy <= 1; ++dy)
            {
                if (dx == 0 && dy == 0)
                    continue;
                for (int dist = 1; dist < 8; ++dist)
                {
                    int newX = startX + dx * dist;
                    int newY = startY + dy * dist;
                    if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8)
                    {
                        if (!board[newY][newX])
                        {
                            moves.push_back({newX, newY});
                        }
                        else
                        {
                            if (board[newY][newX]->getColor() != getColor())
                            {
                                moves.push_back({newX, newY});
                            }
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
        return moves;
    }
};

class Rook : public Piece
{
public:
    Rook(Color color) : Piece(color) {}
    char getSymbol() const override { return getColor() == WHITE ? 'R' : 'r'; }
    std::vector<std::pair<int, int>> generateValidMoves(int startX, int startY, const std::vector<std::vector<std::shared_ptr<Piece>>> &board) const override
    {
        std::vector<std::pair<int, int>> moves;
        for (int dx = -1; dx <= 1; ++dx)
        {
            for (int dy = -1; dy <= 1; ++dy)
            {
                if (dx != 0 && dy != 0)
                    continue;
                if (dx == 0 && dy == 0)
                    continue;
                for (int dist = 1; dist < 8; ++dist)
                {
                    int newX = startX + dx * dist;
                    int newY = startY + dy * dist;
                    if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8)
                    {
                        if (!board[newY][newX])
                        {
                            moves.push_back({newX, newY});
                        }
                        else
                        {
                            if (board[newY][newX]->getColor() != getColor())
                            {
                                moves.push_back({newX, newY});
                            }
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
        return moves;
    }
};

class Bishop : public Piece
{
public:
    Bishop(Color color) : Piece(color) {}
    char getSymbol() const override { return getColor() == WHITE ? 'B' : 'b'; }
    std::vector<std::pair<int, int>> generateValidMoves(int startX, int startY, const std::vector<std::vector<std::shared_ptr<Piece>>> &board) const override
    {
        std::vector<std::pair<int, int>> moves;
        for (int dx = -1; dx <= 1; ++dx)
        {
            for (int dy = -1; dy <= 1; ++dy)
            {
                if (dx == 0 || dy == 0)
                    continue;
                for (int dist = 1; dist < 8; ++dist)
                {
                    int newX = startX + dx * dist;
                    int newY = startY + dy * dist;
                    if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8)
                    {
                        if (!board[newY][newX])
                        {
                            moves.push_back({newX, newY});
                        }
                        else
                        {
                            if (board[newY][newX]->getColor() != getColor())
                            {
                                moves.push_back({newX, newY});
                            }
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
        return moves;
    }
};

class Knight : public Piece
{
public:
    Knight(Color color) : Piece(color) {}
    char getSymbol() const override { return getColor() == WHITE ? 'N' : 'n'; }
    std::vector<std::pair<int, int>> generateValidMoves(int startX, int startY, const std::vector<std::vector<std::shared_ptr<Piece>>> &board) const override
    {
        std::vector<std::pair<int, int>> moves;
        int dx[] = {1, 1, 2, 2, -1, -1, -2, -2};
        int dy[] = {2, -2, 1, -1, 2, -2, 1, -1};
        for (int i = 0; i < 8; ++i)
        {
            int newX = startX + dx[i];
            int newY = startY + dy[i];
            if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8)
            {
                if (!board[newY][newX] || board[newY][newX]->getColor() != getColor())
                {
                    moves.push_back({newX, newY});
                }
            }
        }
        return moves;
    }
};

class Pawn : public Piece
{
public:
    Pawn(Color color) : Piece(color) {}
    char getSymbol() const override { return getColor() == WHITE ? 'P' : 'p'; }
    std::vector<std::pair<int, int>> generateValidMoves(int startX, int startY, const std::vector<std::vector<std::shared_ptr<Piece>>> &board) const override
    {
        std::vector<std::pair<int, int>> moves;
        int direction = (getColor() == WHITE) ? 1 : -1;
        int startRow = (getColor() == WHITE) ? 1 : 6;
        int endRow = (getColor() == WHITE) ? 7 : 0;

        // Move forward
        int newY = startY + direction;
        if (newY >= 0 && newY < 8 && !board[newY][startX])
        {
            moves.push_back({startX, newY});
            // Move two squares forward from starting position
            if (startY == startRow && !board[newY + direction][startX])
            {
                moves.push_back({startX, newY + direction});
            }
        }

        // Capture diagonally
        for (int dx = -1; dx <= 1; dx += 2)
        {
            int newX = startX + dx;
            if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8 && board[newY][newX] && board[newY][newX]->getColor() != getColor())
            {
                moves.push_back({newX, newY});
            }
        }

        return moves;
    }
};

#endif // PIECE_H