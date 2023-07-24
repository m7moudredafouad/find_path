#include <game.hpp>
#include <X11/Xlib.h>

int main() {
    XInitThreads();
    Game game;
    game.Loop();
    return 0;
}