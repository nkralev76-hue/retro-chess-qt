#pragma once
#include "chess.h"
#include <QWidget>
namespace retro {
class BoardWidget:public QWidget {
 Q_OBJECT
public:
 explicit BoardWidget(QWidget* parent=nullptr);
 Chess position;std::vector<Move> legalMoves;Move lastMove,hint;int selected=-1,focusSquare=4;bool flipped=false;
 QSize sizeHint()const override{return {560,560};}QRect boardRect()const;int squareAt(const QPoint&)const;
 int visualSquare(int row,int col)const;void refresh(){update();}
signals:void squareChosen(int square);
protected:void paintEvent(QPaintEvent*)override;void mousePressEvent(QMouseEvent*)override;void keyPressEvent(QKeyEvent*)override;
};
}
