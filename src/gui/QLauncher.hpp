#ifndef QLAUNCHER_H
#define QLAUNCHER_H

#include <QObject>
#include <QProcess>

/**
 * wrapper-class to hold something like the "launcher"
 *
 *
 */
class QLauncher : public QObject
{
    Q_OBJECT
public:
    QLauncher(QObject* parent, QString binary, QString additionalEnvVar = "");
    ~QLauncher();

    void setBinary(QString binary);
    void addToEnv(QString envVar);

signals:
    void started();
    void finished();

public slots:
    void start(QString argument = "");
    void stop();
    void writeToStdin(const QByteArray data);

private:
    QProcess mProcess;
    QString mBinaryName;

private slots:
    void launcher_execution_finished(int, QProcess::ExitStatus);
    void launcher_execution_started();
};

#endif /*QLAUNCHER_H*/
