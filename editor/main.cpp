#include "Boids.h"
#include "Fractal.h"
#include "Lines.h"
#include "Snake.h"
#include "Water.h"

int main(){

    Application app;

    Scene scene;
    app.LoadScene(scene);

    BoidsSystem * fract = new BoidsSystem();
    app.RegisterSystem(fract);

    app.Loop();

    delete fract;
    return 0;
}
