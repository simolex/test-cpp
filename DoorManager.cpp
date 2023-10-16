#include <DoorGui.cpp>
#include <DoorEngine.cpp>

class DoorManager : public QObject
{
    Q_OBJECT
private:
    DoorState currentState;
    DoorGui* gui;
    DoorEngine* engine;
    void static getStateGui(DoorState state);
    void static getStateResponseEngine(DoorState state);
    void static setStateResponseEngine();
public:
    DoorManager();
    virtual ~DoorManager();

};

void DoorManager::getStateGui(DoorState state){
    currentState = state;
    engine->sendSetStateCommand(state);
}

DoorManager::DoorManager(){
    engine = new DoorEngine(getStateResponseEngine, getStateResponseEngine)
    gui = new DoorGui(getStateGui);
}

