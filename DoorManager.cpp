#include "DoorManager.h"

void DoorManager::getStateGui(DoorState state)
{
    targetState = state;

    if(engine){  // проверка,  что экземпляр DoorManager существует
        this->applyTargetState();
    }
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

    if(engine){ // проверка,  что экземпляр DoorManager существует
        this->applyTargetState();
    }
    retryTimer->stop();

    if(warningTimer->isActive()){
        warningTimer->stop();
    }
    
};

void DoorManager::applyTargetState(){

    if ( currentState == targetState)
    {
        return;
    }

    currentAction = DoorAction::SendingCommand;
    retryCounter = 5;
    retryTimer->start(200);
}

void DoorManager::slotMoveTimout()
{
    retryCounter = 5;
    retryTimer->start(200);
}

void DoorManager::slotWarningTimout()
{
    currentAction = DoorAction::Unknown;
    targetState = gui->currentState();

    retryCounter = 5;
    retryTimer->start(200);
    gui->setFailString("Выполняется подключение к воротам...");
}

void DoorManager::slotRetryTimout(){

    if(currentState == targetState && currentAction == DoorAction::Wait){
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
                retryTimer->stop();
                warningTimer->start(3000);
                break;        
        }
        
    }
}

DoorManager::DoorManager()
{
    instanceCount++;

    if(!engine){
        engine = new DoorEngine(stateEngineIsGetting, stateEngineIsSetting);
    }

    if(!gui){
        gui = new DoorGui(getStateGui);
    }

    moveTimer = new QTimer();
    moveTimer->setSingleShot(true);
    connect(moveTimer, SIGNAL(timeout()), this, SLOT(slotMoveTimout()));

    retryTimer = new QTimer();
    connect(retryTimer, SIGNAL(timeout()), this, SLOT(slotRetryTimout()));

    warningTimer = new QTimer();
    connect(warningTimer, SIGNAL(timeout()), this, SLOT(slotWarningTimout()));

    currentAction = DoorAction::Unknown;
    targetState = gui->currentState();
    retryCounter = 5;
    retryTimer->start(200);
    gui->setFailString("Выполняется подключение к воротам...");
}

DoorManager::~DoorManager(){
    instanceCount--;

    if(instanceCount == 0){
        delete gui;
        gui = nullptr;

        delete engine;
        engine = nullptr; 
    }
}

DoorManager::instanceCount = 0;
DoorManager::gui = nullptr;
DoorManager::engine = nullptr;