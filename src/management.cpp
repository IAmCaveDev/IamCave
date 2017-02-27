#include "management.h"
#include "rectangle.h"
#include "button.h"
#include "game.h"
#include "transformedvector.h"
#include "buttonfunctions.h"

Management::Management(Game& gameRef) : GameState(gameRef) {

    rectangles = {
        new Rectangle({1920, 1080}, {0, 0}, "assets/cave.png")
    };

    buttons = {
        new Button({200, 50}, {-400, -150}, "assets/go.png",
                std::bind(&Game::setCurrentGameState, std::ref(gameRef), EGamestates::roundEnd)),
        new Button({200, 50}, {200, 100}, "assets/hunt.png",
                std::bind(&ButtonFunctions::Managing::Hunting::hunt, std::ref(*this))),
        new Button({200, 50}, {200, 200}, "assets/think.png", nullptr),
        new Button({200, 50}, {200, 300}, "assets/fuck.png", nullptr),
        new Button({200, 50}, {200, 400}, "assets/improve.png", nullptr),
        new Button({200, 50}, {-400, -50}, "assets/abort.png", nullptr)
    };

}

void Management::setCurrentAction(Action* newaction) {
    currentAction = newaction;
}

Action& Management::getCurrentAction() {
    return *currentAction;
}

void Management::pushCurrentAction() {
    game.addActiontoQueue(currentAction);
    currentAction = nullptr;
}

void Management::display(sf::RenderWindow& win) {
    for (auto const& it : rectangles) {
        it->display(win);
    }

    for (auto const& it : buttons) {
        it->display(win);
    }

    for (auto const& it : game.getTribe()) {
        it->display(win);
    }
}
