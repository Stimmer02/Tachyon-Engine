#include "Boids.h"

int main(){

    Application app;

    Scene scene;
    app.LoadScene(scene);

    BoidsSystem * boids = new BoidsSystem();
    app.RegisterSystem(boids);

    app.Loop();

    delete boids;

    return 0;
}
