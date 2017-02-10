#include "QSingleApp/qtsingleapplication.h"

int main(int argc, char *argv[])
{
    QtSingleApplication App(argc, argv);

    if (App.sendMessage("Wake up!"))
    {
        return 0;
    }

    App.setActivationWindow(&App.mainWindow);

    App.setOrganizationName("ARASHz4");
    App.setApplicationName("Local Messenger");
    App.setApplicationVersion("0.7.0");
    App.setOrganizationDomain("arashz4.com");
    App.setWindowIcon(QIcon(":/Icon/Icons/Icon.svg"));

    App.startApp();

    return App.exec();
}
