#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>
#include <algorithm>
void saveBestScore(int score , int hp) {
    std::ofstream file("best_score.txt", std::ios::trunc); 
    if (file.is_open()) {
        file << score+hp*150;
        file.close();
    }
}
int loadBestScore() {
    std::ifstream file("best_score.txt");
    int bestScore = 0;
    if (file.is_open()) {
        file >> bestScore; 
        file.close();
    }
    return bestScore;
}

void centerText(sf::Text& text, int yPosition) {
    int windowWidth = 800; 
    sf::FloatRect bounds = text.getLocalBounds();
    int xPosition = (windowWidth - bounds.width) / 2 - bounds.left; 
    text.setPosition(xPosition, yPosition); 
}


class GameOverOrWin {
public:
    GameOverOrWin(const sf::Font& font) {
        MainText.setFont(font);
        MainText.setCharacterSize(88);

        scoreText.setFont(font);
        scoreText.setCharacterSize(30);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(220, 225);

        EnterText.setFont(font);
        EnterText.setCharacterSize(24);
        EnterText.setFillColor(sf::Color::White);
        EnterText.setString("Nacisnij Enter, aby wrocic do menu!");
        EnterText.setPosition(175, 425);


        BestScore.setFont(font);
        BestScore.setCharacterSize(30);
        BestScore.setFillColor(sf::Color::White);

    }

    void setScore(int points, int hp, bool isVictory, bool NewRecord) {
        if (isVictory) {
            MainText.setString("Wygrales!");
            MainText.setFillColor(sf::Color::Green);

        }
        else {
            MainText.setString("Przegrales!");
            MainText.setFillColor(sf::Color::Red);

        }
        int Score = points + hp * 150;
        scoreText.setString("Twoje punkty: " + std::to_string(points) +
            "\nPunkty za serca: " + std::to_string(hp * 150) +
            "\nCalkowity wynik: " + std::to_string(Score));

        if (NewRecord==true) {
            BestScore.setString("Osiagnales nowy najlepszy wynik: "+ std::to_string(Score));
        }
        else {
            BestScore.setString("");
        }

    }

    void Keyboard(const sf::Event& event, bool& returnToMenu) {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
            returnToMenu = true;
        }
    }

    void draw(sf::RenderWindow& window) {
        centerText(MainText, 10);
        centerText(scoreText, 225);
        centerText(EnterText, 425);
        centerText(BestScore, 150);

        window.clear();
        window.draw(MainText);
        window.draw(scoreText);
        window.draw(EnterText);
        window.draw(BestScore);
        window.display();
    }

private:
    sf::Text BestScore;
    sf::Text MainText;
    sf::Text scoreText;
    sf::Text EnterText;
};
class MENU
{
public:
    MENU(const sf::Font& font, const std::string& backgroundPath = "background.png")
    {
        backgroundTexture.loadFromFile(backgroundPath);
        backgroundSprite.setTexture(backgroundTexture);

        TitleText.setFont(font);
        TitleText.setCharacterSize(65);
        TitleText.setFillColor(sf::Color::White);
        TitleText.setString("Space Geometry");

        StartText.setFont(font);
        StartText.setCharacterSize(42);
        StartText.setString("Start");

        ExitText.setFont(font);
        ExitText.setCharacterSize(42);
        ExitText.setString("Wyjdz");
        
        Version.setFont(font);
        Version.setCharacterSize(23);
        Version.setFillColor(sf::Color::White);
        Version.setString("Version:1.0");
        Version.setPosition(0, 568);

        BestScore.setFont(font);
        BestScore.setCharacterSize(30);
        BestScore.setFillColor(sf::Color::Yellow);
    }

