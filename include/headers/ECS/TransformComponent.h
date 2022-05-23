#pragma once
#include "headers/Vector2D.h"
class TransformComponent : public Component
{

private:

    Vector2D position;

    int height;
    int width;
public:
    ~TransformComponent() = default;

    TransformComponent(Vector2D pos)
    {
        position = pos;
        width = 64;
        height = 64;
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

    Vector2D getPos() const
    {
        return position;
    }

    int x() const {return position.x;}
    int y() const {return position.y;}
 
    void x(int x) {position.x = x;}
    void y(int y) {position.y = y;}

    int h() const {return height;}
    int w() const {return width;}
 
    void h(int h) {height = h;}
    void w(int w) {width = w;}
};