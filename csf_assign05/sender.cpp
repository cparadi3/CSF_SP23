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
  //REMOVE
  //std::cerr << TAG_SLOGIN << std::endl;
  //do rlogin first maybe?
  Message msg = Message(TAG_SLOGIN, username);
  // TODO: send slogin message
  if(conn.send(msg) == false) {
    std::cerr << "send failed" << std::endl;
  }
  if(conn.receive(msg) == false) {
    std::cerr << msg.data << std::endl;
    conn.close();
    return 1;
  } else if (msg.tag != TAG_OK) {
    std::cerr << msg.data << std::endl;
    conn.close();
    return 1;  //check
  }
  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate
  while(1) {
    std::string input;
    std::getline(std::cin, input);
    if(input.compare(0, 6, "/join ") == 0) {
      std::string room_name = input.substr(6, std::string::npos);
      Message msg = Message(TAG_JOIN, room_name);
      conn.send(msg);
      if(conn.receive(msg) == false) {
         std::cerr << msg.data << std::endl;
      } 
      else if (msg.tag != TAG_OK) {
        std::cerr << msg.data << std::endl;  //check
      }
    }
    //Join room here after
    else if(input.compare(0, 6, "/leave") == 0) {
      Message msg = Message(TAG_LEAVE, "Leaving");
      conn.send(msg);
      if (!conn.receive(msg)) {
        std::cerr << msg.data << std::endl;
      }
      if(msg.tag.compare(0, std::string::npos, TAG_OK) != 0) {//wait for recieve back
        std::cerr << msg.data  << std::endl;
      }

    }

    else if(input.compare(0, 5, "/quit") == 0) {
      Message msg = Message(TAG_QUIT, "Quitting");
      conn.send(msg);
      conn.receive(msg);
      if(msg.tag.compare(0, std::string::npos, TAG_OK) == 0) {//wait for recieve back
        conn.close();
        return 0;
      } else {
        std::cerr << msg.data  << std::endl;
      }
    }
    else {
      Message msg = Message(TAG_SENDALL, input);
      conn.send(msg);
      if (conn.receive(msg) == false) {
        std::cerr << msg.data << std::endl;
      }
      if(msg.tag.compare(0, std::string::npos, TAG_OK) != 0) {//wait for recieve back
        std::cerr << msg.data  << std::endl;
      }
    }
    

  }

  return 0;
}
