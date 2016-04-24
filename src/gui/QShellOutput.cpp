#include "QShellOutput.hpp"

#include "ui_QShellOutput.h"

#include <QDateTime>

QShellOutput::QShellOutput(QWidget* parent)
    : QWidget(parent)
    , mpUi(new Ui::QShellOutput)
{
    mpUi->setupUi(this);
}
QShellOutput::~QShellOutput()
{
    delete mpUi;
}
void QShellOutput::appendNewStdout(const QByteArray data)
{
    appendColoredHtmlWithTimestamp(data, "Black");
}

void QShellOutput::appendNewStderr(const QByteArray data)
{
    appendColoredHtmlWithTimestamp(data, "Red");
}

void QShellOutput::clearText()
{
    appendColoredHtmlWithTimestamp("-------- process restarted -------",
                                   "Gray");
}

QStringList QShellOutput::splitLine(QString prefix, const QByteArray data)
{
    QString all = QString::fromUtf8(data.data(), data.size());
    if(all.isEmpty())
    {
        return QStringList();
    }
    QStringList lines = all.split("\n");
    if(lines.last().isEmpty())
    {
        // if there is a newline at the end
        lines.pop_back();
    }
    for(int i = 0; i < lines.size(); i++)
    {
        lines.replace(i, prefix + lines.at(i));
    }
    return lines;
}

void QShellOutput::appendColoredHtmlWithTimestamp(const QByteArray data,
                                                  const QString color)
{
    QDateTime now = QDateTime::currentDateTime();
    QString newText =
        splitLine(QString("<font color=\"%1\">[%2] ").arg(color).arg(
                      now.toString("HH:mm:ss-zzz")),
                  data).join("\n") +
        QString("</font>");
    mpUi->plainTextEdit->appendHtml(newText);
}
