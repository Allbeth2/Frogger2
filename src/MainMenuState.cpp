#include "MainMenuState.h"
#include "Game.h"
#include "Button.h"
#include "Label.h"
#include "PlayState.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

MainMenuState::MainMenuState(Game* game)
    : GameState(game)
{
    loadMaps();

    // Load last selected map from config.txt
    std::ifstream configFile("../config.txt");
    if (configFile.is_open())
    {
        std::string lastMap;
        configFile >> lastMap;
        for (size_t i = 0; i < mapFiles_.size(); ++i)
        {
            if (mapFiles_[i] == lastMap)
            {
                selectedMap_ = i;
                break;
            }
        }
    }

    createButtons();
}

MainMenuState::~MainMenuState()
{
    // Save the last selected map to config.txt
    if (!mapFiles_.empty())
    {
        std::ofstream configFile("../config.txt");
        if (configFile.is_open())
        {
            configFile << mapFiles_[selectedMap_];
        }
    }
}

void MainMenuState::loadMaps()
{
    // Hardcode the map names in the correct order
    mapFiles_ = {
        "Original",
        "Práctica 1",
        "Trivial",
        "Veloz",
        "Avispado"
    };
}

void MainMenuState::createButtons()
{
    // Positions
    const int buttonWidth = 150;
    const int buttonHeight = 50;
    const int padding = 20;

    // "Choose a Map" label
    Label* chooseMapLabel = new Label(this, game_->getTexture(Game::ELIGE_UN_MAPA), Point2D<float>((float)Game::WINDOW_WIDTH / 2 - 100, 150));
    addObject(chooseMapLabel);

    // Map Name Button
    if (!mapFiles_.empty())
    {
        mapNameButton_ = new Button(this, getSelectedMapTexture(), Point2D<float>((float)Game::WINDOW_WIDTH / 2 - buttonWidth / 2, 250));
        mapNameButton_->connect([this]() {
            // Find the full path of the selected map
            std::string selectedMapPath = "../assets/maps/" + this->mapFiles_[this->selectedMap_] + ".txt";
            this->game_->pushState(std::make_shared<PlayState>(this->game_, selectedMapPath)); // Pass selected map path
        });
        addObject(mapNameButton_);
        addEventListener(mapNameButton_);
    }

    // Left Arrow Button
    leftArrowButton_ = new Button(this, game_->getTexture(Game::LEFT_ARROW), Point2D<float>(50, 250));
    leftArrowButton_->connect([this]() {
        if (!mapFiles_.empty())
        {
            selectedMap_ = (selectedMap_ - 1 + mapFiles_.size()) % mapFiles_.size();
        }
    });
    addObject(leftArrowButton_);
    addEventListener(leftArrowButton_);

    // Right Arrow Button
    rightArrowButton_ = new Button(this, game_->getTexture(Game::RIGHT_ARROW), Point2D<float>(Game::WINDOW_WIDTH - 50 - game_->getTexture(Game::RIGHT_ARROW)->getFrameWidth(), 250));
    rightArrowButton_->connect([this]() {
        if (!mapFiles_.empty())
        {
            selectedMap_ = (selectedMap_ + 1) % mapFiles_.size();
        }
    });
    addObject(rightArrowButton_);
    addEventListener(rightArrowButton_);


    // Exit Button (SALIR)
    exitButton_ = new Button(this, game_->getTexture(Game::SALIR), Point2D<float>((float)Game::WINDOW_WIDTH / 2 - buttonWidth / 2, 320));
    exitButton_->connect([this]() {
        this->game_->popState();
    });
    addObject(exitButton_);
    addEventListener(exitButton_);
}

Texture* MainMenuState::getSelectedMapTexture() const
{
    if (mapFiles_.empty()) return nullptr;

    Game::TextureName mapTextureName;
    std::string currentMapName = this->mapFiles_[this->selectedMap_];
    if (currentMapName == "Original") mapTextureName = Game::ORIGINAL_MAP_TEXT;
    else if (currentMapName == "Práctica 1") mapTextureName = Game::PRACTICA_1_MAP_TEXT;
    else if (currentMapName == "Avispado") mapTextureName = Game::AVISPADO_MAP_TEXT;
    else if (currentMapName == "Trivial") mapTextureName = Game::TRIVIAL_MAP_TEXT;
    else if (currentMapName == "Veloz") mapTextureName = Game::VELOZ_MAP_TEXT;
    else return nullptr; // Or a default texture

    return this->game_->getTexture(mapTextureName);
}

void MainMenuState::update()
{
    for (auto& obj : gameObjects_)
    {
        obj->update();
    }

    if (mapNameButton_ != nullptr)
    {
        mapNameButton_->setTexture(getSelectedMapTexture());
    }

    if (leftArrowButton_ != nullptr)
    {
        leftArrowButton_->setVisible(selectedMap_ > 0);
    }
    if (rightArrowButton_ != nullptr)
    {
        rightArrowButton_->setVisible(selectedMap_ < mapFiles_.size() - 1);
    }
}

void MainMenuState::render() const
{
    game_->getTexture(Game::MENU_BACKGROUND)->render();
    for (const auto& obj : gameObjects_)
    {
        obj->render();
    }
}

void MainMenuState::handleEvent(const SDL_Event& event)
{
    for (auto& handler : eventHandlers_)
    {
        handler->handleEvent(event);
    }

    if (event.type == SDL_EVENT_KEY_DOWN)
    {
        if (event.key.key == SDLK_RIGHT)
        {
            if (!mapFiles_.empty())
            {
                selectedMap_ = (selectedMap_ + 1) % mapFiles_.size();
            }
        }
        else if (event.key.key == SDLK_LEFT)
        {
            if (!mapFiles_.empty())
            {
                selectedMap_ = (selectedMap_ - 1 + mapFiles_.size()) % mapFiles_.size();
            }
        }
    }
}
