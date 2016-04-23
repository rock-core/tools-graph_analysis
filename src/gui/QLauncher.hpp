#ifndef QLAUNCHER_H
#define QLAUNCHER_H

#include <QObject>
#include <QProcess>

class QShellOutput;

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
    bool isRunning() const;

signals:
    void started();
    void finished();

public slots:
    void start(QStringList arguments = QStringList());
    void stop();
    void writeToStdin(const QByteArray data);

private:
    QProcess mProcess;
    QString mBinaryName;
    QShellOutput *mpOutput;

private slots:
    void launcher_execution_finished(int, QProcess::ExitStatus);
    void launcher_execution_started();
    void processDataFromProcess();
};

#endif /*QLAUNCHER_H*/