    void ChooseOption(const sf::Event& event, bool& startGame, sf::RenderWindow& window)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Up )
            {
                selectedOption = 0;
            }
            if (event.key.code == sf::Keyboard::Down )
            {
                selectedOption = 1;
            }
            else if (event.key.code == sf::Keyboard::Enter)
            {
                if (selectedOption == 0)
                {
                    startGame = true;
                }
                else if (selectedOption == 1)
                {
                    window.close();
                }
            }
        }
    }

    void draw(sf::RenderWindow& window)
    {
        if (selectedOption == 0) {
            StartText.setFillColor(sf::Color::Green);
        }
        else {
            StartText.setFillColor(sf::Color::White);
        }
        if (selectedOption == 1) {
            ExitText.setFillColor(sf::Color::Red);
        }
        else {
            ExitText.setFillColor(sf::Color::White);
        }


        int score = loadBestScore();
        BestScore.setString("Best: " + std::to_string(score));
        window.clear();
        window.draw(backgroundSprite);
        centerText(BestScore, 150);
        centerText(ExitText, 321);
        centerText(StartText, 250);
        centerText(TitleText, 10);
        window.draw(BestScore);
        window.draw(TitleText);
        window.draw(Version);
        window.draw(StartText);
        window.draw(ExitText);
        window.display();
    }

private:
  
    sf::Text TitleText;
    sf::Text StartText;
    sf::Text ExitText ,Version;
    sf::Text BestScore;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
 
    int selectedOption = 0; 
};

class Pause {
public:
    Pause(const std::string& texturePath = "esc.png") {
        pauseTexture.loadFromFile(texturePath);
        pauseSprite.setTexture(pauseTexture);
    }

    void Keyboard(const sf::Event& event, sf::RenderWindow& window, bool& isGameRunning) {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Enter) {
                paused = false;
            }
            else if (event.key.code == sf::Keyboard::Escape) {
                if (paused) {
                    window.close(); 
                }
                else {
                    paused = true; 
                }
            }
        }
    }

    void draw(sf::RenderWindow& window) {
        window.clear();
        window.draw(pauseSprite);
        window.display();
    }

    bool isPaused() const {
        return paused;
    }

private:
    sf::Texture pauseTexture;
    sf::Sprite pauseSprite;
    bool paused = false;
};


class TextDisplay
{
public:
    TextDisplay( sf::Font& font)
    {
        score.setFont(font);
        score.setCharacterSize(24);
        score.setFillColor(sf::Color::Red);
        score.setPosition(200, 10);
        score.setStyle(sf::Text::Bold);

        hp.setFont(font);
        hp.setCharacterSize(24);
        hp.setFillColor(sf::Color::Red);
        hp.setPosition(10, 10);
        hp.setStyle(sf::Text::Bold);

        level.setFont(font);
        level.setCharacterSize(24);
        level.setFillColor(sf::Color::Red);
        level.setPosition(90, 10);
        level.setStyle(sf::Text::Bold);


        background.setFillColor(sf::Color::Black); 
        background.setPosition(0, 0); 
        background.setSize(sf::Vector2f(800, 50)); 

    }


    void setText(int scores, int hps, int levels) {
        score.setString("Score: " + std::to_string(scores));
        hp.setString("Hp: " + std::to_string(hps));
        level.setString("Level: " + std::to_string(levels));
        sf::FloatRect textBounds = score.getGlobalBounds();


    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(background);
        window.draw(score);
        window.draw(hp);
        window.draw(level);
    }
private:
    sf::Text score;
    sf::Text hp;
    sf::Text level;
    sf::RectangleShape background;

};

class Bullet
{
public:
    Bullet(int x, int y, int speed, const sf::Color& color = sf::Color::Red)
        : speed(speed)
    {
        shape.setSize(sf::Vector2f(5, 10));
        shape.setFillColor(color);
        shape.setPosition(x, y);
    }

    void move()
    {
        shape.move(0, speed);
    }

    bool isOffScreen(float windowHeight) const
    {
        return shape.getPosition().y + shape.getSize().y < 0 || shape.getPosition().y > windowHeight;
    }

    sf::FloatRect getBounds() const
    {
        return shape.getGlobalBounds();
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(shape);
    }

    bool isActive() const
    {
        return active;
    }

