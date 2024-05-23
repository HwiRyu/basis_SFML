#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include "scale.h"
#include "one_variable_function.h"


int main() {
    // 원하는 창의 크기
    const int width = 1200;
    const int height = 800;

    float point_x = 0;
    float point_y = 0;


    sf::RenderWindow window(sf::VideoMode(width, height), "SFML_basis");

    sf::VertexArray graph_1(sf::LineStrip);
    sf::VertexArray axes(sf::LineStrip);
    sf::VertexArray function_1(sf::LineStrip);


    sf::Font font;
    if (!font.loadFromFile("/System/Library/Fonts/Monaco.ttf")) {
        std::cerr << "Error loading font\n";
        return 1;
    }
    sf::View initial_view(sf::FloatRect(-width / 2, -height / 2, width, height));
    window.setView(initial_view);

    // 텍스트 객체 생성 및 초기화
    sf::Text text_1("n =:", font, 12);
    text_1.setPosition(initial_view.getCenter().x + width / 2 - 100, initial_view.getCenter().y - height / 2 + 20);
    text_1.setFillColor(sf::Color::Black);

    sf::Text text_2("x: 0, y:0", font, 12);
    text_2.setPosition(initial_view.getCenter().x + width / 2 - 200, initial_view.getCenter().y - height / 2 + 40);
    text_2.setFillColor(sf::Color::Black);

    sf::Text text_3("view_x: , view_y:", font, 12);
    text_3.setPosition(initial_view.getCenter().x + width / 2 - 200, initial_view.getCenter().y - height / 2 + 60);
    text_3.setFillColor(sf::Color::Black);

    sf::Clock clock;
    sf::Clock clock_t;
    sf::Time lastClickTime = sf::Time::Zero;
    sf::Vector2i lastMousePos; // 마지막 마우스 위치 저장
    sf::Vector2f startPoint; // 초기 클릭 위치 저장
    sf::Time elapsedTime;
    int size = 10;
    bool debug_mod = false;

    if (debug_mod) {
                initial_view.zoom(1.2f);
        }

    // 이벤트 루프
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::W:
                        // 확대축소할때 시야 중심으로 확대축소되게 하는 코드
                        initial_view.move(sf::Vector2f (initial_view.getCenter().x/size, initial_view.getCenter().y/size));
                        size += 1;
                        window.setView(initial_view);
                        break;
                    case sf::Keyboard::S:
                        if (size > 3) {
                            initial_view.move(sf::Vector2f(-initial_view.getCenter().x / size,
                                                           -initial_view.getCenter().y / size));
                            size -= 1;
                            window.setView(initial_view);
                            break;
                        }
                    default:
                        break;
                }

            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Time currentTime = clock.getElapsedTime();
                    lastMousePos = sf::Mouse::getPosition(window);
                    startPoint = window.mapPixelToCoords(lastMousePos); // 초기 클릭 위치 저장
                    if (currentTime - lastClickTime <= sf::seconds(0.2)) {
                        point_x = startPoint.x / size;
                        point_y = -startPoint.y / size;
                    }
                    lastClickTime = currentTime;
                }
            }

            if (sf::Event::MouseMoved) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    sf::Vector2i newMousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f delta = window.mapPixelToCoords(newMousePos) - window.mapPixelToCoords(lastMousePos);
                    initial_view.move(-delta);
                    window.setView(initial_view);
                    lastMousePos = newMousePos;
                }
            }
        }

        // Text와 관련된 것을 다루는 부분

        // Text의 위치

        text_1.setPosition(initial_view.getCenter().x + width / 2 - 100,
                           initial_view.getCenter().y - height / 2 + 20);
        text_2.setPosition(initial_view.getCenter().x + width / 2 - 200,
                           initial_view.getCenter().y - height / 2 + 40);
        text_3.setPosition(initial_view.getCenter().x + width / 2 - 200,
                           initial_view.getCenter().y - height / 2 + 60);

        //Text에 들어갈 문자
        text_1.setString("n = 0");

        text_2.setString("x:" + std::to_string(point_x) + "y:" + std::to_string(point_y));

        text_3.setString("view_x:" + std::to_string(initial_view.getCenter().x/size) + "view_y:" +
                         std::to_string(-initial_view.getCenter().y/size));


        //축에 관련된 것을 다루는 부분

        //축의 길이에 관여(창에 보이는 곳만 표현하기 위해)

        double x_start = -width / 2
                         + fmod(width / 2, size)
                         + initial_view.getCenter().x
                         - fmod(initial_view.getCenter().x, size);
        double x_end = width / 2 + fmod(width / 2, size) + initial_view.getCenter().x - fmod(initial_view.getCenter().x, size);
        double y_start = -height / 2 + fmod(height / 2, size) + initial_view.getCenter().y -
                         fmod(initial_view.getCenter().y, size);
        double y_end = height / 2 + fmod(height / 2, size) + initial_view.getCenter().y -
                       fmod(initial_view.getCenter().y, size);


        //축
        sf::VertexArray axes(sf::Lines);
        axes.append(sf::Vertex(sf::Vector2f(-(width / 2 - initial_view.getCenter().x), 0.0f), sf::Color::Black));
        axes.append(sf::Vertex(sf::Vector2f(width / 2 + initial_view.getCenter().x, 0.0f), sf::Color::Black));
        axes.append(sf::Vertex(sf::Vector2f(0.0f, -height / 2 + initial_view.getCenter().y), sf::Color::Black));
        axes.append(sf::Vertex(sf::Vector2f(0.0f, height / 2 + initial_view.getCenter().y), sf::Color::Black));


        //눈금
        for (double i = std::max(0.0, x_start); i <= x_end; i += size) {
            axes.append(sf::Vertex(sf::Vector2f(i, -3), sf::Color::Black));
            axes.append(sf::Vertex(sf::Vector2f(i, 3), sf::Color::Black));
        }

        for (double i = std::min(0.0, x_end); i >= x_start; i -= size) {
            axes.append(sf::Vertex(sf::Vector2f(i, -3), sf::Color::Black));
            axes.append(sf::Vertex(sf::Vector2f(i, 3), sf::Color::Black));
        }

        for (double i = std::max(0.0, y_start); i <= y_end; i += size) {
            axes.append(sf::Vertex(sf::Vector2f(-3, i), sf::Color::Black));
            axes.append(sf::Vertex(sf::Vector2f(3, i), sf::Color::Black));
        }

        for (double i = std::min(0.0, y_end); i >= y_start; i -= size) {
            axes.append(sf::Vertex(sf::Vector2f(-3, i), sf::Color::Black));
            axes.append(sf::Vertex(sf::Vector2f(3, i), sf::Color::Black));
        }



        // 화면을 지우기
        window.clear(sf::Color::White);


        origin_function_one(window, function_1, size, one_variable_function, x_start, x_end);

        window.draw(text_1);
        window.draw(text_2);
        window.draw(text_3);

        window.draw(axes);

        // 변경된 화면 내용을 표시
        window.display();
    }
}
