#include "Board4x4.h"

using namespace std;

//	盤面初期化
//		　ａｂｃｄ
//		１・・・・
//		２・○●・
//		３・●○・
//		４・・・・
void Board4x4::init()
{
	m_black = 0x0240;
	m_white = 0x0420;
}
std::string Board4x4::text() const
{
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
}
bitboard_t getRev(bitboard_t p, bitboard_t black, bitboard_t white)		//	黒を p に打った場合に、反転する白のパターンを取得
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
	return getRev(p, m_black, m_white);
}
bitboard_t Board4x4::w_getRev(bitboard_t p) const	//	白を p に打った場合に、反転する黒のパターンを取得
{
	return getRev(p, m_white, m_black);
	//Board4x4 b2(m_white, m_black);
	//return b2.b_getRev(p);
}

