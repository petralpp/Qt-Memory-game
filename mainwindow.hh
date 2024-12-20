/* Class MainWindow
 *
 * ----------
 * COMP.CS.110 SPRING 2021
 * ----------
 *
 * Luokka muistipelin graafiselle käyttöliittymälle.
 *
 * Muodostaa pelin käyttöliittymän, jonka widgeteistä
 * pääosa on luotu Qt Designerilla.
 * Sisältää tarvittavat slotit, yksityiset metodit ja muuttujat
 * pelin toteuttamiseksi. Pelin kuvaus ja ohjelman rakenteesta
 * on kerrottu main.cpp ja instructions.txt tiedostoissa.
 *
 * Nimi: Petra Paloniemi
 * E-Mail: petra.paloniemi@tuni.fi
 **/

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QPushButton>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    // Starts the game once the play button is clicked. Does all the
    // needed preparations, like disabling buttons, gathering player
    // information and creating the cards by calling other methods.
    void startGame();

    // Turns the card that was clicked. Does nothing if
    // the continue-button is active.
    void turnCard();

    // Continues the game after two cards have been turned.
    // Calls the pairs_found or pairs_not_found methods.
    void continueGame();

private:
    Ui::MainWindow *ui;

    // Constants for push buttons
    const int NARROW_BUTTON_WIDTH = 20;
    const int DEFAULT_BUTTON_HEIGTH = 30;

    // Used for dealing the cards in a correct formation
    int columns_ = 0;
    int rows_ = 0;

    // The index of the player in turn
    unsigned long int playerInTurn_ = 0;

    bool pairFound_ = false;

    QString cardAmount_ = "";

    // The cards and the letters that are assigned to them
    struct cardTypes_ {
        char letter_;
        QPushButton* card_;
    };

    // The players' names, number of collected pairs and
    // the letters of the collected pairs
    struct playerInfo_ {
        std::string name_;
        int pairs_;
        std::string cardLetters_;
    };

    std::vector<cardTypes_> cards_;  // All cards
    std::vector<cardTypes_> foundCards_;  // Discovered pairs
    std::vector<cardTypes_> turnedCards_;  // The two cards that were turned

    std::vector<playerInfo_> players_;  // Names/ids and pairs of the players'
    std::vector<std::string> playerIds_;  // Ids if names are not given


    // Adds combo boxes into the ui. The boxes ask for the number of
    // cards and players.
    void addComboBoxes();

    // Deals the correct amount of cards to the grid layout.
    void addCards(unsigned int& biggerFactor);

    // Calculates the correct number of rows and columns
    // so the cards are dealt in a correct formation.
    void calculateFactors();

    // Creates the correct amount of cards. The cards are push buttons,
    // and they're added into the cards_ vector as structs, with a
    // letter as their pair.
    void createCards();

    // Adds the correct number of players (2-4), gives them ids and
    // replaces the ids with names if any were given. The players are
    // added into the players_ vector.
    void addPlayers();

    // Keeps the turned cards deactivated, and increases the player's
    // number of gathered pairs.
    void pairsFound();

    // Updates the game situation displaying the pairs per player.
    void updateSituation();

    // Turns the cards back by removing the letters and changing their
    // colors, and reactivates them. Also changes the player in turn.
    void pairsNotFound();

    // Once all the pairs are found, compares the players'
    // numbers of gathered pairs and determines the number of winner(s).
    QString findWinner();

    // Splits the string containing given player names.
    std::vector<std::string> split(std::string names);

    // Checks if the input containing player names is in a correct form.
    // The input can't be empty, start with a space or contain anything
    // other than the letters a-z or A-Z, except the spaces between
    // the names.
    bool isInputCorrect(std::string input);
};
#endif // MAINWINDOW_HH
