#ifndef SHAREVIEW_H
#define SHAREVIEW_H

#include <QGroupBox>

namespace Ui {
class shareview;
}

class shareview : public QGroupBox
{
    Q_OBJECT

public:
    explicit shareview(QWidget *parent = nullptr);
    ~shareview();

private:
    Ui::shareview *ui;
};

#endif // SHAREVIEW_H
