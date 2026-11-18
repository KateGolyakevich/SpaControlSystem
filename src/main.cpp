#include <QApplication>
#include "ui/MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Устанавливаем красивое название приложения
    QApplication::setApplicationName("AuraSPA Control System");

    MainWindow main_window;
    main_window.show();

    return app.exec();
}