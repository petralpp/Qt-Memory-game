#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QtWidgets>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    addComboBoxes();

    connect(ui->playPushButton, &QPushButton::clicked, this, &MainWindow::startGame);

    ui->continuePushButton->setEnabled(false);
    
    connect(ui->continuePushButton, &QPushButton::clicked, this, &MainWindow::continueGame);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addComboBoxes()
{
    // Adds the different number of cards the user can choose from
    ui->cardComboBox->addItem("8");
    ui->cardComboBox->addItem("10");
    ui->cardComboBox->addItem("12");
    ui->cardComboBox->addItem("14");
    ui->cardComboBox->addItem("16");
    ui->cardComboBox->addItem("18");
    ui->cardComboBox->addItem("20");

    // Adds the number of players the user can choose from
    ui->playerComboBox->addItem("2");
    ui->playerComboBox->addItem("3");
    ui->playerComboBox->addItem("4");
}

void MainWindow::addCards(unsigned int& biggerFactor)
{
    createCards();

    unsigned int row = 0;
    unsigned int column = 0;

    unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distr(0, rows_ * columns_ - 1);
    distr(generator);

    // Dealing the cards in a random order
    shuffle(cards_.begin(), cards_.end(), generator);
    for ( cardTypes_ card : cards_ )
    {
        ui->cardGridLayout->addWidget(card.card_, row, column);
        column++;
        if ( column == biggerFactor )
        {
            row++;
            column = 0;
        }
    }
}

void MainWindow::calculateFactors()
{
    unsigned int smallerFactor = 0;
    unsigned int biggerFactor = 0;
    unsigned int product = 0;
    product = cardAmount_.toInt();

    for(unsigned int i = 1; i * i <= product; ++i)
    {
        if(product % i == 0)
        {
            smallerFactor = i;
        }
    }
    biggerFactor = product / smallerFactor;
    rows_ = smallerFactor;
    columns_ = biggerFactor;

    addCards(biggerFactor);
}

void MainWindow::createCards()
{
    QSize size(60, 80);
    std::string alphabets = "AABBCCDDEEFFGGHHIIJJ";

    int cards = cardAmount_.toInt();
    int letterIndex = 0;

    for ( int i = 0; i < cards; i++ )
    {
        QPushButton *button = new QPushButton(this);
        char letter = alphabets.at(letterIndex);
        QPalette pal = button->palette();
        pal.setColor(QPalette::Button, QColor(Qt::blue));
        button->setAutoFillBackground(true);
        button->setPalette(pal);
        button->update();
        button->setFixedSize(size);

        cardTypes_ card = {letter, button};
        cards_.push_back(card);
        connect(button, &QPushButton::clicked,this, &MainWindow::turnCard);
        letterIndex++;
    }
}

void MainWindow::addPlayers()
{
    int playerAmount = 0;
    QString playerNr = ui->playerComboBox->currentText();
    playerAmount = playerNr.toInt();

    playerIds_.push_back("Player1");
    playerIds_.push_back("Player2");
    playerIds_.push_back("Player3");
    playerIds_.push_back("Player4");

    // Adding the players to a vector with 0 pairs and letters
    for ( int i = 0; i < playerAmount; i++ )
    {
        playerInfo_ player = {playerIds_.at(i), 0, ""};
        players_.push_back(player);
    }

    // Changing their names (if the user entered any)
    QString playerNames = ui->playerNameLineEdit->text();
    std::string names = playerNames.toStdString();
    bool input_is_correct = isInputCorrect(names);

    if ( input_is_correct )
    {
        std::vector<std::string> nameList = split(names);
        unsigned int i = 0;
        for ( std::string name : nameList )
        {
            players_.at(i).name_ = name;
            i++;
            if ( i == players_.size() )
            {
                break;
            }
        }
    }
}

void MainWindow::startGame()
{
    cardAmount_ = ui->cardComboBox->currentText();
    calculateFactors();

    addPlayers();
    updateSituation();

    ui->playPushButton->setDisabled(true);
    ui->cardComboBox->setDisabled(true);
    ui->playerComboBox->setDisabled(true);
    ui->playerNameLineEdit->setDisabled(true);

    // Displays the first player as first in turn
    QString player = QString::fromStdString(players_.at(playerInTurn_).name_);
    ui->inTurnLabel->setText(player);
}

