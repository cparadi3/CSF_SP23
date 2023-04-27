#include "guard.h"
#include "message.h"
#include "message_queue.h"
#include "user.h"
#include "room.h"

Room::Room(const std::string &room_name)
  : room_name(room_name) {
  // initialize the mutex
  pthread_mutex_init(&lock, 0);
}

Room::~Room() {
  //  destroy the mutex
  pthread_mutex_destroy(&lock);
}

void Room::add_member(User *user) {
  //  add User to the room
  //pthread_mutex_lock(&lock);
  Guard(this->lock);
  members.insert(user);

}

void Room::remove_member(User *user) {
  // remove User from the room
  Guard(this->lock);
  std::set<User*>::iterator it = members.find(user);
  members.erase(it);
}

void Room::broadcast_message(const std::string &sender_username, const std::string &message_text) {
  //send a message to every (receiver) User in the room
  Guard(this->lock);
  std::string messageContent = room_name + ":" + sender_username + ":" + message_text;
  for (std::set<User*>::iterator it = members.begin(); it != members.end(); it++) {
    Message* msg = new Message(TAG_DELIVERY, messageContent);
    (*it)->mqueue.enqueue(msg);
  }
}
