#include "Boids.h"
#include "Fractal.h"
#include "Lines.h"
#include "PhysicsProcessorSystem.h"

int main(){

    Application app;

    Scene scene;
    app.LoadScene(scene);

    FractalSystem * fract = new FractalSystem();
    app.RegisterSystem(fract);

    PhysicsProcessorSystem * physic = new PhysicsProcessorSystem();
    app.RegisterSystem(physic);

    app.Loop();

    // delete physic;
    delete fract;
    return 0;
}
