enum DoorState
{
    Closed,
    Opened
};

#ifndef DOORENGINE_H
#define DOORENGINE_H

class DoorEngine : public QObject
{
    Q_OBJECT
public:
    DoorEngine();
    DoorEngine(void (*callBackOnReceiveGetStateResponse)(DoorState state),
               void (*callBackOnReceiveSetStateResponse)());
    virtual ~DoorEngine();
    void sendGetStateCommand();
    void sendSetStateCommand(DoorState state);
signals:
    void getStateResponseReceived(DoorState state);
    void setStateResponseReceived();
};

#endif // DOORENGINE_H
