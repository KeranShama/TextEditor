#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextBlock>

QString currentFileName;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit); // To remove space around text edit

    // Connect the textChanged signal to the autoCorrectText slot
    connect(ui->textEdit, &QTextEdit::textChanged, this, &MainWindow::autoCorrectText);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::autoCorrectText()
{
    // Disconnect the signal to avoid recursion
    disconnect(ui->textEdit, &QTextEdit::textChanged, this, &MainWindow::autoCorrectText);

    QTextDocument *document = ui->textEdit->document();
    QTextCursor cursor(document);

    cursor.beginEditBlock();

    bool capitalizeNext = true;

    // Iterate over each block of text in the document
    for (QTextBlock block = document->begin(); block.isValid(); block = block.next())
    {
        QTextCursor blockCursor(block);
        QString text = block.text();

        // Skip empty blocks
        if (text.isEmpty()) continue;

        // Correct the first letter of the sentence
        if (text.at(0).isLetter() && text.at(0).isLower())
        {
            blockCursor.setPosition(block.position());
            blockCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
            blockCursor.insertText(text.at(0).toUpper());
        }

        // Correct the first letter after a full stop and underline if not followed by a space
        for (int i = 0; i < text.length(); ++i)
        {
            QChar ch = text.at(i);

            if (ch == '.')
            {
                capitalizeNext = true;
                // Check for correct spacing after the full stop
                if (i + 1 < text.length() && text.at(i + 1) != ' ')
                {
                    // Find the start of the next word
                    int start = i + 1;
                    while (start < text.length() && text.at(start) == ' ') ++start;

                    // Find the end of the next word
                    int end = start;
                    while (end < text.length() && text.at(end).isLetter()) ++end;

                    if (start < end) // There's a word to format
                    {
                        blockCursor.setPosition(block.position() + start);
                        blockCursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, end - start);
                        QTextCharFormat format;
                        format.setUnderlineColor(Qt::red);
                        format.setUnderlineStyle(QTextCharFormat::SingleUnderline);
                        blockCursor.setCharFormat(format);
                    }
                }
                else if (i + 1 < text.length() && text.at(i + 1) == ' ')
                {
                    // Reset the format if the correct spacing is applied
                    int start = i + 1;
                    while (start < text.length() && text.at(start) == ' ') ++start; // Skip spaces

                    // Find the end of the next word
                    int end = start;
                    while (end < text.length() && text.at(end).isLetter()) ++end; // Find end of word

                    if (start < end) // There's a word to format
                    {
                        blockCursor.setPosition(block.position() + start);
                        blockCursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, end - start);
                        QTextCharFormat format;
                        format.setUnderlineStyle(QTextCharFormat::NoUnderline);
                        blockCursor.setCharFormat(format);
                    }
                }
            }
            else if (ch == ' ' && capitalizeNext)
            {
                continue; // Skip spaces after a full stop
            }
            else if (ch.isLetter() && capitalizeNext)
            {
                blockCursor.setPosition(block.position() + i);
                blockCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
                blockCursor.insertText(ch.toUpper());
                capitalizeNext = false;
            }
        }

        // Remove the underline if the text is corrected
        QTextCursor cleanupCursor(block);
        QTextCharFormat defaultFormat;
        defaultFormat.setUnderlineStyle(QTextCharFormat::NoUnderline);
        cleanupCursor.setCharFormat(defaultFormat);
    }

    cursor.endEditBlock();

    // Reconnect the signal after modifications
    connect(ui->textEdit, &QTextEdit::textChanged, this, &MainWindow::autoCorrectText);
}

void MainWindow::on_actionNew_triggered()
{
    ui->textEdit->clear();
    currentFileName = "";
}

void MainWindow::on_actionOpen_triggered()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Open File");
    QFile file(file_name);

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Error", "File could not be opened");
        return;
    }

    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();
    currentFileName = file_name;
}

QString MainWindow::generateFileNameWithTimestamp(const QString &baseName)
{
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    QString fileNameWithTimestamp = baseName + "_" + timestamp;
    return fileNameWithTimestamp;
}

void MainWindow::on_actionSave_As_triggered()
{
    QString baseName = QFileDialog::getSaveFileName(this, "Save As");

    if (baseName.isEmpty())
    {
        QMessageBox::information(this, "Save As", "Save As operation canceled.");
        return;
    }

    QString fileNameWithTimestamp = generateFileNameWithTimestamp(baseName);
    QFile file(fileNameWithTimestamp);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Save As", "Cannot save file: " + file.errorString());
        return;
    }

    QTextStream out(&file);
    out << ui->textEdit->toPlainText();
    file.close();

    currentFileName = fileNameWithTimestamp;

    QMessageBox::information(this, "Save As", "File saved successfully.");
}

void MainWindow::on_actionSave_triggered()
{
    if (currentFileName.isEmpty())
    {
        on_actionSave_As_triggered();
        return;
    }

    QFile file(currentFileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Save", "Cannot save file: " + file.errorString());
        return;
    }

    QTextStream out(&file);
    out << ui->textEdit->toPlainText();
    file.close();

    QMessageBox::information(this, "Save", "File saved successfully.");
}

void MainWindow::on_actionSelect_All_triggered()
{
    ui->textEdit->selectAll();
}

void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
}

void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}

void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}

void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}

void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}

void MainWindow::on_actionabout_triggered()
{
    QString about_text;
    about_text = "Author : Keran";
    about_text += "(C) Notepad (R)";
    QMessageBox::about(this, "Notepad", about_text);
}

void MainWindow::on_actionDelete_All_triggered()
{
    ui->textEdit->clear();
}

void MainWindow::on_actionClose_triggered()
{
    QString text = "You are going to exit!";
    QMessageBox::warning(this, "Exit", text);
    QApplication::exit(0);
}
