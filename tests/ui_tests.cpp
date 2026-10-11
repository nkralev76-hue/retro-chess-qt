#include "mainwindow.h"
#include <QtTest>
#include <QComboBox>
#include <QDialog>
#include <QPushButton>
#include <QTextEdit>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
using namespace retro;
class UiTest:public QObject{
 Q_OBJECT
private slots:
void playAndMate(){MainWindow w(true);w.show();Settings s;s.cpu=false;w.newGame(s);w.submitCoordinate("bad");QCOMPARE(w.plyCount(),0);w.submitCoordinate("e2e5");QCOMPARE(w.plyCount(),0);for(auto c:{"f2f3","e7e5","g2g4","d8h4"})w.submitCoordinate(c);QCOMPARE(w.plyCount(),4);QCOMPARE(w.resultName(),QString("mate"));w.undoMove();QVERIFY(w.resultName().isEmpty());QCOMPARE(w.plyCount(),3);}
void draws(){MainWindow w(true);Settings s;s.cpu=false;w.newGame(s);for(auto c:{"g1f3","g8f6","f3g1","f6g8","g1f3","g8f6","f3g1","f6g8"})w.submitCoordinate(c);QCOMPARE(w.resultName(),QString("repetition"));w.setTestPosition("7k/5Q2/6K1/8/8/8/8/8 b - - 0 1");QCOMPARE(w.resultName(),QString("stalemate"));w.setTestPosition("7k/8/6K1/8/8/8/8/5R2 w - - 99 50");w.submitCoordinate("f1f2");QCOMPARE(w.resultName(),QString("fifty"));}
void specialMoves(){MainWindow w(true);w.setTestPosition("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");w.submitCoordinate("e1g1");QCOMPARE(w.position().board[squareIndex("f1")],4);w.submitCoordinate("e8c8");QCOMPARE(w.position().board[squareIndex("d8")],-4);w.setTestPosition("4k3/8/8/3pP3/8/8/8/4K3 w - d6 0 1");w.submitCoordinate("e5d6");QCOMPARE(w.position().board[squareIndex("d5")],0);w.setTestPosition("7k/P7/6K1/8/8/8/8/8 w - - 0 1");w.submitCoordinate("a7a8n");QCOMPARE(w.position().board[112],2);QCOMPARE(w.resultName(),QString("material"));}
void mouseAndKeyboard(){MainWindow w(true);w.show();Settings s;s.cpu=false;w.newGame(s);auto b=w.findChild<BoardWidget*>("board");auto rect=b->boardRect();int cell=rect.width()/8;QTest::mouseClick(b,Qt::LeftButton,Qt::NoModifier,{rect.x()+4*cell+cell/2,rect.y()+6*cell+cell/2});QCOMPARE(b->selected,squareIndex("e2"));QTest::keyClick(b,Qt::Key_Up);QTest::keyClick(b,Qt::Key_Up);QTest::keyClick(b,Qt::Key_Return);QCOMPARE(w.plyCount(),1);QCOMPARE(w.position().board[squareIndex("e4")],1);w.flipBoard();QVERIFY(b->flipped);}
void newDialogAndPromotion(){MainWindow w(true);w.show();w.openNewGame();QTest::qWait(30);auto d=w.findChild<QDialog*>("newDialog");QVERIFY(d);d->findChild<QComboBox*>("modeCombo")->setCurrentIndex(1);d->findChild<QComboBox*>("levelCombo")->setCurrentIndex(4);QTest::mouseClick(d->findChild<QPushButton*>("startButton"),Qt::LeftButton);QTest::qWait(20);w.setTestPosition("7k/P7/6K1/8/8/8/8/8 w - - 0 1");w.submitCoordinate("a7a8");QTest::qWait(20);auto p=w.findChild<QDialog*>("promotionDialog");QVERIFY(p);QTest::mouseClick(p->findChild<QPushButton*>("promote5"),Qt::LeftButton);QCOMPARE(w.position().board[112],5);}
void threadedAiAndUndo(){MainWindow w(true);w.show();Settings s;s.cpu=true;s.level=2;w.newGame(s);w.submitCoordinate("e2e4");QTRY_COMPARE_WITH_TIMEOUT(w.plyCount(),2,12000);QVERIFY(!w.cpuBusy());w.undoMove();QCOMPARE(w.plyCount(),0);QCOMPARE(QString::fromStdString(w.position().fen()),QString(StartFen));s.level=5;w.newGame(s);w.submitCoordinate("d2d4");QTest::qWait(220);QVERIFY(w.cpuBusy());w.undoMove();QCOMPARE(w.plyCount(),0);QTest::qWait(400);QCOMPARE(w.plyCount(),0);}
void persistence(){auto oldApp=QCoreApplication::applicationName(),oldOrg=QCoreApplication::organizationName();QCoreApplication::setOrganizationName("RetroChess-QA");QCoreApplication::setApplicationName("PersistenceTest");QStandardPaths::setTestModeEnabled(true);auto file=QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)+"/game.json";QFile::remove(file);std::string expected;{MainWindow w(false);Settings s;s.cpu=false;w.newGame(s);for(auto c:{"e2e4","e7e5","g1f3"})w.submitCoordinate(c);expected=w.position().fen();}{MainWindow w(false);QCOMPARE(w.plyCount(),3);QCOMPARE(w.position().fen(),expected);w.undoMove();QCOMPARE(w.plyCount(),2);}QFile::remove(file);QStandardPaths::setTestModeEnabled(false);QCoreApplication::setApplicationName(oldApp);QCoreApplication::setOrganizationName(oldOrg);}
void hintAndFen(){MainWindow w(true);w.show();Settings s;s.cpu=false;w.newGame(s);w.requestHint();QTRY_VERIFY_WITH_TIMEOUT(!w.cpuBusy(),10000);QCOMPARE(w.plyCount(),0);QVERIFY(w.findChild<BoardWidget*>("board")->hint.valid());w.showFen();QTest::qWait(20);auto output=w.findChild<QTextEdit*>("fenOutput");QVERIFY(output);QCOMPARE(output->toPlainText(),QString(StartFen));}
};
QTEST_MAIN(UiTest)
#include "ui_tests.moc"
