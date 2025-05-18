#ifndef POST_H
#define POST_H

#include<string>

class Post {
public:
	// ==================== CONSTRUCTORS ====================
	// pre: none | post: creates Post with default values
	Post();
	// pre: all int inputs >= 0 | post: creates Post with values passed in
	Post(int messageId, int ownerId, const std::string& message, int likes);

	// === virtuals === so sublcasses can overide
	// destructor for clean up
	virtual ~Post() = default; 


	// ==================== MUTATORS ====================


	// ==================== ACCESSORS ====================
	// pre: object initialized | post: returns int messageId_
	int getMessageId() const;
	// pre: object initialized | post: returns int ownerId_
	int getOwnerId() const;
	// pre: object initialized | post: returns string message_
	const std::string& getMessage() const;
	// pre: object initialized | post: returns likes_
	int getLikes() const;

	// === virtuals === 
	// pre: object initialized | post: returns empty string (owner is always author for base Post)
	virtual std::string getAuthor() const;
	// pre: object initialized | post: returns true (base Post always public)
	virtual bool getIsPublic() const;
	// pre: object initialized | post: "returns (message_) liked by (likes_) people" inside () replaced by var avlue
	virtual std::string toString() const;

private:
	int messageId_;
	int ownerId_;
	std::string message_;
	int likes_;

};




class IncomingPost : public Post {
public:
	// ==================== CONSTRUCTORS ====================
	// pre: none | post: creates IncomingPost with default values
	IncomingPost();
	// pre: all int inputs >= 0 | post: creates IncomingPost with values passed in
	IncomingPost(int messageId, int ownerId, const std::string& message, int likes, bool isPublic, const std::string& author);


	// ==================== MUTATORS ====================


	// ==================== ACCESSORS ====================
	// pre: object initialized | post: returns author_ string
	std::string getAuthor() const override; 
	// pre: object initialized | post: returns isPublic_ bool
	bool getIsPublic() const override;
	// pre: object initialized | post: returns "<author_> wrote[ (private)]: <Post::toString()>"
	std::string toString() const override;

private:
	bool isPublic_;
	std::string author_;
};



#endif













