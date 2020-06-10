#include "mainwindow.h"
#include <QApplication>
#include "compressor.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    // PLAYGROUND //

    uchar c = 0;
    c = 1;
    c <<= 7;
    c >>= 6;

    //------------//

    /*
    Compressor comp;

    QFile input("files/donQuijote.txt");
    if(!input.open(QIODevice::ReadOnly)) return -1;

    comp.startCompression(input);
    */

    return a.exec();
}
