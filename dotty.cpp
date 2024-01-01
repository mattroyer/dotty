#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>

sf::Vector2f handleInput(float speed) {
  sf::Vector2f velocity(0.f, 0.f);

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
    velocity.x = -speed;
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
    velocity.x = speed;
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
    velocity.y = -speed;
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
    velocity.y = speed;
  }

  return velocity;
}

int main() {
  sf::RenderWindow window(sf::VideoMode(800, 600), "Dotty");
  sf::Clock clock;
  sf::Clock trailClock;
  sf::Clock stationaryClock;
  std::vector<sf::Vector2f> positions;
  float speed = 200.f;
  float trailUpdateInterval = 0.01f;
  size_t maxPositions = 100;

  sf::CircleShape dot(5.f);
  dot.setFillColor(sf::Color::Red);
  dot.setPosition(400.f, 300.f);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    sf::Time elapsed = clock.restart();
    sf::Vector2f velocity = handleInput(speed * elapsed.asSeconds());
    sf::Vector2f oldPosition = dot.getPosition();
    sf::Vector2f newPosition = oldPosition + velocity;

    if (velocity != sf::Vector2f(0.f, 0.f)) {
      if (newPosition.x > 0 && newPosition.x < window.getSize().x - dot.getRadius() * 2 && newPosition.y > 0 && newPosition.y < window.getSize().y - dot.getRadius() * 2) {
        dot.setPosition(newPosition);

        if (trailClock.getElapsedTime().asSeconds() > trailUpdateInterval) {
          positions.push_back(oldPosition);
          if (positions.size() > maxPositions) {
            positions.erase(positions.begin());
          }
          trailClock.restart();
        }
      }
      stationaryClock.restart();
    } else {
      if (!positions.empty() && stationaryClock.getElapsedTime().asSeconds() > 0.007f) {
        positions.erase(positions.begin());
        stationaryClock.restart();
      }
    }

    window.clear();

    for (const auto& position : positions) {
      sf::CircleShape trailDot(5.f);
      trailDot.setFillColor(sf::Color::Red);
      trailDot.setPosition(position);
      window.draw(trailDot);
    }

    window.draw(dot);
    window.display();
  }

  return 0;
}
