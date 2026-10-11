#pragma once
#include "boardwidget.h"
#include "enginebridge.h"
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTableWidget>
#include <QFutureWatcher>
namespace retro {
struct Settings {bool cpu=true;int human=1,level=3;QString stockfishPath;};
struct Record{Move move;Undo undo;QString san;int color=1,captured=0;};
class MainWindow:public QMainWindow {
 Q_OBJECT
public:
 explicit MainWindow(bool testMode=false);~MainWindow()override;
 void newGame(Settings settings);void submitCoordinate(const QString& text);void setTestPosition(const std::string& fen);void showDemo(const QString& state);
 const Chess& position()const{return game;}int plyCount()const{return int(records.size());}bool cpuBusy()const{return busy;}QString resultName()const{return outcome;}
public slots:void openNewGame();void undoMove();void requestHint();void flipBoard();void showHelp();void showFen();void selectSquare(int square);
protected:bool eventFilter(QObject*,QEvent*)override;void closeEvent(QCloseEvent*)override;
private:
 Chess game;Settings settings;std::vector<Record> records;std::vector<uint64_t> keys;std::vector<Move> legalMoves;
 BoardWidget* board=nullptr;QLabel *topPlayer,*levelLabel,*statusTitle,*statusDetail,*stateLabel,*moveLabel,*messageLabel,*whiteRole,*blackRole,*savedLabel,*engineLabel;
 QPushButton *undoButton,*hintButton,*soundButton;QLineEdit* coordinate;QTableWidget* log;QWidget* captureStrip;
 QString toast,outcome,engineName="C++ AI";int winner=0,selected=-1;Move hinted;bool flipped=false,busy=false,sound=false,testMode=false;quint64 generation=0;
 std::shared_ptr<std::atomic_bool> cancellation;QFutureWatcher<AiReply>* watcher=nullptr;
 void buildUi();void refresh();void updateState();void commit(Move move);void scheduleComputer();void search(bool hint);void cancelSearch();bool computerTurn()const;
 void promote(const std::vector<Move>& choices);void setMessage(const QString& text);void save();void restore();QString savePath()const;QString pgn()const;void exportPgn();
 void beep(const QString& kind);void chooseStockfish();
};
}
