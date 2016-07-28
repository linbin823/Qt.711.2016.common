#include "example.h"
#include "ui_example.h"

example::example(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::example)
{
    ui->setupUi(this);


    model = new QStandardItemModel(this);
    model->setHorizontalHeaderItem(0, new QStandardItem( QString::fromLocal8Bit("名称")));
    model->setHorizontalHeaderItem(1, new QStandardItem( QString::fromLocal8Bit("IP")));
    model->setHorizontalHeaderItem(2, new QStandardItem( QString::fromLocal8Bit("状态")));
    model->setHorizontalHeaderItem(3, new QStandardItem( QString::fromLocal8Bit("最后错误")));
    model->setHorizontalHeaderItem(4, new QStandardItem( QString::fromLocal8Bit("优先级")));
    model->setVerticalHeaderItem(0, new QStandardItem(QString::fromLocal8Bit("本机")));

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setModel(model);

    manager = new tmEasyAPI();
//    comm = new QThread();
//    manager->moveToThread(comm);
//    comm->start();
    connect(manager,SIGNAL(msgPeersListChanged(int)),this,SLOT(updateModel(int)));
    connect(manager,SIGNAL(msgMasterPeerMessageUpdated()),this,SLOT(updateMasterString()));
    connect(manager,SIGNAL(msgDeviceChanged(QString)),this,SLOT(updateManager(QString) ) );

    connect(manager,SIGNAL(tmEasyTakeInEnableChange(bool)),this,SLOT(updateTakeInEnable(bool)));
    connect(manager,SIGNAL(tmEasyTakeOutEnableChange(bool)),this,SLOT(updateTakeOutEnable(bool)));
    connect(manager,SIGNAL(tmEasyAckEnableChange(bool)),this,SLOT(updateAckEnable(bool)));
    connect(manager,SIGNAL(tmEasyAckStringChange(QString)),this,SLOT(updateAckString(QString)));
    connect(manager,SIGNAL(tmEasyCancelEnableChange(bool)),this,SLOT(updateCancelEnable(bool)));

    tmPeer* s = manager->getSelfPeer();
    ui->peerName->setText( s->getName());

    int current = 0;
    foreach( QHostAddress t, QNetworkInterface::allAddresses()){
        if (t.protocol() == QAbstractSocket::IPv4Protocol && !t.isLoopback()){
            ui->peerIP->addItem(t.toString(),QVariant( t.toIPv4Address() ));
            if(s->getPeerIp() == t.toIPv4Address() ){
                ui->peerIP->setCurrentIndex(current);
            }
            current ++;
        }
    }
    ui->peerPriority->setValue( s->getPeerPriority() );
    ui->start->setEnabled(!manager->isStarted() );
    ui->stop->setEnabled( manager->isStarted() );
    //qDebug()<<"example::example";
    processor = new loadSaveProcessorXml(this);

    updateModel(0);
    updateManager();
}

example::~example()
{
    delete ui;
    manager->deleteLater();
}

void example::updateModel(int index){
    //qDebug()<<"example::updateModel 1#"<<index;
    if(index<0) return;
    //qDebug()<<"example::updateModel 2#";
    if(index==0) updateSelfPeer();

    //qDebug()<<"example::updateModel 3#";
    QModelIndex temp =ui->tableView->currentIndex();
    tmPeer* t = manager->getPeers().at(index);
    model->setItem(index,0,new QStandardItem(t->getName()));
    model->setItem(index,1,new QStandardItem(QHostAddress(t->getPeerIp()).toString()));
    model->setItem(index,2,new QStandardItem(t->getStateString()));
    model->setItem(index,3,new QStandardItem(t->getErrorStringList().join( QString::fromLocal8Bit("；") ) ) );
    model->setItem(index,4,new QStandardItem(QString::number(t->getPeerPriority())));

    ui->tableView->setCurrentIndex(temp);
}

void example::updateManager(QString name){
    ui->errorState_manager->setText(manager->getErrorStringList().join( QString::fromLocal8Bit("；") ) );
    ui->state_manager->setText(manager->getStateString());
    ui->start->setEnabled(!manager->isStarted() );
    ui->stop->setEnabled( manager->isStarted() );
}

