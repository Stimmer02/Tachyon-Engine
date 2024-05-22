#include "Boids.h"
#include "Fractal.h"

int main(){

    Application app;

    Scene scene;
    app.LoadScene(scene);

    BoidsSystem * fract = new BoidsSystem();
    app.RegisterSystem(fract);

    app.Loop();

    return 0;
}
