#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QDateTime>
#include <QRegularExpression>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QTextBlock>

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_As_triggered();

    void on_actionSelect_All_triggered();

    void on_actionCut_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionRedo_triggered();

    void on_actionUndo_triggered();

    void on_actionZoom_In_triggered();

    void on_actionZoom_Out_triggered();

    void on_actionabout_triggered();

    void on_actionDelete_All_triggered();

    void on_actionClose_triggered();

    void on_actionSave_triggered();

private:
    Ui::MainWindow *ui;

    QString generateFileNameWithTimestamp(const QString &baseName);
    void autoCorrectText();

};
#endif // MAINWINDOW_H
