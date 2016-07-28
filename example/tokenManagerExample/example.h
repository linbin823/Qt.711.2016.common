#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <QWidget>
#include <QStandardItemModel>
#include <QHostAddress>
#include <QThread>
#include <QFileDialog>
#include "tmtokenmanager.h"
#include "tmeasyapi.h"
#include "loadsaveprocessorxml.h"

namespace Ui {
class example;
}

class example : public QWidget
{
    Q_OBJECT

public:
    explicit example(QWidget *parent = 0);
    ~example();
    tmEasyAPI * manager;

private:
    Ui::example *ui;
    QStandardItemModel* model;
    QThread *comm;
    loadSaveProcessorXml* processor;
private slots:
    void updateModel(int index);
    void updateMasterString();
    void updateTakeInEnable(bool enable);
    void updateTakeOutEnable(bool enable);
    void updateAckEnable(bool enable);
    void updateAckString(QString str);
    void updateCancelEnable(bool enable);
    void updateSelfPeer();
    void updateManager(QString name = QString::null);

    void on_setSelf_clicked();
    void on_start_clicked();
    void on_stop_clicked();
    void on_Exit_clicked();
    void on_takeIn_clicked();
    void on_takeOut_clicked();
    void on_ack_clicked();
    void on_cancel_clicked();
    void on_masterPeerMsg_editingFinished();
    void on_listRest_clicked();
    void on_resetErrorManager_clicked();
    void on_resetErrorSelf_clicked();
    void on_Save_clicked();
    void on_Load_clicked();
};

#endif // EXAMPLE_H
