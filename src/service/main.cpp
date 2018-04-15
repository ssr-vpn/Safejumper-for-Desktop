#include "common.h"
#include "service.h"

#include <QCoreApplication>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#include "log.h"

const QString kAppName = "Safejumper";

void registerTypes()
{
    qRegisterMetaType<vpnState>("vpnState");
}

int main(int argc, char **argv)
{

#ifdef Q_OS_WIN
    // Initialize COM.
//    CoInitializeEx(0, COINIT_APARTMENTTHREADED);
#endif

    registerTypes();

    Log::instance()->enableLogging(true);

    QCoreApplication::setSetuidAllowed(true);
    QCoreApplication::setOrganizationName("proxy.sh");
    QCoreApplication::setApplicationName(kAppName);

    Service vpnservice(argc, argv);
    return vpnservice.exec();
}
