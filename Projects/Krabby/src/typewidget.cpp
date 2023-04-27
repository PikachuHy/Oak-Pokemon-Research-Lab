#include "typewidget.h"
#include "finishdialog.h"
#include "setting.h"
#include <QtCore>
#include <QtWidgets>
#include <QMediaPlayer>
#include <QSoundEffect>
#include <QFontDatabase>
#include <chrono>
#include "database.h"
using namespace std::chrono_literals;
int TIME_INTERVAL = 1000;
const char* DEFAULT_ARTICLE_FONT = "Courier New";
TypeWidget::TypeWidget(Database *db, QWidget *parent)
    : m_database(db), QWidget(parent)
{
    m_eachLineCharCount = 80;
    m_fontWidth = 36;
    m_lineHeight = 36;
    m_cursorShow = true;
    m_pageNum = 0;
    m_eachPageLineCount = 8;
    m_start = false;
    m_finish = false;
    m_hasShowFinishDialog = false;
    m_time ++;

    m_typeErrorSoundEffect = new QSoundEffect(this);
    m_typeSoundEffect = new QSoundEffect(this);

    m_typeErrorSoundEffect->setSource(QUrl("qrc:/audio/error.wav"));
    m_typeSoundEffect->setSource(QUrl("qrc:/audio/type.wav"));

    m_setting = new Setting();
    setFixedSize(m_eachLineCharCount * 22 + 10, m_eachPageLineCount * m_lineHeight * 2);
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, [=]() {
        if (m_start) {
            m_time ++;
            emit updateTime(formatTime());
            emit updateSpeed(calSpeed());
        }
       this->update();
    });
    m_timer->setInterval(TIME_INTERVAL);
    m_timer->start();
    m_input = "";
    if (QFontDatabase::hasFamily(DEFAULT_ARTICLE_FONT)) {
        m_articleFont = DEFAULT_ARTICLE_FONT;
        qDebug() << "use default font:" << m_articleFont;
    }
    else {
        auto font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
        m_articleFont = font.family();
        qDebug() << "use system font:" << m_articleFont;
    }
    setFocus();
    QFile file(":/test.txt");
    file.open(QIODevice::ReadOnly);
    auto text = file.readAll();
    file.close();
    resetText(QString(text));
}

TypeWidget::~TypeWidget()
{

}

void TypeWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.save();
    auto font = painter.font();
    font.setPixelSize(m_fontWidth);
    font.setFamily(m_articleFont);
    painter.setFont(font);
    QFontMetrics fontMetrics(font);
    int x = 0;
    int y = 0;
    for(int i = 0; i<m_text.length(); i++) {
        int w = 0;
        if (m_input.length() <= i) {
            if (m_input.length() == i) {
                drawCursor(painter, x, y);
            }
            painter.setPen(m_setting->untypedCharColor());
            QString chStr = QString(m_text[i]);
            w = fontMetrics.horizontalAdvance(chStr);
            auto rect = QRect(x, y, w, m_lineHeight);
            painter.drawText(rect, Qt::AlignCenter, chStr);
        } else {
            if (m_text[i] == m_input[i]) {
                QString chStr = QString(m_text[i]);
                w = fontMetrics.horizontalAdvance(chStr);
                drawCorrectChar(painter, x, y, w, chStr);
            } else {
                QString chTarget = QString(m_text[i]);
                QString chInput = QString(m_input[i]);
                w = qMax(fontMetrics.horizontalAdvance(chTarget), fontMetrics.horizontalAdvance(chInput));
                drawWrongChar(painter, x, y, w, chTarget, chInput);
            }
        }
        if (x + 2 * w > width()) {
            x = 0;
            y += 2 * m_lineHeight;
        } else {
            x += w;
        }
    }

    painter.restore();
}

void TypeWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (event->modifiers() & ( Qt::ControlModifier)) {
        return;
    }
    if (m_finish) {
        return;
    }
    switch (event->key()) {
    case Qt::Key_Backspace:
        m_input.remove(m_input.length() -1, 1);
        if (m_inputTotal > 0) {
            m_inputTotal --;
        }
        requestHighlightCurrentKey();
        break;
    case Qt::Key_Escape:
        qDebug() << "ESC";
        break;
    default:
        auto key = event->text();
        if (key.isEmpty()) {
            // ignore
        } else {
            m_start = true;
            if (m_input.size() < m_text.size()) {
                m_input.append(key);
                m_inputTotal++;
            }
            playAudio();
            emit updateAccuracy(calAccuracy());
            int progress = 100 * m_inputTotal / m_textTotal;
            emit updateProgress(progress);
            emit updateSpeed(calSpeed());
            nextPageJudge();
            requestHighlightCurrentKey();
        }
        break;
    }
    update();
}

void TypeWidget::reset()
{
    resetInternal();
    QMessageBox::information(this, tr("krabby"), tr("Reset Success"));
}

