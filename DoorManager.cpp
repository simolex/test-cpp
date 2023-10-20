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
    
}

void DoorManager::stateEngineIsSetting()
{
    currentState = targetState;

    if(currentAction == DoorAction::SendingCommand){
        currentAction = DoorAction::Moving;
        retryTimer->stop();
        moveTimer->start(30000);
    }

    if(gui && currentAction == DoorAction::Moving){
        if(currentState == DoorState::Closed){
            gui->setFailString("Ворота закрываются...");
        }else{
            gui->setFailString("Ворота открываются...");
        }
    }
}

void DoorManager::stateEngineIsGetting(DoorState state)
{
    currentState = state;

    if(gui){
        if(currentState == DoorState::Closed){
            gui->setFailString("Ворота закрыты");
        }else{
            gui->setFailString("Ворота открыты");
        }
    }
    currentAction = DoorAction::Wait;

    this->applyTargetState(); // TODO check availible connecting
    retryTimer->stop();
    // onlineEngine = true;
};

// int DoorManager::verifyChange()
// {
//     int retry = 5;
//     QTime dieTime = QTime::currentTime();

//     while (!onlineEngine && retry > 0)
//     {
//         if (QTime::currentTime() > dieTime)
//         {
//             engine->sendGetStateCommand();
//             dieTime = QTime::currentTime().addMSecs(200);
//             retry--;
//         }
//     }

//     return retry;
// }



void DoorManager::slotWarningTimout()
{
    int retry = this->verifyChange();

    if(retry>0){
        warningTimer->stop();
    }
}

void DoorManager::applyTargetState(){

    if ( currentState == targetState)
    {
        return;
    }

    currentAction = DoorAction::SendingCommand;
    retryCounter = 5;
    retryTimer->start(200);
}


void DoorManager::slotRetryTimout(){

    if(currentState == targetState && currentAction == DoorAction::Wait){ //need print flag TODO
        if(currentState == DoorState::Closed){
            gui->setFailString("Ворота закрыты");
        }else{
            gui->setFailString("Ворота открыты");
        }
    }

    if(retryCounter>0){
        switch (currentAction)
        {
            case DoorAction::SendingCommand:
                engine->sendSetStateCommand(targetState);
                break;
            case DoorAction::Moving:
            case DoorAction::Unknown:
                engine->sendGetStateCommand();
                break;        
        }
        retryCounter--;
    }

    if(retryCounter==0){
        switch (currentAction)
        {
            case DoorAction::SendingCommand:
            case DoorAction::Moving:
            case DoorAction::Unknown:
                retryTimer->stop(); // need print warning State
                warningTimer->start(3000);
                break;        
        }
        
    }
}

void DoorManager::slotMoveTimout()
{
    retryCounter = 5;
    retryTimer->start(200);
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
    moveTimer->setSingleShot(true);
    connect(moveTimer, SIGNAL(timeout()), this, SLOT(slotMoveTimout()));

    warningTimer = new QTimer();
    connect(warningTimer, SIGNAL(timeout()), this, SLOT(slotWarningTimout()));

    retryTimer = new QTimer();
    connect(retryTimer, SIGNAL(timeout()), this, SLOT(slotRetryTimout()));


    currentAction = DoorAction::Unknown;
    targetState = gui->currentState();
    retryCounter = 5;
    retryTimer->start(200);
    gui->setFailString("Выполняется подключение к воротам...");
}

DoorManager::gui = nullptr;
DoorManager::engine = nullptr;