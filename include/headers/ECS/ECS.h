#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>
#include <random>
#include <chrono>


class Component;
class Entity;
class Manager;

using ComponentID = std::size_t; 
using Group = std::size_t;

inline ComponentID getComponentTypeID() 
{
    static ComponentID lastID = 0;
    return lastID++; 
}

template <typename T> inline ComponentID getComponentTypeID() noexcept
{
    static ComponentID typeID = getComponentTypeID();
    return typeID;
}

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using GroupBitset = std::bitset<maxGroups>;
using ComponentArray = std::array<Component*, maxComponents>;

class Component
{
    public:
    Entity* entity;

    virtual void init() {}
    virtual void update() {}
    virtual void draw() {}
    virtual void handleEvents() {}

    virtual ~Component() {}
};

class Entity
{
    private:
    Manager& manager;
    bool active = true;
    std::vector<std::unique_ptr<Component>> components;

    ComponentArray componentArray;
    ComponentBitSet componentBitSet;
    GroupBitset groupBitset;
    
    public:
    Entity(Manager& mng) : manager(mng){} //rewrite everything in this style


    void update()
    {
        for (auto& c : components) c->update();
    }
    void handleEvents()
    {
        for (auto& c : components) c->handleEvents();
    }
    void draw()
    {
        for (auto& c : components) c->draw();
    }
    bool isActive() const {return active;}
    void destroy() {active = false;}

    bool hasGroup(Group grp)
    {
        return groupBitset[grp];
    }

    //void addGroup(Group grp);
    const std::vector<Entity*>& getGroup(Group grp);
    void addGroup(Group grp);
    size_t getGroupSize(Group grp) const;

    void delGroup(Group grp)
    {
        groupBitset[grp] = false;
    }

    template <typename T> bool hasComponent() const
    {
        return componentBitSet[getComponentTypeID<T>];
    }

    template <typename T, typename... TArgs>
    T& addComponent(TArgs&&... mArgs)
    {
        T* c(new T(std::forward<TArgs>(mArgs)...)); //???
        c->entity = this;
        std::unique_ptr<Component> uPtr{ c };
        components.emplace_back(std::move(uPtr));

        componentArray[getComponentTypeID<T>()] = c;
        componentBitSet[getComponentTypeID<T>()] = true;

        c->init();
        return *c;
    }

    
    

    template<typename T> T& getComponent() const
    {
        auto ptr(componentArray[getComponentTypeID<T>()]);
        return *static_cast<T*>(ptr);
    }
};

class Manager
{
    private:

    std::vector<std::unique_ptr<Entity>> entities;
    std::array<std::vector<Entity*>, maxGroups> groupedEntities;

    public:
    void update()
    {
        for (auto& e : entities) e->update();
    }
    void handleEvents()
    {
        for (auto& e : entities) e->handleEvents();
    }
    void draw()
    {
        for (auto& e : entities) e->draw();
    }

    void refresh()
    {

        for (Group i = 0; i < maxGroups; i++)
        {
            auto& v(groupedEntities[i]); // Vector of entites that belong to a current group    
            v.erase(
                std::remove_if(std::begin(v), std::end(v),
                    [i](Entity* ent){return !ent->isActive() || !ent->hasGroup(i);} // Ensuring if current entity is not deleted or still in the group
                ), std::end(v));
        }
        entities.erase(std::remove_if(std::begin(entities), std::end(entities),
            [](const std::unique_ptr<Entity> &mEntity)
            {
                return !mEntity->isActive();
            }),
                std::end(entities));
    }

    void destroyAll()
    {
        for (auto& e : entities)
        {
            e->destroy();
        }
    }
    void addToGroup(Entity* ent, Group grp)
    {
        groupedEntities[grp].emplace_back(ent);
    }

    void shuffleGroup(Group grp)
    {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine rd(seed);
        std::mt19937 gen(rd());   
        std::shuffle(groupedEntities[grp].begin(), groupedEntities[grp].end(), gen);
    }

    void clearGroup(Group grp)
    {
        for (auto& ent : groupedEntities[grp])
        {
            ent->delGroup(grp);
        }
    }

    const std::vector<Entity*>& getGroup(Group grp)
    {
        return groupedEntities[grp];
    }

    Entity* getCharByIndex(Group grp, size_t i)
    {
        return groupedEntities[grp].at(i);
    }

    Entity& addEntity()
    {
        Entity* e = new Entity(*this);
        std::unique_ptr<Entity> uPtr{ e };
        entities.emplace_back(std::move(uPtr));
        return *e;
    }
};