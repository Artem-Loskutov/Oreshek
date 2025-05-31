#pragma once

#include <SFML/Graphics.hpp>

#include <unordered_map>
#include <vector>

struct Weapon
{
    int index;
    std::string name;
    int total;
};

struct Cell
{
    sf::Vector2f coord;
    sf::Texture cell_texture;
    Weapon* weapon;
};

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
    std::vector<Cell> cells;
    int cell_size = 50;
    std::vector<Weapon> weapons = { {0, "empty", 1000}, {1, "artillery", 3}, {2, "sniper", 2}};

    std::vector<sf::Vector2f> cell_coords = { {147.f, 343.f}, { 85.f, 488.f }, { 369.f, 462.f },
        { 540.f, 353.f }, { 474.f, 122.f }, { 300.f, 198.f }, { 151.f, 474.f }, { 222.f, 464.f }, { 294.f, 457.f } };
    //remove to json when i have time 

    std::unordered_map<std::string, sf::Texture> textures_list;

    void operation_Iscra();
    void date_maker();
    void event_reactions();
    void create_cell(sf::Vector2f coord);
    void draw();
    void bombing();
    void fire();
public:
    Game(unsigned int window_width, unsigned int window_height, const std::string& window_name);
    void run();
};
