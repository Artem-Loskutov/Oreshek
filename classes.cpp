#include "classes.h"

#include <SFML/Graphics.hpp>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <thread>
#include <chrono>

sf::Texture load_texture(const std::string& path)
{
    sf::Texture texture(path);
    return texture;
}

Game::Game(unsigned int window_width, unsigned int window_height, const std::string& window_name) :
    game_hour(0), game_day(9), game_month(9), game_year(1941),
    seconds_per_hour(4.0f)
{
    window.create(sf::VideoMode(sf::Vector2u{ window_width, window_height }), window_name);
    window.setFramerateLimit(60);

    if (!font.openFromFile("Assets/arial.ttf"))
    {
        window.close();
    }

    textures_list["island"] = std::move(load_texture("Assets/island.png"));
    textures_list["night"] = std::move(load_texture("Assets/night.png"));
    textures_list["empty"] = std::move(load_texture("Assets/empty.png"));
    textures_list["end"] = std::move(load_texture("Assets/end.png"));

    textures_list["attention"] = std::move(load_texture("Assets/attention.png"));
    textures_list["bomber"] = std::move(load_texture("Assets/bomber.png"));
    textures_list["explosion"] = std::move(load_texture("Assets/explosion.png"));

    textures_list["sniper"] = std::move(load_texture("Assets/sniper.png"));
    textures_list["artillery"] = std::move(load_texture("Assets/artillery.png"));
    textures_list["ammo"] = std::move(load_texture("Assets/ammo.png"));
    textures_list["antitank"] = std::move(load_texture("Assets/antitank.png"));
    textures_list["minomet"] = std::move(load_texture("Assets/minomet.png"));
};

void Game::operation_Iscra()
{
    if (game_year == 1943 && game_month == 1 && game_day == 18 && game_hour == 5)
    {
        sf::Sprite sprite(textures_list["end"]);
        sprite.setColor(sf::Color(255,255,255,128));
        window.draw(sprite);
        window.display();
        std::this_thread::sleep_for(std::chrono::nanoseconds(500000000));
        sprite.setColor(sf::Color(255, 255, 255, 255));
        window.draw(sprite);
        window.display();
        std::this_thread::sleep_for(std::chrono::nanoseconds(3500000000));
        window.close();
    }
}

void Game::run()
{
    for (auto& coord : cell_coords)
    {
        create_cell(coord);
    }

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
        operation_Iscra();
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
        else if (const auto* mouse_pressed = event->getIf<sf::Event::MouseButtonPressed>())
        {
            switch (mouse_pressed->button)
            {
            case (sf::Mouse::Button::Right):
                /*std::cout << mouse_pressed->position.x << " " << mouse_pressed->position.y;
                create_cell({ static_cast<float>(mouse_pressed->position.x), static_cast<float>(mouse_pressed->position.y) }); */
                for (auto& cell : cells)
                {
                    int x = cell.coord.x - 25.f;
                    int y = cell.coord.y - 25.f;
                    sf::IntRect rec({ x,y }, { 50, 50 });
                    if (rec.contains(mouse_pressed->position))
                    {
                        int current_index = cell.weapon->index;
                        int attempts = weapons.size();

                        int next_index = current_index;

                        do
                        {
                            next_index = (next_index + 1) % weapons.size();
                            if (weapons[next_index].total > 0)
                            {
                                cell.weapon = &weapons[next_index];
                                cell.cell_texture = textures_list[cell.weapon->name];
                                weapons[current_index].total++;
                                weapons[next_index].total--;
                                break;
                            }
                            attempts--;
                        } while (next_index != current_index && attempts > 0);
                    }
                }
                break;
            case (sf::Mouse::Button::Left):
                for (auto& cell : cells)
                {
                    int x = cell.coord.x - 25.f;
                    int y = cell.coord.y - 25.f;
                    sf::IntRect rec({ x,y }, { 50, 50 });
                    if (rec.contains(mouse_pressed->position))
                    {
                        int current_index = cell.weapon->index;
                        int attempts = weapons.size();

                        int next_index = current_index;

                        do
                        {
                            next_index = (next_index + weapons.size() - 1) % weapons.size();
                            if (weapons[next_index].total > 0)
                            {
                                cell.weapon = &weapons[next_index];
                                cell.cell_texture = textures_list[cell.weapon->name];
                                weapons[current_index].total++;
                                weapons[next_index].total--;
                                break;
                            }
                            attempts--;
                        } while (next_index != current_index && attempts > 0);
                    }
                }
                break;
            default:
                break;
            }
        }
    }
}

void Game::fire()
{
    if (game_hour == 12)
    {
        for (auto& cell : cells)
        {
            if (cell.weapon->name == "artillery" || cell.weapon->name == "minomet" || cell.weapon->name == "antitank")
            {
                sf::Sprite sprite3(textures_list["explosion"]);
                sprite3.setPosition(cell.coord);
                window.draw(sprite3);
            }
        }
    }
    if (game_hour == 5 || game_hour == 21)
    {
        for (auto& cell : cells)
        {
            if (cell.weapon->name == "sniper")
            {
                sf::Sprite sprite3(textures_list["explosion"]);
                sprite3.setPosition(cell.coord);
                window.draw(sprite3);
            }
        }
    }
}

