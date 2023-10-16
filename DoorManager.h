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
    DoorState  currentState;
    DoorState  newState;
    bool  onlineEngine;
    QTimer* moveTimer;

    DoorGui* gui;
    DoorEngine* engine;
    void  getStateGui(DoorState state);
    void  stateEngineIsGetting(DoorState state);
    void  stateEngineIsSetting();

    void setStateEngine(DoorState state)
    void verifyChange()
public:
    DoorManager();
    virtual ~DoorManager();

private slots:
    void slotMoveTimout();

};

#endif // DOORMANAGER_H