#include "classes.h"

#include <SFML/Graphics.hpp>
#include <sstream>
#include <iomanip>

sf::Texture load_texture(const std::string& path)
{
    sf::Texture texture(path);
    return texture;
}

Game::Game(unsigned int window_lengh, unsigned int window_height, const std::string& window_name) :
    current_game_hour(0), seconds_per_hour(1.0f)
{
    window.create(sf::VideoMode(sf::Vector2u{ window_lengh, window_height }), window_name);
    window.setFramerateLimit(60);

    if (!font.openFromFile("arial.ttf"))
    {
        //error
        window.close();
    }
};

void Game::run()
{
    sf::Text clock_text(font);
    clock_text.setCharacterSize(16);
    clock_text.setPosition({ 0.f, 0.f });
    clock_text.setFillColor(sf::Color::White);
    
    sf::Clock clock;

    textures_list["island"] = std::move(load_texture("island.png"));
    
    while (window.isOpen())
    {
        event_reactions();

        draw(clock, clock_text);
    }
}

void Game::event_reactions()
{
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }
        else if (const auto* key_pressed = event->getIf<sf::Event::KeyPressed>())
        {
            switch (key_pressed->scancode)
            {
            case (sf::Keyboard::Scancode::Escape):
                window.close();
                break;
            default:
                break;
            }
        }
    }
}

void Game::draw(sf::Clock& clock, sf::Text& clock_text)
{
    window.clear();

    sf::Sprite sprite(textures_list["island"]);
    window.draw(sprite);

    if (clock.getElapsedTime().asSeconds() >= seconds_per_hour)
    {
        clock.restart();
        current_game_hour = (current_game_hour + 1) % 24;
    }
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << current_game_hour << ":00";
    clock_text.setString(oss.str());
    window.draw(clock_text);

    window.display();
}
