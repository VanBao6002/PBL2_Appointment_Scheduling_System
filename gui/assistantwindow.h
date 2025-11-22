#ifndef ASSISTANTWINDOW_H
#define ASSISTANTWINDOW_H

#include <QMainWindow>

namespace Ui {
class AssistantWindow;
}

class AssistantWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AssistantWindow(QWidget *parent = nullptr);
    ~AssistantWindow();

private:
    Ui::AssistantWindow *ui;
};

#endif // ASSISTANTWINDOW_H
