
#include "Game.h"
#include <iostream>
#include <cmath>
Game::Game(sf::RenderWindow& game_window)
  : window(game_window)
{
  srand(time(NULL));
  currentState = MENU;

}

Game::~Game()
{
}

bool Game::init()
{
    if (!font.loadFromFile("Data/Fonts/OpenSans-Bold.ttf")) {
        std::cout << "Failed to load font" << std::endl;
        return false;
    }
        
    menuText.setFont(font);
    menuText.setString("CRITTER CROSSING \n\n PRESS ENTER TO START");
    menuText.setCharacterSize(40);
    menuText.setPosition(300, 400);
    menuText.setFillColor(sf::Color::White);

    gameOverText.setFont(font);
    gameOverText.setString("GAME OVER\n\nPress ENTER to Restart");
    gameOverText.setCharacterSize(40);
    gameOverText.setPosition(300, 300);
    gameOverText.setFillColor(sf::Color::Red);

    scoreText.setFont(font);
    scoreText.setPosition(20, 20);
    scoreText.setCharacterSize(24);

    livesText.setFont(font);
    livesText.setPosition(20, 50);
    livesText.setCharacterSize(24);
    livesText.setFillColor(sf::Color::Green);

    passportDateText.setFont(font);
    passportDateText.setCharacterSize(14);
    passportDateText.setFillColor(sf::Color::Black);

    speechBubbleText.setFont(font);
    speechBubbleText.setCharacterSize(18);
    speechBubbleText.setFillColor(sf::Color::Black);

    speechBubbleShape.setFillColor(sf::Color::White);
    speechBubbleShape.setOutlineThickness(2);
    speechBubbleShape.setOutlineColor(sf::Color::Black);

    if (!stampBuffer.loadFromFile("Data/Sounds/stamp.mp3")) {
        std::cout << "Failed stamp.mp3" << std::endl;
    }
    if (!goodBuffer.loadFromFile("Data/Sounds/good.mp3")) {
        std::cout << "Failed good.mp3" << std::endl;
    }

    if (!wrongBuffer.loadFromFile("Data/Sounds/wrong.mp3")) {
        std::cout << "Failed wrong.mp3" << std::endl;
    }


    stampSound.setBuffer(stampBuffer);
    goodSound.setBuffer(goodBuffer);
    wrongSound.setBuffer(wrongBuffer);

    if (!animalTextures[0].loadFromFile("Data/Images/elephant.png")) {
        std::cout << "Failed to load elephant.png" << std::endl;
        return false;
    }
    if (!passportTextures[0].loadFromFile("Data/Images/elephant passport.png")) {
        std::cout << "Failed to load elephant passport.png" << std::endl;
        return false;
    }


    if (!animalTextures[1].loadFromFile("Data/Images/moose.png")) {
        std::cout << "Failed to load moose.png" << std::endl;
        return false;
    }
    if (!passportTextures[1].loadFromFile("Data/Images/moose passport.png")) {
        std::cout << "Failed to load moose passport.png" << std::endl;
        return false;
    }

    if (!animalTextures[2].loadFromFile("Data/Images/penguin.png")) {
        std::cout << "Failed to load penguin.png" << std::endl;
        return false;
    }
    if (!passportTextures[2].loadFromFile("Data/Images/penguin passport.png")) {
        std::cout << "Failed to load penguin passport.png" << std::endl;
        return false;
    }
    if (!bannedSignTexture.loadFromFile("Data/Images/banned.png")) {
        std::cout << "Failed to load banned.png" << std::endl;
        return false;
    }
    if (!acceptBtnTexture.loadFromFile("Data/Images/accept button.png")) return false;
    if (!rejectBtnTexture.loadFromFile("Data/Images/reject button.png")) return false;
    if (!acceptStampTexture.loadFromFile("Data/Images/accept.png")) return false;
    if (!rejectStampTexture.loadFromFile("Data/Images/reject.png")) return false;

    acceptBtnSprite.setTexture(acceptBtnTexture);
    rejectBtnSprite.setTexture(rejectBtnTexture);


    bannedSignSprite.setTexture(bannedSignTexture);
    sf::Vector2u signSize = bannedSignTexture.getSize();
    bannedSignSprite.setOrigin(signSize.x / 2.0f, signSize.y / 2.0f);

    sf::Vector2u animalSize = animalTextures[0].getSize();
    bannedAnimalSprite.setOrigin(animalSize.x / 2.0f, animalSize.y / 2.0f);

    float animalScale = 0.5f;
    bannedAnimalSprite.setScale(animalScale, animalScale);

    
    float signScale = 0.15f;
    bannedSignSprite.setScale(signScale, signScale);

 
    bannedAnimalSprite.setPosition(80, 600);
    bannedSignSprite.setPosition(80, 600);

    stampSprite.setScale(1.0f, 1.0f); 

    animalSprite = std::make_unique<sf::Sprite>();
    passportSprite = std::make_unique<sf::Sprite>();

    resetGame();

    return true;
}

