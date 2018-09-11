#include <graphics.h>
#include <conio.h>
#include <graphics.h>
#include <stdlib.h>
#include <time.h>

#define UP 72
#define LEFT 75
#define DOWN 80
#define RIGHT 77
#define ESC 27
#define ENTER 13

int score = 0, highscore = 0, inputkey, game = 0, startmenu = 1, gameover = 0, quit = 0, paused = 0, selectsnake = 0,pausemenu = 0, custommenu = 0, radius = 8, spdext = 0, bendtarget = 0, box = 0;

class point{
public:
    int x,y;
};

class Snake{

public:
    int length, diretion = RIGHT, leftBend , rightBend, shift = 0;
    point body[50*2];

    Snake(){
        length = 3 * 2;
        leftBend = 1, rightBend = 0;
        diretion = RIGHT;
        //make snake shape
        body[0].x = getmaxx() / 2;
        body[0].y = getmaxy() / 2;

        for(int i = 1; i < 100; ++i) body[i].x = 0, body[i].y = 0;
    }

    void Move(){

        switch(diretion){
            case DOWN :
                this->body[0].y += radius + spdext;if(!bendtarget)break;
                if(this->leftBend ){this->body[0].x += radius;this->leftBend -= 1;if(this->leftBend == 0)this->rightBend = bendtarget;break;}
                if(this->rightBend ){this->body[0].x -= radius;this->rightBend -= 1;if(this->rightBend == 0)this->leftBend = bendtarget;break;}
            case UP :
                this->body[0].y -= radius + spdext;if(!bendtarget)break;
                if(this->leftBend ){this->body[0].x += radius;this->leftBend -= 1;if(this->leftBend == 0)this->rightBend = bendtarget;break;}
                if(this->rightBend ){this->body[0].x -= radius;this->rightBend -= 1;if(this->rightBend == 0)this->leftBend = bendtarget;break;}
            case RIGHT :
                this->body[0].x += radius + spdext;if(!bendtarget)break;
                if(this->leftBend ){this->body[0].y += radius;this->leftBend -= 1;if(this->leftBend == 0)this->rightBend = bendtarget;break;}
                if(this->rightBend ){this->body[0].y -= radius;this->rightBend -= 1;if(this->rightBend == 0)this->leftBend = bendtarget;break;}
            case LEFT :
                this->body[0].x -= radius + spdext;if(!bendtarget)break;
                if(this->leftBend ){this->body[0].y += radius;this->leftBend -= 1;if(this->leftBend == 0)this->rightBend = bendtarget;break;}
                if(this->rightBend ){this->body[0].y -= radius;this->rightBend -= 1;if(this->rightBend == 0)this->leftBend = bendtarget;break;}
        }

        for(register int i=length - 1; i > 0; --i){
            this->body[i].x = this->body[i-1].x;
            this->body[i].y = this->body[i-1].y;
        }

        if(this->body[0].x > 600)if(box){gameover = 1, paused = 1;}else this->body[0].x = 40;
        if(this->body[0].x < 40)if(box){gameover = 1, paused = 1;}else this->body[0].x = 600;
        if(this->body[0].y > 400)if(box){gameover = 1, paused = 1;}else this->body[0].y = 80;
        if(this->body[0].y < 80)if(box){gameover = 1, paused = 1;}else this->body[0].y = 400;
    }

    void display(){
        setcolor(BLUE);
        if(paused) setcolor(BLUE),rectangle (30, 70, 610, 410);
        if(!paused) {
            if(box) setfillstyle(SOLID_FILL,BROWN),bar (30, 70, 610, 410);
            setfillstyle(SOLID_FILL,GREEN),bar (35, 75, 605, 405);
        }

        for(register int i = 1; i < length; ++i){

            setcolor(RED);
            if(bendtarget)setfillstyle(SOLID_FILL, MAGENTA);else setfillstyle(SOLID_FILL, BROWN);
            fillellipse(this->body[i].x, this->body[i].y, radius, radius);
            //cannibalism check
            if(i > 3)
            if( this->body[0].y > this->body[i].y - 2 * radius &&
                this->body[0].y < this->body[i].y + 2 * radius &&
                this->body[0].x > this->body[i].x - 2 * radius &&
                this->body[0].x < this->body[i].x + 2 * radius   ){
                gameover = 1;
                paused = 1;
            }
            setcolor(CYAN);
            setfillstyle(SOLID_FILL, RED);
            fillellipse(this->body[0].x, this->body[0].y, radius, radius);
        }
    }

