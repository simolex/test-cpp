#ifndef DOORMANAGER_H
#define DOORMANAGER_H

enum DoorAction{
    SendingCommand,
    Moving,
    Wait,
    Unknown
}

#include <QObject>
#include <QTimer>
#include <QTime>
#include "DoorGui.h"
#include "DoorEngine.h"

class DoorManager : public QObject
{
    Q_OBJECT
private:
    static DoorState newState; //depricate

    static DoorState currentState;
    static DoorState targetState;
    static DoorAction currentAction;

    static bool onlineEngine;
    static DoorGui *gui;
    static DoorEngine *engine;

    static void getStateGui(DoorState state);
    static void stateEngineIsGetting(DoorState state);
    static void stateEngineIsSetting();

    QTimer *moveTimer;
    QTimer *warningTimer;
    QTimer *retryTimer;

    // void verifyChange();
    int retryCounter;
    void applyTargetState();
    // void sendTargetStateToEngine(DoorState state);


public:
    DoorManager();
    virtual ~DoorManager();

signals:

private slots:
    void slotMoveTimout();
    void slotWarningTimout();
    void slotRetryTimout();
};

#endif // DOORMANAGER_H