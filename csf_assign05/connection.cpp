#include <sstream>
#include <cctype>
#include <cassert>
#include <iostream>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

const std::string WHITESPACE = " \n\r\t\f\v";

std::string ltrim_1(const std::string &s) {
  size_t start = s.find_first_not_of(WHITESPACE);
  return (start == std::string::npos) ? "" : s.substr(start);
}
 
std::string rtrim_1(const std::string &s) {
  size_t end = s.find_last_not_of(WHITESPACE);
  return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}
 
std::string trim_1(const std::string &s) {
  return rtrim_1(ltrim_1(s));
}

Connection::Connection()
  : m_fd(-1)
  , m_last_result(SUCCESS) {
}

Connection::Connection(int fd)
  : m_fd(fd)
  , m_last_result(SUCCESS) {
  // TODO: call rio_readinitb to initialize the rio_t object
  rio_readinitb(&m_fdbuf, m_fd);
}

void Connection::connect(const std::string &hostname, int port) {
  // TODO: call open_clientfd to connect to the server
  std::string portNum = std::to_string(port);
  const char* hostName = hostname.c_str();
  m_fd = open_clientfd(hostName, portNum.c_str());
  // TODO: call rio_readinitb to initialize the rio_t object
  if (m_fd >= 0) {
    rio_readinitb(&m_fdbuf, m_fd);
  }
}

Connection::~Connection() {
  // TODO: close the socket if it is open
  close();
}

bool Connection::is_open() const {
  // TODO: return true if the connection is open
  if (m_fd >= 0) {
    return true;
  }
  return false;
}

void Connection::close() {
  // TODO: close the connection if it is open
  if(is_open()) {
    ::close(m_fd);
    m_fd = -1;
  }
  
}

bool Connection::send(const Message &msg) {
  // TODO: send a message
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
  std::string dataTrim = trim_1(msg.data);
  std::string message = msg.tag + ":" + msg.data + "\n";
  //Might need to add check for propery formatted room name and username
  ssize_t writeCheck = rio_writen(m_fd, message.c_str(), message.length());
  if(writeCheck == -1) {
    m_last_result = INVALID_MSG;
    return false;
  }
  else if ((size_t) writeCheck != message.length()) {
    m_last_result = INVALID_MSG;
    return false;  
  }
  else if (writeCheck == 0) {
    m_last_result = EOF_OR_ERROR;
    return false;
  }
  else {
    m_last_result = SUCCESS;
  }
  return true;
}

bool Connection::receive(Message &msg) {
  // TODO: receive a message, storing its tag and data in msg
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
  char buffer[msg.MAX_LEN + 1];
  ssize_t readCheck = rio_readlineb(&m_fdbuf, buffer, msg.MAX_LEN + 1);
  //fix this - what do you mean by trim?
  //std::stringstream ss(trim_1(buffer));
  std::stringstream ss(buffer);
  std::getline(ss, msg.tag, ':');
  std::getline(ss, msg.data);
  if(readCheck == -1) {
    m_last_result = INVALID_MSG;
    std::cerr << "negative readcheck" << std::endl; //delete later
    return false;
  
  }
  else if (readCheck == 0) {
    m_last_result = EOF_OR_ERROR;
    std::cerr << "zero readcheck" << std::endl; //delete later
    return false;
  }
  else {
    m_last_result = SUCCESS;
  }
  return true;
}


