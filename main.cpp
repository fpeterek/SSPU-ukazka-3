#include <iostream>
#include <vector>
#include <cmath>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

double toRad(double degrees) {
    return degrees * M_PI / 180.0;
}

std::vector<sf::CircleShape> initCirclesCos(const sf::Vector2u initPos, const uint64_t count, const float scale) {

    std::vector<sf::CircleShape> vector;

    for (uint64_t i = 0; i < count; ++i) {

        vector.emplace_back();
        sf::CircleShape & c = vector.back();

        int64_t opacity = (count*10.f - i*10.f) / (count * 10.f) * 255.f;
        opacity = std::max(opacity, 0l);
        opacity = std::min(opacity, 255l);
        c.setFillColor(sf::Color(255, 255, 255, opacity));

        const uint64_t radius = 16.f * i/5.f * scale;

        c.setRadius(radius);
        c.setOrigin(c.getLocalBounds().width / 2, c.getLocalBounds().width / 2);

        const uint64_t x = initPos.x;
        const uint64_t y = initPos.y - (i * 8.f * scale);

        c.setPosition(x, y);

    }

    return vector;

}

std::vector<sf::CircleShape> initCirclesLin(const sf::Vector2u initPos, const uint64_t count, const float scale) {

    std::vector<sf::CircleShape> vector;

    for (uint64_t i = 0; i < count; ++i) {

        vector.emplace_back();
        sf::CircleShape & c = vector.back();

        int64_t opacity = (count*10.f - i*10.f) / (count * 10.f) * 255.f;
        opacity = std::max(opacity, 0l);
        opacity = std::min(opacity, 255l);
        c.setFillColor(sf::Color(255, 255, 255, opacity));

        const uint64_t radius = 16.f * i/5.f * scale;

        c.setRadius(radius);
        c.setOrigin(c.getLocalBounds().width / 2, c.getLocalBounds().width / 2);

        const uint64_t x = initPos.x;
        const uint64_t y = initPos.y - (i * 8.f * scale);

        c.setPosition(x, y);

    }

    return vector;

}

void updateCirclesCos(std::vector<sf::CircleShape> & circles, const uint64_t initX) {

    static uint64_t begin = 0;

    uint64_t angle = begin;

    for (size_t i = 0; i < circles.size(); ++i) {

        sf::CircleShape & c = circles[i];

        float x = initX + (i*4.0 * std::cos(toRad(angle)) * c.getScale().x);
        float y = c.getPosition().y;

        c.setPosition(x, y);

        angle += 10;
        angle %= 360;

    }

    begin += 10;
    begin %= 360;

}

void updateCircleLin(std::vector<sf::CircleShape> & circles, const uint64_t initX) {

    static float modifier = 0.0;
    static int64_t mult = 1;

    float mod = modifier;
    int64_t m = mult;

    for (size_t i = 0; i < circles.size(); ++i) {

        sf::CircleShape & c = circles[i];

        float x = initX + i * 2.0 * mod * c.getScale().x;
        float y = c.getPosition().y;

        c.setPosition(x, y);

        mod += m * 0.2f;
        if (mod > 2.15f) {
            m = -1;
        }
        if (mod < -2.15f) {
            m = 1;
        }

    }

    modifier += mult * 0.2f;
    if (modifier > 2.15f) {
        mult = -1;
    }
    if (modifier < -2.15f) {
        mult = 1;
    }

}

sf::CircleShape initCircle(const sf::Vector2u winSize, const float scale) {

    sf::CircleShape circle;

    circle.setScale(scale, scale);
    circle.setRadius(75);
    circle.setPointCount(circle.getPointCount() * 2);
    circle.setOrigin(circle.getLocalBounds().width / 2, circle.getLocalBounds().width / 2);
    circle.setPosition(winSize.x / 4 * 3, winSize.y / 2);
    circle.setFillColor(sf::Color::White);
    circle.setOutlineColor(sf::Color::Black);
    circle.setOutlineThickness(2);

    return circle;

}

std::vector<sf::CircleShape> randSpots(const sf::CircleShape & circle) {

    std::vector<sf::CircleShape> spots;

    const int64_t baseX = circle.getPosition().x;
    const int64_t baseY = circle.getPosition().y;
    const int64_t radius = circle.getRadius();

    const float scale = circle.getScale().x;

    for (uint64_t i = 0; i < 15; ++i) {

        spots.emplace_back();

        sf::CircleShape & spot = spots.back();
        spot.setFillColor(sf::Color::Black);
        spot.setRadius(3 + std::rand() % 4);
        spot.setScale(scale, scale);
        spot.setOrigin(spot.getLocalBounds().width / 2, spot.getLocalBounds().height / 2);

        const uint64_t max = (radius - spot.getRadius()) * scale;

        const float angle = toRad(std::rand() % 360);
        const uint64_t dist = std::rand() % max;

        const int64_t x = baseX + std::cos(angle) * dist;
        const int64_t y = baseY + std::sin(angle) * dist;

        spot.setPosition(x, y);

    }

    return spots;

}

int main() {

    std::srand(std::time(nullptr));

    sf::RenderWindow win(sf::VideoMode::getDesktopMode(), "Test", sf::Style::Fullscreen);

    win.setFramerateLimit(30);

    const float scale = win.getSize().x / 800.f;

    constexpr uint64_t count = 20;

    sf::Vector2u initPos(win.getSize().x / 4, win.getSize().y / 10 * 9);
    auto v1 = initCirclesLin(initPos, count, scale);
    sf::Vector2u initPos2 = initPos;
    initPos2.x = win.getSize().x / 2;
    auto v2 = initCirclesCos(initPos2, count, scale);

    sf::CircleShape circle = initCircle(win.getSize(), scale);
    std::vector<sf::CircleShape> spots;

    uint64_t spotCounter = 0;

    while (win.isOpen()) {

        sf::Event ev;
        while (win.pollEvent(ev)) {

            if (ev.type == sf::Event::Closed) {
                win.close();
                return 0;
            }

            if (ev.type == sf::Event::KeyPressed and ev.key.code == sf::Keyboard::Escape) {
                win.close();
                return 0;
            }

        }

        updateCircleLin(v1, initPos.x);
        updateCirclesCos(v2, initPos2.x);

        if (not (spotCounter % 30)) {
            spots = randSpots(circle);
        }
        ++spotCounter;

        win.clear(sf::Color(230, 119, 119));
        for (size_t i = 0; i < v1.size(); ++i) {
            win.draw(v1[i]);
            win.draw(v2[i]);
        }
        win.draw(circle);
        for (const auto & spot : spots) {
            win.draw(spot);
        }
        win.display();

    }

}
