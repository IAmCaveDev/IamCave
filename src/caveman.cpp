#include "caveman.h"

#include <random>
#include <vector>

#include "buttonfunctions.h"

short Caveman::counter = 0;

Caveman::Caveman(int maxAge, int minAge) : id(counter){
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> unid(minAge, maxAge);

    age = unid(rng);

    std::uniform_int_distribution<int> stats(1, 5);

    fitness = stats(rng);
    intelligence = stats(rng);

    infobox = new Textbox({200, 200}, {450, 400}, "assets/info.png", "TEST");

    counter += 1;
}

Caveman::~Caveman() {
    delete button;
}


void Caveman::setInfoboxVisible(bool visible){
    infoboxVisible = visible;
}


short Caveman::getId(){
    return id;
}
std::string Caveman::getName(){
    return name;
}
unsigned short Caveman::getAge(){
    return age;
}
short Caveman::getFitness(){
    return fitness;
}
void Caveman::setFitness(short newFit) {
    fitness = newFit;
}
short Caveman::getIntelligence(){
    return intelligence;
}

Button& Caveman::getButton() {
    return *button;
}
Textbox& Caveman::getInfobox() {
    return *infobox;
}

void Caveman::initButton() {
    button = new Button({ 100, 200 }, { 0, 0 }, texPath, nullptr,
        std::bind(&ButtonFunctions::Tribe::displayInfo,
            shared_from_this()));
}

void Caveman::setPosition(TransformedVector<> newPosition){
    button->setTransformedPosition(newPosition);
    infobox->setTransformedPosition(
        {newPosition.getRealX() - infobox->getTransformedSize().getRealX(),
         newPosition.getRealY() - infobox->getTransformedSize().getRealY()});
}

EActions Caveman::getCurrentAction(){
    return currentAction;
}

void Caveman::setCurrentAction(EActions newOccupation) {
    currentAction = newOccupation;
}

void Caveman::onResize(){
    button->setPosition(button->getTransformedPosition());
    button->setSize(button->getTransformedSize());
    infobox->setPosition(infobox->getTransformedPosition());
    infobox->setSize(infobox->getTransformedSize());
}

void Caveman::display(sf::RenderWindow& win) const {
    win.draw(*button);
    if(infoboxVisible){
        win.draw(*infobox);
    }
}
