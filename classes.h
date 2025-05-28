#pragma once

#include <SFML/Graphics.hpp>

class Game
{
private:
    sf::RenderWindow window;
    sf::Font font;

    // unique vars
    int current_game_hour;
    const float seconds_per_hour;

    void event_reactions();
    void draw(sf::Clock& clock, sf::Text& clock_text);
public:
    Game(unsigned int window_lengh, unsigned int window_height, std::string window_name);
    void run();
};
