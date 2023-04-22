#ifndef KEYBOARDWIDGET_H
#define KEYBOARDWIDGET_H

#include <QWidget>
#include <vector>
#include <unordered_map>
class KeyboardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KeyboardWidget(QWidget *parent = nullptr);
public slots:
    void highlight(char ch);
signals:


    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
private:
    void initKeyboard();
    void drawChar(QPainter& painter, QRect rect, std::string ch);
    struct KeyButton {
        int x;
        int y;
        int w;
        bool highlight;
    };
    std::unordered_map<std::string, KeyButton> m_btnMap;
    int keyboard_width;
    int keyboard_height;
    QString m_keyboardFont;
};

#endif // KEYBOARDWIDGET_H
