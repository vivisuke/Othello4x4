#include <algorithm>
#include <unordered_map>
#include <iostream>
#include "Board4x4.h"

using namespace std;

unordered_map<uint32, int8> g_tt;		//	トランスポジションテーブル、黒番での評価値

//----------------------------------------------------------------------
std::string boardText(bitboard_t black, bitboard_t white)
{
	string txt;
	for (int mask = MSB, i = 0; mask != 0; mask>>=1) {
		if( (black & mask) != 0 )
			txt += "●";
		else if( (white & mask) != 0 )
			txt += "○";
		else
			txt += "・";
		if( ++i % 4 == 0 )
			txt += "\n";
	}
	return txt;
}
//  立っているビット数を数える。分割統治法を用いる版（処理時間は O(logN)）少し最適化
int numOfBits(bitboard_t bits)
{
    bits = bits - (bits >> 1 & 0x5555);
    //bits = (bits & 0x5555) + (bits >> 1 & 0x5555);
    bits = (bits & 0x3333) + (bits >> 2 & 0x3333);
    bits = (bits & 0x0f0f) + (bits >> 4 & 0x0f0f);
    return (bits + (bits >> 8)) & 0x001f;
    //return (bits & 0x00ff) + (bits >> 8 & 0x00ff);
}
int numSpace(bitboard_t black, bitboard_t white)		//	空欄数を返す
{
	return numOfBits(~(black|white));
}
void	b_doPut(bitboard_t& black, bitboard_t& white, bitboard_t p, bitboard_t rev)		//	黒を p に打つ
{
	black ^= p | rev;
	white ^= rev;
}
void	w_doPut(bitboard_t& black, bitboard_t& white, bitboard_t p, bitboard_t rev)		//	白を p に打つ
{
	white ^= p | rev;
	black ^= rev;
}
int negaMax(bitboard_t black, bitboard_t white, int nspc, bool pass)			//	黒番深さ優先探索
{
	if( nspc == 0 ) {
		//cout << boardText(black, white) << "\n";
		return numOfBits(black) - numOfBits(white);
	}
	bitboard_t space = ~(black | white);    //  空欄の部分だけビットを立てる
	int maxev = -9999;
	bool put = false;
	while( space != 0 ) {
		const bitboard_t p = space & -space;      //  一番右のビットのみ取り出す
		bitboard_t rev = getRev(black, white, p);	//  反転パターン取得
        if( rev != 0 ) {									//  石が返る場合
        	int ev = -negaMax(white^rev, black|p|rev, nspc-1);
        	maxev = max(maxev, ev);
        	put = true;
        }
		space ^= p;                     //  一番右のビットをOFFにする
	}
	if( put )		//	パスでない場合
		return maxev;
	if( pass )	//	白黒双方がパスの場合
		return numOfBits(black) - numOfBits(white);
	else
		return -negaMax(white, black, nspc, true);;
}
int negaMaxTT(bitboard_t black, bitboard_t white, int nspc, bool pass)			//	黒番深さ優先探索、トランスポジションテーブル使用版
{
	uint32 key = (black<<16) | white;
	auto itr = g_tt.find(key);
	if( itr != g_tt.end() )
		return itr->second;
	if( nspc == 0 ) {
		//cout << boardText(black, white) << "\n";
		auto v = numOfBits(black) - numOfBits(white);
		g_tt[key] = v;
		g_tt[(white<<16) | black] = -v;		//	白の手番であっても終局
		return v;
	}
	bitboard_t space = ~(black | white);    //  空欄の部分だけビットを立てる
	int maxev = -9999;
	bool put = false;
	while( space != 0 ) {
		const bitboard_t p = space & -space;      //  一番右のビットのみ取り出す
		bitboard_t rev = getRev(black, white, p);	//  反転パターン取得
        if( rev != 0 ) {									//  石が返る場合
        	int ev = -negaMax(white^rev, black|p|rev, nspc-1);
        	maxev = max(maxev, ev);
        	put = true;
        }
		space ^= p;                     //  一番右のビットをOFFにする
	}
	
	if( put ) {		//	パスでない場合
		//return maxev;
	} else if( pass ) {	//	白黒双方がパスの場合
		maxev = numOfBits(black) - numOfBits(white);
	} else {
		maxev = -negaMax(white, black, nspc, true);;
	}
	g_tt[key] = maxev;
	return maxev;
}
//----------------------------------------------------------------------
//	盤面初期化
//		　ａｂｃｄ
//		１・・・・
//		２・○●・
//		３・●○・
//		４・・・・
void Board4x4::init()
{
	m_black = B3 | C2;
	m_white = B2 | C3;
}
std::string Board4x4::text() const
{
	return ::boardText(m_black, m_white);
#if	0
	string txt;
	for (int mask = MSB, i = 0; mask != 0; mask>>=1) {
		if( (m_black & mask) != 0 )
			txt += "●";
		else if( (m_white & mask) != 0 )
			txt += "○";
		else
			txt += "・";
		if( ++i % 4 == 0 )
			txt += "\n";
	}
	return txt;
#endif
}
bitboard_t getRev(bitboard_t black, bitboard_t white, bitboard_t p)		//	黒を p に打った場合に、反転する白のパターンを取得
{
	if( ((black | white) & p) != 0 ) return 0;      //  p が空白ではない場合
	bitboard_t rev = 0;                         //  反転パターン
	const bitboard_t wh = white & 0x6666;           //  水平方向用マスク
	//  右方向に返る石をチェック
	rev |= (p >> 1) & wh & ((black << 1) | ((wh  & (black << 1)) << 1));    //  隣の石
	rev |= (p >> 2) & (wh >> 1) & wh & (black << 1);    //  もう一個先の石
	//  左方向に返る石をチェック
	rev |= (p << 1) & wh & ((black >> 1) | ((wh  & (black >> 1)) >> 1));    //  隣の石
	rev |= (p << 2) & (wh << 1) & wh & (black >> 1);    //  もう一個先の石
	//
	const bitboard_t wv = white & 0x0ff0;           //  垂直方向用マスク
	//	下方向に返る石をチェック
	rev |= (p >> 4) & wv & ((black << 4) | ((wv  & (black << 4)) << 4));    //  隣の石
	rev |= (p >> 8) & (wv >> 4) & wv & (black << 4);    //  もう一個先の石
	//  上方向に返る石をチェック
	rev |= (p << 4) & wv & ((black >> 4) | ((wv  & (black >> 4)) >> 4));    //  隣の石
	rev |= (p << 8) & (wv << 4) & wv & (black >> 4);    //  もう一個先の石
	const bitboard_t wd = white & 0x0660;           //  対角線方向用マスク
	//	左下方向に返る石をチェック
	rev |= (p >> 3) & wv & ((black << 3) | ((wv  & (black << 3)) << 3));    //  隣の石
	rev |= (p >> 6) & (wv >> 3) & wv & (black << 3);    //  もう一個先の石
	//	右下方向に返る石をチェック
	rev |= (p >> 5) & wv & ((black << 5) | ((wv  & (black << 5)) << 5));    //  隣の石
	rev |= (p >> 10) & (wv >> 5) & wv & (black << 5);    //  もう一個先の石
	//	左上方向に返る石をチェック
	rev |= (p << 5) & wv & ((black >> 5) | ((wv  & (black >> 5)) >> 5));    //  隣の石
	rev |= (p << 10) & (wv << 5) & wv & (black >> 5);    //  もう一個先の石
	//	右上方向に返る石をチェック
	rev |= (p << 3) & wv & ((black >> 3) | ((wv  & (black >> 3)) >> 3));    //  隣の石
	rev |= (p << 6) & (wv << 3) & wv & (black >> 3);    //  もう一個先の石
	//
	return rev;
}
bitboard_t Board4x4::b_getRev(bitboard_t p) const		//	黒を p に打った場合に、反転する白のパターンを取得
{
	return getRev(m_black, m_white, p);
}
bitboard_t Board4x4::w_getRev(bitboard_t p) const	//	白を p に打った場合に、反転する黒のパターンを取得
{
	return getRev(m_white, m_black, p);
	//Board4x4 b2(m_white, m_black);
	//return b2.b_getRev(p);
}
int Board4x4::negaMax() const
{
	int nspc = numSpace(m_black, m_white);
	return ::negaMax(m_black, m_white, nspc);
}
int Board4x4::negaMaxTT() const
{
	g_tt.clear();
	int nspc = numSpace(m_black, m_white);
	return ::negaMaxTT(m_black, m_white, nspc);
}
