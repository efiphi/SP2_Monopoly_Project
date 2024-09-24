#ifndef DICE_HPP
#define DICE_HPP

#include <random>
#include <utility>
#include <memory>

class Dice {
protected:
    bool mockEnabled = false;           // Flag to enable/disable mock results
    std::pair<int, int> mockResult;     // Holds the mock dice result

public:
    // Virtual destructor to allow inheritance
    virtual ~Dice() = default;

    // Roll two dice and return the results as a pair
    virtual std::pair<int, int> roll() {
        if (mockEnabled) {
            return mockResult;  // Return the mocked result if mocking is enabled
        }

        static std::random_device rd;  // Seed for random number generator
        static std::mt19937 gen(rd()); // Mersenne Twister RNG
        static std::uniform_int_distribution<> dis(1, 6); // Dice roll between 1 and 6

        int dice1 = dis(gen);
        int dice2 = dis(gen);
        return {dice1, dice2};
    }

    // Mock the roll by setting a specific result
    void mockRoll(int dice1, int dice2) {
        mockResult = {dice1, dice2};
        mockEnabled = true;  // Enable mock result
    }

    // Disable the mock result, so random rolls happen
    void disableMock() {
        mockEnabled = false;
    }

    // Check if the two dice rolled the same number (doubles)
    bool isDouble(const std::pair<int, int>& dice) const {
        return dice.first == dice.second;
    }

    // Calculate the total movement (sum of two dice)
    int total(const std::pair<int, int>& dice) const {
        return dice.first + dice.second;
    }
};

// MockDice class for unit testing, inherits from Dice
class MockDice : public Dice {
public:
    // Constructor to directly set the mock roll result
    MockDice(int dice1, int dice2) {
        mockRoll(dice1, dice2);  // Set the mock result
    }

    // Override the roll function to always return the mock result
    std::pair<int, int> roll() override {
        return mockResult;  // Always return the mocked result
    }

    // Allow changing the mock result later on
    void setMockRoll(int dice1, int dice2) {
        mockRoll(dice1, dice2);
    }
};

#endif // DICE_HPP
