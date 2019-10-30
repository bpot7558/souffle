#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <string>
#include <memory>
#include <QString>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    virtual void paintEvent(QPaintEvent *event);

private:
    Ui::MainWindow *ui;
};

// Tree node copied from souffle

class TreeNode {
public:
    TreeNode(std::string t = "") : txt(std::move(t)) {}
    virtual ~TreeNode() = default;

    // get width
    uint32_t getWidth() const {
        return width;
    }

    // get height
    uint32_t getHeight() const {
        return height;
    }

    // place the node
    virtual void place(uint32_t xpos, uint32_t ypos) = 0;

    // render node in screen buffer
    /*
    virtual void render(ScreenBuffer& s) = 0;
    */

    virtual void render(QPainter &painter) = 0;

    size_t getSize() {
        return size;
    }

    void setSize(size_t s) {
        size = s;
    }

    //virtual void printJSON(std::ostream& os, int pos) = 0;

    //virtual void printJSON(std::ostream& os, int pos) = 0;

protected:
    std::string txt;      // text of tree node
    uint32_t width = 0;   // width of node (including sub-trees)
    uint32_t height = 0;  // height of node (including sub-trees)
    int xpos = 0;         // x-position of text
    int ypos = 0;         // y-position of text
    uint32_t size = 0;
};

// Implementation copied from souffle
class InnerNode : public TreeNode {
public:
    InnerNode(const std::string& nodeText = "", std::string label = "")
            : TreeNode(nodeText), label(std::move(label)) {}


    ~InnerNode() {}
    // add child to node
    void add_child(std::unique_ptr<TreeNode> child) {
        children.push_back(std::move(child));
    }


    // place node and its sub-trees
    void place(uint32_t x, uint32_t y) override {
        // there must exist at least one kid

        // set x/y pos
        xpos = x;
        ypos = y;

        height = 0;

        // compute size of bounding box
        for (const std::unique_ptr<TreeNode>& k : children) {
            k->place(x, y - 20);
            x += 10*k->getWidth();
            width += k->getWidth() + 1;
            height = std::max(height, k->getHeight());
        }
        height += 2;

        // text of inner node is longer than all its sub-trees
        if (width < txt.length()) {
            width = txt.length();
        }
    };

    // render node text and separator line
    /*
    void render(ScreenBuffer& s) override {
        s.write(xpos + (width - txt.length()) / 2, ypos, txt);
        for (const std::unique_ptr<TreeNode>& k : children) {
            k->render(s);
        }
        std::string separator(width - label.length(), '-');
        separator += label;
        s.write(xpos, ypos + 1, separator);
    }
    */

    void render(QPainter &painter) override {
        painter.drawText(QPoint(xpos, ypos), QString::fromStdString(txt));
        for (const std::unique_ptr<TreeNode>&k : children) {
            k->render(painter);
        }
        painter.drawLine(xpos, ypos-15, xpos + 9*width, ypos-15);
    }

    // print JSON
    /*
    void printJSON(std::ostream& os, int pos) override {
        std::string tab(pos, '\t');
        os << tab << R"({ "premises": ")" << stringify(txt) << "\",\n";
        os << tab << R"(  "rule-number": ")" << label << "\",\n";
        os << tab << "  \"children\": [\n";
        bool first = true;
        for (const std::unique_ptr<TreeNode>& k : children) {
            if (first)
                first = false;
            else
                os << ",\n";
            k->printJSON(os, pos + 1);
        }
        os << tab << "]\n";
        os << tab << "}";
    }
    */

private:
    std::vector<std::unique_ptr<TreeNode>> children;
    std::string label;
};

// Implementation copied from souffle
class LeafNode : public TreeNode {
public:
    LeafNode(const std::string& t = "") : TreeNode(t) {}

    ~LeafNode() {}



    // place leaf node
    void place(uint32_t x, uint32_t y) override {
        xpos = x;
        ypos = y;
        width = txt.length();
        height = 1;
    }

    // render text of leaf node
    /*
    void render(ScreenBuffer& s) override {
        s.write(xpos, ypos, txt);
    }
    */

    void render(QPainter &painter) override {
        painter.drawText(QPoint(xpos, ypos), QString::fromStdString(txt));
    }

    // print JSON
    /*
    void printJSON(std::ostream& os, int pos) override {
        std::string tab(pos, '\t');
        os << tab << R"({ "axiom": ")" << stringify(txt) << "\"}";
    }
    */
};

#endif // MAINWINDOW_H
