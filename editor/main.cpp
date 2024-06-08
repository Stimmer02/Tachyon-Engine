#include "Boids.h"
#include "Fractal.h"
#include "Lines.h"
#include "Snake.h"
#include "Landscape.h"

int main(){

    Application app;

    Scene scene;
    app.LoadScene(scene);

    LandscapeSystem * fract = new LandscapeSystem();
    app.RegisterSystem(fract);

    app.Loop();

    delete fract;
    return 0;
}
