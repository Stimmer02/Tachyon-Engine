#include "Boids.h"
#include "Fractal.h"
#include "Lines.h"

int main(){

    Application app;

    Scene scene;
    app.LoadScene(scene);

    LinesSystem * fract = new LinesSystem();
    app.RegisterSystem(fract);

    app.Loop();

    delete fract;
    return 0;
}
