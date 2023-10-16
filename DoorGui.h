#ifndef DOORGUI_H
#define DOORGUI_H

class DoorGui : public QObject
{
    Q_OBJECT
public:
    DoorGui();
    DoorGui(void (*callBackOnStateChanged)(DoorState state));
    virtual ~DoorGui();
    DoorState currentState();
    void setFailString(const QString &str);
    void setFailString(const std::string &str);
signals:
    void stateChanged(DoorState state);
};
#endif // DOORGUI_H