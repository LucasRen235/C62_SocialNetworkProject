#include "post.h"

// Post
// ==================== CONSTRUCTORS ====================
Post::Post() : messageId_(0), ownerId_(0), message_(), likes_(0) {}
Post::Post(int messageId, int ownerId, const std::string& message, int likes)
	: messageId_(messageId), ownerId_(ownerId), message_(message), likes_(likes) {}



// ==================== MUTATORS ====================



// ==================== ACCESSORS ====================
int Post::getMessageId() const { return messageId_; }
int Post::getOwnerId() const { return ownerId_; }
const std::string& Post::getMessage() const { return message_; }
int Post::getLikes() const {return likes_; }

// === virtuals === 
std::string Post::getAuthor() const { return ""; }
bool Post::getIsPublic() const {return true; }
std::string Post::toString() const { return message_ + " Liked by " + std::to_string(likes_) + " people. "; }





// IncomingPost
// ==================== CONSTRUCTORS ====================
IncomingPost::IncomingPost() : Post(), isPublic_(true), author_() {} // Post() sets base defaults (0,0,"",0)
IncomingPost::IncomingPost(int messageId, int ownerId, const std::string& message, int likes, bool isPublic, const std::string& author)
	: Post(messageId, ownerId, message, likes), isPublic_(isPublic), author_(author) {}



// ==================== MUTATORS ====================



// ==================== ACCESSORS ====================
std::string IncomingPost::getAuthor() const { return author_; }
bool IncomingPost::getIsPublic() const { return isPublic_; }
std::string IncomingPost::toString() const {
	std::string out = author_ + " wrote";
	if (!isPublic_) { out += " (private)"; }
	out += ": ";
	out += Post::toString();
	return out;
}