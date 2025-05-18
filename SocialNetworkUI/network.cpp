#include "network.h"
#include <fstream>
#include <sstream>
#include <iostream> 
#include <queue>
#include <algorithm>

// ==================== CONSTRUCTORS ====================
Network::Network() : nextMessageId_(0) {}

Network::~Network() {
	for (auto post : allPosts_) { delete post; }
	for (auto user : users_) { delete user; }
}


// ==================== USER MANAGEMENT =================
// ==== Mutators ====
int Network::readUsers(const char* fname) {
	std::ifstream in(fname); // creates input file stream named in which opens fname(a char* to a file name)
	if (!in) { return -1; } // if can't open return -1
	int num_users;
	in >> num_users;
	std::string line;
	std::getline(in, line); // clears leftover \n new line 

	for (int i = 0; i < num_users; ++i) {
		int id, year, zip;
		std::string name, friends_line; 
		std::set<int> friends; 

		in >> id;
		in.ignore(); // ignore new line after id
		std::getline(in, name);
		if (!name.empty() && name[0] == '\t') {
		    name = name.substr(1); // removes leading tab
		} 
		in >> year >> zip; // auto skip whitespace/lines
		std::getline(in, line); 
		std::getline(in, friends_line);

		std::stringstream ss(friends_line); 
		int friend_id;
		while (ss >> friend_id) { friends.insert(friend_id); } // adds friend ids into friends untill friends_line empty

		addUser(new User(id, name, year, zip, friends));
	}

	return 0; 
}
void Network::addUser(User* user) { users_.push_back(user); }

// ==== Acessors ====
// iterates through each user in network if name matches return id
int Network::getId(const std::string& name) const { 
	for (User* user: users_) {
		if (user->getName() == name) { return user->getId(); }
	}
	return -1; // name not in network
}
int Network::writeUsers(const char* fname) const {
	std::ofstream out(fname); // make out file stream 
	if (!out) { return -1; } // if can't open return -1

	out << users_.size() << "\n";
	for (User* user : users_) { // output data for each user in users_
    out << user->getId() << "\n";
    out << "\t" << user->getName() << "\n";
    out << "\t" << user->getYear() << "\n";
    out << "\t" << user->getZip() << "\n";

    for (int f : user->getFriends()) { out << f << " "; } 
    out << "\n";
  }

	return users_.size(); 
}
User* Network::getUser(int id) const {
	if (id >= 0 && id < (int)users_.size()) { return users_[id]; }
	return nullptr;
}
int Network::numUsers() const { return users_.size(); }



// =============== RELATIONSHIP MANAGEMENT ===============
// ==== Mutators ====
int Network::addConnection(const std::string& s1, const std::string& s2) {
	int id1 = getId(s1), id2 = getId(s2); // get id num matched with name
	if (id1 == -1 || id2 == -1) { return -1; } // either name not found

	users_[id1]->addFriend(id2);
	users_[id2]->addFriend(id1);
	return 0; // sucess 
}
int Network::deleteConnection(const std::string& s1, const std::string& s2) {
	int id1 = getId(s1), id2 = getId(s2); // get id num matched with name
	if (id1 == -1 || id2 == -1) { return -1; } // either name not found

	users_[id1]->deleteFriend(id2);
	users_[id2]->deleteFriend(id1);
	return 0; // sucess 
}

// ==== Acessors ====




