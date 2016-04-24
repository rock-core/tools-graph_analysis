#include "QLauncher.hpp"

#include <QDebug>
#include <graph_analysis/gui/QShellOutput.hpp>

#include <base/Logging.hpp>

QLauncher::QLauncher(QObject* parent, QString binary, QString additionalEnvVar)
    : QObject(parent)
    , mProcess(this)
    , mBinaryName(binary)
    /* with "NULL" parent so that the widget will become a new window */
    , mpOutput(new QShellOutput(NULL))
{
    if(!additionalEnvVar.isEmpty())
    {
        addToEnv(additionalEnvVar);
    }

    connect(&mProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this,
            SLOT(launcher_execution_finished(int, QProcess::ExitStatus)));
    connect(&mProcess, SIGNAL(started()), this,
            SLOT(launcher_execution_started()));

    // no matter what kind of output is available: we'll process it
    connect(&mProcess, SIGNAL(readyReadStandardError()), this,
            SLOT(processDataFromProcess()));
    connect(&mProcess, SIGNAL(readyReadStandardOutput()), this,
            SLOT(processDataFromProcess()));
}

QLauncher::~QLauncher()
{
    // at first delete the output window, set the pointer to NULL
    delete mpOutput;
    mpOutput = NULL;
    // so that we can safely terminate the running process -- any signal
    // arriving at this class now can check the null-pointer of the window to
    // check its validity
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

void QLauncher::start(QStringList arguments)
{
    if(mProcess.state() != QProcess::NotRunning)
    {
        LOG_ERROR_S << "cannot start, state is " << mProcess.state();
        return;
    }

    if(arguments.empty())
    {
        mProcess.start(mBinaryName);
    }
    else
    {
        mProcess.start(mBinaryName, arguments);
    }
    if(!mProcess.waitForStarted())
    {
        LOG_ERROR_S << "timeout while starting";
    }
    else
    {
        LOG_INFO_S << "process seems to have started";
    }
}

bool QLauncher::isRunning() const
{
    return (mProcess.state() != QProcess::NotRunning);
}

void QLauncher::stop()
{
    mProcess.terminate();
}

void QLauncher::writeToStdin(const QByteArray data)
{
    mProcess.write(data);
    mProcess.write("\n");
}

void QLauncher::launcher_execution_finished(int exitCode,
                                            QProcess::ExitStatus exitStatus)
{
    LOG_DEBUG_S << "process closed with exitCode " << mProcess.exitCode();
    emit finished();
    // the window might not be present
    if(mpOutput)
    {
        mpOutput->hide();
        mpOutput->clearText();
    }
}

void QLauncher::launcher_execution_started()
{
    LOG_INFO_S << "process started";
    emit started();
    mpOutput->show();
}

void QLauncher::processDataFromProcess()
{
    mpOutput->insertNewStderr(mProcess.readAllStandardError());
    mpOutput->insertNewStdout(mProcess.readAllStandardOutput());
}
