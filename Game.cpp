#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

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
    int lives = 3000;
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
            return 1.5f + static_cast<float>(rand() % 50) / 100.0f; 
        case 2:
            return 1.0f + static_cast<float>(rand() % 50) / 100.0f; 
        case 3:
            return 0.75f + static_cast<float>(rand() % 25) / 100.0f; 
        default:
            return 1.0f;
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

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "The best game ever!");
    window.setFramerateLimit(60);

    sf::Font font;
    font.loadFromFile("arial.ttf");

    Player player;
    TextDisplay scoreDisplay(font, 24, sf::Color::White, 10, 10);
    TextDisplay livesDisplay(font, 24, sf::Color::White, 10, 40);

    int speed = 5;
    sf::Clock clock;
    sf::Time shootDelay = sf::seconds(0.1f);

    std::vector<Enemy> enemies;
    std::vector<Bullet> enemyBullets;
    std::vector<sf::Clock> enemyShootClocks; 
    int score = 0;

    enemies.emplace_back(1, 100, 100);
    enemies.emplace_back(2, 300, 150);
    enemies.emplace_back(3, 500, 200);

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
                window.close();
        }

        player.handleInput(speed, window);
        player.shoot(clock, shootDelay);
        player.updateBullets();

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

        scoreDisplay.setText("Score: " + std::to_string(score));
        livesDisplay.setText("Lives: " + std::to_string(player.getLives()));

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
        window.display();

        if (player.getLives() <= 0)
        {
            window.close();
        }
    }

    return 0;
}
