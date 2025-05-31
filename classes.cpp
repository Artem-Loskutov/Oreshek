#include "classes.h"

#include <SFML/Graphics.hpp>
#include <sstream>
#include <iomanip>

sf::Texture load_texture(const std::string& path)
{
    sf::Texture texture(path);
    return texture;
}

Game::Game(unsigned int window_width, unsigned int window_height, const std::string& window_name) :
    game_hour(0), game_day(7), game_month(9), game_year(1941),
    seconds_per_hour(1.0f)
{
    window.create(sf::VideoMode(sf::Vector2u{ window_width, window_height }), window_name);
    window.setFramerateLimit(60);

    if (!font.openFromFile("Assets/arial.ttf"))
    {
        //error
        window.close();
    }

    textures_list["island"] = std::move(load_texture("Assets/island.png"));
};

void Game::run()
{
    clock_text.setCharacterSize(16);
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << game_hour << ":00 | "
        << std::setw(2) << std::setfill('0') << game_day << "."
        << std::setw(2) << std::setfill('0') << game_month << "."
        << game_year;
    clock_text.setString(oss.str());
    sf::FloatRect text_rect = clock_text.getLocalBounds();
    clock_text.setOrigin({ text_rect.getCenter().x, 0.f});
    clock_text.setPosition({ static_cast<float>(window.getSize().x / 2), 0.f });
    
    while (window.isOpen())
    {
        date_maker();
        event_reactions();
        draw();
    }
}

void Game::date_maker()
{
    if (clock.getElapsedTime().asSeconds() >= seconds_per_hour*speed_mod)
    {
        clock.restart();
        game_hour = game_hour + 1;

        if (game_hour >= 24)
        {
            game_hour = 0;
            game_day++;

            if (game_day > days_in_month[game_month - 1])
            {
                game_day = 1;
                game_month++;

                if (game_month > 12)
                {
                    game_month = 1;
                    game_year++;
                }
            }
        }
        std::ostringstream oss;
        oss << std::setw(2) << std::setfill('0') << game_hour << ":00 | "
            << std::setw(2) << std::setfill('0') << game_day << "."
            << std::setw(2) << std::setfill('0') << game_month << "."
            << game_year;
        clock_text.setString(oss.str());
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
            case (sf::Keyboard::Scancode::Num1):
                speed_mod = 1.0f;
                break;
            case (sf::Keyboard::Scancode::Num2):
                speed_mod = 0.5f;
                break;
            case (sf::Keyboard::Scancode::Num3):
                speed_mod = 0.1f;
                break;
            case (sf::Keyboard::Scancode::Num4):
                speed_mod = 0.01f;
                break;
            default:
                break;
            }
        }
    }
}

void Game::draw()
{
    window.clear();

    sf::Sprite sprite(textures_list["island"]);
    if (game_hour >= 20 || game_hour < 4)
    {
        sprite.setColor(sf::Color(128, 128, 128));
    }
    else
    {
        sprite.setColor(sf::Color(255, 255, 255));
    }
    window.draw(sprite);
    
    window.draw(clock_text);

    window.display();
}
