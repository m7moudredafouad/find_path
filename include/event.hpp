#include <SFML/Graphics.hpp>
#include <queue>
#include <unordered_map>
#include <unordered_set>

#include <object.hpp>

class EventManager {
  public:
    enum class event_t : uint32_t {
        kClick,
        kLeftPress,
        kLeftRelease,
        kRightPress,
        kRightRelease,
    };

    void AddEvent(sf::Event event) { m_events.push(event); }

    void AddEventListener(event_t event, Object* obj) {
        m_listeners[event].push_back(obj);
    }

    void Handle() {
        while (!m_events.empty()) {
            auto e = m_events.front();
            m_events.pop();
            switch (e.type) {
            default:
                break;
            case sf::Event::MouseButtonPressed: {
                if (e.mouseButton.button == sf::Mouse::Left) {
                    m_left_mouse_pressed = true;
                    m_last_click_pos = e.mouseButton;
                    for (auto& ob : m_listeners[event_t::kLeftPress]) {
                        if (m_clicked_objects.count(ob) == 0 &&
                            ob->Contains(e.mouseButton.x, e.mouseButton.y)) {
                            m_clicked_objects.insert(ob);
                            ob->LeftPress();
                        }
                    }
                }
                break;
            }

            case sf::Event::MouseButtonReleased: {
                if (e.mouseButton.button == sf::Mouse::Left) {
                    m_left_mouse_pressed = false;
                    m_clicked_objects.clear();

                    for (auto& ob : m_listeners[event_t::kClick]) {
                        if (ob->Contains(m_last_click_pos.x,
                                         m_last_click_pos.y) &&
                            ob->Contains(e.mouseButton.x, e.mouseButton.y)) {

                            ob->Click();
                        }
                    }
                }
                break;
            }

            case sf::Event::MouseMoved: {
                if (m_left_mouse_pressed) {
                    for (auto& ob : m_listeners[event_t::kLeftPress]) {
                        if (m_clicked_objects.count(ob) == 0) {
                            if (ob->Contains(e.mouseMove.x, e.mouseMove.y)) {
                                m_clicked_objects.insert(ob);
                                ob->LeftPress();
                            }
                        } else if (!ob->Contains(e.mouseMove.x,
                                                 e.mouseMove.y)) {
                            m_clicked_objects.erase(ob);
                        }
                    }
                }
                break;
            }
            }
        }
    }

  private:
    std::queue<sf::Event> m_events;

    std::unordered_map<event_t, std::vector<Object*>> m_listeners;

    std::unordered_set<Object*> m_clicked_objects;

    bool m_left_mouse_pressed = false;
    sf::Event::MouseButtonEvent m_last_click_pos;
};