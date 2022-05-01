#pragma once
#include "headers/ECS/Components.h"
#include "headers/Vector2D.h"
class TransformComponent : public Component
{

public:

    Vector2D position;

    int height = 32;
    int width = 32;
    float scale = 1.0f;

    TransformComponent()
    {
        position.x = 0.0f;
        position.y = 0.0f;
    }

    TransformComponent(float x, float y)
    {
        position.x = x;
        position.y = y;
    }

    TransformComponent(float x, float y, int h, int w, float sc)
    {
        position.x = x;
        position.y = y;
        height = h;
        width = w;
        scale = sc;
    }

    int x() {return position.x;}
    int y() {return position.y;}

    void update() override
    {

    }

    void setPose(float x, float y)
    {
        position.x = x;
        position.y = y;
    }
 
    void x(float x) {position.x = x;}
    void y(float y) {position.y = y;}
};