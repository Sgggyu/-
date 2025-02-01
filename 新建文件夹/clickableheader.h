#ifndef CLICKABLEHEADER_H
#define CLICKABLEHEADER_H


#include <QHeaderView>
#include <QMouseEvent>

class ClickableHeader : public QHeaderView {
    Q_OBJECT
public:
    explicit ClickableHeader(Qt::Orientation orientation, QWidget *parent = nullptr)
        : QHeaderView(orientation, parent) {}

signals:
    void columnClicked(int column); // 表头列点击信号

protected:
    void mousePressEvent(QMouseEvent *event) override {
        int column = logicalIndexAt(event->pos());
        if (column != -1) {
            emit columnClicked(column); // 发送信号
        }
        QHeaderView::mousePressEvent(event);
    }
};

#endif // CLICKABLEHEADER_H
