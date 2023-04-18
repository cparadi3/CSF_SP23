#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

int main(int argc, char **argv) {
  if (argc != 5) {
    std::cerr << "Usage: ./receiver [server_address] [port] [username] [room]\n";
    return 1;
  }

  std::string server_hostname = argv[1];
  int server_port = std::stoi(argv[2]);
  std::string username = argv[3];
  std::string room_name = argv[4];

  Connection conn;

  // TODO: connect to server
  conn.connect(server_hostname, server_port);
  if(!conn.is_open()) {
    std::cerr << "Could not connect" << std::endl;
    return 1;
  }
  // TODO: send rlogin and join messages (expect a response from
  //       the server for each one)
  Message msg = Message(TAG_RLOGIN, username);
  conn.send(msg);
  if(conn.receive(msg) == false) {
    std::cerr << "Login failed" << std::endl;
    return 1;
  }
  msg = Message(TAG_JOIN, room_name);
  conn.send(msg);
  if(conn.receive(msg) == false) {
    std::cerr << "Could not join room" << std::endl;
    return 1;
  }

  while(1) {
    conn.receive(msg);
    if(msg.tag == TAG_DELIVERY) {
      //TODO: make this its own function?
      std::string output;
      //get the part right after the first colon
      output = msg.data.substr(msg.data.find_first_of(":", 0) + 1, std::string::npos);
      std::cout << output + "\n";
    }
    else if (msg.tag == TAG_ERR) {
      std::cerr << "Failed to receive message" << std::endl;
      return 1;
    }
    //print message
    //else if tagerr print err
  }
  // TODO: loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)



  return 0;
}
