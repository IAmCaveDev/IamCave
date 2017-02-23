#include "caveman.h"

#include <random>
#include <vector>

short Caveman::counter = 0;

std::vector<std::string> femaleNames = {
    "Hillary",
    "Liz",
    "Gina",
    "Chelsea",
    "Luna",
    "Giny",
    "Selina",
    "Lillian",
    "Harleen"
};

std::vector<std::string> maleNames = {
    "Donald",
    "Bernie",
    "Harambe",
    "Gabe",
    "Matthew",
    "Harry",
    "Ron",
    "Oswald",
    "Bruce"
};

std::vector<std::string> femaleTextures = {
    "assets/cavewoman.png",
    "assets/cavewoman2.png",
    "assets/cavewoman3.png"
};

std::vector<std::string> maleTextures = {
    "assets/caveman.png",
    "assets/caveman1.png",
    "assets/caveman2.png"
};

Caveman::Caveman(int maxAge, int minAge) : id(counter){
    idle = true;

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> unid(minAge, maxAge);

    age = unid(rng);


    std::uniform_int_distribution<int> unbd(0, 1);

    if(counter < 3){
        male = true;
    }else if(counter < 5){
        male = false;
    }else{
        male = (unbd(rng) != 0);
    }

    std::uniform_int_distribution<int> maleD(0, maleNames.size() - 1);
    std::uniform_int_distribution<int> femaleD(0, femaleNames.size() - 1);

    if(male){
        name = maleNames[maleD(rng)];
    }else{
        name = femaleNames[femaleD(rng)];
    }


    std::uniform_int_distribution<int> stats(1, 5);

    fitness = stats(rng);
    intelligence = stats(rng);

    std::uniform_int_distribution<int> maleT(0, maleTextures.size() - 1);
    std::uniform_int_distribution<int> femaleT(0, femaleTextures.size() - 1);

    if (male) {
        texPath = maleTextures[maleT(rng)];
    }else{
        texPath = femaleTextures[femaleT(rng)];
    }

    button = new Button({0, 0}, {0, 0}, texPath,
        nullptr, std::bind(&ButtonFunctions::Tribe::displayInfo, std::ref(*this)));

    infobox = new Textbox({100, 200 }, { 450, 400 }, "assets/go.png", "");

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
bool Caveman::isMale(){
    return male;
}
unsigned short Caveman::getAge(){
    return age;
}
short Caveman::getFitness(){
    return fitness;
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

void Caveman::display(sf::RenderWindow& win) const {
    if(infoboxVisible){
        win.draw(*infobox);
    }
    win.draw(*button);
}
