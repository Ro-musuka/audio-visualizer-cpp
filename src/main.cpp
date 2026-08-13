#include "Application.h"

int main(int argc, char **argv)
{
    Application app;
    bool success = app.Init("../src/music.wav");
    if (success)
    {
        app.Execution();
    }
    app.Shutdown();
}