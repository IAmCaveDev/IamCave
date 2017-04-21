#include "techtree.h"

#include <stdexcept>
#include <fstream>

void Techtree::positionTree(json data, short level,
                            TransformedVector<> lastPos) {
    std::string name = data["name"].get<std::string>();

    auto p = tree[name];

    if (level - 1 == p->getLevel()) {
        p->getButton().setTransformedPosition(lastPos);
    }

    for (auto& it : p->getParents()) {
        std::shared_ptr<Tech> parent;
        while (parent != nullptr) {
            parent = it->getParents()[0];
        }
    }

    for (auto& it : data["children"]) {
        int posY;

        if (sizePerLevel[level] == 1) {
            posY = getTransformedPosition().getRealY()
                   + getTransformedSize().getRealY()/2 - techSize/2;
        } else {
            posY = getTransformedPosition().getRealY()
                   + getTransformedSize().getRealY()
                   - getTransformedSize().getRealY() / (sizePerLevel[level] - 1)
                   * iteratorPerLevel[level];

            if (iteratorPerLevel[level] > 0 && iteratorPerLevel[level] < (sizePerLevel[level] - 1)) {
                posY -= techSize / 2;
            } else if(iteratorPerLevel[level] == 0) {
                posY -= techSize;
            }
        }

        positionTree(it, level + 1, {lastPos.getRealX()+techSize+padding,posY});

        iteratorPerLevel[level] += 1;
    }
}

void Techtree::parse(std::shared_ptr<Tech> parent, json data, short level) {
    std::string name = data["name"].get<std::string>();
    std::string techPath = "assets/tech/" + name + ".json";
    bool straight;
    try {
        straight = data.at("straight");
    }
    catch(std::out_of_range& oor) {
        straight = false;
    }
    auto newParent = tree.emplace(name, new Tech(techPath, level, {parent},
                                  straight));

    // if already exists in tree
    if (!newParent.second) {
        newParent.first->second->getParents().push_back(parent);
        if (level > newParent.first->second->getLevel()) {
            newParent.first->second->setLevel(level);
        }
    } else {
        try {
            sizePerLevel.at(level);

            sizePerLevel[level] += 1;
        }
        catch(const std::out_of_range& oor) {
            sizePerLevel.push_back(1);
            iteratorPerLevel.push_back(0);
        }
    }

    for (auto& it : data["children"]) {
        parse(newParent.first->second, it, level+1);
    }
}

Techtree::Techtree(std::string backgroundPath, std::string path,
                   TransformedVector<> newSize, TransformedVector<> newPos)
                   : Rectangle(newSize, newPos, backgroundPath) {
    visibility = false;

    std::ifstream in(path);
    if (in.good()) {
        json data;
        in >> data;

        parse(nullptr, data, 0);
        positionTree(data, 1, {getTransformedPosition().getRealX(),
                               getTransformedPosition().getRealY()
                               + getTransformedSize().getRealY() / 2
                               - techSize / 2});
        for (auto& it : tree) {
            it.second->createArrowsToParents();
        }
    } else {
        throw std::runtime_error("Could not open file at " + path);
    }

    properThinking = new Button({ 200, 80 }, { -250, -130 }, "assets/confirm.png", nullptr);
    abortThinking = new Button({ 200, 80 }, { -250, -330 }, "assets/abort.png", nullptr);
}

void Techtree::setVisibility(bool newVisibility) {
    visibility = newVisibility;
    for (auto& it : tree) {
        it.second->getButton().setVisibility(newVisibility);
    }
}

bool Techtree::getVisibility() {
    return visibility;
}

void Techtree::display(sf::RenderWindow& win) {
    if (visibility) {
        this->Rectangle::display(win);
        for (auto& it : tree) {
            it.second->getButton().display(win);
            win.draw(it.second->getArrowsToParents().tip);
            for(auto& line : it.second->getArrowsToParents().lines) {
                win.draw(line);
            }
        }
        properThinking->display(win);
        abortThinking->display(win);
    }
}

void Techtree::onResize() {
    for (auto& it : tree) {
        auto& button = it.second->getButton();
        button.setPosition(button.getTransformedPosition());
        button.setSize(button.getTransformedSize());

        it.second->updateArrowsToParents();
    }
    properThinking->setPosition(properThinking->getTransformedPosition());
    abortThinking->setPosition(abortThinking->getTransformedPosition());
    properThinking->setSize(properThinking->getTransformedSize());
    abortThinking->setSize(abortThinking->getTransformedSize());
    setPosition(getTransformedPosition());
    setSize(getTransformedSize());
}

std::map<std::string, std::shared_ptr<Tech>>& Techtree::getTree() {
    return tree;
}

Button& Techtree::getProperThinking() {
    return *properThinking;
}

Button& Techtree::getAbortThinking() {
    return *abortThinking;
}