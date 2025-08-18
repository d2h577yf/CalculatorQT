#include "widget.h"
#include "./ui_widget.h"
#include <QKeyEvent>
#include <QMessageBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
      , ui(new Ui::Widget) {
    ui->setupUi(this);

    this->setWindowTitle("计算器");

    QIcon icon("/Users/why/Project/Calculate/left.png");
    ui->deleteButton->setIcon(icon);

    ui->equalButton->setStyleSheet("background-color:rgb(73,92,152)");

    connect(ui->resultLineEdit,SIGNAL(returnPressed()),this,SLOT(on_equalButton_clicked()));

}

Widget::~Widget() {
    delete ui;
}

void Widget::updataLineEdit(QString &expression) {
    ui->resultLineEdit->setText(expression);
}


void Widget::on_oneButton_clicked() {
    ui->resultLineEdit->insert("1");
}

void Widget::on_twoButton_clicked() {
    ui->resultLineEdit->insert("2");
}

void Widget::on_threeButton_clicked() {
    ui->resultLineEdit->insert("3");
}

void Widget::on_fourButton_clicked() {
    ui->resultLineEdit->insert("4");
}

void Widget::on_fiveButton_clicked() {
    ui->resultLineEdit->insert("5");
}

void Widget::on_sixButton_clicked() {
    ui->resultLineEdit->insert("6");
}

void Widget::on_sevenButton_clicked() {
    ui->resultLineEdit->insert("7");
}

void Widget::on_eightButton_clicked() {
    ui->resultLineEdit->insert("8");
}

void Widget::on_nineButton_clicked() {
    ui->resultLineEdit->insert("9");
}

void Widget::on_zeroButton_clicked() {
    ui->resultLineEdit->insert("0");
}

void Widget::on_mulButton_clicked() {
    ui->resultLineEdit->insert("*");
}

void Widget::on_divisionButton_clicked() {
    ui->resultLineEdit->insert("/");
}

void Widget::on_leftButton_clicked() {
    ui->resultLineEdit->insert("(");
}

void Widget::on_rightButton_clicked() {
    ui->resultLineEdit->insert(")");
}

void Widget::on_addButton_clicked() {
    ui->resultLineEdit->insert("+");
}

void Widget::on_minusButton_clicked() {
    ui->resultLineEdit->insert("-");
}

void Widget::on_dotButton_clicked() {
    ui->resultLineEdit->insert(".");
}

void Widget::on_clearButton_clicked() {
    ui->resultLineEdit->clear();
}

void Widget::on_deleteButton_clicked() {
    ui->resultLineEdit->backspace();
}



void Widget::on_equalButton_clicked() {
    QString expression = ui->resultLineEdit->text();
    if (judgeLegal(expression)) {
        calculate(expression);
        updataLineEdit(expression);
    } else {
        QMessageBox::information(this,"错误提示",expression);
    }
}
