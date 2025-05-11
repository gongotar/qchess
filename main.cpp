#include "board.h"
#include "controller.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Board board;
    Controller controller(&board);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.rootContext()->setContextProperty("board", &board);
    engine.rootContext()->setContextProperty("controller", &controller);

    engine.loadFromModule("qchess", "Main");

    return app.exec();
}
