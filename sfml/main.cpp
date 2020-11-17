#include<SFML/Graphics.hpp>
#include<iostream>
#include<vector>

int main()
{
    const unsigned int WINDOW_WIDTH = 1440;
    const unsigned int WINDOW_HEIGHT = 1080;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Rick and Morty!");
    window.setFramerateLimit(60);

    //Basics stats
    unsigned int enemySpawnTimer = 1;
    unsigned int lvl = 70;
    unsigned int hpBoostLimit = 100;
    unsigned int ammo = 5;
    unsigned int hp = 20;
    unsigned int maxScore = 2;
    unsigned int maxLvl = 70;

    //Font
    sf::Font font;
    if (!font.loadFromFile("Fonts/BRUSHSCI.TTF"))
        std::cout << "Could not load font!";
    

    //Background
    sf::Texture backgroundTex;
    sf::Sprite background;

    if (!backgroundTex.loadFromFile("Textures/background.png"))
        std::cout << "Could not load background.png";

    background.setTexture(backgroundTex);
    background.setScale(1.5f, 1.5f);

    
    //Player
    sf::Texture playerTex;
    sf::Sprite player;
    
    if (!playerTex.loadFromFile("Textures/rick.png"))
        std::cout << "Could not load rick.png";

    player.setTexture(playerTex);
    player.setScale(sf::Vector2f(0.3f, 0.3f));
    player.setPosition(window.getSize().x / 2, WINDOW_HEIGHT - player.getGlobalBounds().height);

    //HP
    sf::Text hpT;
    hpT.setFont(font);     hpT.setCharacterSize(52);   hpT.setFillColor(sf::Color::Red);
    hpT.setOutlineThickness(2.f);   hpT.setOutlineColor(sf::Color::Black);  hpT.setPosition(10.f, 10.f);

    //Enemy
    sf::Texture enemyTex;
    sf::Sprite enemy;

    if (!enemyTex.loadFromFile("Textures/alien.png"))
        std::cout << "Could not load alien.png";

    enemy.setTexture(enemyTex);
    enemy.setScale(sf::Vector2f(0.3f, 0.3f));

    std::vector<sf::Sprite> enemies;


    //Ammo
    sf::Texture flaskTex; 
    sf::Sprite flask;
    unsigned int score{};
    unsigned int flaskTimer{};

    if (!flaskTex.loadFromFile("Textures/flask.png"))
        std::cout << "Could not load flask.png";

    flask.setTexture(flaskTex);
    flask.setScale(sf::Vector2f(0.17f, 0.17f)); flask.setColor(sf::Color::Yellow); flask.setRotation(6.f);
    std::vector<sf::Sprite> flasks;
    sf::Text ammoT;
    ammoT.setFont(font);    ammoT.setCharacterSize(52);     ammoT.setFillColor(sf::Color::Yellow);
    ammoT.setOutlineThickness(2.f);     ammoT.setOutlineColor(sf::Color::Black);    ammoT.setPosition(10.f, 110.f);

    //HP UP
    sf::Sprite hpBoost;
    unsigned int hpBoostTimer{};
    std::vector<sf::Sprite> hpBoosts;
    hpBoost = flask;
    hpBoost.setColor(sf::Color(255, 0, 0, 255));
    hpBoost.setScale(sf::Vector2f(0.12f, 0.12f));


    //Score
    sf::Text scoreT;
    scoreT.setFont(font);   scoreT.setCharacterSize(52);    scoreT.setFillColor(sf::Color::White);
    scoreT.setOutlineThickness(2.f);    scoreT.setOutlineColor(sf::Color::Black);   scoreT.setPosition(10.f, 60.f);

    //Bullets
    sf::Texture bulletTex;
    sf::Sprite bullet;
    if (!bulletTex.loadFromFile("Textures/portal.png"))
        std::cout << "Could not load portal.png";
    bullet.setTexture(bulletTex);
    bullet.setScale(sf::Vector2f(0.2f, 0.2f));
    unsigned int bulletTimer{};

    std::vector<sf::Sprite> bullets;

    //Game over
    sf::Text gameOverText;
    gameOverText.setFont(font);     gameOverText.setCharacterSize(120);     gameOverText.setFillColor(sf::Color::White);
    gameOverText.setPosition(window.getSize().x / 2, window.getSize().y / 2); gameOverText.setString("GAME OVER");
    gameOverText.setOutlineThickness(2.f);    gameOverText.setOutlineColor(sf::Color::Black);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
        }


        if (hp > 0)
        {
            //UPDATE===============================================UPDATE
        //Controls
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                player.move(15.f, 0);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                player.move(-15.f, 0);

            //Collision
            if (player.getPosition().x < 0)
                player.setPosition(0.f, player.getPosition().y);
            if (player.getPosition().x > WINDOW_WIDTH - player.getGlobalBounds().width)
                player.setPosition(WINDOW_WIDTH - player.getGlobalBounds().width, player.getPosition().y);



            //Bullets
            if (bulletTimer < 15)
                bulletTimer++;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && bulletTimer >= 15 && ammo > 0)
            {
                bullet.setPosition(player.getPosition());
                bullets.push_back(sf::Sprite(bullet));
                bulletTimer = 0;
                ammo--;
            }

            for (size_t i = 0; i < bullets.size(); i++)
            {
                bullets[i].move(0, -5.f);

                if (bullets[i].getPosition().y < 0)
                {
                    bullets.erase(bullets.begin() + i);
                    break;
                }

                for (size_t j = 0; j < enemies.size(); j++)
                {
                    if (bullets[i].getGlobalBounds().intersects(enemies[j].getGlobalBounds()))
                    {
                        score++;
                        bullets.erase(bullets.begin() + i);
                        enemies.erase(enemies.begin() + j);
                        break;
                    }
                }
            }


            //Enemies
            if (enemySpawnTimer < lvl)
                enemySpawnTimer++;

            if (enemySpawnTimer >= lvl)
            {
                enemy.setPosition(rand() % static_cast<int>(window.getSize().x - enemy.getGlobalBounds().width), 0.f);
                enemies.push_back(sf::Sprite(enemy));
                enemySpawnTimer = 0;
            }

            for (size_t i = 0; i < enemies.size(); i++)
            {
                enemies[i].move(0.f, 5.f);

                if (enemies[i].getGlobalBounds().intersects(player.getGlobalBounds()))
                {
                    hp--;
                    enemies.erase(enemies.begin() + i);
                }
                else if (enemies[i].getPosition().y > WINDOW_HEIGHT) //!! if
                {
                    enemies.erase(enemies.begin() + i);
                    break;
                }
            }


            //HIP FLASH YELLOW - AMMO
            if (flaskTimer < 50)
                flaskTimer++;

            if (flaskTimer >= 50)
            {
                flask.setPosition(rand() % static_cast<int>(window.getSize().x - flask.getGlobalBounds().width), 0.f);
                flasks.push_back(sf::Sprite(flask));
                flaskTimer = 0;
            }

            for (size_t i = 0; i < flasks.size(); i++)
            {
                flasks[i].move(0.f, 5.f);

                if (player.getGlobalBounds().intersects(flasks[i].getGlobalBounds()))
                {
                    if (ammo < 10)
                        ammo++;
                    flasks.erase(flasks.begin() + i);
                }

                else if (flasks[i].getPosition().y > WINDOW_HEIGHT)
                {
                    flasks.erase(flasks.begin() + i);
                    break;
                }
            }

            //HIP FLASH RED - HP
            if (hpBoostTimer < hpBoostLimit)
                hpBoostTimer++;

            if (hpBoostTimer >= hpBoostLimit)
            {
                hpBoost.setPosition(rand() % static_cast<int>(window.getSize().x - hpBoost.getGlobalBounds().width), 0.f);
                hpBoosts.push_back(sf::Sprite(hpBoost));
                hpBoostTimer = 0;
            }

            for (size_t i = 0; i < hpBoosts.size(); i++)
            {
                hpBoosts[i].move(0.f, 10.f);
                if (player.getGlobalBounds().intersects(hpBoosts[i].getGlobalBounds()))
                {
                    if (hp < 20)
                        hp++;
                    hpBoosts.erase(hpBoosts.begin() + i);
                }

                else if (hpBoosts[i].getPosition().y > WINDOW_HEIGHT)
                {
                    hpBoosts.erase(hpBoosts.begin() + i);
                    break;
                }
            }

            

            //Change bg color and lvl
            if (score > 2 && lvl >= 70)
            {
                background.setColor(sf::Color(255, 255, 255, 170));
                lvl -= 5;
                hpBoostLimit += 30;
            }          
            else if (score > 4 && lvl >= 65)
            {
                background.setColor(sf::Color(255, 255, 255, 140));
                lvl -= 5;
                hpBoostLimit += 30;
            }
            else if (score > 8 && lvl >= 60)
            {
                background.setColor(sf::Color(255, 255, 255, 100));
                lvl -= 5;
                hpBoostLimit += 50;
            }
            else if (score > 12 && lvl >= 55)
            {
                background.setColor(sf::Color(255, 255, 255, 70));
                lvl -= 10;
                hpBoostLimit += 50;
            }
            else if (score > 16 && lvl >= 45)
            {
                background.setColor(sf::Color(255, 255, 255, 40));
                lvl -= 10;
                hpBoostLimit += 50;
            }
            else if (score > 20 && lvl >= 35)
            {
                background.setColor(sf::Color(255, 255, 255, 50));
                lvl -= 10;
                hpBoostLimit += 30;
            }
            else if (score > 24 && lvl >= 25)
            {
                background.setColor(sf::Color(90, 66, 66, 255));
                lvl -= 5;
                hpBoostLimit += 10;
            }
            else if (score > 30 && lvl >= 20)
            {
                background.setColor(sf::Color(148, 71, 71, 255));
                lvl -= 5;
                hpBoostLimit += 10;
            }
            else if (score > 35 && lvl >= 15)
            {
                background.setColor(sf::Color(200, 39, 39, 255));
            }
              

            //Text
            //UI - HP, SCORE
            scoreT.setString("Score: " + std::to_string(score));
            hpT.setString("HP: " + std::to_string(hp) + " / 20");
            ammoT.setString("Ammo: " + std::to_string(ammo) + " / 10");
        }

        //DRAW===============================================DRAW
        window.clear();

        window.draw(background);
        window.draw(player);
        for (size_t i = 0; i < enemies.size(); i++)
            window.draw(enemies[i]);

        for (size_t i = 0; i < bullets.size(); i++)
            window.draw(bullets[i]);

        for (size_t i = 0; i < flasks.size(); i++)
            window.draw(flasks[i]);

        for (size_t i = 0; i < hpBoosts.size(); i++)
            window.draw(hpBoosts[i]);

        window.draw(scoreT);
        window.draw(hpT);
        window.draw(ammoT);

        if (hp <= 0)
        {
            window.clear(sf::Color::Black);
            window.draw(gameOverText);
        }
            

        window.display();
    }

    return 0;
}