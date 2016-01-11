#include "index.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    index w;
    w.show();
    return a.exec();
}
