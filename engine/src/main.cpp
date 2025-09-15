#include "server.hpp"

int main() {
    MatchingEngine engine;
    Server server(engine, 9999);
    engine.setServer(&server);
    server.start();
}