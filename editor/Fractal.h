#ifndef FRACTAL_H
#define FRACTAL_H

#include "Application.h"
#include <vector>
#include <thread>
#include <cmath>
#include <mutex>
#include <queue>

class FractalSystem : public System {
    Scene* scene;
    Timer* timer;

    Image img;
    int cellSize;

    Sprite2D * sprite;
    CanvasElement * canvas;
    SharedNameResolver* resourceManager;
    Input * input;

    static constexpr int maxIteration = 16;

    Color colorLookup[maxIteration];

    void PopulateColorLookup() {
        for (int i = 0; i < maxIteration; ++i) {
            colorLookup[i] = CalculateColor(i);
        }
    }

    Color CalculateColor(int iteration) {
        if (iteration == maxIteration)
            return Color{0, 0, 0};
        else {
            float t = static_cast<float>(iteration) / maxIteration;
            unsigned char val = static_cast<unsigned char>(255 * t);
            return Color{val, val, val};
        }
    }

    void Execute() override {
        // static int currentY = 0;

        // Vector3 pos = input->GetMousePosition();
        // float cx = ((pos.x - GraphicConfig::windowWidth * 0.5f) / GraphicConfig::windowWidth) + 0.5f;
        // float cy = ((pos.y - GraphicConfig::windowHeight * 0.5f) / GraphicConfig::windowHeight) + 0.5f;

        // for (int currentX = 0; currentX < img.width; currentX++) {

        //     float x = (float)currentX / img.width * 3.5f - 2.0f;
        //     float y = (float)currentY / img.height * 2.0f - 1.0f;
        //     int iteration = 0;

        //     while (x*x + y*y <= 4 && iteration < maxIteration) {
        //         float xtemp = x*x - y*y + cx;
        //         y = 2*x*y + cy;
        //         x = xtemp;
        //         iteration++;
        //     }

        //     img.pixels[currentY * img.width + currentX] = colorLookup[iteration];
        // }

        // canvas->UploadImage(img);

        // currentY++;

        // if( currentY >= img.height ){
        //     currentY = 0;
        // }
    }

public:

    FractalSystem(){
        this->input = &Input::GetInstance();
        this->cellSize = 1;
        PopulateColorLookup();

        img.pixels = new Color[GraphicConfig::windowWidth * GraphicConfig::windowHeight/(cellSize*cellSize)];
        img.width = GraphicConfig::windowWidth/cellSize;
        img.height = GraphicConfig::windowHeight/cellSize;
    }

    ~FractalSystem() {
        delete[] img.pixels;
        delete canvas;
    }

    void OnLoad() override {
        this->sprite = new Sprite2D(&img);
        canvas = new CanvasElement(GraphicConfig::windowWidth, GraphicConfig::windowHeight);
        canvas->SetSprite(sprite);
        scene->AddGUIToScene(canvas);
        resourceManager->Emplace("tbo", sprite, sizeof(Sprite2D));
    }

    void Share(SharedNameResolver* resourceManager) override {
        this->scene = (Scene*)resourceManager->Find("scene");
        this->timer = (Timer*)resourceManager->Find("timer");
        this->resourceManager = resourceManager;
    }
};

#endif
