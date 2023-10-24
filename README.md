# Тестовое задание по С++

## Необходимо, не внося изменения в механизм ворот и пользовательскую форму, реализовать управление воротами так, чтобы отображаемое на форме состояние соответствовало реальному состоянию ворот

<br/>

# Решение

-   [Блок-схема](Diagram.png) взаимодействия с устройством ворот
-

```cpp
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
    static int instanceCount;

    static DoorState currentState;
    static DoorState targetState;
    static DoorAction currentAction;

    static DoorGui *gui;
    static DoorEngine *engine;

    static void getStateGui(DoorState state);
    static void stateEngineIsGetting(DoorState state);
    static void stateEngineIsSetting();

    QTimer *moveTimer;
    QTimer *warningTimer;
    QTimer *retryTimer;

    int retryCounter;
    void applyTargetState();


public:
    DoorManager();
    virtual ~DoorManager();

private slots:
    void slotMoveTimout();
    void slotWarningTimout();
    void slotRetryTimout();
};

```
