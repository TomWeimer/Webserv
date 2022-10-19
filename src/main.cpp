#include "server/server.hpp"

int main(){
    Server server;

    server.listen_connection();
    server.handle_connection();
}