#include "chess.h"
#include "search.h"
#include <iostream>
#include <stdexcept>
#include <algorithm>
using namespace retro;
void require(bool b,const char* text){if(!b)throw std::runtime_error(text);}
Move find(Chess& g,const std::string& s){for(auto m:g.legal())if(uci(m)==s)return m;throw std::runtime_error("Illegal test move: "+s);}
int main(){try{Chess g;for(auto [d,n]:std::vector<std::pair<int,uint64_t>>{{1,20},{2,400},{3,8902},{4,197281}}){require(perft(g,d)==n,"Initial perft");std::cout<<"Initial perft "<<d<<" = "<<n<<"\n";}
g=Chess("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");for(auto [d,n]:std::vector<std::pair<int,uint64_t>>{{1,48},{2,2039},{3,97862}})require(perft(g,d)==n,"Kiwipete");
g=Chess("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");require(perft(g,4)==43238,"En passant perft");
g=Chess("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");require(perft(g,3)==9467,"Promotion perft");
g=Chess();for(auto code:{"f2f3","e7e5","g2g4","d8h4"}){auto m=find(g,code);auto s=g.san(m,g.legal());g.make(m);if(std::string(code)=="d8h4")require(s=="Qh4#","SAN checkmate");}require(g.inCheck()&&g.legal().empty(),"Mate");
g=Chess("7k/5Q2/6K1/8/8/8/8/8 b - - 0 1");require(!g.inCheck()&&g.legal().empty(),"Stalemate");
g=Chess("4k3/8/8/r4pPK/8/8/8/8 w - f6 0 1");auto k=g.key();g.ep=-1;require(g.key()==k,"Pinned ep key");
g=Chess("7k/8/6K1/8/8/8/2b5/1B6 w - - 0 1");require(g.insufficient(),"Same color bishops");
Search search;g=Chess("7k/8/5KQ1/8/8/8/8/8 w - - 0 1");auto r=search.bestMove(g,{6,800});require(g.san(r.move,g.legal()).back()=='#',"AI mate in one");
g=Chess("r1bqkbnr/pppp1ppp/2n5/4p3/6P1/5P2/PPPPP2P/RNBQKBNR b KQkq - 0 2");r=search.bestMove(g,{6,800});require(uci(r.move)=="d8h4","AI mate pattern");
g=Chess("6k1/5ppp/8/8/8/8/5PPP/3R2K1 w - - 0 1");r=search.bestMove(g,{6,1000});require(g.san(r.move,g.legal()).back()=='#',"AI back rank mate");
g=Chess();r=search.bestMove(g,{7,1400});require(r.move.valid(),"AI move");require(r.depth>=3,"AI completes deeper search");std::cout<<"C++ AI: "<<uci(r.move)<<", depth "<<r.depth<<", nodes "<<r.nodes<<"\n";
// Exact restoration of all mutable fields after legal generation / search.
for(auto m:g.legal()){auto f=g.fen();auto hash=g.key();auto u=g.make(m);g.legal();g.unmake(m,u);require(g.fen()==f&&g.key()==hash,"Unmake restores");}
std::cout<<"ALL ENGINE TESTS PASSED\n";return 0;}catch(const std::exception& e){std::cerr<<"TEST FAILED: "<<e.what()<<"\n";return 1;}}
