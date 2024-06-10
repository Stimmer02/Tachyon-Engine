#ifndef FRACTAL_H
#define FRACTAL_H

#include "Application.h"
#include <vector>
#include <thread>
#include <cmath>
#include <mutex>
#include <future>
#include <queue>
#include <atomic>

#define NUMTHREADS 4

class FractalSystem : public System {
    Scene* scene;
    Timer* timer;

    Image img;
    int cellSize;

    CanvasElement * canvas;
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

    void ComputeFractalRow(int startRow, int endRow, int imgWidth, int imgHeight, int maxIteration, float cx, float cy) {
        for (int currentY = startRow; currentY < endRow; currentY++) {
            for (int currentX = 0; currentX < imgWidth; currentX++) {
                float x0 = (float)currentX / imgWidth * 3.5f - 2.0f;
                float y0 = (float)currentY / imgHeight * 2.0f - 1.0f;
                float x = x0;
                float y = y0;
                int iteration = 0;

                while (x*x + y*y <= 4 && iteration < maxIteration) {
                    float xtemp = x*x - y*y + cx;
                    y = 2*x*y + cy;
                    x = xtemp;
                    iteration++;
                }

                img.pixels[currentY * imgWidth + currentX] = colorLookup[iteration];
            }
        }
    }

    void Execute() override {
        static float angle = 0.0f;

        float cx = cos(angle) + 0.5f;
        float cy = sin(angle) + 0.5f;

        int imgWidth = img.width;
        int imgHeight = img.height;

        std::vector<std::thread> threads;
        std::atomic<int> nextRow(0);

        auto threadFunc = [&]() {
            int currentRow;
            while ((currentRow = nextRow.fetch_add(1)) < imgHeight) {
                ComputeFractalRow(currentRow, currentRow + 1, imgWidth, imgHeight, maxIteration, cx, cy);
            }
        };

        for (int i = 0; i < NUMTHREADS; i++) {
            threads.emplace_back(threadFunc);
        }

        for (auto& thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }

        canvas->UploadImage(img);

        angle += timer->GetDeltaFrame();
    }

public:

    FractalSystem(){
        this->input = &Input::GetInstance();
        this->cellSize = 3;
        PopulateColorLookup();
    }

    ~FractalSystem() {
        delete[] img.pixels;
        delete canvas;
    }

    void OnLoad() override {
        canvas = new CanvasElement(GraphicConfig::windowWidth, GraphicConfig::windowHeight);
        canvas->SetCellSize(cellSize);
        img.pixels = new Color[GraphicConfig::windowWidth * GraphicConfig::windowHeight/(cellSize*cellSize)];
        img.width = GraphicConfig::windowWidth/cellSize;
        img.height = GraphicConfig::windowHeight/cellSize;
        scene->AddGUIToScene(canvas);
    }

    void Share(SharedNameResolver* resourceManager) override {
        this->scene = (Scene*)resourceManager->Find("scene");
        this->timer = (Timer*)resourceManager->Find("timer");
    }
};

#endif
