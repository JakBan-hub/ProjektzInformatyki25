#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

class Pause
{
public:

    Pause(const std::string& texturePath)
    {
        pauseTexture.loadFromFile(texturePath);
        pauseSprite.setTexture(pauseTexture);

    }


    void handleEvent(const sf::Event& event, sf::RenderWindow& window, bool showHelp)
    {
        if (showHelp)
        {
            return;
        }

        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
            {
                if (!paused)
                    paused = true;     
                else
                    window.close();    
            }
            else if (event.key.code == sf::Keyboard::Enter)
            {
                if (paused)
                    paused = false;    
            }
        }
    }


    void draw(sf::RenderWindow& window)
    {
        window.clear();
        window.draw(pauseSprite);
        window.display();
    }

    bool isPaused() const
    {
        return paused;
    }

private:
    sf::Texture pauseTexture;
    sf::Sprite  pauseSprite;
    bool paused = false;
};

class TextDisplay
{
public:
    TextDisplay(const sf::Font& font, int size, const sf::Color& color, float x, float y)
    {
        text.setFont(font);
        text.setCharacterSize(size);
        text.setFillColor(color);
        text.setPosition(x, y);
    }

    void setText(const std::string& value)
    {
        text.setString(value);
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(text);
    }
private:
    sf::Text text;
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

    void handleInput(int speed, const sf::RenderWindow& window)
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
        lives--;
    }

    int getLives() const
    {
        return lives;
    }

private:
    sf::Sprite sprite;
    sf::Texture texture;
    std::vector<Bullet> bullets;
    int lives = 10;
};

class Enemy
{
public:
    Enemy(int type, float x, float y)
        : type(type), health(type), score(getScoreForType(type))
    {
        switch (type)
        {
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
        }
    }

    sf::FloatRect getBounds() const
    {
        if (shapeType == 1)
            return rectangleShape.getGlobalBounds();
        else
            return circleShape.getGlobalBounds();
    }

    bool takeDamage()
    {
        health--;
        return health <= 0;
    }

    int getScore() const
    {
        return score;
    }

    void draw(sf::RenderWindow& window)
    {
        if (shapeType == 1)
            window.draw(rectangleShape);
        else
            window.draw(circleShape);
    }

    void move(float dx, float dy)
    {
        if (shapeType == 1)
            rectangleShape.move(dx, dy);
        else
            circleShape.move(dx, dy);
    }

    Bullet shoot() const
    {
        float x = getBounds().left + getBounds().width / 2 - 2.5f;
        float y = getBounds().top + getBounds().height;
        return Bullet(static_cast<int>(x), static_cast<int>(y), 5);
    }

    float getShootInterval() const
    {
        switch (shapeType)
        {
        case 1:
            return 5.0f + static_cast<float>(rand() % 200) / 100.0f;
        case 2:
            return 4.0f + static_cast<float>(rand() % 100) / 100.0f;
        case 3:
            return 3.0f + static_cast<float>(rand() % 100) / 100.0f;

        }
    }


private:
    int type;
    int health;
    int score;
    int shapeType;
    sf::RectangleShape rectangleShape;
    sf::CircleShape circleShape;

    static int getScoreForType(int type)
    {
        switch (type)
        {
        case 1: return 100;
        case 2: return 500;
        case 3: return 1000;
        default: return 0;
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
        break;
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "The best game ever!");
    window.setFramerateLimit(60);

    sf::Font font;
    font.loadFromFile("arial.ttf");

    sf::Texture helpTexture;
    if (!helpTexture.loadFromFile("help.png")) {
        return -1;
    }
    sf::Sprite helpSprite(helpTexture);
    Pause pause("esc.png");

    Player player;
    TextDisplay scoreDisplay(font, 24, sf::Color::White, 10, 10);
    TextDisplay livesDisplay(font, 24, sf::Color::White, 10, 40);
    TextDisplay levelDisplay(font, 24, sf::Color::White, 10, 70);

    int speed = 5;
    sf::Clock clock;
    sf::Time shootDelay = sf::seconds(0.3f);

    std::vector<Enemy> enemies;
    std::vector<Bullet> enemyBullets;
    std::vector<sf::Clock> enemyShootClocks;
    int score = 0;
    int level = 1;

    bool moveRight = true;
    float enemySpeed = 0.09f;

    bool showHelp = false;

    setupLevel(level, enemies);

    for (size_t i = 0; i < enemies.size(); ++i)
    {
        enemyShootClocks.emplace_back();
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }


            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F1)
            {
                if (!pause.isPaused())
                {
                    showHelp = !showHelp;
                }
            }


            pause.handleEvent(event, window, showHelp);
        }

        if (showHelp)
        {
            window.clear();
            window.draw(helpSprite);
            window.display();
            continue;
        }
        if (pause.isPaused())
        {
            pause.draw(window);
            continue;
        }

        player.handleInput(speed, window);
        player.shoot(clock, shootDelay);
        player.updateBullets();

        float moveDistance;
        if (moveRight)
        {
            moveDistance = enemySpeed;
        }
        else
        {
            moveDistance = -enemySpeed;
        }

        bool changeDirection = false;
        for (auto& enemy : enemies)
        {
            enemy.move(moveDistance, 0);
            if (enemy.getBounds().left <= 0 || enemy.getBounds().left + enemy.getBounds().width >= window.getSize().x)
            {
                changeDirection = true;
            }
        }
        if (changeDirection)
        {
            moveRight = !moveRight;
        }

        for (size_t i = 0; i < enemies.size(); ++i)
        {
            if (enemyShootClocks[i].getElapsedTime() > sf::seconds(enemies[i].getShootInterval()))
            {
                enemyBullets.push_back(enemies[i].shoot());
                enemyShootClocks[i].restart();
            }
        }

        for (auto& bullet : enemyBullets)
        {
            bullet.move();
            if (bullet.getBounds().intersects(player.getBounds()))
            {
                bullet.deactivate();
                player.takeDamage();
            }
        }

        enemyBullets.erase(
            std::remove_if(enemyBullets.begin(), enemyBullets.end(), [&](const Bullet& bullet) { return bullet.isOffScreen(600) || !bullet.isActive(); }),
            enemyBullets.end());

        for (auto& bullet : player.getBullets())
        {
            if (!bullet.isActive())
                continue;

            for (auto it = enemies.begin(); it != enemies.end(); )
            {
                if (bullet.getBounds().intersects(it->getBounds()))
                {
                    bullet.deactivate();
                    if (it->takeDamage())
                    {
                        score += it->getScore();
                        it = enemies.erase(it);
                    }
                    else
                    {
                        ++it;
                    }
                    break;
                }
                else
                {
                    ++it;
                }
            }
        }

        if (enemies.empty())
        {
            level++;
            if (level > 5)
            {
                window.close();
            }
            setupLevel(level, enemies);
            enemyShootClocks.clear();
            for (size_t i = 0; i < enemies.size(); ++i)
            {
                enemyShootClocks.emplace_back();
            }
        }

        scoreDisplay.setText("Score: " + std::to_string(score));
        livesDisplay.setText("Lives: " + std::to_string(player.getLives()));

        levelDisplay.setText("Level: " + std::to_string(level));

        window.clear();
        player.draw(window);
        for (auto& enemy : enemies)
        {
            enemy.draw(window);
        }
        for (auto& bullet : enemyBullets)
        {
            bullet.draw(window);
        }
        scoreDisplay.draw(window);
        livesDisplay.draw(window);
        levelDisplay.draw(window);
        window.display();

        if (player.getLives() <= 0)
        {
            window.close();
        }
    }

    return 0;
}