    void control(int key){
        if(key != this->diretion)
        switch (key){
            case UP : {this->diretion = key; break;}
            case LEFT : {this->diretion = key; break;}
            case DOWN : {this->diretion = key; break;}
            case RIGHT : {this->diretion = key; break;}
        }
    }

    void reset(){
        if(highscore < score)highscore = score;
        score = 0;
        this->length = 3 * 2;
        this->leftBend = 1, this->rightBend = 0;
        this->diretion = RIGHT;

        this->body[0].x = getmaxx() / 2;
        this->body[0].y = getmaxy() / 2;

        for(int i = 1; i < 100; ++i) this->body[i].x = 0, this->body[i].y = 0;
    }
};

class Food{

public:
    point food;

    void spawnfood(){
        srand(time(0)%this->food.x);
        this->food.x = rand()%580 + 60;if((this->food.x > 580))this->food.x -= 60;if((this->food.x < 60))this->food.x += 60;
        srand(time(0)%this->food.x);
        this->food.y = rand()%390 + 90;if((this->food.y > 390))this->food.y -= 90;if((this->food.y < 90))this->food.y += 90;

        this->food.x -= this->food.x%(2 * radius);
        this->food.y -= this->food.y%(2 * radius);
    }

    void drawfood(){
        setcolor(WHITE);
        setfillstyle(SOLID_FILL, YELLOW);
        fillellipse(this->food.x, this->food.y, radius / 2, radius / 2);
    }

    void eat(Snake& snake){
        if( snake.body[0].y >= this->food.y - radius &&
            snake.body[0].y <= this->food.y + radius &&
            snake.body[0].x >= this->food.x - radius &&
            snake.body[0].x <= this->food.x + radius   ){
            this->spawnfood();
            if(bendtarget)snake.length += 2;else {snake.length++;}
            score++;
        }
    }
};

int main()