void Game::bombing()
{
    if (game_hour == 8 || game_hour == 20 || game_hour == 17)
    {
        if (game_year == 1941)
        {
            intensivnost = 20;
        }
        if (game_year == 1942)
        {
            intensivnost = 30;
        }
        if (game_year == 1942 && game_month >= 9)
        {
            intensivnost = 10;
        }
        if (game_year == 1943)
        {
            intensivnost = 5;
        }
        if (game_year == 1943 && game_day >=10)
        {
            intensivnost = 0;
        }
    }

    if (intensivnost > 0)
    {
        if (game_hour == 6 || game_hour == 15 || game_hour == 18)
        {
            sf::Sprite sprite(textures_list["attention"]);
            sprite.setPosition({ 10.f,10.f });
            sprite.setColor(sf::Color(255, 255, 255, 128));
            window.draw(sprite);
        }
        else if (game_hour == 7 || game_hour == 19)
        {
            sf::Sprite sprite(textures_list["attention"]);
            sprite.setPosition({ 10.f,10.f });
            window.draw(sprite);

            sf::Sprite sprite2(textures_list["artillery"]);
            sprite2.setPosition({ 65.f,10.f });
            window.draw(sprite2);

            for (int i = 0; i < intensivnost; intensivnost--)
            {
                sf::Sprite sprite3(textures_list["explosion"]);
                sf::Vector2f boom = cell_coords[rand() % cells.size()];
                sf::Vector2f boom2 = boom;
                boom -= {25.f, 25.f};
                sprite3.setPosition(boom);
                for (auto& cell : cells)
                {
                    if (cell.coord == boom2 && cell.weapon->name != "empty")
                    {
                        weapons[cell.weapon->index].total--;
                        cell.weapon = &weapons[0];
                        cell.cell_texture = textures_list[cell.weapon->name];
                    }
                }
                window.draw(sprite3);
            }
        }
        else if (game_hour == 16)
        {
            sf::Sprite sprite(textures_list["attention"]);
            sprite.setPosition({ 10.f,10.f });
            window.draw(sprite);

            sf::Sprite sprite2(textures_list["bomber"]);
            sprite2.setPosition({ 65.f,10.f });
            window.draw(sprite2);

            for (int i = 0; i < intensivnost; intensivnost--)
            {
                sf::Sprite sprite3(textures_list["explosion"]);
                sf::Vector2f boom = cell_coords[rand() % cells.size()];
                sf::Vector2f boom2 = boom;
                boom -= {25.f, 25.f};
                sprite3.setPosition(boom);
                for (auto& cell : cells)
                {
                    if (cell.coord == boom2 && cell.weapon->name != "empty")
                    {
                        weapons[cell.weapon->index].total--;
                        cell.weapon = &weapons[0];
                        cell.cell_texture = textures_list[cell.weapon->name];
                    }
                }
                window.draw(sprite3);
            }
        }
    }
}

void Game::create_cell(sf::Vector2f coord)
{
    Cell cell;
    cell.coord = coord;
    cell.weapon = &weapons[0];
    cell.cell_texture = textures_list[cell.weapon->name];
    cells.push_back(cell);
}

void Game::draw()
{
    window.clear();

    sf::Sprite sprite(textures_list["island"]);
    window.draw(sprite);

    for (auto& cell : cells)
    {
        sf::Sprite cell_sprite(cell.cell_texture);
        auto mid = cell_sprite.getLocalBounds().getCenter();
        cell_sprite.setPosition({ cell.coord - mid });
        window.draw(cell_sprite);
    }

    sf::Sprite cell_sprite2(textures_list["ammo"]);
    auto mid = cell_sprite2.getLocalBounds().getCenter();
    cell_sprite2.setPosition({ sf::Vector2f{ 448.f, 220.f } - mid });
    window.draw(cell_sprite2);

    bombing();
    fire();

    sf::Sprite night_sprite(textures_list["night"]);
    int alpha = 0;
    if (game_hour >= 20 || game_hour < 4)
    {
        alpha = 128;
    }
    else if (game_hour >= 16 && game_hour < 20)
    {
        alpha = static_cast<int>((game_hour - 16) / 4.0f * 128);
    }
    else if (game_hour >= 4 && game_hour < 8)
    {
        alpha = static_cast<int>((1.0f - (game_hour - 4) / 4.0f) * 128);
    }
    else
    {
        alpha = 0;
    }
    night_sprite.setColor(sf::Color(255, 255, 255, alpha));
    window.draw(night_sprite);

    window.draw(clock_text);
    operation_Iscra();

    window.display();
}
