#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <filesystem>
#include <iostream>
#include <map>
#include <vector>
#include "Board.h"

const int TILE_SIZE = 50;
const int BOARD_SIZE = 8;
const sf::Color LIGHT_TILE_COLOR = sf::Color(240, 217, 181);
const sf::Color DARK_TILE_COLOR = sf::Color(181, 136, 99);

class ChessGUI {
   private:
    sf::RenderWindow window;
    Board board;
    std::map<std::string, sf::Texture> textures;
    sf::Vector2i selectedSquare;
    std::vector<Move> highlightedMoves;

    void loadTextures() {
        std::vector<std::string> pieces = {
            "white_P", "white_R", "white_N", "white_B", "white_Q", "white_K",
            "black_p", "black_r", "black_n", "black_b", "black_q", "black_k"};
        for (const auto& piece : pieces) {
            sf::Texture texture;
            if (!texture.loadFromFile("textures\\piece_" + piece + ".png")) {
                std::cerr << "Failed to load texture for piece: " << piece
                          << std::endl;
                exit(1);
            }
            textures[piece] = texture;
        }
    }

    void drawBoard() {
        for (int y = BOARD_SIZE - 1; y >= 0; --y) {
            for (int x = 0; x < BOARD_SIZE; ++x) {
                sf::RectangleShape tile({TILE_SIZE, TILE_SIZE});
                tile.setPosition({x * TILE_SIZE, (7 - y) * TILE_SIZE});
                tile.setFillColor((x + y) % 2 == 0 ? LIGHT_TILE_COLOR
                                                   : DARK_TILE_COLOR);
                window.draw(tile);

                // Highlight available moves
                for (const auto& move : highlightedMoves) {
                    if (move.endX == x && move.endY == y) {
                        tile.setFillColor(sf::Color(200, 100, 100, 150));
                        window.draw(tile);
                    }
                }
            }
        }
    }

    void drawPieces() {
        for (int y = BOARD_SIZE - 1; y >= 0; --y) {
            for (int x = 0; x < BOARD_SIZE; ++x) {
                if (board.squares[y][x]) {
                    std::string pieceSymbol(1,
                                            board.squares[y][x]->getSymbol());
                    std::string pieceColor =
                        board.squares[y][x]->getColor() == WHITE ? "white"
                                                                 : "black";
                    sf::Sprite pieceSprite(
                        textures[pieceColor + "_" + pieceSymbol]);
                    pieceSprite.setPosition(
                        {x * TILE_SIZE, (7 - y) * TILE_SIZE});
                    pieceSprite.setScale(
                        {TILE_SIZE / pieceSprite.getLocalBounds().size.x,
                         TILE_SIZE / pieceSprite.getLocalBounds().size.y});
                    window.draw(pieceSprite);
                }
            }
        }
    }

    void trySelectSquare(int x, int y) {
        const Color activeColor = board.activeColor;
        if (!board.squares[y][x] ||
            board.squares[y][x]->getColor() != activeColor) {
            return;
        }

        selectedSquare = sf::Vector2i(x, y);
        highlightedMoves = board.getValidMovesForSquare(x, y, true);
    }

    void unselectSquare() {
        selectedSquare = sf::Vector2i(-1, -1);
        highlightedMoves.clear();
    }

    void handleMouseClick(int x, int y) {
        std::cout << "Mouse clicked at: " << x << ", " << y << std::endl;
        int boardX = x / TILE_SIZE;
        int boardY = BOARD_SIZE - 1 - (y / TILE_SIZE);
        std::cout << "Board coordinates: " << boardX << ", " << boardY
                  << std::endl;

        if (selectedSquare.x == -1) {
            trySelectSquare(boardX, boardY);
            return;
        }

        char selectedPromotion;
        bool wasMoveMade = false;
        for (const auto& move : highlightedMoves) {
            if (move.endX != boardX || move.endY != boardY) {
                continue;
            }

            if (move.type == PROMOTION) {
                // Open a promotion dialog
                sf::RenderWindow promotionWindow(sf::VideoMode({200u, 50u}),
                                                 "Promotion");
                std::vector<std::string> promotionPieces =
                    move.promotionPiece->getColor() == WHITE
                        ? std::vector<std::string>{"white_Q", "white_R",
                                                   "white_N", "white_B"}
                        : std::vector<std::string>{"black_Q", "black_R",
                                                   "black_N", "black_B"};

                while (promotionWindow.isOpen()) {
                    while (const std::optional event =
                               promotionWindow.pollEvent()) {
                        if (event->is<sf::Event::Closed>()) {
                            promotionWindow.close();
                        } else if (event->is<sf::Event::MouseButtonPressed>() &&
                                   event->getIf<sf::Event::MouseButtonPressed>()
                                           ->button ==
                                       sf::Mouse::Button::Left) {
                            int px =
                                event->getIf<sf::Event::MouseButtonPressed>()
                                    ->position.x /
                                50;
                            if (px >= 0 && px < promotionPieces.size()) {
                                selectedPromotion = promotionPieces[px].back();
                                promotionWindow.close();
                            }
                        }
                    }

                    promotionWindow.clear();
                    for (int i = 0; i < promotionPieces.size(); ++i) {
                        sf::RectangleShape box({50, 50});
                        box.setPosition({i * 50, 0});
                        box.setFillColor(sf::Color::White);
                        promotionWindow.draw(box);

                        sf::Sprite sprite(textures[promotionPieces[i]]);
                        sprite.setPosition({i * 50, 0});
                        sprite.setScale({0.5, 0.5});
                        promotionWindow.draw(sprite);
                    }
                    promotionWindow.display();
                }

                if (move.promotionPiece->getSymbol() != selectedPromotion) {
                    continue;
                }
            }

            board.makeMove(move);
            wasMoveMade = true;
            break;
        }

        if (wasMoveMade) {
            unselectSquare();
            tryMakeAIMove();
        } else {
            trySelectSquare(boardX, boardY);
        }
    }

    void tryMakeAIMove() {
        if (board.makeAIMove(BLACK)) {
                } else {
            std::cout << "AI has no valid moves!" << std::endl;
        }
    }

   public:
    ChessGUI()
        : window(sf::VideoMode(
                     {TILE_SIZE * BOARD_SIZE * 2, TILE_SIZE * BOARD_SIZE}),
                 "Chess Game"),
          selectedSquare(-1, -1) {
        loadTextures();
    }

    void run() {
        if (!window.isOpen()) {
            std::cerr << "Failed to open SFML window." << std::endl;
            exit(1);
        }
        while (window.isOpen()) {
            while (const std::optional event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                    window.close();
                } else if (event->is<sf::Event::MouseButtonPressed>() &&
                           event->getIf<sf::Event::MouseButtonPressed>()
                                   ->button == sf::Mouse::Button::Left) {
                    handleMouseClick(
                        event->getIf<sf::Event::MouseButtonPressed>()
                            ->position.x,
                        event->getIf<sf::Event::MouseButtonPressed>()
                            ->position.y);
                }
            }

            window.clear();
            drawBoard();
            drawPieces();
            window.display();
        }
    }
};

int main() {
    std::cout << "Running Chess GUI..." << std::endl;

    ChessGUI gui;
    gui.run();
    return 0;
}