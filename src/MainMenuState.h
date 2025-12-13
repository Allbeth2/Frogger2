#ifndef MAIN_MENU_STATE_H
#define MAIN_MENU_STATE_H

#include "gameState.h"
#include <vector>
#include <string>

class Button;
class Label;
class Texture;

class MainMenuState : public GameState
{
public:
    MainMenuState(Game* game);
    virtual ~MainMenuState();

    void update() override;
    void render() const override;
    void handleEvent(const SDL_Event& event) override;

private:
    /**
     * @brief Carga los mapas disponibles desde la carpeta de assets.
     */
    void loadMaps();

    /**
     * @brief Crea los botones de la interfaz de usuario para el menú principal.
     */
    void createButtons();

    /**
     * @brief Obtiene la textura para el mapa actualmente seleccionado.
     * @return Un puntero a la textura del mapa seleccionado.
     */
    Texture* getSelectedMapTexture() const;

    std::vector<std::string> mapFiles_;
    int selectedMap_ = 0;

    // UI elements
    Button* mapNameButton_ = nullptr;
    Button* exitButton_ = nullptr;
    Button* leftArrowButton_ = nullptr;
    Button* rightArrowButton_ = nullptr;
};

#endif // MAIN_MENU_STATE_H
