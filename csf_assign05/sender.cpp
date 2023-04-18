#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cerr << "Usage: ./sender [server_address] [port] [username]\n";
    return 1;
  }

  std::string server_hostname;
  int server_port;
  std::string username;

  server_hostname = argv[1];
  server_port = std::stoi(argv[2]);
  username = argv[3];

  Connection conn;

  // TODO: connect to server
  conn.connect(server_hostname, server_port);
  if(!conn.is_open()) {
    std::cerr << "Could not connect" << std::endl;
    return 1;
  }

  // TODO: send slogin message
  Message msg = Message(TAG_SLOGIN, username);
  conn.send(msg);
  if(conn.receive(msg) == false) {
    std::cerr << "Login failed\n";
    return 1;
  }
  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate
  while(1) {
    std::string input;
    std::cin >> input;
    if(input.compare(0, 6, "/join ") == 0) {
      std::string room_name = input.substr(6, std::string::npos);
      Message msg = Message(TAG_JOIN, room_name);
      conn.send(msg);
      if(conn.receive(msg) == false) {
         std::cerr << "Could not join room\n";
          return 1;
        }
    }
    //Join room here after
    if(input.compare(0, 6, "/leave")) {
      Message msg = Message(TAG_LEAVE, "yeet");
      
    }

    if(input.compare(0, 5, "/quit")) {
      Message msg = Message(TAG_QUIT, "yaga");
      return 1;
    }
    

  }

  return 0;
}
