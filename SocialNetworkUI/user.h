#ifndef USER_H
#define USER_H

#include <string>
#include <set>
#include <vector>

#include "post.h"

class User {
public:
  // ==================== CONSTRUCTORS ====================
  // pre: none | post: creates user with default values
  User();
  // pre: none | post: creates user with provided values
  User(int id, std::string name, int year, int zip, std::set<int> friends);


  // ==================== MUTATORS ====================
  // pre: object initialized | post: adds friend to friends_ set
  void addFriend(int id);
  // pre: object initialized | post: removes friend from friends_ set
  void deleteFriend(int id);


  // ==================== ACCESSORS ====================
  // pre: object initialized | post: returns user ID
  int getId() const;
  // pre: object initialized | post: returns user name
  std::string getName() const;
  // pre: object initialized | post: returns user's birth year
  int getYear() const;
  // pre: object initialized | post: returns user's zip code
  int getZip() const;
  // pre: object initialized | post: returns reference to friends_ set
  std::set<int>& getFriends();


  // ==================== POSTS  ====================
  // pre: p != nullptr | post: add the given Post* to the user's message vector
  void addPost(Post* post);
  // pre: object initialized | post: returns vector<Post*> messages_
  const std::vector<Post*>& getPosts() const;
  // pre: object initialized, howMany >=0 | 
  // post: returns the most recent howMany posts if they exist, formatted by toString(), 
    // sepearated by two new lines, skipping private IncomingPosts if showOnlyPublic=true
  std::string getPostsString(int howMany, bool showOnlyPublic) const;


private:
  // user
  int id_;
  std::string name_;
  int year_;
  int zip_;
  std::set<int> friends_;

  // post
  std::vector<Post*> messages_;
};

#endif