void TypeWidget::resetText(QString text)
{
//    qDebug().noquote().nospace() << text;
    text = prehanlde(text);
    m_pageText.clear();
    m_textTotal = text.length();
    m_inputTotal = 0;
    for(int i = 0; i * m_eachPageLineCount * m_eachLineCharCount < text.length(); i++ ) {
        int index = i * m_eachPageLineCount * m_eachLineCharCount;
        int len = m_eachPageLineCount * m_eachLineCharCount;
        m_pageText.append(text.mid(index, len));
    }
    QTimer::singleShot(100ms, this, [this](){
        resetInternal();
    });

}

void TypeWidget::pause()
{
    m_start = false;
}



void TypeWidget::drawWrongChar(QPainter &painter, int x, int y, int w, QString targetCh, QString inputCh)
{
    painter.save();
    painter.setPen(m_setting->wrongCharColor());
    auto targetRect = QRect(x, y, w, m_lineHeight);
    painter.drawText(targetRect, Qt::AlignCenter, QString(targetCh));
    auto inputRect = QRect(x, y + m_lineHeight, w, m_lineHeight);
    painter.drawText(inputRect, Qt::AlignCenter, QString(inputCh));
    painter.restore();
}

void TypeWidget::drawCorrectChar(QPainter &painter, int x, int y, int w, QString chStr)
{
    painter.save();
    painter.setPen(m_setting->correctCharColor());
    auto targetRect = QRect(x, y, w, m_lineHeight);
    painter.drawText(targetRect, Qt::AlignCenter, chStr);
    auto inputRect = QRect(x, y + m_lineHeight, w, m_lineHeight);
    painter.drawText(inputRect, Qt::AlignCenter, chStr);
    painter.restore();
}

void TypeWidget::drawCursor(QPainter &painter, int x, int y)
{
    if (m_cursorShow) {
        auto line = QLineF(
                    x + 1,
                    y + m_lineHeight + 5,
                    x + 1,
                    y + m_lineHeight + m_lineHeight - 2 -5
                    );
        painter.drawLine(line);
    }
    m_cursorShow = !m_cursorShow;
}

void TypeWidget::nextPageJudge()
{
    if (m_input.length() == m_text.length()) {
        if (m_pageNum + 1 == m_pageText.length()) {
            QTimer::singleShot(100, this, &TypeWidget::finishTest);

            return;
        }
        m_pageNum++;
        m_prevWrongCount = countWrongCh();
        m_input.clear();
        m_text = m_pageText[m_pageNum];
    }
}

int TypeWidget::countWrongCh()
{
    int count = 0;
    for(int i=0;i < m_input.length();i++) {
        if (m_input[i] != m_text[i]) {
            count ++;
        }
    }
    return count;
}

void TypeWidget::playAudio()
{
    if (!m_setting->keyboardAudio()) {
        return;
    }
    if(m_input[m_input.length() - 1] == m_text[m_input.length()-1]) {
        m_typeSoundEffect->play();
    } else {
        m_typeErrorSoundEffect->play();
    }
}

int TypeWidget::calAccuracy()
{
    return 100 - 100 * (m_prevWrongCount + countWrongCh()) / (m_eachPageLineCount * m_eachLineCharCount * m_pageNum + m_input.length());
}

int TypeWidget::calSpeed()
{
    if (m_time == 0) {
        return 0;
    }
    return 1.0 * m_inputTotal / m_time * 60;
}

void TypeWidget::resetInternal()
{
    m_hasShowFinishDialog = false;
    m_input.clear();
    m_inputTotal = 0;
    m_time = 0;
    emit updateTime("00:00::00");
    emit updateProgress(0);
    emit updateAccuracy(100);
    emit updateSpeed(0);
    m_pageNum = 0;
    if (m_pageText.empty()) {
        QMessageBox::critical(this, tr("Krabby"), tr("Document Error!!!\nNo Content."));
        return ;
    }
    m_text = m_pageText[m_pageNum];
    if (m_finish) {
        m_finish = false;
        m_start = false;
        m_timer->start();
    }
    requestHighlightCurrentKey();
    update();
    setFocus();
}

QString TypeWidget::formatTime()
{
    return QDateTime::fromSecsSinceEpoch(m_time / (1000 / TIME_INTERVAL)).toUTC().toString("hh:mm:ss");
}

QString TypeWidget::prehanlde(QString text)
{
    text = text.replace("\n\n", "\n");
    text = text.replace(":\n", ":");
    text = text.replace("\n", ". ");
    return text;
}

void TypeWidget::requestHighlightCurrentKey()
{
    if (m_input.size() < m_text.size()) {
        auto qt_ch = m_text.at(m_input.size());
        auto ch = qt_ch.toLatin1();
        emit requestHighlightKey(ch);
    }
}

void TypeWidget::finishTest()
{
    if (m_hasShowFinishDialog) {
        return;
    }
    m_timer->stop();
    m_finish = true;
    int speed = calSpeed();
    int accuracy = calAccuracy();
    int score = speed * accuracy / 100;
    auto username = QDir().home().dirName();
    m_database->insert_score(Score(username.toStdString(), score));
    auto scores = m_database->fetch_scores();
    for(auto score: scores) {
        qDebug() << score.to_string();
    }
    FinishDialog d(
                score,
                formatTime(),
                m_inputTotal,
                speed,
                accuracy,
                this
                );
    d.init_score_chart(scores);
    m_hasShowFinishDialog = true;
    d.exec();
}

