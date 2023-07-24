#include <game.hpp>
#include <iostream>

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <unordered_set>

static void Events(Game* game) {
    auto& window = game->GetWindow();

    sf::RectangleShape temp_shape;
    sf::Event event;
    std::unordered_set<Cell*> map;

    bool is_mouse_presed = false;

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            switch (event.type) {
            default:
                break;
            case sf::Event::Closed: {
                window.close();
                exit(EXIT_SUCCESS);
                return;
            }
            case sf::Event::Resized: {
                sf::View view(sf::FloatRect(0.f, 0.f, event.size.width,
                                            event.size.height));
                window.setView(view);
                break;
            }
            case sf::Event::MouseButtonPressed: {
                map.clear();
                if (event.mouseButton.button == sf::Mouse::Left) {
                    is_mouse_presed = true;
                    game->ForEachObject([&event, &map](Cell& cell) {
                        if (map.count(&cell) == 0 &&
                            cell.Contains(event.mouseButton.x,
                                          event.mouseButton.y)) {
                            map.insert(&cell);
                            cell.Click();
                        }
                    });
                }
                break;
            }

            case sf::Event::MouseButtonReleased: {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    is_mouse_presed = false;
                    map.clear();
                }
                break;
            }

            case sf::Event::MouseMoved: {
                if (is_mouse_presed) {
                    game->ForEachObject([&event, &map](Cell& cell) {
                        if (map.count(&cell) == 0) {
                            if (cell.Contains(event.mouseMove.x,
                                              event.mouseMove.y)) {
                                map.insert(&cell);
                                cell.Click();
                            }
                        } else {
                            if (!cell.Contains(event.mouseMove.x,
                                               event.mouseMove.y)) {
                                map.erase(&cell);
                            }
                        }
                    });
                }
                break;
            }
            }
        }
    }
}

static void Render(Game* game) {
    auto& window = game->GetWindow();
    auto& matrix = game->GetMatrix();

    window.setActive(true);

    sf::Font font;
    sf::Text text;
    if (!font.loadFromFile("./fonts/arial.ttf"))
        assert(0);

    text.setFont(font);
    text.setCharacterSize(16); // in pixels, not points!
    text.setFillColor(sf::Color::White);

    int fps = 0;
    sf::RectangleShape temp_shape;
    sf::Clock clock;
    while (window.isOpen()) {
        text.setString(std::to_string(fps));

        sf::Time elapsed1 = clock.restart();
        window.clear();

        game->ForEachObject(
            [&window](Cell& cell) { window.draw(*cell.GetShape()); });

        window.draw(text);
        window.display();
        fps = 1 / elapsed1.asSeconds();
    }
}

// static void Random(Game* game) {
//     auto& window = game->GetWindow();
//     auto& matrix = game->GetMatrix();

//     while (window.isOpen()) {
//         auto x = rand() % game->cols;
//         auto y = rand() % game->rows;

//         matrix[y][x].color = matrix[y][x].color == sf::Color::White
//                                  ? sf::Color::Red
//                                  : sf::Color::White;

//         sf::sleep(sf::milliseconds(100));
//     }
// }

void Game::Loop() {
    m_window.setTitle("2dgame");

    m_matrix = std::vector<std::vector<Cell>>(rows, std::vector<Cell>(cols));

    int offset_y = 30;

    for (uint32_t i = 0; i < rows; i++) {
        for (uint32_t j = 0; j < cols; j++) {
            m_matrix[i][j] = Cell(j * padding, i * padding + offset_y, width,
                                  height, sf::Color::White);
        }
    }

    m_window.setActive(false);

    sf::Thread render_thread(&Render, this);
    render_thread.launch();

    sf::Thread event_thread(&Events, this);
    event_thread.launch();

    // sf::Thread random_thread(&Random, this);
    // random_thread.launch();

    while (m_window.isOpen()) {
    }

    render_thread.wait();
    event_thread.wait();
}

template <typename T> void Game::ForEachObject(T apply) {
    for (auto& mrow : m_matrix) {
        for (auto& mcol : mrow) {
            apply(mcol);
        }
    }
}
