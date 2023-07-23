// #include <Threads.hpp>

// void RenderThread(sf::RenderWindow* window) {
//     window->setActive(true);

//     sf::RectangleShape temp_shape;

//     // the rendering loop
//     while (window->isOpen()) {
//         sf::Event event;
//         while (window->pollEvent(event)) {
//             if (event.type == sf::Event::Closed)
//                 window->close();
//             if (event.type == sf::Event::Resized) {
//                 sf::View view(sf::FloatRect(0.f, 0.f, event.size.width,
//                                             event.size.height));
//                 window->setView(view);
//             }
//         }

//         window->clear();

//         for (uint32_t i = 0; i < matrix.size(); i++) {
//             for (uint32_t j = 0; j < matrix[i].size(); j++) {
//                 auto& el = matrix[i][j];
//                 temp_shape.setSize(sf::Vector2f(el.width, el.height));
//                 temp_shape.setFillColor(el.color);
//                 temp_shape.setPosition(sf::Vector2f(j * padding, i * padding));

//                 window->draw(temp_shape);
//             }
//         }

//         window->display();
//     }
// }

// void RandomThread(sf::RenderWindow* window, uint32_t& cols, uint32_t& rows) {
//     while (window->isOpen()) {
//         auto x = rand() % cols;
//         auto y = rand() % rows;

//         matrix[y][x].color = matrix[y][x].color == sf::Color::White
//                                  ? sf::Color::Red
//                                  : sf::Color::White;

//         sf::sleep(sf::milliseconds(100));
//     }
// }