    void deactivate()
    {
        active = false;
    }

private:
    sf::RectangleShape shape;
    int speed;
    bool active = true;
};

class Player
{
public:
    Player(const std::string& texturePath = "spaceship.png")
    {
        texture.loadFromFile(texturePath);
        sprite.setTexture(texture);
        sprite.setPosition(375, 525);
    }

    void Keyboard(int speed, const sf::RenderWindow& window)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            if (sprite.getPosition().x > 0)
                sprite.move(-speed, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            if (sprite.getPosition().x + sprite.getGlobalBounds().width < window.getSize().x)
                sprite.move(speed, 0);
        }
    }

    void shoot(sf::Clock& clock, sf::Time shootDelay)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && clock.getElapsedTime() > shootDelay)
        {
            int bulletX = static_cast<int>(sprite.getPosition().x + sprite.getGlobalBounds().width / 2 - 2.5f);
            int bulletY = static_cast<int>(sprite.getPosition().y);
            bullets.emplace_back(bulletX, bulletY, -10, sf::Color::Green);
            clock.restart();
        }
    }

    void updateBullets()
    {
        for (auto& bullet : bullets)
        {
            if (bullet.isActive())
            {
                bullet.move();
            }
        }

        bullets.erase(
            std::remove_if(bullets.begin(), bullets.end(), [&](const Bullet& bullet) { return bullet.isOffScreen(600) || !bullet.isActive(); }),
            bullets.end());
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(sprite);
        for (auto& bullet : bullets)
        {
            bullet.draw(window);
        }
    }

    std::vector<Bullet>& getBullets()
    {
        return bullets;
    }

    sf::FloatRect getBounds() const
    {
        return sprite.getGlobalBounds();
    }

    void takeDamage()
    {
        hp--;
    }

    int gethp() const
    {
        return hp;
    }

private:
    sf::Sprite sprite;
    sf::Texture texture;
    std::vector<Bullet> bullets;
    int hp = 10;
};

class Enemy {
public:
    Enemy(int type, float x, float y)
        : type(type), health(type), score(getScoreForType(type)) {
        switch (type) {
        case 1:
            rectangleShape = sf::RectangleShape(sf::Vector2f(30, 30));
            rectangleShape.setFillColor(sf::Color::Green);
            rectangleShape.setPosition(x, y);
            shapeType = 1;
            break;
        case 2:
            circleShape = sf::CircleShape(15);
            circleShape.setFillColor(sf::Color::Yellow);
            circleShape.setPosition(x, y);
            shapeType = 2;
            break;
        case 3:
            circleShape = sf::CircleShape(20, 3);
            circleShape.setFillColor(sf::Color::Red);
            circleShape.setPosition(x, y);
            shapeType = 3;
            break;
        case 4:
            square1 = sf::RectangleShape(sf::Vector2f(25, 25));
            square1.setFillColor(sf::Color::Blue);
            square1.setPosition(x, y);

            square2 = sf::RectangleShape(sf::Vector2f(25, 25));
            square2.setFillColor(sf::Color::Green);
            square2.setPosition(x + 27 , y);

            triangle1 = sf::CircleShape(12.5, 3);
            triangle1.setFillColor(sf::Color::White);
            triangle1.setPosition(x, y-(12.5+0.33*12,5+0.01+19) );

            triangle2 = sf::CircleShape(12.5, 3);
            triangle2.setFillColor(sf::Color::Magenta);
            triangle2.setPosition(x + 27, y - (12.5 + 0.33 * 12, 5 + 0.01 + 19));

            shapeType = 4;
            break;
        }
    }

    sf::FloatRect getBounds() const {
        if (shapeType == 1)
            return rectangleShape.getGlobalBounds();
        else if (shapeType == 2 || shapeType == 3)
            return circleShape.getGlobalBounds();
        else if (shapeType == 4) {
            sf::FloatRect bounds = square1.getGlobalBounds();
            bounds = AddingFigures(bounds, square2.getGlobalBounds());
            bounds = AddingFigures(bounds, triangle1.getGlobalBounds());
            bounds = AddingFigures(bounds, triangle2.getGlobalBounds());
            return bounds;
        }
        return sf::FloatRect();
    }

