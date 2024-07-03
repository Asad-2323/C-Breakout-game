#include "Game.h"
#include <SFML/Graphics.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
using namespace sf;
Game::Game(int w, int h, std::string title)
{
    width = w;
    height = h;
    window = new RenderWindow(VideoMode(w, h), title); // window in which our game runs

    ball.setRadius(8.0f); // radius of ball
    ball.setFillColor(Color::Magenta); // color of ball
    ball.setOutlineThickness(1.0f); // ball outline thickness
    ball.setOutlineColor(Color(255, 255, 255)); // outline color

    paddle.setSize(Vector2f(100.0f, 20.f)); // size of paddle
    paddle.setFillColor(Color::Cyan); // color of paddle
    paddle.setOutlineThickness(1.0f); 
    paddle.setOutlineColor(Color(255, 255, 255));

    brick.setSize(Vector2f(40,30)); // size of brick
    brick.setOutlineThickness(1.0f);

    initialspeed = 400.0f; // initial speed
    angle = 0.0f;
  
    

    window->setFramerateLimit(60); //framerate limit

    font.loadFromFile("YacimientoExtraBoldEx.ttf"); // loading font file
    score.setFont(font); 
    score.setOutlineThickness(5.0f); 
    score.setOutlineColor(Color::Black); // font color
    
    score.setPosition(Vector2f(0, 0)); // position for score
    name.setFont(font);
    name.setOutlineThickness(5.0f);
    name.setOutlineColor(Color::Black);
    name.setPosition(Vector2f(320, 300)); // position for breakout
    name.setString("Breakout"); // text for game name
    int totalbricks = (width / brick.getSize().x) * (height / brick.getSize().y); // calculation for total number bricks
    isbrick = new bool [totalbricks]; // setting size of array for brick
    for (int i = 0; i < totalbricks; i++)
    {
        isbrick[i] = false; // setting all bricks to off initially
    }
 
    
    reset();
   
}
void Game::setspeed(float sped)
{
    speedinc = sped;
}
Game::~Game()
{
    delete[] isbrick;
    delete window;
}
void Game::reset()
{
    speed = Vector2f(0.0f, 0.0f);  
    speed.x = speed.y = 0; // x and y of ball speed 0
    score.setString("0"); // score string
    for (int i = 0; i < 140; i++)
    {
        isbrick[i] = true; // turning selected amount of bricks on
    }
}
void Game::event() // all events happen here
{
    if (e.type == Event::Closed) // if we press on close window closes
        window->close();
    else if (e.type == Event::MouseMoved) // mouse movement input
    {
        paddle.setPosition(Vector2f(Mouse::getPosition(*window).x - (paddle.getSize().x / 2), height - 40)); // moves paddle to our mouse input
        if (paddle.getPosition().x < 0)
        {
            paddle.setPosition(Vector2f(0, paddle.getPosition().y)); // if mouse goes beyond left paddle stops
        }
        else if (paddle.getPosition().x > width - paddle.getSize().x)
        {
            paddle.setPosition(Vector2f(width - paddle.getSize().x, paddle.getPosition().y)); // if mouse goes beyond right paddle stops
        }
    }
    else if (e.type == Event::MouseButtonPressed) // for starting game  
    {
        ball.setPosition(Vector2f(paddle.getPosition().x + (paddle.getSize().x / 2) - ball.getRadius(), paddle.getPosition().y - paddle.getSize().y)); // puts ball on paddle
        angle = (-20 - (rand() % 140)) * (M_PI/ 180.0); // random angle ball bounced from
        speed.x = cos(angle) * initialspeed; // trajectory of ball on x axis
        speed.y = sin(angle) * initialspeed; // trajectory of ball on y axis
    }
    
}
void Game::update(float DT) // updates to game after event happens
{
    if (speed.y == 0.0f)
    {
        ball.setPosition(Vector2f(paddle.getPosition().x + (paddle.getSize().x / 2) - ball.getRadius(), paddle.getPosition().y - paddle.getSize().y)); // sets ball on middle and top of paddle
    }
    else
    {
        ball.setPosition(Vector2f(ball.getPosition().x + (speed.x * DT), ball.getPosition().y + (speed.y * DT))); // sends out ball with trajectory and speed adjusted for frame rate with delay delta DT
        if (ball.getPosition().x <= 0.0f) // collison with left side
        {
            ball.setPosition(Vector2f(0.0f, ball.getPosition().y));
            speed.x = abs(speed.x);
        }

        else if (ball.getPosition().x + (ball.getRadius() * 2.0f) >= width) // collison with right side
        {
            ball.setPosition(Vector2f(width - (ball.getRadius() * 2.0f), ball.getPosition().y));
            speed.x = -abs(speed.x);
        }
        if (ball.getPosition().y <= 0.0f) // collison with top
        {
            ball.setPosition(Vector2f(ball.getPosition().x, 0.0f));
            speed.y = abs(speed.y);
        }
        else if (ball.getPosition().y + (ball.getRadius() * 2.0f) >= height) // collision with bottom
        {
            reset(); // resets as level is lost
        }
        if (ball.getPosition().x + (ball.getRadius() * 2.0f) >= paddle.getPosition().x &&
            ball.getPosition().y + (ball.getRadius() * 2.0f) >= paddle.getPosition().y &&
            ball.getPosition().x < paddle.getPosition().x + paddle.getSize().x &&
            ball.getPosition().y < paddle.getPosition().y + paddle.getSize().y) // collision with paddly
        {
            float speedinc = 10.0f; // increases speed to increase difficulty 
            float r = abs(speed.x) / abs(speed.y); // ratio of increase of speed 
            if (speed.x < 0.0f)
            {
                speed.x -= speedinc;
            }
            else
            {
                speed.x += speedinc;
            }
            if (speed.y < 0.0f)
            {
                speed.y -= speedinc * r;
            }
            else
            {
                speed.y += speedinc * r;
            }
            ball.setPosition(Vector2f(ball.getPosition().x, paddle.getPosition().y - (ball.getRadius() * 2.0f)));
            speed.y = -abs(speed.y);
        }
        // brick collison loops
        for (int y = 0; y < height / brick.getSize().y; y++) 
        {
            for (int x = 0; x < width / brick.getSize().x; x++)
            {
                if (isbrick[(int)(x + (y * (width / brick.getSize().x)))])
                {
                    if (ball.getPosition().x + (ball.getRadius() * 2.0f) >= x * brick.getSize().x
                        && ball.getPosition().y + (ball.getRadius() * 2.0f) >= y * brick.getSize().y
                        && ball.getPosition().x < (x + 1) * brick.getSize().x
                        && ball.getPosition().y < (y + 1) * brick.getSize().y) // checks when collison
                    {
                        int scoreint = stoi(((string)(score.getString())).c_str()); // converts text in string to int
                        scoreint += 1; // adds 1 to int
                        score.setString(to_string(scoreint)); // converts int to string and puts back as score on display
                        isbrick[(int)(x + (y * (width / brick.getSize().x)))] = false; // turns that brick off
                        speed.y = abs(speed.y); 
                        ball.setPosition(Vector2f(ball.getPosition().x, (y + 1) * brick.getSize().y));
                    }
                }
            }
        }
    }
}
void Game::render()
{// loops to render bricks
    for (int y = 0; y < height; y += brick.getSize().y)
    {
        for (int x = 0; x < width; x += brick.getSize().x)
        {
            if (isbrick[(int)((x / brick.getSize().x) + ((y / brick.getSize().y) * (width / brick.getSize().x)))])
            {
                brick.setFillColor(Color(255, 42, 152));
               
                brick.setOutlineColor(Color(255, 255, 255));
            }
            else
            {
                brick.setFillColor(Color::Black);
                brick.setOutlineColor(Color(40, 40, 40));
            }
            brick.setPosition(Vector2f(x, y)); // increments position of each brick to next brick
            window->draw(brick); // draws brick
        }
    }
    window->draw(paddle); // draws paddle
    window->draw(ball); // draws ball

    window->draw(score); // displays score
    window->draw(name); // display game name
}
// run function
void Game::run()
{
    Clock time; // to measure time between each frame so we can calculate DT
    float dt = 0.0f;

    while (window->isOpen()) 
    {
        time.restart();
        while (window->pollEvent(e))
        {
            event(); // performs all events
        }
        update(dt); // game updates 
        window->clear(); //frame clears
        render(); // new frame rendered 
        window->display(); // new frame displayed
        dt = time.getElapsedTime().asSeconds(); // calculates time between each frame

    } 
}