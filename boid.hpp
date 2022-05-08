#include <vector>
#include "SDL2/SDL_vector2D.hpp"
#include "SDL2/SDL_intvector2D.hpp"

#ifndef _BOID_
#define _BOID_

class Boid{
    private:
        Vector2D mPosition;
        IntVector2D mSize;
        Vector2D mVelocity;
        float mSeperationStrenght;
        float mAlignmentStrenght;
        float mCohesionStrenght;
        float mAvoidanceStrenght;
        float mMaxSpeed = 3;
        float mVisionRadius = 50;
        SDL_Rect mBody;
    public:
        Boid(Vector2D position, IntVector2D size, float seperationStrenght, float alignmentStrenght, float cohesionStrenght, float avoidanceStrenght);
        Vector2D Position() {return mPosition;}
        Vector2D Velocity() {return mVelocity;}
        void move(std::vector<Boid> &boids, Vector2D mousePosition);
        void seperation(int size, Vector2D sum);
        void alignment(int size, Vector2D sum);
        void cohesion(int size, Vector2D sum);
        void checkWallIntersect();
        void draw(SDL_Renderer* renderer);
};

Boid::Boid(Vector2D position, IntVector2D size, float seperationStrenght, float alignmentStrenght, float cohesionStrenght, float avoidanceStrenght){
    mPosition = position;
    mSize = size;
    mVelocity = {(float)rand()/(float)RAND_MAX * 10 - 5, (float)rand()/(float)RAND_MAX * 10 - 5};

    mSeperationStrenght = seperationStrenght;
    mAlignmentStrenght = alignmentStrenght;
    mCohesionStrenght = cohesionStrenght;
    mAvoidanceStrenght = avoidanceStrenght;

    mBody.w = (int) mSize.X;
    mBody.h = (int) mSize.Y;
}

void Boid::move(std::vector<Boid> &boids, Vector2D mousePosition){
    Vector2D sumSeperation(0,0);
    Vector2D sumAlignment(0,0);
    Vector2D sumCohesion(0,0);
    int size = 0;

    for (int i = 0; i < boids.size(); i++){
        float distance = abs((boids[i].Position() - mPosition).length());
        if(distance != 0 && distance < mVisionRadius){
            sumSeperation += (boids[i].Position() - mPosition) / distance;
            sumAlignment += boids[i].Velocity();
            sumCohesion += boids[i].Position();
            size++;
        }
    }

    seperation(size, sumSeperation);
    alignment(size, sumAlignment);
    cohesion(size, sumCohesion);

    Vector2D dirToMouse = mousePosition - mPosition;
    if(dirToMouse.length() < mVisionRadius * 2) mVelocity -= dirToMouse * mAvoidanceStrenght;

    Vector2D random((float)rand()/(float)RAND_MAX * 0.4 - 0.2, (float)rand()/(float)RAND_MAX * 0.4 - .2);
    mVelocity += random;

    checkWallIntersect();

    if(mVelocity.X > mMaxSpeed) mVelocity.X = mMaxSpeed;
    if(mVelocity.X < -mMaxSpeed) mVelocity.X = -mMaxSpeed;
    if(mVelocity.Y > mMaxSpeed) mVelocity.Y = mMaxSpeed;
    if(mVelocity.Y < -mMaxSpeed) mVelocity.Y = -mMaxSpeed;

    mPosition += mVelocity;

    mBody.x = mPosition.X;
    mBody.y = mPosition.Y;
}

void Boid::seperation(int size, Vector2D sum){
    if(size == 0) return;

    Vector2D dir = sum / size;
    mVelocity -= dir * mSeperationStrenght;
}

void Boid::alignment(int size, Vector2D sum){
    if(size == 0) return;

    Vector2D dir = sum / size;
    mVelocity += dir * mAlignmentStrenght;
}

void Boid::cohesion(int size, Vector2D sum){
    if(size == 0 || sum.length() == 0) return;

    Vector2D center = sum / size;
    Vector2D dirToCenter = center - mPosition;
    mVelocity += dirToCenter * mCohesionStrenght;
}

void Boid::checkWallIntersect(){
    if(mPosition.X > constants::WIDTH){
        mPosition.X = 0;
    }
    if(mPosition.X < -mSize.X){
        mPosition.X = constants::WIDTH;
    }
    if(mPosition.Y > constants::HEIGHT){
        mPosition.Y = 0;
    }
    if(mPosition.Y < -mSize.Y){
        mPosition.Y = constants::HEIGHT;
    }
}

void Boid::draw(SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
    SDL_RenderDrawLine(renderer, mPosition.X + mSize.X/2, mPosition.Y + mSize.Y/2, mPosition.X + mSize.X/2 + mVelocity.normalized().X * mSize.X * 2, mPosition.Y + mSize.Y/2 + mVelocity.normalized().Y * mSize.Y * 2);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    SDL_RenderFillRect(renderer, &mBody);
}

#endif