/**************************************************************************************************
 * standard interface
 * load & save
 * 状态保存&读取的标准接口
 *
 **************************************************************************************************/
#ifndef SILOADSAVE_H
#define SILOADSAVE_H

#include <QObject>
#include "siLoadSaveProcessor.h"

class siLoadSave
{
public:
    explicit siLoadSave(){}
    //每个对象都要实现的存取接口
    //参数1：读写本实例所对应的读写处理器。
    virtual int save(siLoadSaveProcessor* processor)=0;
    virtual int load(siLoadSaveProcessor* processor)=0;
};

#endif // SILOADSAVE_H
