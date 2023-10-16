#include "DoorManager.h"


void DoorManager::getStateGui(DoorState state){

    newState = state;

    int retry = 5;
    QTime dieTime= QTime::currentTime();

    while(newState != currentState && retry>0){
        if(QTime::currentTime()>dieTime){
            engine->sendSetStateCommand(state);
            dieTime= QTime::currentTime().addMSecs(200);
            retry--;
        }
        
    }

    if (retry == 0) {
        gui->setFailString("Ворота не доступны. Проверьте свяэь с воротами!");
        return;
    }

    onlineEngine = false;
    moveTimer->start(30000);    
}

void DoorManager::stateEngineIsSetting(){
    currentState = newState;
}

void  DoorManager::stateEngineIsGetting(DoorState state){
    currentState = state;
    onlineEngine = true;
};


void DoorManager::slotMoveTimout(){
    int retry = 5;
    QTime dieTime= QTime::currentTime();

    while(!onlineEngine && retry>0){
        if(QTime::currentTime()>dieTime){
            this->engine->sendGetStateCommand();
            dieTime= QTime::currentTime().addMSecs(200);
            retry--;
        }
    }

    if (retry == 0) {
        gui->setFailString("Превышано время открытия/закрытия ворот");
        return;
    }
}

DoorManager::DoorManager(){
    engine = new DoorEngine(stateEngineIsGetting, stateEngineIsSetting);
    gui = new DoorGui(getStateGui);
    moveTimer = new Qtimer();
    connect(moveTimer, SIGNAL(timeout()), this, SLOT(slotMoveTimout()));
}

