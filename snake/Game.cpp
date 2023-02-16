#include "Game.h"
#include <iostream>
#include <chrono>
#include <thread>

namespace Snake {

    void Game::run() {

        while (m_Window.isOpen())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(40));

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
                growSnake();
                spawnFood();
            }
            drawFood();
            drawSnake();
            m_Window.display();
        }
    }

    void Game::updateSnake() {

        // Keep track of the position of the last bodypart
        sf::Vector2i last = m_Player.body.back();
        m_Player.lastX = last.x;
        m_Player.lastY = last.y;
        m_Player.body.pop_back();

        // Set new position of head
        auto dir = m_Player.dir;
        sf::Vector2i oldHead = m_Player.body.front();
        auto newHead = sf::Vector2i(oldHead.x, oldHead.y);
        if (dir == RIGHT) {
            if (newHead.x + m_TileSize > m_WindowWidth || hasSnakeEatenHimself()) m_Window.close();
            newHead.x += m_TileSize;

        } else if (dir == DOWN) {
            if (newHead.y + m_TileSize > m_WindowHeight || hasSnakeEatenHimself()) m_Window.close();
            newHead.y += m_TileSize;

        } else if (dir == LEFT) {
            if (newHead.x < 0 || hasSnakeEatenHimself()) m_Window.close();
            newHead.x -= m_TileSize;

        } else if(dir == UP) {
            if (newHead.y < 0 || hasSnakeEatenHimself()) m_Window.close();
            newHead.y -= m_TileSize;

        } else {
            // Incorrect direction... should never occur.
        }
        m_Player.body.push_front(newHead);
    }

    void Game::growSnake() {
        m_Player.body.emplace_back(m_Player.lastX, m_Player.lastY);
    }

    bool Game::hasSnakeEatenFood() {
        return m_Player.body[0].x == m_Food.x && m_Player.body[0].y == m_Food.y;
    }

    bool Game::hasSnakeEatenHimself() {
        auto it = m_Player.body.begin();
        sf::Vector2i head = *it++;
        while(it != m_Player.body.end()) {
            sf::Vector2i bp = *it++;
            if (head.x == bp.x && head.y == bp.y)
                return true;
        }
        return false;
    }

    void Game::drawSnake() {
        float tileOffset = 1.0f;
        sf::RectangleShape shape(sf::Vector2f(m_TileSize - tileOffset, m_TileSize - tileOffset));
        int i = 0;
        for (auto &bp : m_Player.body) {
            shape.setPosition(bp.x + tileOffset, bp.y + tileOffset);
            if (i == 0) shape.setFillColor(GREENEST);
            else if (i % 2) shape.setFillColor(GREENER);
            else shape.setFillColor(GREEN);
            m_Window.draw(shape);
            i++;
        }
    }

    void Game::spawnFood() {
        bool invalidSpawn = false;
        do {
            int randX = rand() % m_WindowWidth;
            int randY = rand() % m_WindowHeight;
            m_Food.x = randX - (randX % m_TileSize);
            m_Food.y = randY - (randY % m_TileSize);
            // Spawned on snake?
            auto it = m_Player.body.begin();
            while (it != m_Player.body.end()) {
                sf::Vector2i bp = *it++;
                if (m_Food.x == bp.x && m_Food.y == bp.y) {
                    invalidSpawn = true;
                    break;
                }
            }
        } while (invalidSpawn);
    }

    void Game::drawFood() {
        sf::RectangleShape shape(sf::Vector2f(static_cast<float>(m_TileSize), static_cast<float>(m_TileSize)));
        shape.setPosition(static_cast<float>(m_Food.x), static_cast<float>(m_Food.y));
        shape.setFillColor(RED);
        m_Window.draw(shape);
    }

    void Game::initSnake() {
        int startX = m_WindowWidth / 2;
        int startY = m_WindowHeight / 2;
        m_Player.body.emplace_back(startX, startY);
        m_Player.body.emplace_back(startX - m_TileSize, startY);
        m_Player.dir = RIGHT;
        m_Player.lastX = startX;
        m_Player.lastY = startY;
    }

    Game::Game() :
        m_TileSize(20),
        m_WindowWidth(30 * m_TileSize),
        m_WindowHeight(30 * m_TileSize),
        m_Window(sf::VideoMode(m_WindowWidth, m_WindowHeight), "Snake", sf::Style::Titlebar | sf::Style::Close),
        m_Player({}),
        m_Food({})
    {
        srand(static_cast<unsigned int>(time(NULL)));
        initSnake();
        spawnFood(); // Initial piece of food
    }

}