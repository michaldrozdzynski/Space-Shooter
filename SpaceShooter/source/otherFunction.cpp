#include <SFML/Graphics.hpp>
#include "contents.h"

//argumenty funkcji: okno, pozycja myszki, t�o, napis start i napis wyjd� z gry
//funkcja zwraca 1 gdy u�ytkownik kliknie na sprite start, lub 0 gdy kliknie na sprite koniec 
int startGame(sf::RenderWindow& window,sf::Vector2i& pozycja,sf::Sprite& space,sf::Sprite&start, sf::Sprite&koniec)
{
    while(window.isOpen())
    {
        sf::Event e;
        pozycja = sf::Mouse::getPosition(window);
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
                window.close();

            if(start.getGlobalBounds().contains(window.mapPixelToCoords(mousePos)))
            {
                start.setColor(sf::Color(255, 255,255 , 180));
                if (e.type == sf::Event::MouseButtonPressed)
                    {
                        if (e.mouseButton.button == sf::Mouse::Left)
                        {
                            return 1;
                        }
                    }
            }
            else start.setColor(sf::Color(255, 255,255 , 128));
            if(koniec.getGlobalBounds().contains(window.mapPixelToCoords(mousePos)))
            {
               koniec.setColor(sf::Color(255, 255,255 , 180));
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    return 0; 
            }
            else koniec.setColor(sf::Color(255, 255,255 , 128));
        }

        window.draw(space);
        window.draw(start);
        window.draw(koniec);
        window.display();
    }
    return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//argumenty funkcji: okno, pozycja myszki, t�o, napis wzn�w gr� i napis wyjd� z gry
int pauzeGame(sf::RenderWindow& window,sf::Vector2i& pozycja,sf::Sprite& space,sf::Sprite&cont, sf::Sprite&kc)
{
    while(window.isOpen())
    {
        sf::Event e;
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
                window.close();
            if(cont.getGlobalBounds().contains(window.mapPixelToCoords(mousePos)))
            {
                cont.setColor(sf::Color(255, 255,255 , 180));
                 if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    return 1;//po klikni�ciu wzn�w gr� program zwraca 1 co wznawia gr�

            }
            else cont.setColor(sf::Color(255, 255,255 , 128));
            if(kc.getGlobalBounds().contains(window.mapPixelToCoords(mousePos)))
            {
               kc.setColor(sf::Color(255, 255,255 , 180));
               /* if(pozycja.x>=510&&pozycja.x<=910&&pozycja.y>=150&&pozycja.y<=350)
                    return 1;//po klikni�ciu start program zwraca 1 co rozpoczyna gr�*/
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    return 0; // po klikni�ciu wyjd� z gry program zwraca 0 co ko�czy prac� programu
            }
            else kc.setColor(sf::Color(255, 255,255 , 128));
        }

        window.draw(space);
        window.draw(cont);
        window.draw(kc);
        window.display();
    }
    return 0;
}

