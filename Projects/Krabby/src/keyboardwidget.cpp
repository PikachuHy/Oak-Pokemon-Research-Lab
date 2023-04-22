#include "keyboardwidget.h"
#include <QtWidgets>
#include <QPainter>
#include <QKeyEvent>
#include <QFontDatabase>
const static int KEY_HEIGHT = 64;
const char* KEY_TAB = "<tab>";
const char* KEY_BACKSPACE = "<backspace>";
const char* KEY_CAPS_LOCK = "<Caps Lock>";
const char* KEY_ENTER = "<enter>";
const char* KEY_SHIFT = "<shift>";
const char* KEY_RIGHT_SHIFT = "<right-shift>";
const char* KEY_FN = "<fn>";
const char* KEY_CTRL = "<ctrl>";
const char* KEY_OPTION = "<option>";
const char* KEY_COMMAND = "<command>";
const char* KEY_RIGHT_COMMAND = "<right-command>";
const char* KEY_RIGHT_OPTION = "<right-option>";
const char* DEFAULT_KEYBOARD_FONT = "American Typewriter";
KeyboardWidget::KeyboardWidget(QWidget *parent)
    : QWidget{parent}
{
    if (QFontDatabase::hasFamily(DEFAULT_KEYBOARD_FONT)) {
        m_keyboardFont = DEFAULT_KEYBOARD_FONT;
        qDebug() << "use default font:" << m_keyboardFont;
    }
    else {
        auto font = QFontDatabase::systemFont(QFontDatabase::GeneralFont);
        m_keyboardFont = font.family();
        qDebug() << "use system font:" << m_keyboardFont;
    }
    initKeyboard();
    setFixedSize(keyboard_width, keyboard_height);
}

void KeyboardWidget::highlight(char ch)
{
    for(auto &[k, v]: m_btnMap) {
        v.highlight = false;
    }
    if (std::isupper(ch)) {
        ch = std::tolower(ch);
        std::string left = "qwertasdfgzxcvb";
        if (left.find(ch) == std::string::npos) {
            m_btnMap[KEY_SHIFT].highlight = true;
        }
        else {
            m_btnMap[KEY_RIGHT_SHIFT].highlight = true;
        }
    }
    else {
        std::string left = R"(!@#$%^)";
        std::string left_map = R"(123456)";
        std::string right = R"(&*()_+{}|:"<>?)";
        std::string right_map = R"(7890-=[]\;',./)";
        if (left.find(ch) != std::string::npos) {
            m_btnMap[KEY_RIGHT_SHIFT].highlight = true;
            ch = left_map[left.find(ch)];
        }
        else if (right.find(ch) != std::string::npos) {
            m_btnMap[KEY_SHIFT].highlight = true;
            ch = right_map[right.find(ch)];
        }
    }
    std::string s;
    s += ch;
    m_btnMap[s].highlight = true;
    update();
}


void KeyboardWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.save();
    for(const auto &[ch, key]: m_btnMap) {
        QRect rect(key.x, key.y, key.w, KEY_HEIGHT);
        if (key.highlight) {
            painter.fillRect(rect, QBrush(Qt::GlobalColor::darkGreen));
        }
        else {
            painter.drawRect(rect);
        }
        drawChar(painter, rect, ch);
    }
    painter.restore();
}