void example::updateSelfPeer(){
    //qDebug()<<"example::updateSelfPeer 1#";
    ui->state->setText(manager->getSelfPeer()->getStateString());
    //qDebug()<<"example::updateSelfPeer 2#"<<manager->getSelfPeer()->getError();
    ui->errorState->setText(manager->getSelfPeer()->getErrorStringList().join( QString::fromLocal8Bit("；")  ) );
    //qDebug()<<"example::updateSelfPeer 3#";
    ui->masterPeerMsg->setReadOnly(!manager->getSelfPeer()->isWithToken() );
}

void example::on_setSelf_clicked()
{
    tmPeer s;
    s.update(ui->peerName->text(),tmPeer::stateDisable,0,ui->peerPriority->value(),ui->peerIP->currentData().toUInt());
    manager->setSelfPeer(&s);
}

void example::on_start_clicked()
{
    manager->start();
}

void example::on_stop_clicked()
{
    manager->stop();
}

void example::on_Exit_clicked()
{
    this->close();
}

void example::on_takeIn_clicked()
{
    int row = ui->tableView->currentIndex().row();
    manager->tmEasyTakeIn(row);
}

void example::on_takeOut_clicked()
{
    int row = ui->tableView->currentIndex().row();
    manager->tmEasyTakeOut(row);
}

void example::on_ack_clicked()
{
    manager->tmEasyAck();
}

void example::on_cancel_clicked()
{
    manager->tmEasyCancel();
}

void example::on_masterPeerMsg_editingFinished()
{
    manager->setMasterPeerMessage( ui->masterPeerMsg->text() );
}

void example::updateMasterString(){
    ui->masterPeerMsg->setText( manager->getMasterPeerMessage() );
}

void example::on_listRest_clicked()
{
    manager->tmEasyListClear();
}

void example::updateTakeInEnable(bool enable){
    ui->takeIn->setEnabled(enable);
}

void example::updateTakeOutEnable(bool enable){
    ui->takeOut->setEnabled(enable);
}

void example::updateAckEnable(bool enable){
    ui->ack->setEnabled(enable);
}

void example::updateAckString(QString str){
    ui->ack->setText(str);
}

void example::updateCancelEnable(bool enable){
    ui->cancel->setEnabled(enable);
}

void example::on_resetErrorManager_clicked()
{
    manager->resetAll();
}

void example::on_resetErrorSelf_clicked()
{
    manager->getSelfPeer()->resetAll();
}

void example::on_Save_clicked()
{
    if( processor->transactionStart() )
        return;
    //qDebug()<<"MainWindow::on_pushButton_clicked 1";
    processor->createNewInstance( QString("tokenManager"), QString::number( 1 ) );
    //qDebug()<<"MainWindow::on_pushButton_clicked 2";
    processor->moveToInstance(    QString("tokenManager"), QString::number( 1 ) );
    //qDebug()<<"MainWindow::on_pushButton_clicked 3";
    manager->save(processor);
    //qDebug()<<"MainWindow::on_pushButton_clicked 4";
    processor->moveBackToParent();
    //qDebug()<<"MainWindow::on_pushButton_clicked 5";
    processor->saveFile( QFileDialog::getSaveFileName(this, tr("Save File"),
                                                      QDir::currentPath(),
                                                      tr("xml Fille (*.xml)")) );
    //qDebug()<<"MainWindow::on_pushButton_clicked 6";
    processor->transactionEnd();
    //qDebug()<<"MainWindow::on_pushButton_clicked 7";
}

void example::on_Load_clicked()
{
    if( processor->transactionStart() )
        return;
    processor->loadFile( QFileDialog::getOpenFileName(this, tr("Load File"),
                                                      QDir::currentPath(),
                                                      tr("xml Fille (*.xml)"))  );
    //qDebug()<<"MainWindow::on_PB_load_clicked 1";
    processor->moveToInstance( QString("tokenManager"), QString::number( 1 ) );
    //qDebug()<<"MainWindow::on_PB_load_clicked 2";
    manager->load( processor );
    //qDebug()<<"MainWindow::on_PB_load_clicked 3";
    processor->moveBackToParent();
    //qDebug()<<"MainWindow::on_PB_load_clicked 4";
    processor->transactionEnd();
    //qDebug()<<"MainWindow::on_PB_load_clicked 5";
}
