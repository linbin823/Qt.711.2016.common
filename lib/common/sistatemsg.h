/**************************************************************************************************
 * standard interface
 * state & message
 * 对象状态及其消息的标准接口
 *
 * state,是许多独立的状态值
 *
 * setState 子类程序实现，调用该方法改变状态
 *
 * getStateString 子类程序实现，用于返回状态描述。
 *
 * getState 子类程序实现，用于返回状态。

 **************************************************************************************************/
#ifndef SISTATEMSG_H
#define SISTATEMSG_H

#include <QObject>

class siStateMsg
{
public:
    explicit siStateMsg(){}
    virtual QString getStateString(int lang=0) const = 0;
    virtual inline quint64 getState() const = 0;

    virtual inline void setState(quint64 newState) = 0;

private:
};

#endif // SISTATEMSG_H
