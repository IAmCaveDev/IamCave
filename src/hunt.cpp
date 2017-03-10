#include "hunt.h"

Hunt::Hunt(EActions newtype, short time) : Action(time){
    if (newtype == EasyHunt)
        type = EActions::EasyHunt;
    else
        type = EActions::HardHunt;
}

void Hunt::addActor(Caveman* newactor) {
    actors.push_back(newactor);
}

void Hunt::resolve() {
    currentDuration += 1;
    if (currentDuration == totalDuration) {
        //add resources
        //set cavemen to idle
        //delete action
    }
}