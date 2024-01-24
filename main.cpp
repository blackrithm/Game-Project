
#include <bits/stdc++.h>
#include "inc/SDL.h"
#include "inc/SDL_ttf.h"
#include "inc/SDL_mixer.h"
#undef main

using namespace std;

const int SCREEN_WIDTH = 1020;
const int SCREEN_HEIGHT = 615;
const int GRID_SIZE = 15;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
Mix_Chunk* wavFile;
Mix_Chunk* gameOver;

struct Snake {
    vector<pair<int, int>> body;
    char direction;
};

struct Food {
    int x, y;
};

Snake snake;
Food food;
Food sp;

int ptime = 0;

bool quit ;
bool pause ;
bool f=true;
int score;
int regfoodcount = 0;

void drawCirc (int X, int Y, int radius)
{
    while (radius--) {
            for (int angle = 0; angle < 360; angle++) {
                int x = X + radius * cos(angle * M_PI / 180);
                int y = Y + radius * sin(angle * M_PI / 180);
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
}

void generateFood() {
    food.x = rand() % (SCREEN_WIDTH / GRID_SIZE) * GRID_SIZE;
    food.y = rand() % (SCREEN_HEIGHT / GRID_SIZE) * GRID_SIZE;
    if ((food.x >= 60 && food.x < 60+3*GRID_SIZE &&
        food.y >= 60 && food.y < 60+8*GRID_SIZE) ||
        (food.x >= 60 && food.x < 60+8*GRID_SIZE &&
        food.y >= 60 && food.y < 60+3*GRID_SIZE) ||
        (food.x >= (SCREEN_WIDTH-60-(3*GRID_SIZE)) && food.x < (SCREEN_WIDTH-60-(3*GRID_SIZE))+3*GRID_SIZE &&
        food.y >= (SCREEN_HEIGHT-60-(8*GRID_SIZE)) && food.y < (SCREEN_HEIGHT-60-(8*GRID_SIZE))+8*GRID_SIZE) ||
        (food.x >= (SCREEN_WIDTH-60-(8*GRID_SIZE)) && food.x < (SCREEN_WIDTH-60) &&
        food.y >= (SCREEN_HEIGHT-60-(3*GRID_SIZE)) && food.y < (SCREEN_HEIGHT-60)) ||
        (food.x >= ((SCREEN_WIDTH/GRID_SIZE)/2*GRID_SIZE) && food.x < (((SCREEN_WIDTH/GRID_SIZE)/2*GRID_SIZE)+3*GRID_SIZE) &&
        food.y >= ((SCREEN_HEIGHT/GRID_SIZE)/2*GRID_SIZE-6*GRID_SIZE) && food.y < ((SCREEN_HEIGHT/GRID_SIZE)/2*GRID_SIZE+6*GRID_SIZE)) ||
        (food.x >= (((SCREEN_WIDTH/GRID_SIZE)/2*GRID_SIZE)-4*GRID_SIZE) && food.x < ((SCREEN_WIDTH/GRID_SIZE)/2*GRID_SIZE)-4*GRID_SIZE+11*GRID_SIZE) &&
        food.y >= ((SCREEN_HEIGHT/GRID_SIZE)/2*GRID_SIZE-6*GRID_SIZE) && food.y < (((SCREEN_HEIGHT/GRID_SIZE)/2*GRID_SIZE-6*GRID_SIZE)+3*GRID_SIZE)) 
        {
            generateFood();
        }
}

void generateSpFood() {
    sp.x = rand() % (SCREEN_WIDTH / GRID_SIZE) * GRID_SIZE;
    sp.y = rand() % (SCREEN_HEIGHT / GRID_SIZE) * GRID_SIZE;
    if ((sp.x >= 60 && sp.x < 60+3*GRID_SIZE &&
        sp.y >= 60 && sp.y < 60+8*GRID_SIZE) ||
        (sp.x >= 60 && sp.x < 60+8*GRID_SIZE &&
        sp.y >= 60 && sp.y < 60+3*GRID_SIZE) ||
        (sp.x >= (SCREEN_WIDTH-60-(3*GRID_SIZE)) && sp.x < (SCREEN_WIDTH-60-(3*GRID_SIZE))+3*GRID_SIZE &&
        sp.y >= (SCREEN_HEIGHT-60-(8*GRID_SIZE)) && sp.y < (SCREEN_HEIGHT-60-(8*GRID_SIZE))+8*GRID_SIZE) ||
        (sp.x >= (SCREEN_WIDTH-60-(8*GRID_SIZE)) && sp.x < (SCREEN_WIDTH-60) &&
        sp.y >= (SCREEN_HEIGHT-60-(3*GRID_SIZE)) && sp.y < (SCREEN_HEIGHT-60)) ||
        (sp.x >= ((SCREEN_WIDTH/GRID_SIZE)/2*GRID_SIZE) && sp.x < (((SCREEN_WIDTH/GRID_SIZE)/2*GRID_SIZE)+3*GRID_SIZE) &&
        sp.y >= ((SCREEN_HEIGHT/GRID_SIZE)/2*GRID_SIZE-6*GRID_SIZE) && sp.y < ((SCREEN_HEIGHT/GRID_SIZE)/2*GRID_SIZE+6*GRID_SIZE)) ||
        (sp.x >= (((SCREEN_WIDTH/GRID_SIZE)/2*GRID_SIZE)-4*GRID_SIZE) && sp.x < ((SCREEN_WIDTH/GRID_SIZE)/2*GRID_SIZE)-4*GRID_SIZE+11*GRID_SIZE) &&
        sp.y >= ((SCREEN_HEIGHT/GRID_SIZE)/2*GRID_SIZE-6*GRID_SIZE) && sp.y < (((SCREEN_HEIGHT/GRID_SIZE)/2*GRID_SIZE-6*GRID_SIZE)+3*GRID_SIZE)) 
        {
            generateSpFood();
        }
        ptime = SDL_GetTicks();
        regfoodcount = 0;
}

void initialize() {

    quit = false;
    pause = false;
    score = 0;
    regfoodcount = 0;
    window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    //SDL_SetWindowBordered(window, SDL_TRUE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Init();

    snake.body.push_back({0, 0});
    snake.body.push_back({1, 0});
    snake.body.push_back({2, 0});
    snake.body.push_back({3, 0});
    snake.direction = 'R';

    sp.x=-100;
    sp.y=-100;
    generateFood();

}

void close() {
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void processInput (){
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                exit(1);
            } 
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        if (snake.direction != 'D') 
                        {
                            snake.direction = 'U';
                            if (pause) pause= !pause;
                        }
                        break;
                    case SDLK_DOWN:
                        if (snake.direction != 'U') 
                        {
                            snake.direction = 'D';
                            if (pause) pause= !pause;
                        }
                        break;
                    case SDLK_LEFT:
                        if (snake.direction != 'R') 
                        {
                            snake.direction = 'L';
                            if (pause) pause= !pause;
                        }
                        break;
                    case SDLK_RIGHT:
                        if (snake.direction != 'L') 
                        {
                            snake.direction = 'R';
                            if (pause) pause= !pause;
                        }
                        break;
                    case SDLK_SPACE:
                        pause = !pause;
                        break;
                }
            }
        }
}

