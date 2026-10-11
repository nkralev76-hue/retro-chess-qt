#pragma once
#include "chess.h"
#include <atomic>
#include <chrono>
#include <unordered_map>
#include <memory>
namespace retro {
struct SearchConfig{int depth=8,milliseconds=1800;};
struct SearchResult{Move move;int score=0,depth=0;uint64_t nodes=0;};
SearchConfig levelConfig(int level);
class Search {
public:
 SearchResult bestMove(Chess position,SearchConfig config,const std::vector<uint64_t>& gameKeys={},const std::shared_ptr<std::atomic_bool>& cancel={});
 int evaluate(const Chess& g)const;
private:
 struct Entry{uint64_t key=0;Move move;int score=0,depth=-1,bound=0;};
 Chess game;std::vector<Entry> table;std::unordered_map<uint64_t,int> repetition;std::array<std::array<Move,2>,128> killers{};int history[2][128][128]{};
 std::chrono::steady_clock::time_point deadline;std::shared_ptr<std::atomic_bool> cancelled;uint64_t nodes=0;
 void tick();bool drawn();uint64_t push();void pop(uint64_t key);uint64_t ttKey(uint64_t positionKey)const;
 int negamax(int depth,int alpha,int beta,int ply);int quiescence(int alpha,int beta,int ply,int left);
 void order(std::vector<Move>& moves,Move tt,int ply);int priority(Move m,Move tt,int ply)const;
};
}
