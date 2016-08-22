#ifndef BASEDEVICE_H
#define BASEDEVICE_H

#include <QObject>
#include <QDebug>
#include "ierrmsg.h"
#include "istatemsg.h"
#include "iloadsave.h"
#include "iloadsaveprocessor.h"

class baseDevice: public QObject, public iErrMsg, public iStateMsg, public iLoadSave
{
    Q_OBJECT
public:

    baseDevice(QObject *parent = 0);
    //State
    virtual inline quint64 getState() const;

    //error
    virtual QStringList getErrorStringList(int lang=0) const;
    virtual bool getError(quint64 errorCode) const;
    virtual quint64 getError() const;

    //errorMask,对应位=1则不能被外部复位,（必须使用protected:int resetErrorState(quint64 errorCode)复位）
    //默认都可以被外部复位
    virtual quint64 getErrorMask() const;

    //name
    QString getName(void) const;

    //外部复位
    virtual void resetAll();//外部复位，全复位
    virtual void reset(quint64 resetCode);//外部复位，

    virtual int save(iLoadSaveProcessor* processor);
    virtual int load(iLoadSaveProcessor* processor);

    //state修改
    virtual inline void setState(quint64 newState);

    //error修改
    virtual int updateError(quint64 errorCode);
    virtual int setError(quint64 errorCode);
    virtual int resetError(quint64 resetCode);

    //errorMask修改
    virtual int setErrorMask(quint64 errorCode);

    //name修改
    void setName(QString newName);



signals:
    void msgStateChanged(quint64 state);
    void msgErrorSet    (quint64 errorCode);
    void msgErrorReset  (quint64 errorCode);
    void msgNameChanged(QString name);
    void msgDeviceChanged(QString name);

private:
    quint64 error;
    quint64 errorMask;
    quint64 state;
    QString name;
};

#endif // BASEDEVICE_H
