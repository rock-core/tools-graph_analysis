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

    /**
     * converts the ByteArray into an utf8-string, splits the string after
     * every new line into a list and prepends the given prefix for every line
     * (list-item).
     */
    static QStringList splitLine(QString prefix, const QByteArray data);

    /**
     * appends a new html-coloured text-paragraph onto the document.
     *
     * not that this automatically creates an "empty" line, as qt will
     * automatically create a paragraph upon "appending"... did not find out
     * how to prevent or undo this. unfortunate.
     */
    void appendColoredHtmlWithTimestamp(const QByteArray data,
                                        const QString color);

public slots:
    void appendNewStdout(const QByteArray data);
    void appendNewStderr(const QByteArray data);

    void clearText();
};

#endif /*QSHELLOUTPUT_H*/
