#include <utility>

class HistoryItem {
   public:
    HistoryItem(const std::pair<int, int>& enPassantTarget,
                bool whiteKingMoved,
                bool blackKingMoved,
                bool whiteRookMoved[2],
                bool blackRookMoved[2],
                int halfmoveClock,
                int fullmoveNumber)
        : enPassantTarget(enPassantTarget),
          whiteKingMoved(whiteKingMoved),
          blackKingMoved(blackKingMoved),
          halfmoveClock(halfmoveClock),
          fullmoveNumber(fullmoveNumber) {
        this->whiteRookMoved[0] = whiteRookMoved[0];
        this->whiteRookMoved[1] = whiteRookMoved[1];
        this->blackRookMoved[0] = blackRookMoved[0];
        this->blackRookMoved[1] = blackRookMoved[1];
    };

    std::pair<int, int> enPassantTarget;
    bool whiteKingMoved;
    bool blackKingMoved;
    bool whiteRookMoved[2];
    bool blackRookMoved[2];
    int halfmoveClock;
    int fullmoveNumber;
};