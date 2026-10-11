#pragma once
#include <array>
#include <string>
#include <vector>
#include <cstdint>
namespace retro {
constexpr const char* StartFen="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
struct Move { int from=-1,to=-1,promotion=0; bool ep=false,castle=false; bool valid() const {return from>=0&&to>=0;} bool operator==(const Move& o)const{return from==o.from&&to==o.to&&promotion==o.promotion;} };
struct Undo { int piece=0,captured=0,castle=0,ep=-1,half=0,full=1; };
std::string squareName(int s); int squareIndex(const std::string& s); std::string uci(const Move& m);
class Chess {
public:
 std::array<int,128> board{}; int turn=1,castle=15,ep=-1,half=0,full=1;
 explicit Chess(const std::string& fen=StartFen){load(fen);}
 void load(const std::string& fen); std::string fen()const; uint64_t key();
 int king(int color)const; bool attacked(int square,int color)const; bool inCheck(int color=0)const;
 std::vector<Move> pseudo(bool capturesOnly=false)const; std::vector<Move> legal(bool capturesOnly=false);
 Undo make(const Move& m); void unmake(const Move& m,const Undo& u);
 bool insufficient()const; std::string san(const Move& m,const std::vector<Move>& legalMoves);
};
uint64_t perft(Chess& game,int depth);
}
