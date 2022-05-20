#include "ECS/ECS.h"

void Entity::addGroup(Group grp) // Needed to be put in cpp class to ensure that manager class is declared before the first definition
{
    groupBitset[grp] = true;
    manager.addToGroup(this, grp);
}

size_t Entity::getGroupSize(Group grp) const
{
    return manager.getGroup(grp).size();
}

const std::vector<Entity*>& Entity::getGroup(Group grp)
{
    return manager.getGroup(grp);
}