    bool takeDamage() {
        health--;
        return health <= 0;
    }

    int getScore() const {
        return score;
    }

    void draw(sf::RenderWindow& window) {
        if (shapeType == 1)
            window.draw(rectangleShape);
        else if (shapeType == 2 || shapeType == 3)
            window.draw(circleShape);
        else if (shapeType == 4) {
            window.draw(square1);
            window.draw(square2);
            window.draw(triangle1);
            window.draw(triangle2);
        }
    }

    void move(float dx, float dy) {
        if (shapeType == 1)
            rectangleShape.move(dx, dy);
        else if (shapeType == 2 || shapeType == 3)
            circleShape.move(dx, dy);
        else if (shapeType == 4) {
            square1.move(dx, dy);
            square2.move(dx, dy);
            triangle1.move(dx, dy);
            triangle2.move(dx, dy);
        }
    }

    Bullet shoot() const {
        float x = getBounds().left + getBounds().width / 2 - 2.5f;
        float y = getBounds().top + getBounds().height;
        return Bullet(static_cast<int>(x), static_cast<int>(y), 5);
    }

    float getShootInterval() const {
        switch (shapeType) {
        case 1:
            return 5.0f + static_cast<float>(rand() % 200) / 100.0f;
        case 2:
            return 4.0f + static_cast<float>(rand() % 100) / 100.0f;
        case 3:
            return 3.0f + static_cast<float>(rand() % 100) / 100.0f;
        case 4:
            return 1.5f;
        }
    }

private:
    int type;
    int health;
    int score;
    int shapeType;
    sf::RectangleShape rectangleShape;
    sf::CircleShape circleShape;
    sf::RectangleShape square1;
    sf::RectangleShape square2;
    sf::CircleShape triangle1;
    sf::CircleShape triangle2;
    // ustala hitboxy
    static sf::FloatRect AddingFigures(const sf::FloatRect& a, const sf::FloatRect& b) {
        float left = std::min(a.left, b.left);
        float top = std::min(a.top, b.top);
        float right = std::max(a.left + a.width, b.left + b.width);
        float bottom = std::max(a.top + a.height, b.top + b.height);

        return sf::FloatRect(left, top, right - left, bottom - top);
    }

    static int getScoreForType(int type) {
        switch (type) {
        case 1: return 100;
        case 2: return 500;
        case 3: return 1000;
        case 4: return 5000; 

        }
    }
};


