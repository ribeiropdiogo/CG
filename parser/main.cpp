#include "includes/Txml.h"

#include <Engine.h>

int main(int argc, char **argv) {
    Engine engine;

    Txml txml;

    txml.loadFile(argv[1]);

    engine.start(&argc, argv);

    txml.loadEngine(&engine);

    engine.loop();

    return 0;
}