//Funkcja wy�wietla napis "wygra�e�" lub "przegra�e�" po zako�czonej rozgrywce
int winOrLoseGame(sf::RenderWindow& window,sf::Vector2i& pozycja,sf::Sprite& space,sf::Sprite&wl)
{
    while(window.isOpen())
    {
        sf::Event e;
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
                window.close();

            if(wl.getGlobalBounds().contains(window.mapPixelToCoords(mousePos)))
            {
                wl.setColor(sf::Color(255, 255,255 , 180));
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                return 1;

            }
            else wl.setColor(sf::Color(255, 255,255 , 128));
        }


        window.draw(space);
        window.draw(wl);
        window.display();
    }
    return 0;
}
int gameMechanics(std::vector<std::unique_ptr<Enemys>>& enemys,SpaceShip& s,sf::RenderWindow& window,
sf::Music&music,sf::Vector2i& pozycja,sf::Sprite& space,sf::Sprite& lose,int& licz,sf::Sound& sound2,
int& licz2,int& licz3,int& experience,bool& m_b,int& stage,sf::Sound& sound,sf::Sprite& win)
{
    for(std::vector<std::unique_ptr<Enemys>>::iterator itr = enemys.begin(), koniec = enemys.end(); itr != koniec; ++itr)
        {

            if((*itr)->poza_mapa())
            {
                enemys.erase(itr);
                return 2;
            }
            else if(!(*itr)->Visible&&(((*itr)->Type==1)||(*itr)->Type==2||(*itr)->Type==4))
            {
                enemys.erase(itr);
                return 2;
            }
            else if(((*itr)->Type==3||(*itr)->Type==5||(*itr)->Type==6)&&!(*itr)->Visible&&(*itr)->bron.empty())
            {
                enemys.erase(itr);
                return 2;
            }

            if((*itr)->pozycja_wroga.intersects(s.pozycja_statku)&&(*itr)->Visible)
            {
                if(!s.hp_down())
                ///////////pora�ka/////////////////////
                {
                    window.setMouseCursorVisible(true);
                    music.pause();
                    return winOrLoseGame(window,pozycja,space,lose);
                }
            }

            if(licz>=100)
            {
                if((*itr)->Type==3&&(*itr)->Visible)
                {
                    (*itr)->attack();
                    sound2.play();
                }
            }
            if(licz2>=25)
            {
                if((*itr)->Type==5&&(*itr)->Visible)
                {
                    (*itr)->attack();
                    sound2.play();
                }
            }
            if((*itr)->Type==6&&(*itr)->model==1&&(*itr)->Visible)
            switch(licz3)
            {
             case 70:
            {
                (*itr)->kind_attack = 1;
                (*itr)->attack();
                sound2.play();
            }
             break;
             case 85:
            {
                (*itr)->kind_attack = 2;
                (*itr)->attack();
                sound2.play();
            }
            break;
             case 100:
             {
                (*itr)->kind_attack = 3;
                (*itr)->attack();
                sound2.play();
                licz3=55;
             }
             break;
            }

            if((*itr)->Type==3||(*itr)->Type==5||(*itr)->Type==6)
            {
                for(std::vector<std::unique_ptr<Weapon>>::iterator i = (*itr)->bron.begin(),k = (*itr)->bron.end(); i != k; ++i)
                {
                    if((*i)->poza_mapa()||!(*i)->Visible)
                    {
                        (*itr)->bron.erase(i);
                        return 2;
                    }
                    else if((*i)->pozycja_broni.intersects(s.pozycja_statku)&&(*i)->Visible)
                    {
                        if(!s.hp_down())
                        {
                        ////////////////pora�ka///////////////////////////
                            window.setMouseCursorVisible(true);
                            music.pause();
                            return winOrLoseGame(window,pozycja,space,lose);
                        }
                        (*i)->Visible = false;
                    }
                }
            }
            for(std::vector<std::unique_ptr<Weapon>>::iterator iter = s.bron.begin(), kon = s.bron.end(); iter != kon; ++iter)
            {
                if((*iter)->poza_mapa()||!(*iter)->Visible)
                {
                    s.bron.erase(iter);
                    return 2;
                }
                else if((*iter)->pozycja_broni.intersects((*itr)->pozycja_wroga)&&(*itr)->Visible&&(*iter)->Visible)
                {
                    if((*itr)->hp_down((*iter)->show_damage()))
                    {
                        experience+=(*itr)->exp;
                        (*itr)->Visible=false;
                        if((*itr)->Type==5)
                        {
                            m_b=false;
                            if(stage==3)
                            stage=4;

                        }
                        sound.play();
                        if((*itr)->Type==6)
                        {
                        /////////zwyci�stwo//////////////////////////
                            window.setMouseCursorVisible(true);
                            music.pause();
                            return winOrLoseGame(window,pozycja,space,win);
                        }

                    }
                    else if((*itr)->Type==6&&(*itr)->hp<=2500&&(*itr)->hp>1500)
                    {
                        stage = 2;
                    }
                    else if((*itr)->Type==6&&(*itr)->hp<=1500&&stage==2)
                    {
                        stage = 3;
                    }
                    else if((*itr)->Type==6&&(*itr)->hp<=1000&&stage==4)
                    {
                        stage = 5;
                    }
                    (*iter)->Visible=false;
                }
            }
        }
        return 2;
}
void upgrade(SpaceShip& s,Upgrades& upgrades)
{


    
            if(upgrades.pozycja.intersects(s.pozycja_statku))
            {
                switch(upgrades.Type)
                {
                    case 1:
                    {
                        s.upgrade(1);
                        upgrades.destroy();
                    }
                    break;
                    case 2:
                    {
                        s.upgrade(2);
                        upgrades.destroy();
                    }
                    break;
                    case 3:
                    {
                        s.upgrade(3);
                        upgrades.destroy();
                    }
                    break;
                    case 4:
                    {
                        s.hp_up();
                        upgrades.destroy();
                    }
                    break;
                }
            }
        
}