void setupLevel(int level, std::vector<Enemy>& enemies)
{
    enemies.clear();

    switch (level)
    {
    case 1:
        for (int i = 0; i < 9; ++i)
        {
            enemies.emplace_back(1, 50 + i * 70, 100);
        }
        for (int i = 0; i < 4; ++i)
        {
            enemies.emplace_back(1, 50 + i * 70, 175);
        }
        enemies.emplace_back(2, 50 + 4 * 70, 175);
        for (int i = 5; i < 9; ++i)
        {
            enemies.emplace_back(1, 50 + i * 70, 175);
        }
        for (int i = 0; i < 9; ++i)
        {
            enemies.emplace_back(1, 50 + i * 70, 250);
        }

        break;

    case 2:
        for (int i = 0; i < 10; ++i)
        {
            enemies.emplace_back(1, 50 + i * 70, 50);
            enemies.emplace_back(1, 50 + i * 70, 200);
        }


        for (int i = 0; i < 10; ++i)
        {
            if (i % 2 == 0) {
                enemies.emplace_back(1, 50 + i * 70, 100);
                enemies.emplace_back(2, 50 + i * 70, 150);
            }
            else
            {
                enemies.emplace_back(2, 50 + i * 70, 100);
                enemies.emplace_back(1, 50 + i * 70, 150);
            }
        }
        break;

    case 3:
        for (int i = 0; i < 9; ++i)
        {
            enemies.emplace_back(2, 50 + i * 70, 100);
        }
        for (int i = 0; i < 4; ++i)
        {
            enemies.emplace_back(1, 50 + i * 70, 175);
        }
        enemies.emplace_back(3, 50 + 4 * 70, 175);
        for (int i = 5; i < 9; ++i)
        {
            enemies.emplace_back(2, 50 + i * 70, 175);
        }
        for (int i = 0; i < 9; ++i)
        {
            enemies.emplace_back(1, 50 + i * 70, 250);
        }

        break;

    case 4:
        for (int i = 0; i < 6; ++i)
        {
            enemies.emplace_back(3, 50 + i * 135, 100);
        }

        for (int i = 0; i < 8; ++i)
        {
            enemies.emplace_back(1, 75 + i * 86, 200);
        }
        for (int i = 0; i < 3; ++i)
        {
            enemies.emplace_back(2, 125 + i * 250, 300);
        }

        break;

    case 5:
        int changeY = -132;
        int changeY2 = -65;
        enemies.emplace_back(3, 400 + 45, 150 + changeY);
        enemies.emplace_back(3, 400 + 45, 200 + changeY);
        enemies.emplace_back(3, 400 + 45, 250 + changeY);
        enemies.emplace_back(3, 450 + 45, 200 + changeY);
        enemies.emplace_back(3, 350 + 45, 200 + changeY);
        enemies.emplace_back(2, 350 + 45, 250 + changeY);
        enemies.emplace_back(2, 450 + 45, 250 + changeY);
        enemies.emplace_back(2, 350 + 45, 150 + changeY);
        enemies.emplace_back(2, 450 + 45, 150 + changeY);


        enemies.emplace_back(3, 200 - 100, 150 + changeY2);
        enemies.emplace_back(3, 200 - 100, 200 + changeY2);
        enemies.emplace_back(3, 200 - 100, 250 + changeY2);
        enemies.emplace_back(3, 250 - 100, 200 + changeY2);
        enemies.emplace_back(3, 150 - 100, 200 + changeY2);
        enemies.emplace_back(2, 150 - 100, 250 + changeY2);
        enemies.emplace_back(2, 250 - 100, 250 + changeY2);
        enemies.emplace_back(2, 150 - 100, 150 + changeY2);
        enemies.emplace_back(2, 250 - 100, 150 + changeY2);
        for (int i = 0; i < 3; ++i)
        {
            enemies.emplace_back(4, 150 + i *130, 300);
        }
        break;
    }
}

