/*
You are asked to simulate a chat room where multiple User objects can join.
  - Each User has a name.
  - Each User can send a message to the ChatRoom.
  - The ChatRoom keeps a list of all users using weak_ptr (not shared_ptr) so that when a user leaves, the chat room doesn’t prevent them from being destroyed.
  - When broadcasting a message:
    - The chat room must use .lock() on each weak_ptr to check if the user is still alive.
    - Expired users should be ignored.

Expected Behavior
  - Create a ChatRoom.
  - Add 2–3 users with shared_ptr<User>.
  - Send a message from one user → all active users should see it.
  - Destroy one user (goes out of scope).
  - Send another message → only the remaining active users should see it.
*/

/* g++ -std=c++14 -Wall test_weak.cpp */

#include <iostream>
#include <vector>
#include <memory>

class User {
  public:
    std::string user_name;

    User(std::string name) {
      user_name = name;
    }

    void receive(std::string msg) {
      std::cout << "User: " << user_name << ", received msg: " << msg << std::endl;
    }
};

class ChatRoom {
  public:

    std::vector<std::weak_ptr<User>> users;

    ChatRoom() {
      std::cout << "ChatRoom Created\n";
    }

    void join_user(std::shared_ptr<User> user) {
      users.push_back(user);
    }

    void broadcast(std::string msg) {
      for (auto it = users.begin(); it != users.end();)
      {
        if (auto u = it->lock())
        {
          u->receive(msg);
          it++;
        }
        else
        {
          it = users.erase(it);
        }
      }
    }
};

int main()
{
  ChatRoom room;

  std::shared_ptr<User> alice = std::make_shared<User>("Alice");
  auto bob = std::make_shared<User>("Bob");

  room.join_user(alice);
  room.join_user(bob);

  room.broadcast("Welcome to the chat");

  {
    auto charlie = std::make_shared<User>("Charlie");
    room.join_user(charlie);
    room.broadcast("One more added");
  }

  room.broadcast("Charlie left");

  return 0;
}