{

   int gdriver = DETECT, gmode;

   initgraph(&gdriver, &gmode, "");

   point boxselector;
   boxselector.x = 220, boxselector.y = 260;
   Snake snake;
   Food food;
   food.spawnfood();

   while(1){

    if(startmenu){
        static int xradius = 0;

        settextstyle(GOTHIC_FONT, HORIZ_DIR, 3);
        setcolor(GREEN);
        outtextxy(220, 260, "PLAY");
        settextstyle(GOTHIC_FONT, HORIZ_DIR, 3);
        setcolor(BLUE);
        outtextxy(130, 300, "SELECT SNAKE");
        settextstyle(GOTHIC_FONT, HORIZ_DIR, 3);
        setcolor(RED);
        outtextxy(220, 340, "QUIT");

        switch(boxselector.y){
            case 260 : xradius = 14 * 4;break;
            case 300 : xradius = 14 * 12;break;
            case 340 : xradius = 14 * 4;break;
        }

        startmenu++;
        setcolor(YELLOW);
        ellipse(boxselector.x + 45, boxselector.y + 10, 0 + 15 * startmenu, 45 + 15 * startmenu, xradius, 20);
        startmenu == 24 ? startmenu = 1 : 0;

        if(kbhit()){
            inputkey = getch();
            switch(inputkey){
                case ENTER:
                    if(boxselector.y == 260){game = 1, paused = 0, startmenu = 0, gameover = 0, snake.reset(); break;}
                    else if(boxselector.y == 300){selectsnake = 1, startmenu = 0, boxselector.y = 220; break;}
                    else if(boxselector.y == 340){exit(0); break;}
                case UP:if(boxselector.y - 40 >= 260) boxselector.y -= 40;break;
                case DOWN:if(boxselector.y + 40 <= 340) boxselector.y += 40;break;
            }
        }
    }

    if(pausemenu){
        static int xradius = 0,boxrotation = 1;

        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 4);
        setcolor(RED);
        outtextxy(200, 150, "PAUSED :|");
        settextstyle(GOTHIC_FONT, HORIZ_DIR, 3);
        setcolor(RED);
        outtextxy(190, 260, "RESUME");
        settextstyle(GOTHIC_FONT, HORIZ_DIR, 3);
        setcolor(YELLOW);
        outtextxy(160, 300, "MAIN MENU");

        switch(boxselector.y){
            case 260 : xradius = 14 * 6;break;
            case 300 : xradius = 14 * 9;break;
        }

        boxrotation++;
        setcolor(GREEN);
        ellipse(boxselector.x + 45, boxselector.y + 10, 0 + 15 * boxrotation, 45 + 15 * boxrotation, xradius, 20);
        boxrotation == 24 ? boxrotation = 1 : 0;

        if(kbhit()){
            inputkey = getch();
            switch(inputkey){
                case ENTER:
                    if(boxselector.y == 260){game = 1, gameover = 0, paused = 0, pausemenu = 0;break;}
                    else if(boxselector.y == 300){gameover = 0, startmenu = 1, paused = 0, game = 0, pausemenu = 0, boxselector.y = 260;break;}
                case UP:if(boxselector.y - 40 >= 260) boxselector.y -= 40;break;
                case DOWN:if(boxselector.y + 40 <= 300) boxselector.y += 40;break;
            }
        }
    }

    if(selectsnake){
        static int xradius = 0;

        settextstyle(GOTHIC_FONT, HORIZ_DIR, 3);
        setcolor(RED);
        outtextxy(200, 220, "CUSTOM");
        settextstyle(GOTHIC_FONT, HORIZ_DIR, 3);
        setcolor(YELLOW);
        outtextxy(230, 260, "SLUG");
        settextstyle(GOTHIC_FONT, HORIZ_DIR, 3);
        setcolor(GREEN);
        outtextxy(221, 300, "WORM");
        settextstyle(GOTHIC_FONT, HORIZ_DIR, 3);
        setcolor(LIGHTRED);
        outtextxy(181, 340, "ANACONDA");
        settextstyle(GOTHIC_FONT, HORIZ_DIR, 3);
        setcolor(YELLOW);
        outtextxy(179, 380, "MAIN MENU");

        switch(boxselector.y){
            case 220 : xradius = 14 * 7;break;
            case 260 : xradius = 14 * 4;break;
            case 300 : xradius = 14 * 5;break;
            case 340 : xradius = 14 * 9;break;
            case 380 : xradius = 14 * 9;break;
        }

        selectsnake++;
        setcolor(BLUE);
        ellipse(boxselector.x + 60, boxselector.y + 10, 0 + 15 * selectsnake, 45 + 15 * selectsnake, xradius, 20);
        selectsnake == 24 ? selectsnake = 1 : 0;

        if(kbhit()){
            inputkey = getch();
            switch(inputkey){
                case ENTER:
                    if(boxselector.y == 220){selectsnake = 0, custommenu = 1;break;}
                    else if(boxselector.y == 260){radius = 8, bendtarget = 0, box = 0, selectsnake = 0, startmenu = 1, spdext = 1;break;}
                    else if(boxselector.y == 300){radius = 8, bendtarget = 0, box = 0, selectsnake = 0, startmenu = 1, spdext = 8;break;}
                    else if(boxselector.y == 340){radius = 8, bendtarget = 2, box = 1, selectsnake = 0, startmenu = 1, spdext = 8;break;}
                    else if(boxselector.y == 380){startmenu = 1, selectsnake = 0, boxselector.y = 260;break;}
                case UP:if(boxselector.y - 40 >= 220) boxselector.y -= 40;break;
                case DOWN:if(boxselector.y + 40 <= 380) boxselector.y += 40;break;
            }
        }
    }

    if(gameover){
        static int xradius = 0,boxrotation = 1;

        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 4);
        setcolor(RED);
        outtextxy(200, 150, "GAME OVER :(");
        settextstyle(GOTHIC_FONT, HORIZ_DIR, 3);
        setcolor(BLUE);
        outtextxy(153, 260, "PLAY AGAIN");
        settextstyle(GOTHIC_FONT, HORIZ_DIR, 3);
        setcolor(YELLOW);
        outtextxy(160, 300, "MAIN MENU");

        switch(boxselector.y){
            case 260 : xradius = 14 * 10;break;
            case 300 : xradius = 14 * 9;break;
        }

        boxrotation++;
        setcolor(GREEN);
        ellipse(boxselector.x + 45, boxselector.y + 10, 0 + 15 * boxrotation, 45 + 15 * boxrotation, xradius, 20);
        boxrotation == 24 ? boxrotation = 1 : 0;

        if(kbhit()){
            inputkey = getch();
            switch(inputkey){
                case ENTER:
                    if(boxselector.y == 260){game = 1, gameover = 0, paused = 0, snake.reset();break;}
                    else if(boxselector.y == 300){gameover = 0, startmenu = 1, paused = 0, game = 0, boxselector.y = 260;break;}
                case UP:if(boxselector.y - 40 >= 260) boxselector.y -= 40;break;
                case DOWN:if(boxselector.y + 40 <= 300) boxselector.y += 40;break;
            }
        }
    }

    if(custommenu){
        static int xradius = 0,boxrotation = 1;

        char radiustext[11],speedtext[12],bendtext[11];
        settextstyle(GOTHIC_FONT, HORIZ_DIR, 4);
        setcolor(GREEN);
        outtextxy(150, 100, "CUSTOM SNAKE");
        setcolor(WHITE);
        if(box)outtextxy(190, 220, "Box  <ON>");
        else   outtextxy(190, 220, "Box <OFF>");
        sprintf(radiustext, "Radius <-%d->", radius);
        outtextxy(179, 260, radiustext);
        sprintf(speedtext, "Speed <-%d->", spdext);
        outtextxy(186, 300, speedtext);
        sprintf(bendtext, "Bend <-%d->", bendtarget);
        outtextxy(193, 340, bendtext);
        setcolor(YELLOW);
        outtextxy(178, 380, "MAIN MENU");

        switch(boxselector.y){
            case 220 : xradius = 14 * 10;break;
            case 260 : xradius = 14 * 11;break;
            case 300 : xradius = 14 * 10;break;
            case 340 : xradius = 14 * 9;break;
            case 380 : xradius = 14 * 11;break;
        }

        boxrotation++;
        setcolor(RED);
        ellipse(boxselector.x + 90, boxselector.y + 15, 0 + 15 * boxrotation, 45 + 15 * boxrotation, xradius, 20);
        boxrotation == 24 ? boxrotation = 1 : 0;

        if(kbhit()){
            inputkey = getch();
            switch(inputkey){
                case LEFT:
                    switch(boxselector.y){
                        case 220 : box = 0;break;
                        case 260 : if(radius - 1 >= 0)radius--;break;
                        case 300 : if(spdext - 1 >= 0)spdext--;break;
                        case 340 : bendtarget--;break;
                    }
                    break;
                case RIGHT:
                    switch(boxselector.y){
                        case 220 : box = 1;break;
                        case 260 : radius++;break;
                        case 300 : spdext++;break;
                        case 340 : bendtarget++;break;
                    }
                    break;
                case UP:if(boxselector.y - 40 >= 220) boxselector.y -= 40;break;
                case DOWN:if(boxselector.y + 40 <= 380) boxselector.y += 40;break;
                case ENTER:if(boxselector.y == 380){custommenu = 0, startmenu = 1, boxselector.y = 260;break;}
            }
        }
    }

    if(game){

        if(kbhit() && !paused){
            inputkey = getch();
            snake.control(inputkey);
            if(inputkey == ESC)paused = 1,pausemenu = 1,game = 1,startmenu = 0,gameover = 0;
        }

        //Update
        if(!paused)snake.Move();

        //eat
        food.eat(snake);

        //draw
        snake.display();
        food.drawfood();


        settextstyle(TRIPLEX_SCR_FONT, HORIZ_DIR, 5);
        char scoretext[10], highscoretext[12];
        sprintf(scoretext, "SCORE %d",score);
        sprintf(highscoretext, "HIGHSCORE %d",highscore);
        outtextxy(10, 10, scoretext);
        settextstyle(TRIPLEX_SCR_FONT, HORIZ_DIR, 2);
        outtextxy(420, 430, highscoretext);
        if(!paused){
            settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
            outtextxy(10, 430, "Pres ESC to PAUSE");
        }
    }

    //fps fix
    if(bendtarget)delay(80);else delay(50);
    cleardevice();
    }

    return 0;
}
