#include <algo.hpp>
#include <game.hpp>

#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <unordered_set>

SimplePathFind<Cell> simple_find;

static void Events(Game* game) {
    auto& window = game->GetWindow();

    sf::Event event;
    std::unordered_set<Object*> map;

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
                    game->ForEachObject([&event, &map](auto& cell) {
                        if (map.count(&cell) == 0 &&
                            cell.Contains(event.mouseButton.x,
                                          event.mouseButton.y)) {
                            map.insert(&cell);
                            cell.Click();
                        }
                    });
                }

                if (event.mouseButton.button == sf::Mouse::Right) {
                    simple_find.Find(game->GetMatrix());
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
                    game->ForEachObject([&event, &map](auto& cell) {
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
    auto& font = game->GetFont();

    window.setActive(true);

    sf::Text text;

    text.setFont(font);
    text.setCharacterSize(16); // in pixels, not points!
    text.setFillColor(sf::Color::White);

    int fps = 0;
    sf::Clock clock;
    while (window.isOpen()) {
        text.setString("FPS: " + std::to_string(fps));

        sf::Time elapsed1 = clock.restart();
        window.clear();

        game->ForEachObject(
            [&window](auto& cell) { window.draw(*cell.GetShape()); });

        window.draw(text);
        window.display();
        fps = 1 / elapsed1.asSeconds();
    }
}

void Game::Loop() {
    m_window.setTitle("2dgame");

    m_matrix = std::vector<std::vector<Cell>>(rows, std::vector<Cell>(cols));

    Restart();

    m_window.setActive(false);

    sf::Thread render_thread(&Render, this);
    render_thread.launch();

    sf::Thread event_thread(&Events, this);
    event_thread.launch();

    while (m_window.isOpen()) {
        ;
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

    for (auto& btn : m_buttons) {
        apply(btn);
    }
}

template <typename T> void Game::ForEachCell(T apply) {
    for (auto& mrow : m_matrix) {
        for (auto& mcol : mrow) {
            apply(mcol);
        }
    }
}

void Game::Restart() {

    int offset_y = 30;
    int offset_x = 100;

    for (uint32_t i = 0; i < rows; i++) {
        for (uint32_t j = 0; j < cols; j++) {
            m_matrix[i][j] = Cell(j * padding + offset_x,
                                  i * padding + offset_y, width, height);
        }
    }

    m_matrix[0][0].UpdateType(Cell::Type::kStart);
    m_matrix[rows - 1][cols - 1].UpdateType(Cell::Type::kEnd);

    m_buttons.clear();

    m_buttons.push_back(Button(0.f, 50.f, 60.f, 20.f, m_font, "Restart"));
    m_buttons[0].SetClickFuntion([&] () {
        this->Restart();
    });
}