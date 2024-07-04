#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class TextEditor; }
QT_END_NAMESPACE

class TextEditor : public QMainWindow
{
    Q_OBJECT

public:
    TextEditor(QWidget *parent = nullptr);
    ~TextEditor();

private:
    Ui::TextEditor *ui;
    void connectSignals();

    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_As_triggered();
    void on_actionCut_triggered();
    void on_actionCopy_triggered();
    void on_actionPaste_triggered();
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionAbout_triggered();
    void on_actionZoom_in_triggered();
    void on_actionZoom_out_triggered();

    void autoCorrectText();
    QString generateFileNameWithTimestamp(const QString &baseName);
};

#endif
