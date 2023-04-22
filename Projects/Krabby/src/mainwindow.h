#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class TypeWidget;
class ChooseArticleDialog;
class KeyboardWidget;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // QWidget interface
protected:
    void keyReleaseEvent(QKeyEvent *event) override;
private:
    TypeWidget* m_typeWidget;
    ChooseArticleDialog* m_chooseArticleDialog;
    KeyboardWidget* m_keyboardWidget;
};

#endif // MAINWINDOW_H
