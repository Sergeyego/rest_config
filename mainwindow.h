#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QInputDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include "rest/resttablemodel.h"
#include "rest/restmapper.h"
#include "rest/relmodels.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    RestTableModel *modelTbl;
    RestTableModel *modelTblData;
    RestTableModel *modelRels;
    RestTableModel *modelUsers;
    RestTableModel *modelUserGroup;
    RestTableModel *modelOlaps;
    RestMapper *mapper;
    RestMapper *mapperUser;

    void loadSettings();
    void saveSettings();

private slots:
    void updData(int index);
    void updAutorest();
    void updGroup(int index);
    void createPassword();
    void setDefaultProg();
};
#endif // MAINWINDOW_H
