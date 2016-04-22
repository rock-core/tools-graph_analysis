#include "QShellOutput.hpp"

#include "ui_QShellOutput.h"

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
void QShellOutput::insertNewStdout(const QByteArray data)
{
    mpUi->plainTextEdit->appendPlainText(splitLine("out: ", data).join("\n"));
}

void QShellOutput::insertNewStderr(const QByteArray data)
{
    mpUi->plainTextEdit->appendPlainText(splitLine("err: ", data).join("\n"));
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

void QShellOutput::clearText()
{
    mpUi->plainTextEdit->appendPlainText("--------------------");
}
