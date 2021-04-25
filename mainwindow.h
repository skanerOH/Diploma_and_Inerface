#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QFileDialog"
#include "FileCompressor.h"
#include "FileDecompressor.h"
#include "RecordParser.h"
#include "QMessageBox"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ChooseFileButton_clicked();

    void on_ActButton_clicked();

private:
    Ui::MainWindow *ui;
    std::string filePath = "";
};
#endif // MAINWINDOW_H
