#include "action.h"

int Action::counter = 0;

Action::Action(short time, Resources cost) : cost(cost), totalDuration(time), id(counter) {
    currentDuration = 0;
    counter += 1;
}

const Resources Action::getCost() {
    return cost;
}

const int Action::getID() {
    return id;
}

EActions Action::getType() {
    return type;
}

short Action::getDuration() {
    return totalDuration;
}

std::vector<std::shared_ptr<Caveman>>& Action::getActors() {
    return actors;
}

void Action::removeActor(std::shared_ptr<Caveman> caveman) {
    actors.erase(std::remove_if(actors.begin(), actors.end(), [caveman](std::shared_ptr<Caveman> x) {return (x->getId() == caveman->getId());}));
}
