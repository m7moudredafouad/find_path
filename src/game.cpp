#include <algo.hpp>
#include <event.hpp>
#include <game.hpp>

#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <unordered_set>

SimplePathFind<Cell> simple_find;
TmpPathFind<Cell> tmp_find;
Algorithm<std::vector<std::vector<Cell>>>* algo;

EventManager event_manager;

static void PollEvents(Game* game) {
    auto& window = game->GetWindow();

    sf::Event event;

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
            case sf::Event::MouseButtonPressed:
            case sf::Event::MouseButtonReleased:
            case sf::Event::MouseMoved: {
                event_manager.AddEvent(event);
                break;
            }
            }
        }
    }
}

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

    sf::Thread poll_event_thread(&PollEvents, this);
    poll_event_thread.launch();

    sf::Thread handle_event_thread(&HandleEvents, this);
    handle_event_thread.launch();

    while (m_window.isOpen()) {
        ;
    }

    render_thread.wait();
    poll_event_thread.wait();
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

    algo = &simple_find;

    int offset_y = 30;
    int offset_x = 100;

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

    m_objects.push_back(std::unique_ptr<Object>(
        new Button(10.f, 50.f, 80.f, 20.f, m_font, "Restart")));

    m_objects.push_back(std::unique_ptr<Object>(
        new Button(10.f, 72.f, 80.f, 20.f, m_font, "Find")));

    m_objects.push_back(std::unique_ptr<Object>(
        new Button(10.f, 94.f, 80.f, 20.f, m_font, "Simple find")));

    m_objects.push_back(std::unique_ptr<Object>(
        new Button(10.f, 116.f, 80.f, 20.f, m_font, "Tmp find")));

    m_objects[0]->SetClickFuntion([&]() {
        this->ResetMatrix();
    });

    m_objects[1]->SetClickFuntion([&]() {
        ResetMatrix();
        algo->Apply(GetMatrix());
    });

    m_objects[2]->SetClickFuntion([&]() {
        algo = &simple_find;
        std::cout << "Simple find algorithm was selected" << std::endl;
    });

    m_objects[3]->SetClickFuntion([&]() {
        algo = &tmp_find;
        std::cout << "Tmp find algorithm was selected" << std::endl;
    });

    event_manager.AddEventListener(EventManager::event_t::kClick,
                                   m_objects[0].get());
    event_manager.AddEventListener(EventManager::event_t::kClick,
                                   m_objects[1].get());
    event_manager.AddEventListener(EventManager::event_t::kClick,
                                   m_objects[2].get());
    event_manager.AddEventListener(EventManager::event_t::kClick,
                                   m_objects[3].get());
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
