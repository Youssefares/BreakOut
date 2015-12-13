//
// breakout.c
//
// Computer Science 50
// Problem Set 3
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include <spl/gevents.h>
#include <spl/gobjects.h>
#include <spl/gwindow.h>

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3
#define SCORE 0

#define RECTHEIGHT 12
#define RECTWIDTH 70

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
GLabel updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);
    

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;
    

    // number of points initially
    int points = 0;
    

    // keep playing until game over
    
    
    double Xvelocity = 5*drand48();
    double Yvelocity = Xvelocity;
    
    while (lives > 0 && bricks > 0)
    {
        GEvent event = getNextEvent(MOUSE_EVENT);
        
        
        if (event != NULL)
        {
            if (getEventType(event) == MOUSE_MOVED)
            {
                double x = getX(event) - RECTWIDTH/ 2;
                setLocation(paddle, x, HEIGHT- 2*RECTHEIGHT);
            }
        }
        
        
        move(ball, Xvelocity, Yvelocity);
        
        if (getX(ball) + getWidth(ball) >= getWidth(window))
        {
            Xvelocity = -Xvelocity;
        }
        
        else if (getX(ball) <= 0)
        {
            Xvelocity = -Xvelocity;
        }
        
        if (getY(ball)>= getHeight(window))
        {
            
            lives--;
            if (lives > 0)
            {
                waitForClick();
                setLocation(ball, WIDTH/2 - RADIUS, HEIGHT/2 - RADIUS);
            }
            
        }
        
        else if (getY(ball) <= 0)
        {
            Yvelocity = -Yvelocity;
        }
        
        GObject object = detectCollision(window,ball);
        
        if (object != NULL)
        {
            if (object == paddle)
            {
                Yvelocity = -Yvelocity;
            }
            
            
            else if (strcmp(getType(object), "GRect") == 0 && object != paddle)
            {
                Yvelocity = -Yvelocity;
                removeGWindow(window, object);
                points++;
                bricks--;
                updateScoreboard(window, label, points);
            }
        }
        
        
        
        
        
        
        pause(10);
    }

    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    for (int j = 0; j < COLS; j++)
    {
        for (int i = 0; i < ROWS; i++)
        {
           
            GRect brick = newGRect(j*33 + 10 + 5*j, i*12 + 40 + 5*i, 33, 12);
            setFilled(brick, true);
            setColor(brick,"BLUE");
            add(window, brick);
            
        }
    }
    
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    GOval ball = newGOval (WIDTH/2 - RADIUS, HEIGHT/2 - RADIUS, RADIUS*2, RADIUS*2);
    setFilled(ball, true);
    setColor(ball,"RED");
    add(window, ball);
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    GRect paddle = newGRect((WIDTH/2) - (RECTWIDTH/2),HEIGHT- 2*RECTHEIGHT,RECTWIDTH,RECTHEIGHT);
    setFilled(paddle, true);
    setColor(paddle, "YELLOW");
    add(window, paddle);
    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    GLabel label = newGLabel("0");
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
    add (window, label);
    return label;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
GLabel updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
    return label;
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
