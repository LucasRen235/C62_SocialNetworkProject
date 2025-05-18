#ifndef SOCIALNETWORKWINDOW_H
#define SOCIALNETWORKWINDOW_H

#include <QMainWindow>
#include "network.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SocialNetworkWindow; }
QT_END_NAMESPACE

class SocialNetworkWindow : public QMainWindow
{
    Q_OBJECT

public:
    // pre: none | post: sets up UI, connects signals, loads users/posts, shows login page
    explicit SocialNetworkWindow(QWidget *parent = nullptr);
    // pre: object constructed | post: releases UI resources
    ~SocialNetworkWindow();
private slots:
    // ==== LOGIN ====
    // pre: login page visible | post: attempts login; on success sets me_ and displayedUser_, switches to profile page; on failure shows warning
    void handleLogin();

    // ==== PROFILE ====
    // pre: profile page visible, row and col in range | post: sets displayedUser_ to friend at [row], refreshes profile UI
    void handelFriendTableClick(int row, int col);
    // pre: profile page visible, suggestions shown, row and col in range | post: sets displayedUser_ to suggestion at [row], refreshes profile UI
    void handleSuggestionTableClicked(int row, int col);
    // pre: profile page visible, displayedUser_ ≠ me_ | post: adds friendship between me_ and displayedUser_, writes users.txt, refreshes own profile
    void handleAddFriend();
    // pre: profile page visible, any state | post: resets displayedUser_ to me_, refreshes profile UI
    void handleBackToProfile();
    // pre: profile page visible, newPostEdit contains nonempty text | post: adds appropriate Post/IncomingPost to network, writes posts.txt, refreshes profile UI
    void handleAddPost();

private:
    Ui::SocialNetworkWindow *ui;
    Network network_;
    User* me_ = nullptr;
    User* displayedUser_ = nullptr;

    // pre: UI initialized | post: shows login page in stackedWidget
    void goLoginPage();
    // pre: UI initialized | post: calls refreshProfilePage() and shows profile page in stackedWidget
    void goProfilePage();
    // pre: me_ and displayedUser_ non-null | post: updates all profile widgets (title, tables, posts, buttons) to reflect displayedUser_’s data
    void refreshProfilePage();
};

#endif // SOCIALNETWORKWINDOW_H

