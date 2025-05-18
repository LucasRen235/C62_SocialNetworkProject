#include "socialnetworkwindow.h"
#include "ui_socialnetworkwindow.h"

#include <QMessageBox>
#include <sstream>

SocialNetworkWindow::SocialNetworkWindow(QWidget *parent): QMainWindow(parent) , ui(new Ui::SocialNetworkWindow)
{
    ui->setupUi(this);

    // ==== Connections ====
    // login Buttons
    connect(ui->loginButton, &QPushButton::clicked, this, &SocialNetworkWindow::handleLogin);
    // profile buttons
    // Qt auto connect these two table cellclick because of slot function naming used
    connect(ui->friendTable,  &QTableWidget::cellClicked, this, &SocialNetworkWindow::handelFriendTableClick);
    connect(ui->suggestionTable, &QTableWidget::cellClicked, this, &SocialNetworkWindow::handleSuggestionTableClicked);
    connect(ui->addFriendButton, &QPushButton::clicked, this, &SocialNetworkWindow::handleAddFriend);
    connect(ui->backToProfileButton, &QPushButton::clicked, this, &SocialNetworkWindow::handleBackToProfile);
    connect(ui->addPostButton, &QPushButton::clicked, this, &SocialNetworkWindow::handleAddPost);


    // load data files
    if (network_.readUsers("users.txt") == -1) { QMessageBox::critical(this, "Error", "Failed to load users.txt"); }
    if (network_.readPosts("posts.txt") == -1) { QMessageBox::critical(this, "Error", "Failed to load posts.txt"); }

    // Page initializations
    goLoginPage();
}

SocialNetworkWindow::~SocialNetworkWindow()
{
    delete ui;
}

void SocialNetworkWindow::handleLogin() {
    QString name = ui -> loginNameEdit -> text().trimmed();
    int id = network_.getId(name.toStdString());
    if (id < 0) {QMessageBox::warning(this, "Login Failed", "user " + name + " not found. Try again."); return; }
    me_ = network_.getUser(id);
    displayedUser_ = me_;
    goProfilePage();
}

void SocialNetworkWindow::goLoginPage() {
    ui -> stackedWidget -> setCurrentWidget(ui -> loginPage);
}

void SocialNetworkWindow::goProfilePage() {
    refreshProfilePage();
    ui -> stackedWidget -> setCurrentWidget(ui -> profilePage);
}

void SocialNetworkWindow::refreshProfilePage() {
    // 1) Title
    if (displayedUser_ == me_) {
        ui->profileTitleLabel->setText("My Profile");
    } else {
        ui->profileTitleLabel->setText(QString::fromStdString(displayedUser_->getName()) + "'s Profile");
    }

    // 2) Friends table
    ui->friendTable->clearContents(); // remove any old items
    ui->friendTable->setColumnCount(2); // columsn: id, names
    ui->friendTable->setHorizontalHeaderLabels( QStringList() << "id" << "Friends" );
    const auto& friendIds = displayedUser_->getFriends();
    ui->friendTable->setRowCount(friendIds.size());
    int row = 0;
    for (int id : friendIds) {
        User* u = network_.getUser(id);
        ui->friendTable->setItem(row, 0, new QTableWidgetItem(QString::number(id)));
        ui->friendTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(u->getName())));
        ++row;
    }

    // 3) Suggestions (only for own profile)
    if (displayedUser_ == me_) {
        ui->addFriendButton->hide();
        ui->suggestionTable->show();
        ui->suggestionTable->clearContents();
        ui->suggestionTable->setColumnCount(2);
        ui->suggestionTable->setHorizontalHeaderLabels(QStringList() << "id" << "Friend Suggestions");
        int score = 0;
        auto suggIds = network_.suggestFriends(me_->getId(), score);
        ui->suggestionTable->setRowCount(suggIds.size());
        for (int i = 0; i < (int)suggIds.size(); ++i) {
            int sid = suggIds[i];
            User* u = network_.getUser(suggIds[i]);
            ui->suggestionTable->setItem(i, 0, new QTableWidgetItem(QString::number(sid)));
            ui->suggestionTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(u->getName())));
        }
    } else {
        ui->suggestionTable->hide();
    }

    // 4) Recent posts via QListWidget
    ui->postList->clear();                              // make sure your widget is really named postList
    bool publicOnly = (displayedUser_ != me_);
    std::string all = network_.getPostsString(displayedUser_->getId(), 5, publicOnly);

    std::istringstream in(all);
    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty())
            ui->postList->addItem(QString::fromStdString(line));
    }

    // 5) Buttons
    bool isMe = (displayedUser_ == me_);
    bool isFriend = me_->getFriends().count(displayedUser_->getId()) > 0;
    ui->addFriendButton->setVisible(!isMe && !isFriend);
    ui->backToProfileButton->setVisible(!isMe);
}

void SocialNetworkWindow::handelFriendTableClick(int row, int /*col*/) {
    int id = ui->friendTable->item(row, 0)->text().toInt();
    if (id < 0) return;

    displayedUser_ = network_.getUser(id);
    refreshProfilePage();
}

void SocialNetworkWindow::handleSuggestionTableClicked(int row, int /*col*/) {
    ui->addFriendButton->show();
    int id = ui->suggestionTable->item(row, 0)->text().toInt();
    if (id < 0) return;

    // Stay on your own profile and refresh the suggestions + friends list
    displayedUser_ = network_.getUser(id);
    refreshProfilePage();
}

void SocialNetworkWindow::handleBackToProfile() {
    displayedUser_ = me_;
    refreshProfilePage();
}

void SocialNetworkWindow::handleAddPost() {
    QString text = ui->newPostEdit->toPlainText().trimmed();
    if (text.isEmpty()) return;

    if (displayedUser_ == me_) {
        // posting on your own page ( regular post)
        network_.addPost(me_->getId(), text.toStdString(), 0, false, "", true);
    } else {
        // posting on someone else’s page (incoming post)
        network_.addPost(displayedUser_->getId(), text.toStdString(), 0, true, me_->getName(), true);
    }
    network_.writePosts("posts.txt");

    ui->newPostEdit->clear();
    refreshProfilePage();
}

void SocialNetworkWindow::handleAddFriend() {
    if (displayedUser_ == me_) return;  // shouldn’t happen, button is hidden

    network_.addConnection(me_->getName(), displayedUser_->getName());
    network_.writeUsers("users.txt");

    // After friending them, refresh so the button hides and the friend appears
    refreshProfilePage();
}
