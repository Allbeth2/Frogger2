#include "MainMenuState.h"
#include "Game.h"
#include "Button.h"
#include "PlayState.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

MainMenuState::MainMenuState(Game* game)
    : GameState(game)
{
    loadMaps();
    createButtons();
}

void MainMenuState::loadMaps()
{
    const std::string path = "../assets/maps";
    for (const auto& entry : fs::directory_iterator(path))
    {
        if (entry.is_regular_file())
        {
            mapFiles_.push_back(entry.path().stem().string());
        }
    }
}

void MainMenuState::createButtons()
{
    // For now, let's use SALIR.png for the exit button
    // and a placeholder for the play button
    // The positions are just guesses for now
    
    // Play Button
    // playButton_ = new Button(this, game_->getTexture(Game::???), Point2D<float>(100, 200));
    // playButton_->connect([this]() {
    //     game_->pushState(new PlayState(game_)); // Later, pass the selected map
    // });
    // addObject(playButton_);

    // Exit Button
    exitButton_ = new Button(this, game_->getTexture(Game::SALIR), Point2D<float>(100, 300));
    exitButton_->connect([this]() {
        game_->popState();
    });
    addObject(exitButton_);
    addEventListener(exitButton_);
}

void MainMenuState::update()
{
    for (auto& obj : gameObjects_)
    {
        obj->update();
    }
}

void MainMenuState::render() const
{
    game_->getTexture(Game::MENU_BACKGROUND)->render();
    for (const auto& obj : gameObjects_)
    {
        obj->render();
    }

    // Render selected map name
    // For this, I would need a way to render text, or have textures for each map name
    // I will implement this later. For now, I will just print it to the console.
    if (!mapFiles_.empty())
    {
        // For debugging purposes
        // std::cout << "Selected map: " << mapFiles_[selectedMap_] << std::endl;
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
