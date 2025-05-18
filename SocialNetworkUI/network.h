#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <string>
#include "user.h"
#include "post.h"

class Network {
public:
  // ==================== CONSTRUCTORS ====================
  // pre: none | post: creates network with default values
  Network();

  // destructor
  ~Network();


  // ==================== USER MANAGEMENT =================
  // ==== Mutators ====
  // pre: valid filename and formatted data | 
  // post: creates users from file data and adds them to this network; returns 0 if successful, -1 if not
  int readUsers(const char* fname);
  // pre: none | post: adds user to users_ vector
  void addUser(User*);

  // ==== Accessors ====
  // pre: object initialized | post: returns user ID by name or -1 if not found
  int getId(const std::string& name) const;
  // pre: valid filename and formatted data | post: writes this network into provided file
  int writeUsers(const char* fname) const;
  // pre: object initialized | post: returns pointer to user with input ID, nullptr if invalid ID
  User* getUser(int id) const;
  // pre: object initialized | post: returns number of users in users_
  int numUsers() const;


  // =============== RELATIONSHIP MANAGEMENT ===============
  // ==== Mutators ====
  // pre: object initialized | post: creates mutual connection between s1 and s2; returns 0 if successful, -1 if not
  int addConnection(const std::string& s1, const std::string& s2);
  // pre: object initialized | post: removes mutual connection between s1 and s2; returns 0 if successful, -1 if not
  int deleteConnection(const std::string& s1, const std::string& s2);

  // ==== Accessors ====


  // =============== User Processors ===============
  // pre: 0<=from, to<numUsers() | 
  // post: returns a vector of user IDs representing the shortest path from "from" to "to"(inclusive); 
    // if no path exists, returns empty vector {}
  std::vector<int> shortestPath(int from, int to) const; // uses BFS
  // pre: 0<=from<numUsers(), distance>=0 | 
  // post: returns a vector user IDs representing the shortest path to a user that is exactly "distance" 
    // away from "from". int &to is set to id of user found (if not found = -1)
  std::vector<int> distanceUser(int from, int& to, int distance) const; // uses BFS
  // pre:  0 ≤ who < numUsers() | 
  // post: returns list of IDs whose mutual friends count equals the highest "score". 
    // Set int& score to the count (0 if no suggestions) 
  std::vector<int> suggestFriends(int who, int& score) const; // uses BFS
  // pre: object initialized with int 0=<who<=users_.size | 
  // post: return nested vector. One vector for each groups=components in network. 
    // Each group is not connected with other group and has a path to each member of group.
	std::vector<std::vector<int>> groups() const; // uses DFS


  // =============== Post ===============
  // pre: object initialized | 
  // post: creates a new Post or IncomingPost with a unique messageId,adds it to the specified owner's page,
    // and records it in allPosts_
  void addPost(int ownerId, const std::string& message, int likes, bool isIncoming, const std::string& authorName, bool isPublic);
  // pre: object initialized |
  // post: returns the most recent howMany posts for ownerId formatted with toString(),
    // skipping private incoming posts if showOnlyPublic=true. Returns empt str if ownerId not found;
  std::string getPostsString(int ownerId, int howMany, bool showOnlyPublic) const;
  // pre: fname valid path to readable file in right format |
  // post: reads all posts from fname, adds them to this Network and update nextMessageId_.
    // returns the number of posts read, or -1 if file can't be opened
  int readPosts(const char* fname);
  // pre: fname valid path to a writable file |
  // post: writes all posts (in increasing msgID order) to fname in specified format,
    // returns number of posts written, or -1 if file acess issue
  int writePosts(const char* fname) const;

private:
  // =============== Variables ===============
  // users
  std::vector<User*> users_;
  // posts
  std::vector<Post*> allPosts_;
  int nextMessageId_;


  // =============== Functions ===============
  //helper to group()
  // pre: 0 ≤ userId < numUsers(), visited and component are sized properly |
  // post: marks all nodes reachable from userId, adding them to component
  void dfsVisit(int userId, std::vector<bool>& visited, std::vector<int>& component) const;


};


#endif 