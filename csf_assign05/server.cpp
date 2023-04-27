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
#include "client_util.h"
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


const std::string WHITESPACE = " \n\r\t\f\v";
std::string ltrim_2(const std::string &s) {
  size_t start = s.find_first_not_of(WHITESPACE);
  return (start == std::string::npos) ? "" : s.substr(start);
}
 
std::string rtrim_2(const std::string &s) {
  size_t end = s.find_last_not_of(WHITESPACE);
  return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}
 
std::string trim_2(const std::string &s) {
  return rtrim_2(ltrim_2(s));
}
////////////////////////////////////////////////////////////////////////
// Client thread functions
////////////////////////////////////////////////////////////////////////

namespace {
void chat_with_sender(ClientInfo *info, std::string username) {
  Message msg;
  std::string room_name;
  Room *room = NULL;

  //Connection* info->conn = info->conn; //double check this

  while(1) {
    if (info->conn->receive(msg) == false) {
      if (info->conn->get_last_result() == Connection::INVALID_MSG) {
        info->conn->send(Message(TAG_ERR, "Invalid message"));
      }
      return;
    } else if (msg.tag == TAG_SENDALL) {
      if (room != NULL) {
        if(info->conn->send(Message(TAG_OK, "successful send")) == false) { //double check this
          return;
        }
        room->broadcast_message(username, trim_2(msg.data));
      } else {
        if (info->conn->send(Message(TAG_ERR, "must be in a room")) == false) {
          return;
        }
      }
    } else if (msg.tag == TAG_JOIN) {
      room_name = trim_2(msg.data);
      room = info->server->find_or_create_room(room_name);
      if (info->conn->send(Message(TAG_OK, "joined " + room_name)) == false) {
        return;
      }

    }
      else if (msg.tag == TAG_LEAVE) {
      if (room != NULL) {
        room = NULL;
        if (info->conn->send(Message(TAG_OK, "left " + room_name)) == false) {
            return;
        }
      } else {
        if (info->conn->send(Message(TAG_ERR, "not in a room")) == false) {
          return;
        }
      }
    } else if (msg.tag == TAG_QUIT) {
      info->conn->send(Message(TAG_OK, "quitting"));
      return;
    } else {
      if (info->conn->send(Message(TAG_ERR, "invalid message tag")) == false) {
        return;
      }
    }
  }
}


void chat_with_receiver(ClientInfo *info, std::string username) {
  //Connection* info->conn = info->conn;
  Room* room = NULL;
  std::string room_name;
  Message msg;

  if (info->conn->receive(msg) == false) {
    if (info->conn->get_last_result() == Connection::INVALID_MSG) {
      info->conn->send(Message(TAG_ERR, "Invalid Message"));
    }
    return;
  }
  if (msg.tag != TAG_JOIN) {
    info->conn->send(Message(TAG_ERR, "cannot send message without joining"));
    return;
  }
  room_name = trim_2(msg.data);
  if (info->conn->send(Message(TAG_OK, "joined " + room_name)) == false) {
    return;
  }

  room = info->server->find_or_create_room(room_name);
  User user(username);
  room->add_member(&user);

  while (1) {
    Message* dqMsg = NULL;
    dqMsg = user.mqueue.dequeue();
    if (dqMsg != NULL) {
      if (info->conn->send(*dqMsg) == false) {
        delete dqMsg;
        break;
      }
      delete dqMsg;
    }

  }
  room->remove_member(&user);

}


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

  Message temp = Message(TAG_OK, msg.data);
  if(info->conn->send(temp) == false) {
    return nullptr;
  }

  if (msg.tag.compare(TAG_SLOGIN) == 0) {
    chat_with_sender(info, msg.data);
  }
  else if (msg.tag.compare(TAG_RLOGIN) == 0) {
    chat_with_receiver(info, msg.data);
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

