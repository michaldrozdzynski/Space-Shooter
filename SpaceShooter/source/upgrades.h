#include <SFML/Graphics.hpp>

class Upgrades
{
private:
    int x,y; //współrzędne
    sf::Sprite s;
    bool visible;
public:
    int Type; // typ ulepszenia
    void draw(sf::RenderWindow& window);
    void move();
    void create(sf::Texture&, int,int,int);
    void destroy();
    bool isVisible();
    Upgrades();
    sf::FloatRect pozycja;
};
