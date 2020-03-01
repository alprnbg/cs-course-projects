#include "memorygame.h"
#include "ui_memorygame.h"
#include <utility>
#include <numeric>
#include <QTimer>
#include <QTest>

enum gameStatus {FIRST_PICK, SECOND_PICK};
int remainingCards = 24;
std::vector< std::vector<char> > table;
gameStatus status = FIRST_PICK;
void resetTable();
QPushButton* firstPickedButton;
QPushButton *buttons[4][6];
int pairs = 0; // num of pairs
int tries = 0; // num of tries

// constructor
memorygame::memorygame(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::memorygame)
{
    ui->setupUi(this);

    resetTable(); // Resets the table

    for(unsigned int i = 0; i < 4; i++){ // 4 x 6 table
        for(unsigned int j = 0; j < 6; j++){
            QString buttonName = "pushButton_" + QString::number(i) + QString::number(j); // Finds the buttons according to their names and put them to their places
            QPushButton* button = this->findChild<QPushButton *>(buttonName);
            buttons[i][j] = button;
            button->setText("");
            connect(button, SIGNAL(released()), this, SLOT(buttonClicked()));  // Buttons are connected to buttonClicked() function
        }
    }

    QPushButton* resetButton = this->findChild<QPushButton *>("ResetButton"); // the reset button
    connect(resetButton, SIGNAL(released()), this, SLOT(resetClicked()));   // connect the reset button to its function
}

// if any button other then reset is clicked, this function is called
void memorygame::buttonClicked(){

    QPushButton* button = (QPushButton *)sender(); // finds the button which is clicked

    if(status == FIRST_PICK){ // if it is the first one that is clicked

        firstPickedButton = button;

        // finds the corresponding char that should be written inside the button
        button->setText(QChar(table[firstPickedButton->objectName().at(11).digitValue()][firstPickedButton->objectName().at(12).digitValue()]));

        status = SECOND_PICK; // change the status

    }else { // SECOND_PICK

        // char of the second button
        QChar secondPickedLetter = QChar(table[button->objectName().at(11).digitValue()][button->objectName().at(12).digitValue()]);


        // if the char at the FIRST_PICK and SECOND_PICK are the same and the two buttons are note same
        if(firstPickedButton->text() == secondPickedLetter && firstPickedButton != button){
            tries += 1;
            pairs += 1; // increment tries and pairs
            ui->lcdNumber->display(tries);
            ui->lcdNumber_2->display(pairs); // displar them
            button->setText(secondPickedLetter); // show the char of the card
            QTest::qWait(200); // wait
            firstPickedButton->setEnabled(0);
            button->setEnabled(0); // take the cards out of the game
            remainingCards -= 2;


        }else {

            tries +=1; // increment tries
            ui->lcdNumber->display(tries); // display it
            button->setText(secondPickedLetter); // show the char of the card
            QTest::qWait(200); // wait
            button->setText(""); // close the card
            firstPickedButton->setText(""); // close the first card


        }
        status = FIRST_PICK;
    }
}
// this function is called when reset is clicked
void memorygame::resetClicked(){
    // reset the table, pairs and tries
    resetTable();
    pairs = 0;
    tries = 0;
    ui->lcdNumber->display(tries); // update the lcds
    ui->lcdNumber_2->display(pairs);



    for(unsigned int i = 0; i < 4; i++){
        for(unsigned int j = 0; j < 6; j++){
            buttons[i][j]->setDisabled(0); // sets all buttons to their initial states
            buttons[i][j]->setText("");
        }
    }


}


// deconstructor
memorygame::~memorygame()
{
    delete ui;
}


// It returns a randomly choosen char from charVector and 2 randomly choosen integer from intVector.
std::pair<std::pair<int, int>, char> randomChar(std::vector<char>& charVector, std::vector<int>& intVector){
    srand(time(0));

    // Choose the first random integer
    int randCharIndex = rand()%charVector.size();
    char randChar = charVector[randCharIndex];
    // Remove the choosen integer from the vector so that it cannot be choosen again.
    charVector.erase(charVector.begin() + randCharIndex);

    // Choose the second random integer. Same steps with the first one.
    int randIntIndex1 = rand()%intVector.size();
    int randIndex1 = intVector[randIntIndex1];
    intVector.erase(intVector.begin() + randIntIndex1);

    // Choose the char and remove it from charVector.
    int randIntIndex2 = rand()%intVector.size();
    int randIndex2 = intVector[randIntIndex2];
    intVector.erase(intVector.begin() + randIntIndex2);

    std::pair<int, int> first(randIndex1, randIndex2);
    char second = randChar;

    return std::pair<std::pair<int, int>, char>(first, second);
}

// function that resets the table
void resetTable(){
    srand(time(0));
    table.clear();

    // Reset the game variables.
    remainingCards = 24;
    status = FIRST_PICK;

    // Vector that contains all English uppercase letters.
    std::vector<char> charVector(26);
    std::iota(charVector.begin(), charVector.end(), 'A');

    // Vector that contains integers from 0 to 24.
    std::vector<int> intVector;
    for (int i = 0; i < 24; i++) {
        intVector.push_back(i);
    }

    // Construct the table vector with '0'.
    for (unsigned int i = 0; i < 4; i++) {
        std::vector<char> row;
        table.push_back(row);
        for (unsigned int j = 0; j < 6; j++) {
            table[i].push_back('0');
        }
    }

    // In each iteration, a char and two distinct integers are obtained from randomChar function.
    // Then, the char is put into the table vector twice. Index is calculated from the random integers obtained from randomChar function.
    for (int i = 0; i < 12; i++) {
        char letter;
        std::pair<int, int> indexPair;
        std::pair<std::pair<int, int>, char> randoms = randomChar(charVector, intVector);
        letter = randoms.second;
        indexPair = randoms.first;
        int index1_row = indexPair.first/6;
        int index1_col = indexPair.first%6;
        int index2_row = indexPair.second/6;
        int index2_col = indexPair.second%6;
        table[index1_row][index1_col] = letter;
        table[index2_row][index2_col] = letter;
    }

}
