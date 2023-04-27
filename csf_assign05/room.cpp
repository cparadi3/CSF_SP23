#include "guard.h"
#include "message.h"
#include "message_queue.h"
#include "user.h"
#include "room.h"

Room::Room(const std::string &room_name)
  : room_name(room_name) {
  // TODO: initialize the mutex
  pthread_mutex_init(&lock, 0);
}

Room::~Room() {
  // TODO: destroy the mutex
  pthread_mutex_destroy(&lock);
}

void Room::add_member(User *user) {
  // TODO: add User to the room
  //pthread_mutex_lock(&lock);
  Guard(this->lock);
  members.insert(user);
  //pthread_mutex_unlock(&lock);

}

void Room::remove_member(User *user) {
  // TODO: remove User from the room
  //pthread_mutex_lock(&lock);
  Guard(this->lock);
  std::set<User*>::iterator it = members.find(user);
  members.erase(it);
  //members.find(user);
  //members.erase(user);
  //pthread_mutex_unlock(&lock);
}

void Room::broadcast_message(const std::string &sender_username, const std::string &message_text) {
  // TODO: send a message to every (receiver) User in the room
  Guard(this->lock);
  std::string messageContent = room_name + ":" + sender_username + ":" + message_text;
  for (std::set<User*>::iterator it = members.begin(); it != members.end(); it++) {
    Message* msg = new Message(TAG_DELIVERY, messageContent);
    (*it)->mqueue.enqueue(msg);
  }
  //MessageQueue.enqueue(msg);
}