void KeyboardWidget::initKeyboard()
{
    auto f = [this](int x, int y, int w, std::string& line, auto cb) {
        for(auto ch: line) {
            std::string s;
            s += ch;
            m_btnMap[s] = KeyButton{
                .x = x,
                .y = y,
                .w = w,
                .highlight = false
            };
            x += KEY_HEIGHT + 10;
        }
        cb(x, y, w);
    };
    // init ` 1 2 ... 9 0 - + delete
    std::string line = "`1234567890-+";
    int x = 0;
    int y = 0;
    int w = KEY_HEIGHT;
    f(x, y, w, line, [this](int x, int y, int w) {
        m_btnMap[KEY_BACKSPACE] = KeyButton {
            .x = x,
            .y = y,
            .w = int(w * 1.5),
            .highlight = false,
        };
    });

    line = "qwertyuiop[]\\";
    keyboard_width = line.size() * (KEY_HEIGHT + 10) + 1.5 * KEY_HEIGHT;
    qDebug() << "max width: " << keyboard_width;
    y += KEY_HEIGHT + 10;
    m_btnMap[KEY_TAB] = KeyButton {
        .x = x,
        .y = y,
        .w = int(w * 1.5),
        .highlight = false
    };
    f(x + KEY_HEIGHT * 1.5 + 10, y, w, line, [](int x, int y, int w){
        // do nothing
        qDebug() << x;
    });
    line = "asdfghjkl;'";
    y += KEY_HEIGHT + 10;
    m_btnMap[KEY_CAPS_LOCK] = KeyButton {
        .x = x,
        .y = y,
        .w = w * 2 - 10,
        .highlight = false
    };
    f(x + KEY_HEIGHT * 2, y, w, line, [this](int x, int y, int w){
        m_btnMap[KEY_ENTER] = KeyButton {
            .x = x,
            .y = y,
            .w = keyboard_width - x,
            .highlight = false,
        };
    });
    line = "zxcvbnm,./";
    y += KEY_HEIGHT + 10;
    m_btnMap[KEY_SHIFT] = KeyButton {
        .x = x,
        .y = y,
        .w = int(w * 2.7),
        .highlight = false
    };
    f(x + KEY_HEIGHT * 2.7 + 10, y, w, line, [this](int x, int y, int w){
        m_btnMap[KEY_RIGHT_SHIFT] = KeyButton {
            .x = x,
            .y = y,
            .w = keyboard_width - x,
            .highlight = false,
        };
    });
    y += KEY_HEIGHT + 10;
    std::vector<std::string> l;
    l.emplace_back(KEY_FN);
    l.emplace_back(KEY_CTRL);
    l.emplace_back(KEY_OPTION);
    for(const auto& s: l) {
        m_btnMap[s] = KeyButton {
            .x = x,
            .y = y,
            .w = w,
            .highlight = false
        };
        x += KEY_HEIGHT + 10;
    }
    int cmd_width = KEY_HEIGHT * 1.7 - 10;
    m_btnMap[KEY_COMMAND] = KeyButton {
        .x = x,
        .y = y,
        .w = cmd_width,
        .highlight = false
    };
    x += cmd_width + 10;
    int space_width = 5 * w + 4 * 10;
    m_btnMap[" "] = KeyButton {
        .x = x,
        .y = y,
        .w = space_width,
        .highlight = false
    };
    x += space_width + 10;
    m_btnMap[KEY_RIGHT_COMMAND] = KeyButton {
        .x = x,
        .y = y,
        .w = cmd_width,
        .highlight = false
    };
    x += cmd_width + 10;
    m_btnMap[KEY_RIGHT_OPTION] = KeyButton {
        .x = x,
        .y = y,
        .w = w,
        .highlight = false
    };
    keyboard_height = y + KEY_HEIGHT + 10;
}

void KeyboardWidget::drawChar(QPainter &painter, QRect rect, std::string ch)
{
    painter.save();
    if (ch.size() == 1 && std::isalpha(ch[0])) {
        ch[0] = std::toupper(ch[0]);
    }
    if (ch.size() == 1 && ch[0] != ' ') {
        auto font = painter.font();
        font.setPixelSize(KEY_HEIGHT - 10);
        font.setFamily(m_keyboardFont);
        painter.setFont(font);
        painter.drawText(rect, Qt::AlignCenter, QString::fromStdString(ch));
    }
    else {
        if (ch == KEY_TAB) {
            painter.drawText(rect, Qt::AlignCenter, "Tab");
        }
        else if (ch == KEY_BACKSPACE) {
            painter.drawText(rect, Qt::AlignCenter, "Backspace");
        }
        else if (ch == KEY_CAPS_LOCK) {
            painter.drawText(rect, Qt::AlignCenter, "Caps Lock");
        }
        else if (ch == KEY_ENTER) {
            painter.drawText(rect, Qt::AlignCenter, "Enter");
        }
        else if (ch == KEY_SHIFT || ch == KEY_RIGHT_SHIFT) {
            painter.drawText(rect, Qt::AlignCenter, "Shift");
        }
        else if (ch == KEY_FN) {
            painter.drawText(rect, Qt::AlignCenter, "fn");
        }
        else if (ch == KEY_CTRL) {
            painter.drawText(rect, Qt::AlignCenter, "control");
        }
        else if (ch == KEY_OPTION || ch == KEY_RIGHT_OPTION) {
            painter.drawText(rect, Qt::AlignCenter, "option");
        }
        else if (ch == KEY_COMMAND || ch == KEY_RIGHT_COMMAND) {
            painter.drawText(rect, Qt::AlignCenter, "command");
        }
    }

    painter.restore();
}
