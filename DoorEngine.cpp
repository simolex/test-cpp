enum DoorState
{
    Closed,
    Opened
};

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
