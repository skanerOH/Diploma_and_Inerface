#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->comboBoxMethod->addItem("Use LZW");
    ui->comboBoxMethod->addItem("Use Huffman");
    ui->comboBoxMethod->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_ChooseFileButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,tr("Select file"),"",tr("CSV (*.csv);; Compressed CSV (*.ccsv)"));
    if (path.size()==0)
        return;
    this->filePath = path.toStdString();
    std::vector<std::string> words = RecordParser::csv_split(filePath, '/');
    ui->labelFilePath->setText(QString((words.end()-1)->c_str()));

    if (filePath.at(filePath.size()-4)=='.')
    {
        ui->ActButton->setText("Compress");
    }
    else if (filePath.at(filePath.size()-4)=='c')
    {
        ui->ActButton->setText("Decompress");
    }
}

void MainWindow::on_ActButton_clicked()
{
    if (filePath.size()==0)
    {
        QMessageBox msgBox;
        msgBox.setText("No file chosen!");
        msgBox.exec();
        return;
    }
    if (filePath.at(filePath.size()-4)=='.')
    {
        std::string outputPath = filePath;
        outputPath.insert(outputPath.size()-3, 1, 'c');

        FileCompressor comp = FileCompressor(filePath, outputPath, ui->comboBoxMethod->currentIndex()==0, ui->checkBoxDoParallel->isChecked());

        if (comp.IsOpened())
        {
            QMessageBox msgBox;
            try {
                comp.Compress();
                msgBox.setText("File was successfully compressed");
            } catch (const std::runtime_error &err) {
                msgBox.setText("Something went wrong!!!");
            }
            msgBox.exec();
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setText("Failed to open a file");
            msgBox.exec();
        }
        return;
    }
    if (filePath.at(filePath.size()-4)=='c')
    {
        std::string outputPath = filePath;
        outputPath.erase(outputPath.size()-4, 1);
        outputPath.insert(outputPath.size()-4, 1, 'D');

        FileDecompressor comp = FileDecompressor(filePath, outputPath, ui->checkBoxDoParallel->isChecked());

        if (comp.IsOpened())
        {
            QMessageBox msgBox;
            try {
                comp.Decompress();
                msgBox.setText("File was successfully decompressed");
            } catch (const std::runtime_error &err) {
                msgBox.setText("Something went wrong!!!");
            }
            msgBox.exec();
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setText("Failed to open a file");
            msgBox.exec();
        }
    }
}
