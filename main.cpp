#include <QtGui/QApplication>
#include "badgewindow.h"

#include "connection.h"

int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    if (!createConnection())
            return 1;
    BadgeWindow w;
    w.show();
    return a.exec();
}