void Game::resetGame() {
    score = 0; 
    lives = 3;
    newAnimal();
    currentState = MENU;

}
void Game::update(float dt)
{
    if (currentState != PLAYING) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
            if (currentState == GAME_OVER) resetGame();
            currentState = PLAYING;
        }
        return;
    }

    scoreText.setString("Score: " + std::to_string(score));
    livesText.setString("Lives: " + std::to_string(lives));
    sf::FloatRect scoreBounds = scoreText.getLocalBounds();
    sf::FloatRect livesBounds = livesText.getLocalBounds();
    scoreText.setOrigin(scoreBounds.width, 0);
    livesText.setOrigin(livesBounds.width, 0);
    scoreText.setPosition(1050, 20);
    livesText.setPosition(1050, 50);


    if (draggedSprite != nullptr) {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);

        draggedSprite->setPosition(mousePos);
    }

    if (passportSprite != nullptr) {
        sf::Vector2f currentScale = sf::Vector2f(1.0f, 1.0f);
        int alpha = 255;

        if (isStamped && draggedSprite == passportSprite.get()) {
            
            sf::Vector2f passPos = passportSprite->getPosition();
            sf::FloatRect animalBounds = animalSprite->getGlobalBounds();
            float animalCenterX = animalBounds.left + (animalBounds.width / 2.0f);
            float animalCenterY = animalBounds.top + (animalBounds.height / 2.0f);

            float dx = passPos.x - animalCenterX;
            float dy = passPos.y - animalCenterY;
            float distance = std::sqrt(dx * dx + dy * dy);


            float startShrinkDistance = 400.0f;
            float minScale = 0.1f;

            if (distance < startShrinkDistance) {

                float ratio = distance / startShrinkDistance;

                float newScale = minScale + (ratio * (1.0f - minScale));
                currentScale = sf::Vector2f(newScale, newScale);
                alpha = static_cast<int>(255 * ratio);
                if (alpha < 0) alpha = 0;
            }
        }
        passportSprite->setScale(currentScale);
        passportSprite->setColor(sf::Color(255, 255, 255, alpha));

        sf::Transform t = passportSprite->getTransform();
        float finalScale = passportSprite->getScale().x;
        sf::Vector2f textPos = t.transformPoint(130, 110);
        passportDateText.setPosition(textPos);
        passportDateText.setFillColor(sf::Color(0, 0, 0, alpha));
        passportDateText.setCharacterSize(static_cast<unsigned int>(14 * finalScale));

        
        if (isStamped) {
            sf::Vector2f stampPos = t.transformPoint(50, 100);
            stampSprite.setPosition(stampPos);
            stampSprite.setScale(finalScale, finalScale);
            stampSprite.setColor(sf::Color(255, 255, 255, alpha));
        }
    }
}
       



void Game::render()
{
    if (currentState == MENU) {
        window.draw(menuText);
        return;
    }
    if (currentState == GAME_OVER) {
        window.draw(gameOverText); 
        window.draw(scoreText);
        return;

    }

	if (animalSprite != nullptr) {
        window.draw(*animalSprite);
    }
    window.draw(speechBubbleShape);
    window.draw(speechBubbleText);

    if (passportSprite != nullptr) {
        window.draw(*passportSprite);
	}
    window.draw(passportDateText);

    window.draw(bannedAnimalSprite);
    window.draw(bannedSignSprite);

    if (isStamped) {
        window.draw(stampSprite);
    }
    if (showContextMenu) {
        window.draw(acceptBtnSprite);
        window.draw(rejectBtnSprite);
    }

    window.draw(scoreText);
    window.draw(livesText);
}


