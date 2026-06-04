#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    RelModels::instance()->getModel("rest_groups")->setEditable(true);

    modelOlaps = new RestTableModel("olaps_prog",this);
    ui->tableViewOlaps->setModel(modelOlaps);
    modelOlaps->select();

    modelUsers = new RestTableModel("rest_users",this);
    ui->tableViewUsers->setModel(modelUsers);
    mapperUser = new RestMapper(ui->tableViewUsers,this);
    mapperUser->addMapping(ui->lineEditUser,"username");
    mapperUser->addEmptyLock(ui->tableViewGroup);
    mapperUser->addEmptyLock(ui->pushButtonPass);

    ui->horizontalLayoutMapUser->insertWidget(0,mapperUser);

    modelUserGroup = new RestTableModel("rest_user_group",this);
    ui->tableViewGroup->setModel(modelUserGroup);

    modelRels = new RestTableModel("rest_rels",this);
    modelRels->select();
    ui->tableViewRel->setModel(modelRels);

    modelTblData = new RestTableModel("rest_tables_data",this);
    modelTblData->setDefaultValue("checkable",false);
    modelTblData->setDefaultValue("editable",true);
    ui->tableViewData->setModel(modelTblData);

    modelTbl = new RestTableModel("rest_tables",this);
    ui->tableViewTable->setModel(modelTbl);

    mapper = new RestMapper(ui->tableViewTable,this);
    mapper->addMapping(ui->lineEditNam,"nam");
    mapper->addMapping(ui->lineEditTbl,"tablename");
    mapper->addMapping(ui->lineEditSort,"sort");

    ui->horizontalLayoutMap->insertWidget(0,mapper);

    connect(mapper,SIGNAL(currentIndexChanged(int)),this,SLOT(updData(int)));
    connect(modelRels,SIGNAL(sigUpd()),RelModels::instance()->getModel("rest_rels"),SLOT(refresh()));
    connect(modelTbl,SIGNAL(sigUpd()),RelModels::instance()->getModel("rest_tables"),SLOT(refresh()));
    connect(ui->action_autorest_api,SIGNAL(triggered(bool)),this,SLOT(updAutorest()));
    connect(mapperUser,SIGNAL(currentIndexChanged(int)),this,SLOT(updGroup(int)));
    connect(ui->pushButtonPass,SIGNAL(clicked(bool)),this,SLOT(createPassword()));
    connect(modelOlaps,SIGNAL(sigAboutToBeInsert()),this,SLOT(setDefaultProg()));

    modelTbl->select();
    modelUsers->select();

    loadSettings();
}

MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}

void MainWindow::loadSettings()
{
    QSettings settings("szsm", QApplication::applicationName());
    this->restoreState(settings.value("main_state").toByteArray());
    this->restoreGeometry(settings.value("main_geometry").toByteArray());
    this->ui->splitter->restoreState(settings.value("table_splitter_width").toByteArray());
}

void MainWindow::saveSettings()
{
    QSettings settings("szsm", QApplication::applicationName());
    settings.setValue("main_state", this->saveState());
    settings.setValue("main_geometry", this->saveGeometry());
    settings.setValue("table_splitter_width",ui->splitter->saveState());
}

void MainWindow::updData(int index)
{
    int id_tbl=mapper->modelData(index,"id").toInt();
    modelTblData->setFilter(modelTblData->tableName()+".id_table="+QString::number(id_tbl));
    modelTblData->setDefaultValue("id_table",id_tbl);
    modelTblData->select();
}

void MainWindow::updAutorest()
{
    QByteArray data;
    bool ok = RestConnection::instance()->sendSyncGet("api/autorest/upddata",data);
    if (ok){
        QMessageBox::information(this,tr("Обновлено успешно"),data,QMessageBox::Ok);
    }
}

void MainWindow::updGroup(int index)
{
    QVariant id_user=mapperUser->modelData(index,"id");
    modelUserGroup->setFilter(modelUserGroup->tableName()+".id_user="+id_user.toString());
    modelUserGroup->setDefaultValue("id_user",id_user);
    modelUserGroup->select();
}

void MainWindow::createPassword()
{
    bool ok;
    QString pass = QInputDialog::getText(this,tr("Введите пароль"),tr("Пароль:"),QLineEdit::Password,"",&ok);
    if (ok) {
        //qDebug() << "Пользователь ввел:" << pass;
        QJsonDocument doc;
        QJsonObject object
            {
                {"username", mapperUser->modelData(mapperUser->currentIndex(),"username").toString()},
                {"password", pass}
            };
        doc.setObject(object);
        QByteArray body=doc.toJson();
        QByteArray data;
        RestConnection::instance()->sendSyncRequest("api/users","PUT",body,data);
    }
}

void MainWindow::setDefaultProg()
{
    if (modelOlaps->rowCount()>0) {
        QString prog = modelOlaps->getModelData(modelOlaps->rowCount()-1,"prog").toString();
        modelOlaps->setDefaultValue("prog",prog);
    }
}