int main() {
   
    sf::RenderWindow window(sf::VideoMode(800, 600), "The best game ever!", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return -1;
    }


    MENU MENU(font);
    GameOverOrWin GameOverOrWin(font);
    sf::Texture helpTexture;
    if (!helpTexture.loadFromFile("help.png")) {
        return -1;
    }
    sf::Sprite helpSprite(helpTexture);


    bool startGame = false;

    while (window.isOpen()) {

        while (!startGame && window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                MENU.ChooseOption(event, startGame, window);
            }
            MENU.draw(window);
        }


        sf::Texture backgroundTexture;
        if (!backgroundTexture.loadFromFile("background.png")) {
            return -1;
        }
        sf::Sprite backgroundSprite(backgroundTexture);

        Player player;
        TextDisplay TextDisplay(font);
        Pause pause;

        int speed = 5;
        int score = 0;
        int level = 1;
        bool gameOver = false;
        bool isHelpActive = true;
        float enemySpeed = 0.12f;
        bool moveRight = true;


        sf::Clock clock;
        sf::Time shootDelay = sf::seconds(0.25);
        std::vector<Enemy> enemies;
        std::vector<Bullet> enemyBullets;
        std::vector<sf::Clock> enemyShootClocks;

        setupLevel(level, enemies);
        for (unsigned int i = 0; i < enemies.size(); ++i) {
            enemyShootClocks.emplace_back();
        }

        while (window.isOpen() && !gameOver) {
           
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F1) {
                    isHelpActive = !isHelpActive;
                }
                if (!isHelpActive) {
                    pause.Keyboard(event, window, gameOver);
                }
            }

            // Wyœwietlanie pomocy
            if (isHelpActive) {
                window.clear();
                window.draw(helpSprite);
                window.display();
                continue;
            }

            // Obs³uga pauzy
            if (pause.isPaused()) {
                pause.draw(window);
                continue;
            }


            player.Keyboard(speed, window);
            player.shoot(clock, shootDelay);
            player.updateBullets();

            // Ruch wrogów
            float moveDistance = moveRight ? enemySpeed : -enemySpeed;
            bool changeDirection = false;

            for (auto& enemy : enemies) {
                enemy.move(moveDistance, 0);
                if (enemy.getBounds().left <= 0 || enemy.getBounds().left + enemy.getBounds().width >= window.getSize().x) {
                    changeDirection = true;
                }
            }

            if (changeDirection) {
                moveRight = !moveRight;
            }

            // Strzelanie wrogów
            for (unsigned int i = 0; i < enemies.size(); ++i) {
                if (enemyShootClocks[i].getElapsedTime() > sf::seconds(enemies[i].getShootInterval())) {
                    enemyBullets.push_back(enemies[i].shoot());
                    enemyShootClocks[i].restart();
                }
            }

            // Kolizje pocisków z graczem
            for (auto& bullet : enemyBullets) {
                bullet.move();
                if (bullet.getBounds().intersects(player.getBounds())) {
                    bullet.deactivate();
                    player.takeDamage();
                    if (player.gethp() <= 0) {
                        gameOver = true;
                        break;
                    }
                }
            }

            if (gameOver) break;

            // Usuwanie nieaktywnych pocisków
            enemyBullets.erase(std::remove_if(enemyBullets.begin(), enemyBullets.end(),
                [&](const Bullet& bullet) { return bullet.isOffScreen(600) || !bullet.isActive(); }),
                enemyBullets.end());

            // Kolizje pocisków gracza z wrogami
            for (auto& bullet : player.getBullets()) {
                if (!bullet.isActive()) continue;

                for (auto it = enemies.begin(); it != enemies.end();) {
                    if (bullet.getBounds().intersects(it->getBounds())) {
                        bullet.deactivate();
                        if (it->takeDamage()) {
                            score += it->getScore();
                            it = enemies.erase(it);
                        }
                        else {
                            ++it;
                        }
                        break;
                    }
                    else {
                        ++it;
                    }
                }
            }

            // Przejœcie do kolejnego poziomu
            if (enemies.empty()) {
                level++;
                if (level > 5) {
                    break;
                }
                setupLevel(level, enemies);
                enemyShootClocks.clear();
                for (unsigned int i = 0; i < enemies.size(); ++i) {
                    enemyShootClocks.emplace_back();
                }
            }

            window.clear();
            window.draw(backgroundSprite);
            player.draw(window);
            //Rysowanie przeciwnikow
            for (auto& enemy : enemies) {
                enemy.draw(window);
            }
            for (auto& bullet : enemyBullets) {
                bullet.draw(window);
            }

            TextDisplay.setText(score, player.gethp(), level);
            TextDisplay.draw(window);
            window.display();
        }

        // Obs³uga koñca gry
        bool NewRecord = false;
        if (player.gethp() <= 0 || level > 5) {
            int BestScore = loadBestScore();
            if (score > BestScore) {
                saveBestScore(score, player.gethp());
                NewRecord = true;
            }
            GameOverOrWin.setScore(score, player.gethp(), level > 5, NewRecord);

            bool returnToMenu = false;
            while (!returnToMenu) {
                sf::Event event;
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) {
                        window.close();
                    }
                    GameOverOrWin.Keyboard(event, returnToMenu);
                }
                GameOverOrWin.draw(window);
            }
        }

        
        startGame = false;
    }

    return 0;
}

