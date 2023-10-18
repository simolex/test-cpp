#ifndef DOORMANAGER_H
#define DOORMANAGER_H

#include <QObject>
#include <QTimer>
#include <QTime>
#include "DoorGui.h"
#include "DoorEngine.h"

class DoorManager : public QObject
{
    Q_OBJECT
private:
    static DoorState currentState;
    static DoorState newState;
    static bool onlineEngine;
    static DoorGui *gui;
    static DoorEngine *engine;

    static void getStateGui(DoorState state);
    static void stateEngineIsGetting(DoorState state);
    static void stateEngineIsSetting();

    QTimer *moveTimer;
    void setStateEngine(DoorState state);
    void verifyChange();

public:
    DoorManager();
    virtual ~DoorManager();

private slots:
    void slotMoveTimout();
};

#endif // DOORMANAGER_H