#ifndef QSHELLOUTPUT_H
#define QSHELLOUTPUT_H

#include <QWidget>

namespace Ui
{
class QShellOutput;
}

class QShellOutput : public QWidget
{
    Q_OBJECT
public:
    QShellOutput(QWidget* parent);
    ~QShellOutput();
    Ui::QShellOutput* mpUi;

    static QStringList splitLine(QString prefix, const QByteArray);

public slots:
    void insertNewStdout(const QByteArray data);
    void insertNewStderr(const QByteArray data);
    void clearText();
};

#endif /*QSHELLOUTPUT_H*/
