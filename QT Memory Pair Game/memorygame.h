#ifndef MEMORYGAME_H
#define MEMORYGAME_H

#include <QMainWindow>
#include <vector>

namespace Ui {
class memorygame;
}

class memorygame : public QMainWindow
{
    Q_OBJECT

public:
    explicit memorygame(QWidget *parent = nullptr);
    ~memorygame();

private:
    Ui::memorygame *ui;

private slots:
    void buttonClicked();
    void resetClicked();

};

#endif // MEMORYGAME_H
