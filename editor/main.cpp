#include "Boids.h"
#include "Fractal.h"
#include "Lines.h"
#include "Snake.h"

int main(){

    Application app;

    Scene scene;
    app.LoadScene(scene);

    SnakeSystem * fract = new SnakeSystem();
    app.RegisterSystem(fract);

    app.Loop();

    delete fract;
    return 0;
}
