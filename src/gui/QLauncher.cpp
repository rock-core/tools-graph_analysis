#include "QLauncher.hpp"

#include <base/Logging.hpp>

QLauncher::QLauncher(QObject* parent, QString binary, QString additionalEnvVar)
    : QObject(parent)
    , mProcess(this)
    , mBinaryName(binary)
{
    if(!additionalEnvVar.isEmpty())
    {
        addToEnv(additionalEnvVar);
    }

    connect(&mProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this,
            SLOT(launcher_execution_finished(int, QProcess::ExitStatus)));
    connect(&mProcess, SIGNAL(started()), this,
            SLOT(launcher_execution_started()));
}

QLauncher::~QLauncher()
{
    if(mProcess.state() != QProcess::NotRunning)
    {
        // killing, just for good measure!
        LOG_ERROR_S << "have to kill the process";
        mProcess.terminate();
    }
}

void QLauncher::setBinary(QString binary)
{
    mBinaryName = binary;
}

void QLauncher::addToEnv(QString envVar)
{
    mProcess.setEnvironment(mProcess.environment() << envVar);
}

void QLauncher::start(QString argument)
{
    if(mProcess.state() != QProcess::NotRunning)
    {
        LOG_ERROR_S << "cannot start, state is " << mProcess.state();
        return;
    }

    mProcess.start(mBinaryName, QStringList() << argument);
    if(!mProcess.waitForStarted())
    {
        LOG_ERROR_S << "timeout while starting";
    }
    else
    {
        LOG_INFO_S << "process seems to have started";
    }
}

void QLauncher::stop()
{
    mProcess.terminate();
}

void QLauncher::writeToStdin(const QByteArray data)
{
    mProcess.write(QByteArray(data));
}

void QLauncher::launcher_execution_finished(int exitCode,
                                            QProcess::ExitStatus exitStatus)
{
    LOG_DEBUG_S << "process closed with exitCode " << mProcess.exitCode();
    emit finished();
}

void QLauncher::launcher_execution_started()
{
    LOG_INFO_S << "process started";
    emit started();
}
