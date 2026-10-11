#include "mainwindow.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QTimer>
#include <QScreen>
#include <QPixmap>
#include <iostream>
int main(int argc,char** argv){QApplication app(argc,argv);app.setApplicationName("RetroChess");app.setOrganizationName("RetroChess");app.setStyle("Fusion");QCommandLineParser parser;parser.setApplicationDescription("Retro Chess — native C++ / Qt6");parser.addHelpOption();parser.addOption({"qa-state","Render a QA state without changing saved games","state"});parser.addOption({"screenshot","Save a screenshot and exit","path"});parser.addOption({"size","Screenshot window size, e.g. 1060x850","WxH","1060x850"});parser.process(app);bool qa=parser.isSet("qa-state")||parser.isSet("screenshot");retro::MainWindow w(qa);auto size=parser.value("size").split('x');if(size.size()==2)w.resize(size[0].toInt(),size[1].toInt());w.show();if(parser.isSet("qa-state"))QTimer::singleShot(30,&w,[&](){w.showDemo(parser.value("qa-state"));});if(parser.isSet("screenshot"))QTimer::singleShot(550,&w,[&](){QWidget* target=QApplication::activeModalWidget();if(!target)target=&w;bool ok=target->grab().save(parser.value("screenshot"));std::cout<<(ok?"Screenshot saved\n":"Screenshot failed\n");app.exit(ok?0:1);});return app.exec();}
