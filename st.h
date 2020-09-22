#ifndef ST_H
#define ST_H

#include <QDialog>

namespace Ui {
class ST;
}

class ST : public QDialog
{
    Q_OBJECT

public:
    explicit ST(QWidget *parent = nullptr);
    ~ST();

private:
    Ui::ST *ui;
};

#endif // ST_H
