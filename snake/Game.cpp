#include "Game.h"

namespace Snake {

    void Game::run() {

        while (m_Window.isOpen())
        {
            sf::Event event;
            while (m_Window.pollEvent(event))
                if (event.type == sf::Event::Closed) m_Window.close();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) m_Window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && m_Player.dir != LEFT) m_Player.dir = RIGHT;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && m_Player.dir != UP) m_Player.dir = DOWN;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && m_Player.dir != RIGHT) m_Player.dir = LEFT;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && m_Player.dir != DOWN) m_Player.dir = UP;

            m_Window.clear(BLACK);
            updateSnake();
            if (hasSnakeEatenFood()) {
                m_Player.score += 1;
                growSnake();
                spawnFood();
            }
            drawSnake();
            drawFood();
            drawScore();
            m_Window.display();
        }
    }

    void Game::updateSnake() {

        // Keep track of the position of the last bodypart
        auto last = &m_Player.body[m_Player.body.size() - 1];
        m_Player.lastX = last->x;
        m_Player.lastY = last->y;

        // Move snake's bodyparts
        for (auto i = m_Player.body.size() - 1; i >= 1; i--) {
            m_Player.body[i].x = m_Player.body[i - 1].x;
            m_Player.body[i].y = m_Player.body[i - 1].y;
        }

        // Move the head... and the tail ^ follows!
        auto dir = m_Player.dir;
        sf::Vector2i *head = &m_Player.body[0];
        if (dir == RIGHT) {
            head->x += m_TileSize;
            if (head->x + m_TileSize > m_WindowWidth || hasSnakeEatenHimself()) m_Window.close();

        } else if (dir == DOWN) {
            m_Player.body[0].y += m_TileSize;
            if (head->y + m_TileSize > m_WindowHeight || hasSnakeEatenHimself()) m_Window.close();

        } else if (dir == LEFT) {
            m_Player.body[0].x -= m_TileSize;
            if (head->x < 0 || hasSnakeEatenHimself()) m_Window.close();

        } else if(dir == UP) {
            m_Player.body[0].y -= m_TileSize;
            if (head->y < 0 || hasSnakeEatenHimself()) m_Window.close();

        } else {
            // Incorrect direction... should never occur.
        }
    }

    void Game::growSnake() {
        m_Player.body.push_back(sf::Vector2i(m_Player.lastX, m_Player.lastY));
    }

    bool Game::hasSnakeEatenFood() {
        return m_Player.body[0].x == m_Food.x && m_Player.body[0].y == m_Food.y;
    }

    bool Game::hasSnakeEatenHimself() {
        auto head = &m_Player.body[0];
        for (size_t i = 2; i < m_Player.body.size(); i++) {
            auto bp = &m_Player.body[i];
            if (head->x == bp->x and head->y == bp->y) 
                return true;
        }
        return false;
    }

    void Game::drawSnake() {
        float tileOffset = 0.0;
        sf::RectangleShape shape(sf::Vector2f(m_TileSize - tileOffset, m_TileSize - tileOffset));
        for(size_t i = 0; i < m_Player.body.size(); i++) {
            auto bodyPart = &m_Player.body[i];
            shape.setPosition(bodyPart->x + tileOffset, bodyPart->y + tileOffset);
            if(i != 0 && i % 2 != 0) shape.setFillColor(GREENER);
            else shape.setFillColor(GREEN);
            m_Window.draw(shape);
        }
    }

    void Game::spawnFood() {
        bool invalidSpawn = false;
        do {
            int randX = rand() % m_WindowWidth;
            int randY = rand() % m_WindowHeight;
            m_Food.x = static_cast<int>(randX - (randX % m_TileSize));
            m_Food.y = static_cast<int>(randY - (randY % m_TileSize));
            // Spawned on snake?
            for (size_t i = 0; i < m_Player.body.size(); i++) {
                sf::Vector2i bp = m_Player.body[i];
                if (bp.x == m_Food.x && bp.y == m_Food.y) {
                    invalidSpawn = true;
                    break;
                };
            }
        } while (invalidSpawn);
    }

    void Game::drawFood() {
        sf::RectangleShape shape(sf::Vector2f(static_cast<float>(m_TileSize), static_cast<float>(m_TileSize)));
        shape.setFillColor(RED);
        shape.setPosition(static_cast<float>(m_Food.x), static_cast<float>(m_Food.y));
        m_Window.draw(shape);
    }

    void Game::drawScore() {
        sf::Text text;
        text.setFont(m_Font);
        text.setString("Score " + std::to_string(m_Player.score));
        text.setCharacterSize(24);
        text.setFillColor(WHITE);
        text.setPosition(sf::Vector2f(10.0, 10.0));
        m_Window.draw(text);
    }

    void Game::initSnake() {
        m_Player = {};
        int startX = m_WindowWidth / 2, startY = m_WindowHeight / 2;
        m_Player.body.push_back(sf::Vector2i(startX, startY));
        m_Player.body.push_back(sf::Vector2i(startX - m_TileSize, startY));
        m_Player.dir = RIGHT;
        m_Player.score = 0;
        m_Player.lastX = startX;
        m_Player.lastY = startY;
    }

    Game::Game() :
        m_TileSize(20),
        m_WindowWidth(30 * m_TileSize),
        m_WindowHeight(30 * m_TileSize),
        m_Window(sf::VideoMode(m_WindowWidth, m_WindowHeight), "Snake", sf::Style::Titlebar | sf::Style::Close),
        m_Food({})
    {
        if(!m_Font.loadFromFile("GameFont.ttf")){}
        m_Window.setVerticalSyncEnabled(true);
        m_Window.setFramerateLimit(15);
        srand(static_cast<unsigned int>(time(NULL)));
        initSnake();
        spawnFood(); // Initial piece of food
    }

}