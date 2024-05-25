#ifndef FRACTAL_H
#define FRACTAL_H

#include "Application.h"
#include <vector>
#include <thread>
#include <math.h>
#include <mutex>
#include <queue>

struct DrawCommand {
    int x, y;
    Color color;
};

class ThreadSafeQueue {
private:
    std::queue<DrawCommand> queue;
    mutable std::mutex mutex;

public:
    void Enqueue(const DrawCommand& cmd) {
        std::lock_guard<std::mutex> lock(mutex);
        queue.push(cmd);
    }

    bool TryDequeue(DrawCommand& cmd) {
        std::lock_guard<std::mutex> lock(mutex);
        if (queue.empty()) {
            return false;
        }
        cmd = queue.front();
        queue.pop();
        return true;
    }
};

class FractalSystem : public System {
    Scene* scene;
    Timer* timer;

    Image img;
    int cellSize;

    ThreadSafeQueue drawQueue;
    int num_threads;
    std::vector<std::thread> threads;

    CanvasElement * canvas;
    Input * input;

    Color HSVtoRGB(int hue, int saturation, int value) {
        float s = saturation / 255.0f;
        float v = value / 255.0f;
        float h = hue / 255.0f * 360.0f;

        int i = static_cast<int>(h / 60.0f) % 6;
        float f = (h / 60.0f) - i;
        float p = v * (1.0f - s);
        float q = v * (1.0f - f * s);
        float t = v * (1.0f - (1.0f - f) * s);

        float r, g, b;
        switch (i) {
            case 0: r = v, g = t, b = p; break;
            case 1: r = q, g = v, b = p; break;
            case 2: r = p, g = v, b = t; break;
            case 3: r = p, g = q, b = v; break;
            case 4: r = t, g = p, b = v; break;
            case 5: r = v, g = p, b = q; break;
        }

        unsigned char R = r * 255;
        unsigned char G = g * 255;
        unsigned char B = b * 255;

        return {R, G, B};
    }


    void Execute() override {
        static float angle = 0.0;
        static int currentY = 0;

            for (int currentX = 0; currentX < img.width; currentX++) {

                float x = (float)currentX / img.width * 3.5f - 2.0f;
                float y = (float)currentY / img.height * 2.0f - 1.0f;
                float cx = 0.7885f * cos(angle);
                float cy = 0.7885f * sin(angle);
                int iteration = 0, max_iteration = 1000;

                while (x*x + y*y <= 4 && iteration < max_iteration) {
                    float xtemp = x*x - y*y + cx;
                    y = 2*x*y + cy;
                    x = xtemp;
                    iteration++;
                }

                Color color = (iteration == max_iteration) ? Color{0, 0, 0} : HSVtoRGB(iteration % 256, 255, 255);
                img.pixels[currentY * img.width + currentX] = color;
            }

        canvas->UploadImage(img);

            currentY++;

        if( currentY >= img.height ){
            currentY = 0;
            angle += 0.05;
        }

    }

public:

    FractalSystem(){
        this->input = &Input::GetInstance();
        this->num_threads = 1;
        threads.resize(num_threads);
    }

    ~FractalSystem() {
        delete[] img.pixels;
        delete canvas;
    }

    void OnLoad() override {
        canvas = new CanvasElement(0, 0, GraphicConfig::windowWidth, GraphicConfig::windowHeight);
        cellSize = 5;
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
