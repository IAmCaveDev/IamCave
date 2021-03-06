#include "management.h"

#include "rectangle.h"
#include "verticalbuttonlist.h"
#include "button.h"
#include "game.h"
#include "transformedvector.h"
#include "buttonfunctions.h"

Management::Management(Game& gameRef) : GameState(gameRef) {
    type = EGamestates::management;
    nextState = type;

    textbox = new Textbox({1580, 160}, {20, 1080 - 180},
                           "assets/state-textbox.png", "", 15, 30);

    rectangles = {
        new Rectangle({1920, 1080}, {0, 0}, "assets/cave.png"),
        textbox
    };

    actionFactory = ActionFactory();

    actionDisplay = new VerticalButtonList({50, 1080}, {-50, 0}, "");
    resourceDisplay = new ResourceDisplay(gameRef);

    grass = new Rectangle({1920, 1080}, {0, 0}, "assets/cave-grass.png");

    buttons = {
        new Button({200, 80}, {-250, -130}, "assets/go.png", [&](){
                    actionDisplay->decreasePriorityOfAll();
                    nextState = EGamestates::roundEnd;
                }),
        new Button({200, 80}, {50, 50}, "assets/hunt.png",
                std::bind(&ButtonFunctions::Managing::Hunting::hunt, std::ref(*this))),
        new Button({200, 80}, {50, 150}, "assets/think.png",
                std::bind(&ButtonFunctions::Managing::Research::think, std::ref(*this), std::ref(gameRef.getTechtree()))),
        new Button({200, 80}, {50, 250}, "assets/makelove.png",
                std::bind(&ButtonFunctions::Managing::Sex::sex, std::ref(*this))),
        new Button({200, 80}, {50, 350}, "assets/improve.png",
                std::bind(&ButtonFunctions::Managing::Improve::improve, std::ref(*this))),
        new Button({200, 80}, {50, 450}, "assets/collect.png",
                std::bind(&ButtonFunctions::Managing::Collecting::collect, std::ref(*this))),
    };

    resetTextbox();
}

void Management::setCurrentAction(EActions newaction, short duration) {
    switch (newaction) {
        case EActions::EasyHunt:
            currentAction = std::move(actionFactory.createEasyHuntingAction(duration));
            break;
        case EActions::HardHunt:
            currentAction = std::move(actionFactory.createHardHuntingAction(duration));
            break;
        case EActions::SexAction:
            currentAction = std::move(actionFactory.createSexAction());
            break;
        case EActions::ImproveAction:
            currentAction = std::move(actionFactory.createImproveAction(duration, { 0,-30,0 }));
            break;
        case EActions::CollectAction:
            currentAction = std::move(actionFactory.createCollectAction(duration));
            break;
        case EActions::ThinkAction:
            std::transform(activeTech.begin(), activeTech.end(), activeTech.begin(), ::tolower);
            Resources cost;
            if (activeTech == "training") {
                cost = game.getTechtree().getTraining()->getCost();
            }
            else {
                cost = game.getTechtree().getTree().find(activeTech)->second->getCost();
            }
            currentAction = std::move(actionFactory.createThinkingAction(activeTech,duration,cost));
            break;
        // TODO: add more Actions here
    }

}

Action& Management::getCurrentAction() {
    return *currentAction;
}

void Management::pushCurrentAction() {
    EActions currentType = currentAction->getType();

    switch (currentType) {
        case EActions::EasyHunt:
            actionDisplay->addButton(currentAction->getID(), new Button({ 200, 50 }, { 200, 300 },
                "assets/hunt-icon.png", nullptr), currentAction->getDuration());
            break;
        case EActions::HardHunt:
            actionDisplay->addButton(currentAction->getID(), new Button({ 200, 50 }, { 200, 300 },
                "assets/hunt-icon.png", nullptr), currentAction->getDuration());
            break;
        case EActions::SexAction:
            actionDisplay->addButton(currentAction->getID(), new Button({ 200, 50 }, { 200, 300 },
                "assets/makelove-icon.png", nullptr), currentAction->getDuration());
            break;
        case EActions::ImproveAction:
            actionDisplay->addButton(currentAction->getID(), new Button({ 200, 50 }, { 200, 300 },
                "assets/improve-icon.png", nullptr), currentAction->getDuration());
            break;
        case EActions::ThinkAction:
            actionDisplay->addButton(currentAction->getID(), new Button({ 200, 50 }, { 200, 300 },
                "assets/think-icon.png", nullptr), currentAction->getDuration());
            if (activeTech != "training") {
                game.getTechtree().setTrainingMode(true);
            }
            break;
        case EActions::CollectAction:
            actionDisplay->addButton(currentAction->getID(), new Button({ 200, 50 }, { 200, 300 },
                "assets/collect-icon.png", nullptr), currentAction->getDuration());
            break;
    }

    for (auto& it : currentAction->getActors()) {
        it->setCurrentAction(currentType);
    }

    game.addAction(std::move(currentAction));
}

void Management::deleteCurrentAction() {
    currentAction.reset();
}

void Management::setActiveTech(std::string newTech) {
    activeTech = newTech;
}

std::string Management::getActiveTech() {
    return activeTech;
}

void Management::deleteActiveTech() {
    activeTech = "";
}

std::vector<std::shared_ptr<Caveman>> Management::getIdlingTribe() {
    std::vector<std::shared_ptr<Caveman>> idlingTribe;
    for (auto& it : game.getTribe()) {
        if (it->getCurrentAction() == EActions::Idle) {
            idlingTribe.push_back(it);
        }
    }
    return idlingTribe;
}

VerticalButtonList& Management::getActionDisplay() {
    return *actionDisplay;
}

void Management::display(sf::RenderWindow& win) {
    for (auto const& it : rectangles) {
        it->display(win);
    }

    actionDisplay->display(win);

    for (auto const& it : buttons) {
        it->display(win);
    }

    resourceDisplay->display(win);

    for (auto const& it : getIdlingTribe()) {
        it->display(win);
    }

    grass->display(win);

    game.getTechtree().display(win);

}

void Management::setTextboxText(std::string str) {
    textbox->setText(str);
}

void Management::resetTextbox() {
    int roundNumber = game.getRoundNumber();
    if (roundNumber == 0) {
        textbox->setText("Welcome to I am Cave, try to have some fun! You have 100 rounds, your goal is to research the neolithic revolution\nin the techtree!");
    } else if (roundNumber%10 == 0) {
        textbox->setText("Time is running! Only " + std::to_string(100 - roundNumber) + " rounds to go! Select your actions!");
    } else if ((roundNumber == 41) || (roundNumber == 42) || (roundNumber == 43)) {
        textbox->setText("The oldest members of the tribe look tired, looks like they won't make it way past 50.\nMake sure a new generation can take over!");
    } else if ((100 - roundNumber <= 10) && (100 - roundNumber > 1)) {
        textbox->setText("Animals and Materials are getting sparse. Only " + std::to_string(100 - roundNumber) + " rounds left!");
    } else if (100 - roundNumber <= 1) {
        textbox->setText("Oh no...");
    } else {
        textbox->setText("Select your actions for round number " + std::to_string(game.getRoundNumber()) + " !");
    }

}

Techtree& Management::getTechtree() {
    return game.getTechtree();
}

void Management::additionalResizes() {
    grass->setSize(grass->getTransformedSize());
    grass->setPosition(grass->getTransformedPosition());
    resourceDisplay->onResize();
    game.getTechtree().onResize();
}
