#include "contents.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1440, 900), "Space Shooter", sf::Style::Close);
    sf::Vector2i pozycja = sf::Mouse::getPosition(window);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    sf::Texture background;
    background.loadFromFile("img/menuback.jpg");
    sf::Sprite space;
    space.setTexture(background);

   


    sf::Sprite koniec,start;
    sf::Texture start_T, koniec_T;
    start_T.loadFromFile("img/start.png");
    koniec_T.loadFromFile("img/koniec.png");
    start.setTexture(start_T);
    koniec.setTexture(koniec_T);
    start.setPosition(20,150);
    koniec.setPosition(20,500);
    start.setColor(sf::Color(255, 255,255 , 128));
    koniec.setColor(sf::Color(255, 255,255 , 128));

    ////////////petla odpalająca grę////////////////////
    while(true)
    {
        if(startGame(window,pozycja,space,start,koniec)==1)
        {
                if(!first_mission(window,koniec)) return 0;           
        }
        else return 0;
    }

    return 0;
}


