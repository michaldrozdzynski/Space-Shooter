#include "contents.h"


int first_mission(sf::RenderWindow& window,sf::Sprite&koniec)
{
    if(window.getSize().x!=1440) return 1;///jeśli wymiary okna się nie zgadzają funkcja zwraca 1
    sf::Vector2i pozycja = sf::Mouse::getPosition(window);
    window.setMouseCursorVisible(false);

    //////tło//////////
    sf::Texture background;
    background.loadFromFile("img/back.png");
    sf::Sprite space;
    space.setTexture(background);

    /////////napis wznów grę//////////
    sf::Texture resume_T;
    resume_T.loadFromFile("img/kontynuuj.png");
    sf::Sprite resume;
    resume.setTexture(resume_T);
    resume.setPosition(20,170);
    resume.setColor(sf::Color(255, 255,255 , 128)); 


  

    sf::Texture bullet_T,boss,boss_bullet_T;
    boss_bullet_T.loadFromFile("img/Boss_weapon.png");

    boss.loadFromFile("img/boss1.png");
    bullet_T.loadFromFile("img/weapon.png");


    SpaceShip s(window,bullet_T);// utworzenie statku kosmicznego brata


    int shot_frequency=10;//częstotliwość strzałów
    int change_frequency=10;//częstotliwość zmiany broni
    bool mini_boss_exist = false;// 
    int experience = 0;
    int neededExp = 500;//potrzebne doświadczenie do upgradu
    int stage = 0;//faza walki z bossem

    ////////napis wygrałeś//////////
    sf::Texture win_T;
    win_T.loadFromFile("img/win.png");
    sf::Sprite win;
    win.setTexture(win_T);
    win.setPosition(20,200);
    win.setColor(sf::Color(255, 255,255 , 128));

    ///////////napis przegrałeś/////////
    sf::Texture lose_T;
    lose_T.loadFromFile("img/lose.png");
    sf::Sprite lose;
    lose.setTexture(lose_T);
    lose.setPosition(20,200);
    lose.setColor(sf::Color(255, 255,255 , 128));

    //////////tekstura ulepszeń////////
    sf::Texture upg;
    upg.loadFromFile("img/upgrade.png");
    
    ////////tekstura wrogów////////////
    sf::Texture enemys_T;
    enemys_T.loadFromFile("img/wrogowie.png");

    ////////////////efekty dźwiękowe///////////
    sf:: SoundBuffer bum,shot;
    if(!bum.loadFromFile("audio/bum.wav")) return 0;
    if(!shot.loadFromFile("audio/shot.wav")) return 0;
    sf::Sound sound,sound2;
    sound.setBuffer(bum);
    sound2.setBuffer(shot);
    sound2.setVolume(3);
    sound.setVolume(10);

    /////////muzyka w tle//////////////////////
    sf::Music music;
    if(!music.openFromFile("audio/1.ogg")) return 0;
    music.setVolume(50);
    music.setLoop(true);
    music.play();
    /////////////////////////////////////

    int licznik = 0;//licznik dzięki któremu pojawiają się przeciwnicy
    int enemy_shot_frequensy = 0;// licznik dzięki któremu przeciwnicy strzelają
    int mini_boss_shot_frequensy = 0;//licznik dzięki któremu mini boss strzela
    int boss_shot_frequensy = 0;//licznik dzięki któremu boss strzela


    bool boss_fight = false;//// zmienna boss_fight ma wartość true gdy walczysz z bossem



    Upgrades boost;
    

    std::vector<std::unique_ptr<Enemys>> enemys;//kontener z przeciwnikami

    int value = 2;//zmienna która przyjmuje warość 2 gdy gra trwa, 1 lub 0 gdy gra się kończy zwycięstwem lub porażką

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)&&shot_frequency>=10)//strzał po kliknięciu LPM
            {
                s.attack();
                shot_frequency=0;
                sound2.play();
            }
            if((sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)||sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))&&change_frequency>=10)
           // zmiana broni po klinięciu w Shift(na początku ma się tylko jedną broń więc Shift nic nie robi)
            {
                s.change_weapon();
                change_frequency=0;
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))//zatrzymanie gry po wciśnięciu ESC
            {
                music.pause();
                window.setMouseCursorVisible(true);
                if(!pauzeGame(window,pozycja,space,resume,koniec)) return 0;
                window.setMouseCursorVisible(false);
                s.position(window);
                music.play();
            }
        }

        if(experience>=neededExp)//sprawdza czy gracz ma wystarczająco doświadczenia
        {
            experience+=-neededExp;
            neededExp+=100;

            int los1,los2;
            los1 = (rand()%10)+1;//wylosowanie upgradu
            los2 = (rand()%1300)+100;//wylosowanie położenia startowego upgradu
            boost.create(upg,los2,-30,los1);//stworzenie upgradu
        }

        //////////////tworzenie przeciwników////////////////////////////////////////////////
        switch(licznik)
        {
        case 0:
        case 100:
        case 200:
        case 300:
        case 400:
        case 600:
        case 800:
        case 1000:
        case 1200:
        case 1400:
        case 1700:
        case 2000:
        case 4900:
        {
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(25,-30,1,enemys_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(335,-30,1,enemys_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(645,-30,1,enemys_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(955,-30,1,enemys_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(1265,-30,1,enemys_T,false)));
        }
        break;
        case 500:
        case 700:
        case 900:
        case 1500:
        case 1800:
        case 2100:
        case 2300:
        case 2500:
        case 2700:
        case 2900:
        case 5100:
        case 5900:
        {
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(25,-30,1,enemys_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(335,-30,1,enemys_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(645,-30,1,enemys_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(955,-30,1,enemys_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(1265,-30,1,enemys_T,false)));
        }
        break;
        case 1100:
        case 1300:
        case 1600:
        case 1900:
        case 2200:
        case 2400:
        case 2600:
        case 2800:
        case 3000:
        case 5300:
        {
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(25,-30,1,enemys_T,bullet_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(335,-30,1,enemys_T,bullet_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(645,-30,1,enemys_T,bullet_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(955,-30,1,enemys_T,bullet_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(1265,-30,1,enemys_T,bullet_T,false)));
        }
        break;
        case 3400:
        {
            enemys.push_back(std::unique_ptr<Enemys>(new Meteor(505,-30,1,enemys_T)));
            enemys.push_back(std::unique_ptr<Enemys>(new Meteor(335,-45,1,enemys_T)));
            enemys.push_back(std::unique_ptr<Enemys>(new Meteor(645,-40,3,enemys_T)));
            enemys.push_back(std::unique_ptr<Enemys>(new Meteor(1205,-42,1,enemys_T)));
            enemys.push_back(std::unique_ptr<Enemys>(new Meteor(1265,-80,4,enemys_T)));
            enemys.push_back(std::unique_ptr<Enemys>(new Meteor(250,-157,1,enemys_T)));
            enemys.push_back(std::unique_ptr<Enemys>(new Meteor(335,-204,2,enemys_T)));
            enemys.push_back(std::unique_ptr<Enemys>(new Meteor(316,-303,1,enemys_T)));
            enemys.push_back(std::unique_ptr<Enemys>(new Meteor(645,-192,2,enemys_T)));
            enemys.push_back(std::unique_ptr<Enemys>(new Meteor(955,-225,1,enemys_T)));
            enemys.push_back(std::unique_ptr<Enemys>(new Meteor(797,-214,4,enemys_T)));
            enemys.push_back(std::unique_ptr<Enemys>(new Meteor(519,-249,3,enemys_T)));
            enemys.push_back(std::unique_ptr<Enemys>(new Meteor(64,-239,3,enemys_T)));
            enemys.push_back(std::unique_ptr<Enemys>(new Meteor(25,-330,1,enemys_T)));
            enemys.push_back(std::unique_ptr<Enemys>(new Meteor(335,-345,1,enemys_T)));
            enemys.push_back(std::unique_ptr<Enemys>(new Meteor(645,-340,3,enemys_T)));
            enemys.push_back(std::unique_ptr<Enemys>(new Meteor(955,-342,1,enemys_T)));
            enemys.push_back(std::unique_ptr<Enemys>(new Meteor(1105,-280,4,enemys_T)));
            enemys.push_back(std::unique_ptr<Enemys>(new Meteor(250,-457,1,enemys_T)));
            enemys.push_back(std::unique_ptr<Enemys>(new Meteor(385,-504,2,enemys_T)));
            enemys.push_back(std::unique_ptr<Enemys>(new Meteor(1245,-492,2,enemys_T)));
            enemys.push_back(std::unique_ptr<Enemys>(new Meteor(1001,-525,1,enemys_T)));
            enemys.push_back(std::unique_ptr<Enemys>(new Meteor(97,-514,4,enemys_T)));
            enemys.push_back(std::unique_ptr<Enemys>(new Meteor(156,-503,1,enemys_T)));
            enemys.push_back(std::unique_ptr<Enemys>(new Meteor(469,-549,3,enemys_T)));
            enemys.push_back(std::unique_ptr<Enemys>(new Meteor(664,-539,3,enemys_T)));
        }
        break;
        case 3600:
        {
            enemys.push_back(std::unique_ptr<Enemys>(new Mini_boss(-150,300,1,enemys_T,bullet_T)));
            mini_boss_exist = true;
        }
        break;
        case 3700:
        case 3900:
        {
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(25,-30,1,enemys_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(335,-30,2,enemys_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(645,-30,1,enemys_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(955,-30,2,enemys_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(1265,-30,1,enemys_T,false)));
        }
        break;
        case 3800:
        case 4000:
        {
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(25,-30,2,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(335,-30,1,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(645,-30,2,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(955,-30,1,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(1265,-30,2,enemys_T,true)));
        }
        break;
        case 4100:
        case 4300:
        {
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(25,-30,1,enemys_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(335,-30,2,enemys_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(645,-30,1,enemys_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(955,-30,2,enemys_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(1265,-30,1,enemys_T,false)));
        }
        break;
        case 4200:
        case 4400:
        {
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(25,-30,2,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(335,-30,1,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(645,-30,2,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(955,-30,1,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(1265,-30,2,enemys_T,true)));
        }
        break;
        case 4500:
        case 4700:
        {
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(25,-30,1,enemys_T,bullet_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(335,-30,2,enemys_T,bullet_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(645,-30,1,enemys_T,bullet_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(955,-30,2,enemys_T,bullet_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(1265,-30,1,enemys_T,bullet_T,false)));
        }
        break;
        case 4600:
        case 4800:
        {
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(25,-30,2,enemys_T,bullet_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(335,-30,1,enemys_T,bullet_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(645,-30,2,enemys_T,bullet_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(955,-30,1,enemys_T,bullet_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(1265,-30,2,enemys_T,bullet_T,true)));
        }
        break;
        case 5000:
        {
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(25,-30,2,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(335,-30,2,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(645,-30,2,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(955,-30,2,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(1265,-30,2,enemys_T,true)));
        }
        break;

        case 5200:
        {
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(25,-30,2,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(335,-30,2,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(645,-30,2,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(955,-30,2,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(1265,-30,2,enemys_T,true)));
        }
        break;

        case 5400:
        {
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(25,-30,2,enemys_T,bullet_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(335,-30,2,enemys_T,bullet_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(645,-30,2,enemys_T,bullet_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(955,-30,2,enemys_T,bullet_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(1265,-30,2,enemys_T,bullet_T,true)));
        }
        break;
        case 5500:
        {
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(25,-30,1,enemys_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(335,-30,1,enemys_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(645,-30,1,enemys_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(955,-30,1,enemys_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(1265,-30,1,enemys_T,false)));
        }
        break;
        case 5600:
        {
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(25,-30,1,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(335,-30,1,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(645,-30,1,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(955,-30,1,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(1265,-30,1,enemys_T,true)));
        }
        break;
        case 5700:
        {
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(25,-30,1,enemys_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(335,-30,1,enemys_T,bullet_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(645,-30,1,enemys_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(955,-30,1,enemys_T,bullet_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(1265,-30,1,enemys_T,false)));
        }
        break;
        case 5800:
        {
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(25,-30,1,enemys_T,bullet_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(335,-30,1,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(645,-30,1,enemys_T,bullet_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(955,-30,1,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(1265,-30,1,enemys_T,bullet_T,true)));
        }
        break;

        case 6000:
        {
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(25,-30,1,enemys_T,bullet_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(335,-30,1,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(645,-30,1,enemys_T,bullet_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(955,-30,1,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(1265,-30,1,enemys_T,bullet_T,true)));
        }
        break;
        case 6100:
        {
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(25,-30,2,enemys_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(335,-30,1,enemys_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(645,-30,2,enemys_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(955,-30,1,enemys_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(1265,-30,2,enemys_T,false)));
        }
        break;
        case 6200:
        {
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(25,-30,2,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(335,-30,1,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(645,-30,2,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(955,-30,1,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(1265,-30,2,enemys_T,true)));
        }
        break;
        case 6300:
        {
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(25,-30,2,enemys_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(335,-30,1,enemys_T,bullet_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(645,-30,2,enemys_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(955,-30,1,enemys_T,bullet_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(1265,-30,2,enemys_T,false)));
        }
        break;
        case 6400:
        {
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(25,-30,2,enemys_T,bullet_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(335,-30,1,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(645,-30,2,enemys_T,bullet_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(955,-30,1,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(1265,-30,2,enemys_T,bullet_T,true)));
        }
        break;
        case 6500:
        {
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(25,-30,2,enemys_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(335,-30,1,enemys_T,bullet_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(645,-30,2,enemys_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(955,-30,1,enemys_T,bullet_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(1265,-30,2,enemys_T,false)));
        }
        break;
        case 6600:
        {
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(25,-30,2,enemys_T,bullet_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(335,-30,1,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(645,-30,2,enemys_T,bullet_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(955,-30,1,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(1265,-30,2,enemys_T,bullet_T,true)));
        }
        break;
        case 6700:
        {
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(25,-30,2,enemys_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(335,-30,2,enemys_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(645,-30,2,enemys_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(955,-30,2,enemys_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(1265,-30,2,enemys_T,false)));
        }
        break;
        case 6800:
        {
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(25,-30,2,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(335,-30,2,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(645,-30,2,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(955,-30,2,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(1265,-30,2,enemys_T,true)));
        }
        break;
        case 6900:
        {
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(25,-30,2,enemys_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(335,-30,2,enemys_T,bullet_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(645,-30,2,enemys_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(955,-30,2,enemys_T,bullet_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(1265,-30,2,enemys_T,false)));
        }
        break;
        case 7000:
        {
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(25,-30,2,enemys_T,bullet_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(335,-30,2,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(645,-30,2,enemys_T,bullet_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Easy(955,-30,2,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(1265,-30,2,enemys_T,bullet_T,true)));
        }
        break;
        case 7100:
        {
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(25,-30,2,enemys_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(335,-30,2,enemys_T,bullet_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(645,-30,2,enemys_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(955,-30,2,enemys_T,bullet_T,false)));
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(1265,-30,2,enemys_T,false)));
        }
        break;
        case 7200:
        {
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(25,-30,2,enemys_T,bullet_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(335,-30,2,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(645,-30,2,enemys_T,bullet_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Medium(955,-30,2,enemys_T,true)));
            enemys.push_back(std::unique_ptr<Enemys>(new Hard(1265,-30,2,enemys_T,bullet_T,true)));
        }
        break;
        case 7600:
        {
            enemys.push_back(std::unique_ptr<Enemys>(new Boss(524,-300,1,boss,boss_bullet_T)));
            boss_fight = true;
        }
        break;

        }

        enemy_shot_frequensy++;
        mini_boss_shot_frequensy++;
        shot_frequency++;
        change_frequency++;

        if(mini_boss_exist &&!boss_fight)licznik = 3690;//po pojawieniu się minibossa licznik zatrzymuje się na 3690(chyba że jednocześnie walczysz z bossem)
        if(!mini_boss_exist)licznik++;//gdy nie ma minibossa licznik normalnie rośnie
        if(stage==3&&!mini_boss_exist)//po rozpoczęciu 3 fazy walki z bossem pojawia się miniboss
        {
            enemys.push_back(std::unique_ptr<Enemys>(new Mini_boss(-150,300,1,enemys_T,bullet_T)));
            mini_boss_exist = true;
        }

        value = gameMechanics(enemys,s,window,music,pozycja,space,lose, enemy_shot_frequensy,sound2,mini_boss_shot_frequensy,boss_shot_frequensy,experience,mini_boss_exist,stage,sound,win);

        if(value==1)return 1;// spełnienie tych warunków oznacza zwycięstwo lub porażkę i przerwanie działania funkcji
        if(value==0)return 0;//

        upgrade(s,boost);


        if(enemy_shot_frequensy >=100) enemy_shot_frequensy = 0;//reset licznika
        if(mini_boss_shot_frequensy>=25) mini_boss_shot_frequensy = 0;//reset licznika
        if(boss_fight)boss_shot_frequensy++;//podczas walki z bossem licznik rośnie


        s.move(window);

        window.clear();
        window.draw(space);


        //////////////pętla w pętli powodująca odbijanie się od siebie przeciwników////////////////////////////////
        for(std::vector<std::unique_ptr<Enemys>>::iterator itr = enemys.begin(), koniec = enemys.end(); itr != koniec; ++itr)
        {
            for(std::vector<std::unique_ptr<Enemys>>::iterator i = enemys.begin(),k = enemys.end(); i != k; ++i)
            {
                if(i!=itr&&(*itr)->pozycja_wroga.intersects((*i)->pozycja_wroga)&&(*itr)->Visible&&(*i)->Visible&&(*itr)->Type!=4)
                {
                    if((*itr)->right == false) (*itr)->right=true;
                    else (*itr)->right = false;
                }
            }
            (*itr)->draw(window);
            (*itr)->move(s.x,stage);
        }

        if(boost.isVisible())
        {
            boost.draw(window);
            boost.move();
        }
        s.draw(window);
        window.display();
    }

    return 0;
}