// =============== Processors ===============
std::vector<int> Network::shortestPath(int from, int to) const {
	int n = numUsers();
	if (from < 0 || to < 0 || from >= n || to >= n) { return {}; } // edge invalid cases
	if (from == to) { return {from}; } // from to same case

	std::vector<bool> visited(n, false); 
	std::vector<int> previous(n, -1);
	std::queue<int> q; 

	// initialize 
	visited[from] = true;
	q.push(from);

	// standard BFS algorithm
	while (!q.empty()) {
		int current = q.front();
		q.pop();
		if (current == to) { break; } // reached target
		for (int neighbor : getUser(current)->getFriends()) { // explore all neighbors
			if (!visited[neighbor]) {
				visited[neighbor] = true;
				previous[neighbor] = current;
				q.push(neighbor);
			}
		}
	}

	// no path found 
	if (!visited[to]) { return {}; }

	std::vector<int> path; 
	// reconstruct path from "previous" vector: start at "to" and walks back to "from" using prev vector
	for (int current = to; current != -1; current = previous[current]) { 
		path.push_back(current); 
	}

	std::reverse(path.begin(), path.end()); // from #include<algorithm> 
	return path; 
}
std::vector<int> Network::distanceUser(int from, int& to, int distance) const {
	int n = numUsers();
	if (from<0 || from>=n || distance<0) { to=-1; return {}; } // edge invalid cases
	if (distance == 0) { to=from; return {from}; } // dist=0 is user themself

	std::vector<bool> visited(n, false);
	std::vector<int>  previous(n, -1);
	std::vector<int> dist(n, -1);
	std::queue<int> q;

	// initialize
	visited[from] = true;
	dist[from] = 0;
	q.push(from);

	int foundId = -1;

	// BFS algorithm
	while (!q.empty()) {
		int current = q.front();
		q.pop();
		for (int neighbor : getUser(current)->getFriends()) {
			if (!visited[neighbor]) {
				visited[neighbor] = true;
				previous[neighbor] = current;
				dist[neighbor] = dist[current] + 1;
				// if neighbor is at right distance
				if (dist[neighbor] == distance) { foundId = neighbor; break; }
				q.push(neighbor); 
			}
		}
		if (foundId != -1) { break; }
	}
	// if nothing found
	if (foundId == -1) {
		to = -1;
		return {};
	}

	// reconstruct path from "previous" vector
	std::vector<int> path;
	for (int curr=foundId; curr!=-1; curr=previous[curr]) {
		path.push_back(curr);
	}
	std::reverse(path.begin(), path.end());

	to = foundId;
	return path;
}
std::vector<int> Network::suggestFriends(int who, int& score) const {
  int n = numUsers();
  if (who < 0 || who >= n) { score = 0; return {}; } // invalid user

  // get direct friends of `who`
  const auto& directFriends = getUser(who)->getFriends();

  // count mutual-friend occurrences for each candidate
  std::vector<int> commonCount(n, 0);
  for (int friendId : directFriends) { // for each direct friend
    for (int candidate : getUser(friendId)->getFriends()) { // for each friend of friend
      if (candidate == who) { continue; } // skip self
      if (directFriends.count(candidate)) { continue; } // skip existing friends
      commonCount[candidate]++; // increase candidate score
    }
  }

  // find the highest mutual-friend count
  int maxScore = 0;
  for (int id = 0; id < n; ++id) {
    if (commonCount[id] > maxScore) {
      maxScore = commonCount[id];
    }
  }

  // collect all users who have that maxScore
  std::vector<int> suggestions;
  if (maxScore > 0) {
    for (int id = 0; id < n; ++id) {
      if (commonCount[id] == maxScore) {
        suggestions.push_back(id);
      }
    }
  }

  score = maxScore;
	return suggestions;
}
void Network::dfsVisit(int userId, std::vector<bool>& visited, std::vector<int>& component) const { // recursive DFS to collect one component
  visited[userId] = true;
  component.push_back(userId);

  for (int neighbor : getUser(userId)->getFriends()) {
    if (!visited[neighbor]) {
      dfsVisit(neighbor, visited, component);
    }	
  }
}
std::vector<std::vector<int>> Network::groups() const {
  int n = numUsers();
  std::vector<bool> visited(n, false);
  std::vector<std::vector<int>> components;

  // for every user, if not yet visited, start a new DFS
  for (int userId = 0; userId < n; ++userId) {
    if (!visited[userId]) {
      std::vector<int> component;
      dfsVisit(userId, visited, component);
      components.push_back(component);
    }
  }

  return components;
}



// =============== Post ===============
void Network::addPost(int ownerId, const std::string& message, int likes, bool isIncoming, const std::string& authorName, bool isPublic) {
	User* user = getUser(ownerId);
	if (user == nullptr) return; // if user doesn't exist

	int messageId = nextMessageId_++; // new id for new message

	Post* post = nullptr;
	if (isIncoming) { post = new IncomingPost(messageId, ownerId, message, likes, isPublic, authorName); }
	else { post = new Post(messageId, ownerId, message, likes); }

	user -> addPost(post);
	allPosts_.push_back(post); // for global list creation order
}
std::string Network::getPostsString(int ownerId, int howMany, bool showOnlyPublic) const {
	User* user = getUser(ownerId);
	if (user == nullptr) { return ""; }
	return user ->  getPostsString(howMany, showOnlyPublic); 
}
int Network::readPosts(const char* fname) {
	std::ifstream in(fname);
	if (!in) { return -1; } // can't open fname
	int n; 
  in >> n;
  std::string line;
  std::getline(in, line); // consume end‑of‑line

  for (int i = 0; i < n; ++i) { // for every post
    int mid, owner, likes;
    bool isPublic;
    std::string msg, author;

    // 1) messageId
    in >> mid; 
    in.ignore();

    // 2) message text
    std::getline(in, line);
    line.erase(0, 1);
    msg = line;

    // 3) ownerId
    std::getline(in, line);
    line.erase(0, 1);
    owner = std::stoi(line);

    // 4) likes
    std::getline(in, line);
    line.erase(0, 1);
    likes = std::stoi(line);

    // 5) visibility
    std::getline(in, line);
    line.erase(0, 1);
    isPublic = (line != "private");

    // 6) author
    std::getline(in, line);
    line.erase(0, 1);
    author = line;

    // keep our ID counter up to date
    nextMessageId_ = std::max(nextMessageId_, mid + 1);

    Post* post = nullptr;
    if (author.empty()) { post = new Post(mid, owner, msg, likes); }
    else { post = new IncomingPost(mid, owner, msg, likes, isPublic, author); }

    allPosts_.push_back(post);

    if (User* user = getUser(owner)) { user->addPost(post); }
  }
  return n;
}
int Network::writePosts(const char* fname) const {
	std::ofstream out(fname);
  if (!out) return -1;

  out << allPosts_.size() << "\n";
  for (Post* p : allPosts_) {
    out << p->getMessageId() << "\n"
    << "\t" << p->getMessage() << "\n"
    << "\t" << p->getOwnerId() << "\n"
    << "\t" << p->getLikes() << "\n";

    std::string auth = p->getAuthor();
    if (auth.empty()) {
      // owner post: two lines each with a single tab
      out << "\t\n\t\n";
    } else {
      out << "\t"
          << (p->getIsPublic() ? "public" : "private")
          << "\n\t" << auth << "\n";
    }
  }
  return static_cast<int>(allPosts_.size());
}












