#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <stdlib.h> /* srand, rand */
#include <deque>

#define WHITE sf::Color(255, 255, 255)
#define RED sf::Color(200, 0, 0)
#define GREEN sf::Color(0, 175, 0)
#define GREENER sf::Color(0, 200, 0)
#define GREENEST sf::Color(0, 255, 0)
#define BLACK sf::Color(30, 30, 30)

enum Direction { RIGHT, DOWN, LEFT, UP };

namespace Snake {

	struct snake {
		std::deque<sf::Vector2i> body;
		Direction dir;
		int lastX, lastY;
	};

	class Game
	{
		public:
			Game();
			void run();

		private:
			snake m_Player;
			void initSnake();
			void updateSnake();
			void drawSnake();
			void growSnake();
			bool hasSnakeEatenFood();
			bool hasSnakeEatenHimself();
			sf::Vector2i m_Food;
			void spawnFood();
			void drawFood();

		private:
			int m_TileSize;
			int m_WindowWidth;
			int m_WindowHeight;
			sf::RenderWindow m_Window;
	};

}

#endif