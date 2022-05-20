#pragma once
#include "headers/ECS/Components.h"
#include "headers/Vector2D.h"
class TransformComponent : public Component
{

public:

    Vector2D position;

    int height;
    int width;
    float scale = 2.0f; // I always use this scale to have a consistent pixelish resolution

    ~TransformComponent() = default;
    
    TransformComponent()
    {
        position.x = 0;
        position.y = 0;
        width = 32;
        height = 32;
    }

    TransformComponent(int x, int y) //need to wrap constructors
    {
        position.x = x;
        position.y = y;
        width = 32;
        height = 32;
    }

    TransformComponent(Vector2D pos)
    {
        position = pos;
        width = 32;
        height = 32;
    }
    TransformComponent(Vector2D pos, int h, int w)
    {
        position = pos;
        height = h;
        width = w;
    }

    TransformComponent(float x, float y, int h, int w)
    {
        position.x = x;
        position.y = y;
        height = h;
        width = w;
    }

    void setPos(Vector2D pos)
    {
        position = pos;
    }

    int x() {return position.x;}
    int y() {return position.y;}
 
    void x(int x) {position.x = x;}
    void y(int y) {position.y = y;}
};