void update() {

    int ctime = SDL_GetTicks();
    if ((ctime-ptime >= 6000))
    {
        f=false;
        sp.x = -100;
        sp.y = -100;
    }
    pair<int, int> newHead = snake.body.front();
    switch (snake.direction) {
        case 'U':
            if (snake.body.front().second > 0)
                newHead.second -= GRID_SIZE;
            else
                newHead.second = SCREEN_HEIGHT-GRID_SIZE;
            break;
        case 'D':
            if (snake.body.front().second >= SCREEN_HEIGHT-GRID_SIZE)
                newHead.second = 0;
            else
                newHead.second += GRID_SIZE;
            break;
        case 'L':
            if (snake.body.front().first > 0)
                newHead.first -= GRID_SIZE;
            else
                newHead.first = SCREEN_WIDTH-GRID_SIZE;
            break;
        case 'R':
            if (snake.body.front().first >= SCREEN_WIDTH-GRID_SIZE)
                newHead.first = 0;
            else
                newHead.first += GRID_SIZE;
            break;
    }

    snake.body.insert(snake.body.begin(), newHead);
    

    if (regfoodcount == 7) 
    { 
        generateSpFood();
    }

    if (newHead.first == food.x && newHead.second == food.y) {
        score++;
        regfoodcount++;
        Mix_PlayChannel(-1, wavFile, 0);
        generateFood();
    } 
    else if((newHead.first >= sp.x - GRID_SIZE && newHead.first < sp.x + GRID_SIZE) && 
    (newHead.second >= sp.y - GRID_SIZE && newHead.second < sp.y + GRID_SIZE))
    {
        score+=10;
        sp.x= -100;
        sp.y= -100;
        Mix_PlayChannel(-1, wavFile, 0);
    }

    else {
        snake.body.pop_back();
    }
}

