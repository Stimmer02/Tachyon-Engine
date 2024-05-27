#include "Boids.h"
#include "Fractal.h"
#include "Lines.h"

int main(){

    Application app;

    Scene scene;
    app.LoadScene(scene);

    FractalSystem * fract = new FractalSystem();
    app.RegisterSystem(fract);

    app.Loop();

    delete fract;
    return 0;
}
