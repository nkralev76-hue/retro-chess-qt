#pragma once
#include "search.h"
#include <QString>
#include <QStringList>
namespace retro {
struct AiReply {SearchResult result;QString engine,error;};
AiReply computeMove(Chess game,SearchConfig config,std::vector<uint64_t> keys,std::shared_ptr<std::atomic_bool> cancel,QString stockfishPath,int level,QStringList moves);
QString findStockfish();
}