void Game::mouseClicked(sf::Event event)
{
    if (currentState != PLAYING) {
        return;
    }

    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);
    if (event.mouseButton.button == sf::Mouse::Right && !isStamped) {
        showContextMenu = !showContextMenu; 

        if (showContextMenu) {
            
            acceptBtnSprite.setPosition(mousePos.x, mousePos.y);
            
            rejectBtnSprite.setPosition(mousePos.x, mousePos.y + acceptBtnSprite.getGlobalBounds().height + 5);
        }
    }
    if (event.mouseButton.button == sf::Mouse::Left) {

        if (showContextMenu) {
            if (acceptBtnSprite.getGlobalBounds().contains(mousePos)) {
            
                isStamped = true;
                playerDecision = true;
                stampSound.play();
                stampSprite.setTexture(acceptStampTexture);
                showContextMenu = false; 
                return; 
            }
            else if (rejectBtnSprite.getGlobalBounds().contains(mousePos)) {
                isStamped = true;
                playerDecision = false;
                stampSound.play();
                stampSprite.setTexture(rejectStampTexture);
                showContextMenu = false; 
                return;
            }
        }
        if (passportSprite->getGlobalBounds().contains(mousePos)) {
            sf::Vector2f localHit = passportSprite->getInverseTransform().transformPoint(mousePos);
            passportSprite->setOrigin(localHit);
            passportSprite->setPosition(mousePos);
            draggedSprite = passportSprite.get();
        }
        showContextMenu = false;
    }
}


void Game::mouseReleased(sf::Event event)
{
    if (event.mouseButton.button == sf::Mouse::Left) {
        if (passportSprite != nullptr) {
            sf::Vector2f topLeft = passportSprite->getTransform().transformPoint(0, 0);
            passportSprite->setOrigin(0, 0);
            passportSprite->setPosition(topLeft);
            passportSprite->setScale(1.0f, 1.0f);
            passportSprite->setColor(sf::Color(255, 255, 255, 255));
        }
        draggedSprite = nullptr;
        if (isStamped && passportSprite->getGlobalBounds().intersects(animalSprite->getGlobalBounds())) {
            if (playerDecision == shouldAccept) {
                float timeTaken = roundClock.getElapsedTime().asSeconds();
                int timeBonus = 0;
                if (timeTaken < 5.0f) timeBonus = 500;
                else if (timeTaken < 10.0f) timeBonus = 200;
                score += (100 + timeBonus);
                goodSound.play();
                std::cout << "Correct! Time:" << timeTaken << "seconds Points:" << (100 + timeBonus) << std::endl;
            }
            else {
                lives--;
                wrongSound.play();
                std::cout << "Incorrect. Lives remain:" << lives << std::endl;
                if (lives <= 0) {
                    currentState = GAME_OVER;

                }
            
            }
            if (lives > 0) {
                newAnimal();
            }
        }
    }
}

void Game::newAnimal()
{
    int animalIndex = rand() % 3;
    int passportIndex = rand() % 3;

    bannedAnimalIndex = rand() % 3;
    bannedAnimalSprite.setTexture(animalTextures[bannedAnimalIndex], true);
    sf::Vector2u uiAnimalSize = animalTextures[bannedAnimalIndex].getSize();
    bannedAnimalSprite.setOrigin(uiAnimalSize.x / 2.0f, uiAnimalSize.y / 2.0f);

    animalSprite->setTexture(animalTextures[animalIndex], true);
    passportSprite->setTexture(passportTextures[passportIndex], true);
    animalSprite->setPosition(50, 250);

    passportSprite->setPosition(500, 300);

    int day = rand() % 28 + 1;
    int month = rand() % 12 + 1;
    int year = 1980 + (rand() % 30);

    std::string correctDate = std::to_string(day) + "/" + std::to_string(month) + "/" + std::to_string(year);
    std::string passportDate = correctDate;
        
    bool visualMatch = (animalIndex == passportIndex);
    bool dateMatch = true;

    if (rand() % 5 == 0) {
        dateMatch = false;
        int fakeDay = (day + 5) % 28 + 1;
        passportDate = std::to_string(fakeDay) + "/" + std::to_string(month) + "/" + std::to_string(year);
    }

    passportDateText.setString("DOB:" + passportDate);

    speechBubbleText.setString("My Birthday is\n" + correctDate);
    speechBubbleText.setPosition(animalSprite->getPosition().x + 120, animalSprite->getPosition().y - 20);

    sf::FloatRect textBounds = speechBubbleText.getGlobalBounds();
    speechBubbleShape.setSize(sf::Vector2f(textBounds.width + 20, textBounds.height + 20));
    speechBubbleShape.setPosition(textBounds.left - 10, textBounds.top - 10);

    if (visualMatch && dateMatch) {
        if (animalIndex == bannedAnimalIndex) {
            shouldAccept = false;
        }
        else {
            shouldAccept = true;
        }   
    }
    else {
        shouldAccept = false;
    }
    isStamped = false;
    showContextMenu = false;
    roundClock.restart(); 
}

