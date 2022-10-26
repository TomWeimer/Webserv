#include "http/Server.hpp"

int main(){
    Server server;

    server.listenConnection();
    server.handleConnection();
}