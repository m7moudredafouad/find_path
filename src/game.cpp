#include <algo.hpp>
#include <event.hpp>
#include <game.hpp>

#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <unordered_set>

SimplePathFind<Cell> simple_find;
TmpPathFind<Cell> tmp_find;

EventManager event_manager;

static void HandleEvents(Game* game) {
    auto& window = game->GetWindow();
    while (window.isOpen()) {
        event_manager.Handle();
    }
}

static void Render(Game* game) {
    auto& window = game->GetWindow();
    auto& font = game->GetFont();

    window.setActive(true);

    Text text(0.f, 0.f, 10.f, 5.f, font, std::string(""));

    int fps = 0;
    sf::Clock clock;
    while (window.isOpen()) {
        text.SetText("FPS: " + std::to_string(fps));

        sf::Time elapsed1 = clock.restart();
        window.clear();

        game->ForEachObject(
            [&window](auto& cell) { window.draw(*cell.GetShape()); });

        window.draw(*text.GetShape());
        window.display();
        fps = 1 / elapsed1.asSeconds();
    }
}

void Game::Loop() {
    m_window.setTitle("2dgame");

    m_matrix = std::vector<std::vector<Cell>>(rows, std::vector<Cell>(cols));

    Init();

    m_window.setActive(false);

    sf::Thread render_thread(&Render, this);
    render_thread.launch();

    sf::Thread handle_event_thread(&HandleEvents, this);
    handle_event_thread.launch();

    sf::Event event;
    while (m_window.isOpen()) {
        while (m_window.pollEvent(event)) {
            switch (event.type) {
            default:
                break;
            case sf::Event::MouseButtonPressed:
            case sf::Event::MouseButtonReleased:
            case sf::Event::MouseMoved: {
                event_manager.AddEvent(event);
                break;
            }
            case sf::Event::Resized: {
                sf::View view(sf::FloatRect(0.f, 0.f, event.size.width,
                                            event.size.height));
                m_window.setView(view);
                break;
            }
            case sf::Event::Closed: {
                m_window.close();
                break;
            }
            }
        }
    }

    render_thread.wait();
    handle_event_thread.wait();
}

template <typename T> void Game::ForEachObject(T apply) {
    for (auto& mrow : m_matrix) {
        for (auto& mcol : mrow) {
            apply(mcol);
        }
    }

    for (auto& btn : m_objects) {
        apply(*btn);
    }
}

void Game::Init() {

    int offset_y = 0;
    int offset_x = 100;
    float padding = 16.f, width = 15.f, height = 15.f;

    for (uint32_t i = 0; i < rows; i++) {
        for (uint32_t j = 0; j < cols; j++) {
            m_matrix[i][j] = Cell(j * padding + offset_x,
                                  i * padding + offset_y, width, height);

            event_manager.AddEventListener(EventManager::event_t::kLeftPress,
                                           &m_matrix[i][j]);
        }
    }

    m_matrix[0][0].UpdateType(Cell::Type::kStart);
    m_matrix[rows - 1][cols - 1].UpdateType(Cell::Type::kEnd);

    float btn_width = 90.f, btn_height = 20.f;

    m_objects.push_back(std::unique_ptr<Object>(
        new Button(0.f, 30.f, btn_width, btn_height, m_font, "Restart")));

    m_objects.push_back(std::unique_ptr<Object>(
        new Button(0.f, 52.f, btn_width, btn_height, m_font, "Loop find")));

    m_objects.push_back(std::unique_ptr<Object>(
        new Button(0.f, 74.f, btn_width, btn_height, m_font, "DFS find")));

    m_objects[0]->SetClickFuntion([&]() { this->ResetMatrix(); });

    m_objects[1]->SetClickFuntion([&]() {
        ResetMatrix();
        simple_find.Apply(GetMatrix());
    });

    m_objects[2]->SetClickFuntion([&]() {
        ResetMatrix();
        tmp_find.Apply(GetMatrix());
    });

    event_manager.AddEventListener(EventManager::event_t::kClick,
                                   m_objects[0].get());
    event_manager.AddEventListener(EventManager::event_t::kClick,
                                   m_objects[1].get());
    event_manager.AddEventListener(EventManager::event_t::kClick,
                                   m_objects[2].get());
}

void Game::ResetMatrix() {
    for (auto& mrow : m_matrix) {
        for (auto& cell : mrow) {
            if (cell.IsVisited()) {
                cell.UpdateType(Cell::Type::kOpen);
            }
        }
    }
    m_matrix[0][0].UpdateType(Cell::Type::kStart);
}
