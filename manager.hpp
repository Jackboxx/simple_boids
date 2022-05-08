#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_vector2D.hpp"
#include "constants.hpp"
#include "boid.hpp"

#ifndef _MANAGER_
#define _MANAGER_

class Manager
{
private:
    SDL_Renderer* mRenderer;
    SDL_Window* mWindow;
    int init();
    void loop();
public:
    Manager();
};

Manager::Manager(){
    if(init() == 0) loop();
}

int Manager::init(){
    srand(time(0));
    if(SDL_Init(SDL_INIT_VIDEO) != 0) return 1;
    
    mWindow = SDL_CreateWindow("Boids", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, constants::WIDTH, constants::HEIGHT, 0);
    if(!mWindow) {
        std::cout << "Window failled to initialize" << std::endl;
        SDL_Quit();
        return 1;
    }
    
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    mRenderer = SDL_CreateRenderer(mWindow, -1, render_flags);
    if(!mRenderer) {
        std::cout << "Renderer failled to initialize" << std::endl;
        SDL_DestroyWindow(mWindow);
        SDL_Quit();
        return 1;
    }   

    return 0;
}

void Manager::loop(){
    bool running = true;
    SDL_Event event;

    std::vector<Boid> boids;

    for (int i = 0; i < constants::BOID_COUNT; i++){
        boids.push_back(Boid(Vector2D(rand() % constants::WIDTH, rand() % constants::HEIGHT), IntVector2D(8, 8), constants::SEPERATION_STRENGHT, constants::ALIGNMENT_STRENGHT, constants::COHESION_STRENGHT, constants::AVOIDANCE_STRENGHT));
    }
    

    while (running){
        while (SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                running = false;
            }
        }

        SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 0);
        SDL_RenderClear(mRenderer);

        int x;
        int y;

        SDL_GetMouseState(&x, &y);
        Vector2D mousePosition(x, y);

        for (int i = 0; i < boids.size(); i++){
            boids[i].move(boids, mousePosition);
            boids[i].draw(mRenderer);
        }
        
    
        SDL_RenderPresent(mRenderer);
        SDL_Delay(1000/constants::FPS);
    }
}

#endif