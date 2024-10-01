#ifndef CARDS_HPP
#define CARDS_HPP

#include <memory>
#include <string>


class Player;  // Forward declaration of Player
class Game;    // Forward declaration of Game

// Base Card class
class Card {
public:
    virtual std::string getDescription() const = 0;
    virtual void execute(std::shared_ptr<Player> player, Game& game) = 0;
    virtual ~Card() = default;
};

// 1. Advance to Go (Collect $200)
class AdvanceToGoCard : public Card {
public:
    std::string getDescription() const override;
    void execute(std::shared_ptr<Player> player, Game& game) override;
};

// 2. Go to Jail (Do not pass Go, do not collect $200)
class GoToJailCard : public Card {
public:
    std::string getDescription() const override;
    void execute(std::shared_ptr<Player> player, Game& game) override;
};

// 3. Get Out of Jail Free
class GetOutOfJailFreeCard : public Card {
public:
    std::string getDescription() const override;
    void execute(std::shared_ptr<Player> player, Game& game) override;
};

// 4. Take a Trip to Reading Railroad
class TripToReadingRailroadCard : public Card {
public:
    std::string getDescription() const override;
    void execute(std::shared_ptr<Player> player, Game& game) override;
};

// 5. Advance to Nearest Utility
class AdvanceToNearestUtilityCard : public Card {
public:
    std::string getDescription() const override;
    void execute(std::shared_ptr<Player> player, Game& game) override;
};

// 6. Advance to Nearest Railroad
class AdvanceToNearestRailroadCard : public Card {
public:
    std::string getDescription() const override;
    void execute(std::shared_ptr<Player> player, Game& game) override;
};

// 7. General Repairs Card
class GeneralRepairsCard : public Card {
public:
    std::string getDescription() const override;
    void execute(std::shared_ptr<Player> player, Game& game) override;
};

#endif // CARDS_HPP
