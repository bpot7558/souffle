#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <memory>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event) {

    InnerNode* root = new InnerNode("demo(1, 2)", "");
    //std::unique_ptr<TreeNode> i1(new InnerNode("demo(1, 2)", ""));
    //std::unique_ptr<TreeNode> i2(new InnerNode("demo(1, 2)", ""));
    /*
    auto i1 = std::make_unique<InnerNode>("demo(1, 2)", "");
    auto i2 = std::make_unique<InnerNode>("demo(1, 2)", "");
    */
    std::unique_ptr<TreeNode> l1(new LeafNode("demo(1, 2)"));
    std::unique_ptr<TreeNode> l2(new LeafNode("demo(1, 2)"));
    std::unique_ptr<TreeNode> l3(new LeafNode("demo(1, 2)"));
    std::unique_ptr<TreeNode> l4(new LeafNode("demo(1, 2)"));

/*
    root->add_child(std::move(i1));
    root->add_child(std::move(i2));

    i1->add_child(std::move(l1));
    i1->add_child(std::move(l2));
*/

    root->add_child(std::move(l1));
    root->add_child(std::move(l2));
    root->add_child(std::move(l3));
    root->add_child(std::move(l4));

    root->place(50, 100);

    QPainter text(this);
    text.setFont(QFont("Times", 8, QFont::Bold));

    root->render(text);


}

