#include <pthread.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <set>
#include <vector>
#include <cctype>
#include <cassert>
#include "message.h"
#include "connection.h"
#include "user.h"
#include "room.h"
#include "guard.h"
#include "server.h"

////////////////////////////////////////////////////////////////////////
// Server implementation data types
////////////////////////////////////////////////////////////////////////

// TODO: add any additional data types that might be helpful
//       for implementing the Server member functions

struct ClientInfo {
    Connection *conn;
    Server *server;
    ClientInfo(Connection *conn, Server *server) : conn(conn), server(server) {}
    ~ClientInfo() {
      delete conn;
    }
};
////////////////////////////////////////////////////////////////////////
// Client thread functions
////////////////////////////////////////////////////////////////////////

namespace {
void chatSender(ClientInfo *info, std::string username) {}
void chatReceiver(ClientInfo *info, std::string username) {}
void *worker(void *arg) {
  pthread_detach(pthread_self());
  ClientInfo *info = static_cast<ClientInfo *>(arg);
  // TODO: use a static cast to convert arg from a void* to
  //       whatever pointer type describes the object(s) needed
  //       to communicate with a client (sender or receiver)

  //server_chat_with_client()
  // TODO: read login message (should be tagged either with
  //       TAG_SLOGIN or TAG_RLOGIN), send response
  Message msg;
  

  // TODO: depending on whether the client logged in as a sender or
  //       receiver, communicate with the client (implementing
  //       separate helper functions for each of these possibilities
  //       is a good idea)
  if (info->conn->receive(msg) == false) {
    if(info->conn->get_last_result() == Connection::INVALID_MSG) {
      msg = Message(TAG_ERR, "Invalid Message");
      info->conn->send(msg);
    }
    return nullptr;
  }

  if(msg.tag.compare(TAG_SLOGIN) != 0 && msg.tag.compare(TAG_RLOGIN) != 0) {
    msg = Message(TAG_ERR, "First message was not a login");
    info->conn->send(msg);
    return nullptr;
  }

  msg = Message(TAG_OK, msg.data);
  if(info->conn->send(msg) == false) {
    return nullptr;
  }

  if (msg.tag.compare(TAG_SLOGIN) == 0) {
    chatSender(info, msg.data);
  }
  else {
    chatReceiver(info, msg.data);
  }
  return nullptr;
}


}

////////////////////////////////////////////////////////////////////////
// Server member function implementation
////////////////////////////////////////////////////////////////////////

Server::Server(int port)
  : m_port(port)
  , m_ssock(-1) {
  // TODO: initialize mutex
  pthread_mutex_init(&m_lock, nullptr);
}

Server::~Server() {
  // TODO: destroy mutex
  pthread_mutex_destroy(&m_lock);
}

bool Server::listen() {
  // TODO: use open_listenfd to create the server socket, return true
  //       if successful, false if not
  std::string portString = std::to_string(m_port);
  m_ssock = open_listenfd(portString.c_str());
  if(m_ssock >= 0) {
    return true;
  }
  else {
    return false;
  }
}

void Server::handle_client_requests() {
  // TODO: infinite loop calling accept or Accept, starting a new
  //       pthread for each connected client
  if(m_ssock < 0) {
    std::cerr << "Socket not open" << std::endl;
    return;
  }
  while (1) {
    int clientfd = accept(m_ssock, nullptr, nullptr);
    if (clientfd < 0) {
      std::cerr << "Error accepting client connection" << std::endl;
      return;
    }
    Connection *conn = new Connection(clientfd);
    ClientInfo *info = new ClientInfo(conn, this);

    pthread_t thr_id;

    if (pthread_create(&thr_id, NULL, worker, info) != 0) {
      //fatal("pthread create failed");
      std::cerr << "pthread create failed" << std::endl;
      return;
    }
  }
}

Room *Server::find_or_create_room(const std::string &room_name) {
  // TODO: return a pointer to the unique Room object representing
  //       the named chat room, creating a new one if necessary
  Guard(this->m_lock);
  Room *tempRoom;
  auto findRoom = m_rooms.find(room_name);
  if(findRoom == m_rooms.end()) {
    tempRoom = new Room(room_name);
    m_rooms[room_name] = tempRoom;
  }
  else {
    tempRoom = findRoom->second;
  }
  return tempRoom;
}
