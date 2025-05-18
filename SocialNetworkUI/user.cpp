#include "user.h"


// ==================== CONSTRUCTORS ====================
User::User() : id_(-1), name_(""), year_(0), zip_(0), friends_() {}
User::User(int id, std::string name, int year, int zip, std::set<int> friends) 
	: id_(id), name_(name), year_(year), zip_(zip), friends_(friends) {}



// ==================== MUTATORS ====================
void User::addFriend(int id) { friends_.insert(id); }
void User::deleteFriend(int id) { friends_.erase(id); }



// ==================== ACCESSORS ====================
int User::getId() const { return id_; }
std::string User::getName() const { return name_; }
int User::getYear() const { return year_; }
int User::getZip() const { return zip_; }
std::set<int>& User::getFriends() { return friends_; }



// ==================== POSTS  ====================
void User::addPost(Post* post) { messages_.push_back(post); }
const std::vector<Post*>& User::getPosts() const { return messages_; }
std::string User::getPostsString(int howMany, bool showOnlyPublic) const {
	std::string out = "";
	int count = 0;
	int total = messages_.size();

	// iterate newest to oldest
	for (int i = total - 1; i >= 0 && count < howMany; --i) {
		Post* post = messages_[i];

		// if only public, skip private 
		if (showOnlyPublic && !post->getIsPublic()) {
			continue;
		}

		// append this post's string
		out += post->toString();
		++count;

		// add spacing if more to print
		if (count < howMany && i > 0) {
			out += "\n\n";
		}
	}
	return out;
}