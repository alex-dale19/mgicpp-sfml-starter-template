
#ifndef PLATFORMER_GAME_H
#define PLATFORMER_GAME_H
#include <memory>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
enum GameState{ MENU, PLAYING, GAME_OVER};

class Game
{
 public:
  Game(sf::RenderWindow& window);
  ~Game();
  bool init();
  void update(float dt);
  void render();
  void newAnimal();
  void mouseClicked(sf::Event event);
  void mouseReleased(sf::Event event);
  void resetGame();

	
 private:
  sf::RenderWindow& window;
  sf::Font font;

  sf::Texture acceptBtnTexture;
  sf::Texture rejectBtnTexture;
  sf::Texture acceptStampTexture;
  sf::Texture rejectStampTexture;

  sf::Texture animalTextures[3];
  sf::Texture passportTextures[3];

  sf::Text menuText;
  sf::Text scoreText;
  sf::Text livesText;
  sf::Text gameOverText;

  sf::Text passportDateText;
  sf::Text speechBubbleText;
  sf::RectangleShape speechBubbleShape;

  sf::SoundBuffer stampBuffer;
  sf::SoundBuffer goodBuffer;
  sf::SoundBuffer wrongBuffer;

  sf::Sound stampSound;
  sf::Sound goodSound;
  sf::Sound wrongSound;

  GameState currentState;

  std::unique_ptr<sf::Sprite> animalSprite;
  std::unique_ptr<sf::Sprite> passportSprite;
  sf::Sprite bannedAnimalSprite; 
  sf::Sprite bannedSignSprite;   
  sf::Texture bannedSignTexture;
  sf::Sprite acceptBtnSprite;
  sf::Sprite rejectBtnSprite;
  sf::Sprite stampSprite; 

  sf::Sprite* draggedSprite = nullptr;
  sf::Vector2f dragOffset;

  bool showContextMenu = false; 
  bool isStamped = false;   
  bool shouldAccept;
  bool playerDecision;

  int score;
  int lives;
  sf::Clock roundClock;
  int bannedAnimalIndex;

};

#endif // PLATFORMER_GAME_H
