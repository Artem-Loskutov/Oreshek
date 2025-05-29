#pragma once

#include <SFML/Graphics.hpp>

#include <unordered_map>

class Game
{
private:
    sf::RenderWindow window;
    sf::Font font;

    // unique vars
    int current_game_hour;
    const float seconds_per_hour;

    std::unordered_map<std::string, sf::Texture> textures_list;

    void event_reactions();
    void draw(sf::Clock& clock, sf::Text& clock_text);
public:
    Game(unsigned int window_lengh, unsigned int window_height, const std::string& window_name);
    void run();
};