void renderScore() {
	SDL_Color Red = { 155, 100, 90 };

	TTF_Font* font = TTF_OpenFont((char*)"Aller_Rg.ttf", 10);
	if (font == NULL) {
		cout << "Font loading error" << endl;
		return;
	}

	SDL_Surface* Score = TTF_RenderText_Solid(font, (string("Score: ") + to_string(score)).c_str(), Red);
	SDL_Texture* scoreMessage = SDL_CreateTextureFromSurface(renderer, Score);
	SDL_Rect scoreRect;
	scoreRect.w = 160;
	scoreRect.h = 30;
	scoreRect.x = ((SCREEN_WIDTH) / 2) - (scoreRect.w / 2);
	scoreRect.y = 0;
	SDL_RenderCopy(renderer, scoreMessage, NULL, &scoreRect);

	TTF_CloseFont(font);
}


void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    renderScore();

    SDL_SetRenderDrawColor(renderer, 0, 25, 74, 255);
    
    SDL_Rect prince = {300, 75, 450, 30};
    SDL_RenderFillRect(renderer, &prince);

    SDL_Rect prince1 = {300, 75, 30, 100};
    SDL_RenderFillRect(renderer, &prince1);

    SDL_Rect prince2 = {720, 75, 30, 100};
    SDL_RenderFillRect(renderer, &prince2);

    SDL_Rect prince3 = {300, 500, 450, 30};
    SDL_RenderFillRect(renderer, &prince3);
    SDL_Rect prince4 = {300, 420, 30, 100};
    SDL_RenderFillRect(renderer, &prince4);
    SDL_Rect prince5 = {720, 420, 30, 100};
    SDL_RenderFillRect(renderer, &prince5);
    
    // Render Snake
    SDL_SetRenderDrawColor(renderer, 160, 155, 158, 0);
    const auto& head = *snake.body.begin();
    SDL_Rect rect = {head.first, head.second, GRID_SIZE, GRID_SIZE};
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 242, 243, 245, 0);
    for (auto it = next(snake.body.begin()); it != snake.body.end(); ++it) {
        const auto& segment = *it;
        SDL_Rect rect = {segment.first, segment.second, GRID_SIZE, GRID_SIZE};
        SDL_RenderFillRect(renderer, &rect);
    }

    // Render Food
    SDL_SetRenderDrawColor(renderer, 255, 174, 66, 255);
    SDL_Rect foodRect = {food.x, food.y, GRID_SIZE, GRID_SIZE};
    SDL_RenderFillRect(renderer, &foodRect);

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    drawCirc (sp.x, sp.y, GRID_SIZE);
    SDL_RenderPresent(renderer);
}



bool checkCollision() {
   
    // Check collision with itself
    for (size_t i = 1; i < snake.body.size(); ++i) {
        if (snake.body.front().first == snake.body[i].first && snake.body.front().second == snake.body[i].second) {
            cout << "Game Over! Collision with itself." << endl;
            return true;
        }
    }

    return false;
}


void renderGameOver() {

    SDL_SetRenderDrawColor(renderer, 90, 100, 70, 255);
    SDL_RenderClear(renderer);

	SDL_Color C = { 255, 255, 255 };

	TTF_Font* font = TTF_OpenFont((char*)"Aller_Rg.ttf", 26);
	if (font == NULL) {
		cout << "Font loading error" << endl;
		return;
	}

    string msg = "GAME OVER!!!";
	SDL_Surface* GO = TTF_RenderText_Solid(font, msg.c_str(), C);
	SDL_Texture* GOMessage = SDL_CreateTextureFromSurface(renderer, GO);
	SDL_Rect GORect;
	GORect.w = 480;
	GORect.h = 180;
	GORect.x = ((SCREEN_WIDTH) / 2) - (GORect.w / 2);
	GORect.y = ((SCREEN_HEIGHT) / 2) - (GORect.h / 2);
	SDL_RenderCopy(renderer, GOMessage, NULL, &GORect);
    SDL_RenderPresent(renderer);
    SDL_Delay(1000);

	TTF_CloseFont(font);
}


int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
    wavFile = Mix_LoadWAV("eat.wav");
    gameOver = Mix_LoadWAV("gameOver.wav");
    Mix_Music* BGM = Mix_LoadMUS("background_music.mp3");
    Mix_PlayMusic(BGM, -1);
    Mix_VolumeMusic(20);
    
    initialize();
    while (!quit) {
        processInput ();
        if (!pause){
            Mix_ResumeMusic();
            update();
        } else
            Mix_PauseMusic();
        if(checkCollision())
        {
            Mix_PlayChannel(-1, gameOver, 0);
            break;
        }

        render();
        SDL_Delay((100-(0.5*score)));
    }

    renderGameOver();
    SDL_Delay(1000);
    snake.body.clear();
    close();
    return 0;
}

