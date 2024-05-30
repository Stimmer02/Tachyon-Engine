#include "Boids.h"
#include "Fractal.h"
#include "Lines.h"
#include "../PhysicsProcessor/PhysicsProcessorSystem.h"
#include "../PhysicsProcessor/FpsLimiter.h"

int main(){

    Application app;

    Scene scene;
    app.LoadScene(scene);

    FractalSystem * fract = new FractalSystem();
    app.RegisterSystem(fract);

    PhysicsProcessorSystem * physic = new PhysicsProcessorSystem();
    app.RegisterSystem(physic);

    FpsLimiter * fps = new FpsLimiter();
    app.RegisterSystem(fps);


    app.Loop();


    delete fps;
    delete physic;
    delete fract;
    return 0;
}