void MainWindow::turnCard()
{
    if (ui->continuePushButton->isEnabled())
    {
        return;
    }

    // Tracking the card that was clicked
    QPushButton* pressedButton = static_cast<QPushButton*>(sender());
    for ( cardTypes_& card : cards_ )
    {
        // "Turning" the card by changing it's color and adding
        // the corresponding letter
        if ( card.card_ == pressedButton )
        {
            QChar addedLetter = card.letter_;
            card.card_->setText(addedLetter);
            card.card_->setDisabled(true);
            QPalette pal = card.card_->palette();
            pal.setColor(QPalette::Button, QColor(Qt::white));
            card.card_->setAutoFillBackground(true);
            card.card_->setPalette(pal);
            card.card_->update();
            turnedCards_.push_back(card);
        }
    }
    if (turnedCards_.size() == 2)
    {
        ui->continuePushButton->setEnabled(true);

        if ( turnedCards_.at(0).letter_ != turnedCards_.at(1).letter_ )
        {
            pairFound_ = false;
        }
        else
        {
            pairFound_ = true;
        }
    }
}

void MainWindow::continueGame()
{
    // Disabling the button so the next cards can be turned
    ui->continuePushButton->setEnabled(false);
    if (not pairFound_)
    {
        pairsNotFound();
    }
    else
    {
        pairsFound();
    }
}


void MainWindow::pairsFound()
{
    foundCards_.push_back(turnedCards_.at(0));
    foundCards_.push_back(turnedCards_.at(1));

    players_.at(playerInTurn_).pairs_++;
    char letter = turnedCards_.at(0).letter_;
    players_.at(playerInTurn_).cardLetters_+=letter;

    turnedCards_.clear();
    updateSituation();
}

void MainWindow::updateSituation()
{
    QString info = "";
    for (playerInfo_ player : players_ )
    {
        QString name = QString::fromStdString(player.name_);
        QString pairs = QString::number(player.pairs_);
        QString letters = QString::fromStdString(player.cardLetters_);
        info.append(name);
        info.append(": ");
        info.append(pairs);
        info.append(" ");
        info.append(letters);
        info.append('\n');
    }

    if ( foundCards_.size() == cards_.size() )
    {
        QString result = findWinner();
        info.append(result);
    }
    ui->statsTextBrowser->setText(info);
}

void MainWindow::pairsNotFound()
{
    QString cleared = " ";
    turnedCards_.at(0).card_->setText(cleared);
    turnedCards_.at(1).card_->setText(cleared);
    turnedCards_.at(0).card_->setEnabled(true);
    turnedCards_.at(1).card_->setEnabled(true);

    QPalette pal1 = turnedCards_.at(0).card_->palette();
    pal1.setColor(QPalette::Button, QColor(Qt::blue));
    turnedCards_.at(0).card_->setAutoFillBackground(true);
    turnedCards_.at(0).card_->setPalette(pal1);

    QPalette pal2 = turnedCards_.at(1).card_->palette();
    pal2.setColor(QPalette::Button, QColor(Qt::blue));
    turnedCards_.at(1).card_->setAutoFillBackground(true);
    turnedCards_.at(1).card_->setPalette(pal2);

    turnedCards_.at(0).card_->update();
    turnedCards_.at(1).card_->update();
    turnedCards_.clear();

    playerInTurn_++;
    if ( playerInTurn_ == players_.size() )
    {
        playerInTurn_ = 0;
    }
    QString player = QString::fromStdString(players_.at(playerInTurn_).name_);
    ui->inTurnLabel->setText(player);
}

QString MainWindow::findWinner()
{
    playerInfo_ winner = players_.at(0);
    int howManyWinners = 1;
    for (unsigned int i = 1; i < players_.size(); i++)
    {
        if (players_.at(i).pairs_ > winner.pairs_)
        {
            winner = players_.at(i);
            howManyWinners = 1;
        }
        else if (players_.at(i).pairs_ == winner.pairs_)
        {
            howManyWinners++;
        }
    }
    QString result = "";
    result.append("Game over! ");
    if (howManyWinners > 1)
    {
        result.append("Tie of ");
        QString winnerAmount = QString::number(howManyWinners);
        result.append(winnerAmount);
        result.append(" players.");
    }
    else
    {
        result.append(QString::fromStdString(winner.name_));
        result.append(" has won with ");
        result.append(QString::number(winner.pairs_));
        result.append(" pairs.");
    }
    return result;
}

std::vector<std::string> MainWindow::split(std::string names)
{
    char separator = ' ';
    std::string::size_type left = 0;
    std::string::size_type right = 0;
    std::vector<std::string> playerNames;

    while (right != std::string::npos)
    {
        right = names.find(separator, left);
        std::string part = names.substr(left, right-left);

        if (part!= "")
        {
            playerNames.push_back(part);
        }
        left = right+1;
    }
    return playerNames;
}

bool MainWindow::isInputCorrect(std::string input)
{
    if ( input == "" or input.at(0) == ' ')
    {
        return false;
    }
    for ( char letter : input )
    {
        if ( letter == ' ')
        {
            continue;
        }
        if (not islower(letter) and not isupper(letter))
        {
            return false;
        }
    }
    return true;
}
