//
// Created by ripopov on 6/8/2020.
//

#include "SDL.h"
#include "SDL_ttf.h"
#include "camera.h"
#include "model.h"
#include "render.h"
#include <memory>
#include <stdexcept>
#include <vector>
#include <glm/ext/matrix_transform.hpp>

using namespace gfx;

struct PlacedModel {
    std::unique_ptr<Model> model;
    glm::mat4x4 modelMtx;
};

static void throwSDLRuntimeError(const std::string& msg) {
    throw std::runtime_error(msg + " , Error: " + SDL_GetError());
}

class GuiApp {
public:
    GuiApp();
    ~GuiApp();

    void runMainLoop();

private:
    void initSDL();
    void createScene();

    void renderText(const char* text);
    void renderScene();
    void handleKeys();
    void updateScene() { needReDraw = true; }

private:
    int screen_width = 640;
    int screen_height = 480;
    bool needReDraw = true;

    Camera camera;
    std::vector<PlacedModel> scene;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    TTF_Font* font = nullptr;
    SDL_Texture* textTexture = nullptr;
    SDL_Texture *sceneTexture = nullptr;
};

GuiApp::GuiApp() { initSDL(); }

void GuiApp::initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        throw std::runtime_error(SDL_GetError());
    }

    if (TTF_Init() != 0) {
        throwSDLRuntimeError("TTF_Init");
    }

    font = TTF_OpenFont("DroidSans.ttf", 13);

    if (font == nullptr) {
        throwSDLRuntimeError("TTF_OpenFont");
    }

    window = SDL_CreateWindow("GuiApp", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height,
                              SDL_WINDOW_RESIZABLE);

    if (window == nullptr) {
        throwSDLRuntimeError("CreateWindow");
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == nullptr) {
        throwSDLRuntimeError("CreateRenderer");
    }

    createScene();

    renderText("Controls : W A S D M");
    renderScene();
}

void GuiApp::createScene() {

    auto& box_center = scene.emplace_back();
    box_center.model = std::make_unique<Model>("cube/cube.obj");
    box_center.modelMtx = glm::mat4{1.0f};

    for (int i = 1; i < 5; ++i) {
        auto& box_dx = scene.emplace_back();
        box_dx.model = std::make_unique<Model>("cube/cube.obj");
        box_dx.modelMtx = glm::translate(glm::mat4{1.0f}, glm::vec3{1.1 * i, 0, 0});

        auto& box_dy = scene.emplace_back();
        box_dy.model = std::make_unique<Model>("cube/cube.obj");
        box_dy.modelMtx = glm::translate(glm::mat4{1.0f}, glm::vec3{0, -1.1 * i, 0});
    }

    auto& teapot = scene.emplace_back();
    teapot.model = std::make_unique<Model>("teapot/teapot.obj");
    teapot.modelMtx = glm::scale(glm::mat4{1.0f}, glm::vec3{0.01f,0.01f,0.01f});
    teapot.modelMtx = glm::translate(teapot.modelMtx, glm::vec3 {50.0f, 50.0f, 0.0f});
}

void GuiApp::renderText(const char* text) {
    SDL_Surface* surface = TTF_RenderText_Shaded(font, text, {255, 255, 255}, {0, 0, 0});
    if (textTexture != nullptr) {
        SDL_DestroyTexture(textTexture);
    }
    textTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void GuiApp::renderScene()
{
    if (needReDraw)
    {
        needReDraw = false;
        camera.setAspectRatio(screen_width, screen_height);
        gfx::Surface renderSurf{screen_width, screen_height};

        const auto viewMtx = camera.viewMtx();
        const auto projectionMtx = camera.projMtx();

        for (const auto& mod : scene)
        {
            const auto transMtx = projectionMtx * viewMtx * mod.modelMtx;
            renderToTarget(*mod.model, renderSurf, transMtx);
        }

        if (sceneTexture != nullptr) {
            SDL_DestroyTexture(sceneTexture);
        }

        sceneTexture = SDL_CreateTextureFromSurface(renderer, renderSurf.sdlSurface());
    }
}

void GuiApp::runMainLoop() {
    bool quit = false;
    SDL_Event event;
    while (!quit) {

        while (SDL_PollEvent(&event)) {

            switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    screen_width = event.window.data1;
                    screen_height = event.window.data2;
                    updateScene();
                }
                break;
            case SDL_MOUSEMOTION:
                camera.rotateRoundUp(-0.001f*event.motion.xrel);
                camera.rotateRoundRight(-0.001f*event.motion.yrel);
                updateScene();
                break;
            default:
                break;
            }
        }

        handleKeys();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        renderScene();
        SDL_RenderCopy(renderer, sceneTexture, nullptr, nullptr);

        int texW = 0;
        int texH = 0;
        SDL_QueryTexture(textTexture, nullptr, nullptr, &texW, &texH);
        SDL_Rect dstrect = {11, 11, texW, texH};
        SDL_RenderCopy(renderer, textTexture, nullptr, &dstrect);

        SDL_RenderPresent(renderer);
    }
}

GuiApp::~GuiApp() {
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);

    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    TTF_Quit();
    SDL_Quit();
}

void GuiApp::handleKeys() {
    /* handle sprite movement */
    auto keystate = SDL_GetKeyboardState(nullptr);

    std::string message = " Keys: W,A,S,D; PRESSED: ";

    if (keystate[SDL_SCANCODE_D]) {
        message += " STRAFE_RIGHT";
        camera.strafe(0.1f);
        updateScene();
    }

    if (keystate[SDL_SCANCODE_A]) {
        message += " STRAFE_LEFT";
        camera.strafe(-0.1f);
        updateScene();
    }

    if (keystate[SDL_SCANCODE_W]) {
        message += " MOVE_FORWARD";
        camera.move(0.1f);
        updateScene();
    }

    if (keystate[SDL_SCANCODE_S]) {
        message += " MOVE_BACKWARD";
        camera.move(-0.1f);
        updateScene();
    }

    if (keystate[SDL_SCANCODE_M]) {

        if (SDL_GetRelativeMouseMode() == SDL_TRUE)
        {
            SDL_SetRelativeMouseMode(SDL_FALSE);
        }
        else
        {
            SDL_SetRelativeMouseMode(SDL_TRUE);
        }
    }

    renderText(message.c_str());
}

int main(int, char**) {
    GuiApp app;
    app.runMainLoop();
    return 0;
}
