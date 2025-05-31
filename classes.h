#pragma once

#include <SFML/Graphics.hpp>

#include <unordered_map>
#include <vector>

class Game
{
private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Clock clock;
    sf::Text clock_text {font};

    // unique vars
    int game_hour, game_day, game_month, game_year;
    const float seconds_per_hour;
    float speed_mod = 1.0f;
    const std::vector<int> days_in_month = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    std::unordered_map<std::string, sf::Texture> textures_list;

    void date_maker();
    void event_reactions();
    void draw();
public:
    Game(unsigned int window_width, unsigned int window_height, const std::string& window_name);
    void run();
};
