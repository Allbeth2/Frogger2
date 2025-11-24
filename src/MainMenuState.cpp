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

	// Carga la última selección de mapa de config.txt
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
	// Guarda la selección actual de mapa en config.txt
    if (!mapFiles_.empty())
    {
        std::ofstream configFile("../config.txt");
        if (configFile.is_open())
        {
            configFile << mapFiles_[selectedMap_];
        }
    }
}

/**
 * @brief Carga los mapas disponibles desde la carpeta de assets.
 */
void MainMenuState::loadMaps()
{
    mapFiles_ = {
        "Original",
        "Práctica 1",
        "Trivial",
        "Veloz",
        "Avispado"
    };
}

namespace {
    // Constantes para la ubicación de los botones
    const int BUTTON_WIDTH = 150;
    const int BUTTON_HEIGHT = 50;
    const int CHOOSE_MAP_LABEL_Y = 150;
    const int MAP_NAME_BUTTON_Y = 250;
    const int EXIT_BUTTON_Y = 320;
    const int ARROW_BUTTON_Y = 250;
    const int LEFT_ARROW_X = 50;
    const int RIGHT_ARROW_X_PADDING = 50;
}

/**
 * @brief Crea los botones de la interfaz de usuario para el menú principal.
 */
void MainMenuState::createButtons()
{
    // Etiqueta "Elige un Mapa"
    Label* chooseMapLabel = new Label(this, game_->getTexture(Game::ELIGE_UN_MAPA), Point2D<float>((float)Game::WINDOW_WIDTH / 2 - 100, (float)CHOOSE_MAP_LABEL_Y));
    GameState::addObject(chooseMapLabel);

    // Botón con el nombre del mapa
    if (!mapFiles_.empty())
    {
        mapNameButton_ = new Button(this, getSelectedMapTexture(), Point2D<float>((float)Game::WINDOW_WIDTH / 2 - BUTTON_WIDTH / 2, (float)MAP_NAME_BUTTON_Y));
        mapNameButton_->connect([this]() {
            // Encuentra la ruta completa del mapa seleccionado
            std::string selectedMapPath = "../assets/maps/" + this->mapFiles_[this->selectedMap_] + ".txt";
            this->game_->pushState(std::make_shared<PlayState>(this->game_, selectedMapPath)); // Pasa la ruta del mapa seleccionado
        });
        GameState::addObject(mapNameButton_);
        addEventListener(mapNameButton_);
    }

    // Botón de flecha izquierda
    leftArrowButton_ = new Button(this, game_->getTexture(Game::LEFT_ARROW), Point2D<float>((float)LEFT_ARROW_X, (float)ARROW_BUTTON_Y));
    leftArrowButton_->connect([this]() {
        if (!mapFiles_.empty())
        {
            selectedMap_ = (selectedMap_ - 1 + mapFiles_.size()) % mapFiles_.size();
        }
    });
    GameState::addObject(leftArrowButton_);
    addEventListener(leftArrowButton_);

    // Botón de flecha derecha
    rightArrowButton_ = new Button(this, game_->getTexture(Game::RIGHT_ARROW), Point2D<float>(Game::WINDOW_WIDTH - RIGHT_ARROW_X_PADDING - game_->getTexture(Game::RIGHT_ARROW)->getFrameWidth(), (float)ARROW_BUTTON_Y));
    rightArrowButton_->connect([this]() {
        if (!mapFiles_.empty())
        {
            selectedMap_ = (selectedMap_ + 1) % mapFiles_.size();
        }
    });
    GameState::addObject(rightArrowButton_);
    addEventListener(rightArrowButton_);


    // Botón de salir (SALIR)
    exitButton_ = new Button(this, game_->getTexture(Game::SALIR), Point2D<float>((float)Game::WINDOW_WIDTH / 2 - BUTTON_WIDTH / 2, (float)EXIT_BUTTON_Y));
    exitButton_->connect([this]() {
        this->game_->popState();
    });
    GameState::addObject(exitButton_);
    addEventListener(exitButton_);
}

/**
 * @brief Obtiene la textura para el mapa actualmente seleccionado.
 * @return Un puntero a la textura del mapa seleccionado.
 */
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
    else return nullptr; // O una textura por defecto

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
