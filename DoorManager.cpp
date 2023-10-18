#include "DoorManager.h"

void DoorManager::getStateGui(DoorState state)
{
    newState = state;

    if (newState == currentState)
    {
        return;
    }

    int retry = 5;
    QTime dieTime = QTime::currentTime();

    while (newState != currentState && retry > 0)
    {
        if (QTime::currentTime() > dieTime)
        {
            engine->sendSetStateCommand(state);
            dieTime = QTime::currentTime().addMSecs(200);
            retry--;
        }
    }

    if (retry == 0)
    {
        gui->setFailString("Ворота не доступны. Проверьте свяэь с воротами!");
        warningTimer->start(3000); // Ожидание выхода из аварийной ситуации (интервал 3 сек)
    }

    onlineEngine = false;
    moveTimer->start(30000);
}

void DoorManager::stateEngineIsSetting()
{
    currentState = newState;

    if(currentState == DoorState::Closed){
        gui->setFailString("Ворота закрываются...");
    }else{
        gui->setFailString("Ворота открываются...");
    }
}

void DoorManager::stateEngineIsGetting(DoorState state)
{
    currentState = state;
    if(currentState == DoorState::Closed){
        gui->setFailString("Ворота закрыты");
    }else{
        gui->setFailString("Ворота открыты");
    }
    onlineEngine = true;
};

int DoorManager::verifyChange()
{
    int retry = 5;
    QTime dieTime = QTime::currentTime();

    while (!onlineEngine && retry > 0)
    {
        if (QTime::currentTime() > dieTime)
        {
            this->engine->sendGetStateCommand();
            dieTime = QTime::currentTime().addMSecs(200);
            retry--;
        }
    }

    return retry;
}

void DoorManager::slotMoveTimout()
{
    int retry = this->verifyChange();
    moveTimer->stop();

    if (retry == 0)
    {
        gui->setFailString("Превышано время открытия/закрытия ворот");
        warningTimer->start(3000); // Ожидание выхода из аварийной ситуации (интервал 3 сек)
    }

}

void DoorManager::slotWarningTimout()
{
    int retry = this->verifyChange();

    if(retry>0){
        warningTimer->stop();
    }
}

DoorManager::DoorManager()
{
    if(!engine){
        engine = new DoorEngine(stateEngineIsGetting, stateEngineIsSetting);
    }
    if(!gui){
        gui = new DoorGui(getStateGui);
    }

    moveTimer = new QTimer();
    connect(moveTimer, SIGNAL(timeout()), this, SLOT(slotMoveTimout()));
    warningTimer = new QTimer();
    connect(warningTimer, SIGNAL(timeout()), this, SLOT(slotWarningTimout()));

    if (this->verifyChange() == 0)
    {
        gui->setFailString("Ворота не доступны. Проверьте свяэь с воротами!");
        warningTimer->start(3000); // Ожидание выхода из аварийной ситуации (интервал 3 сек)
    }
}

DoorManager::gui = nullptr;
DoorManager::engine = nullptr;