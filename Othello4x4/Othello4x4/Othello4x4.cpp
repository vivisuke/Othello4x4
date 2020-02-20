#include <string>
#include <vector>
#include <iostream>
#include <assert.h>
#include "Board4x4.h"
#include "utils.h"

using namespace std;


//	空欄が存在するかどうかチェック
bool isBoardFull(bitboard_t black, bitboard_t white)
{
    return ~(black | white) == 0;
}
//  立っているビット数を数える。ループを用いる版（処理時間は O(N)）
int numOfBits_LOOP(bitboard_t bb)
{
    int count = 0;
    for(bitboard_t mask = 0x8000; mask != 0; mask >>= 1)
        if( (bb & mask) != 0 )
            ++count;
    return count;
}
//  立っているビット数を数える。分割統治法を用いる版（処理時間は O(logN)）
int numOfBits_BB(bitboard_t bits)
{
    bits = (bits & 0x5555) + (bits >> 1 & 0x5555);    //  2bitごとに計算
    bits = (bits & 0x3333) + (bits >> 2 & 0x3333);    //  4bitごとに計算
    bits = (bits & 0x0f0f) + (bits >> 4 & 0x0f0f);    //  8bitごとに計算
    return (bits & 0x00ff) + (bits >> 8 & 0x00ff);    //  16ビット分を計算
}
//  白石・黒石の立っているビット数を同時に数える。
void numOfBits(bitboard_t &b, bitboard_t &w)
{
    uint32 bits = (b << 16) | w;
    bits = bits - (bits >> 1 & 0x55555555);
    //bits = (bits & 0x55555555) + (bits >> 1 & 0x55555555);
    bits = (bits & 0x33333333) + (bits >> 2 & 0x33333333);
    bits = (bits & 0x0f0f0f0f) + (bits >> 4 & 0x0f0f0f0f);
    bits = (bits & 0x00ff00ff) + (bits >> 8 & 0x00ff00ff);
    b = (bitboard_t)(bits >> 16);
    w = (bitboard_t)bits;
}
//	ビット数数え上げチェック
void test_numOfBits()
{
	for (int n = 0; n < 0x10000; ++n) {
		//bitboard_t b = g_mt() & 0xffff;
		bitboard_t b = (bitboard_t)n;
		auto cntLoop = numOfBits_LOOP(b);
		auto cntBB = numOfBits_BB(b);
		auto cnt = numOfBits(b);
		assert( cntLoop == cntBB );
		assert( cntLoop == cnt );
	}
}
void test_getRev()
{
	Board4x4 bd;
//		　ＡＢＣＤ
//		１・・・・
//		２・○●・
//		３・●○・
//		４・・・・
	cout << bd.text() << "\n";
	auto rev = bd.b_getRev(A1);	assert( rev == 0 );
	rev = bd.b_getRev(A2);	assert( rev == B2 );
	rev = bd.b_getRev(A3);	assert( rev == 0 );
	rev = bd.b_getRev(A4);	assert( rev == 0 );
	rev = bd.b_getRev(B1);	assert( rev == B2 );
	rev = bd.b_getRev(B2);	assert( rev == 0 );
	rev = bd.b_getRev(B3);	assert( rev == 0 );
	rev = bd.b_getRev(B4);	assert( rev == 0 );
	rev = bd.b_getRev(C1);	assert( rev == 0 );
	rev = bd.b_getRev(C2);	assert( rev == 0 );
	rev = bd.b_getRev(C3);	assert( rev == 0 );
	rev = bd.b_getRev(C4);	assert( rev == C3 );
	rev = bd.b_getRev(D1);	assert( rev == 0 );
	rev = bd.b_getRev(D2);	assert( rev == 0 );
	rev = bd.b_getRev(D3);	assert( rev == C3 );
	rev = bd.b_getRev(D4);	assert( rev == 0 );
	//		　ＡＢＣＤ
	//		１・・・・
	//		２・○○●
	//		３・●●○
	//		４・・・・
	bd.b_set(D2|C3);
	bd.w_set(C2|D3);
	//cout << bd.text() << "\n";
	rev = bd.b_getRev(A1);	assert( rev == B2 );
	rev = bd.b_getRev(A2);	assert( rev == (B2|C2) );
	rev = bd.w_getRev(A3);	assert( rev == (B3|C3) );
	rev = bd.w_getRev(A4);	assert( rev == B3 );
	rev = bd.b_getRev(D1);	assert( rev == C2 );
	rev = bd.w_getRev(D4);	assert( rev == C3 );
}
#if	0
//	石反転処理
void doPut(	bitboard_t &black,		//  黒石
					bitboard_t &white,		//  白石
					bitboard_t p,				//  黒石を置く位置
					bitboard_t rev)			//  反転するビットパターン（計算方法は次節参照）
{
    black ^= p | rev;
    white ^= rev;
}
#endif
void test_negaMax()
{
	//		　ＡＢＣＤ
	//		１○●・・
	//		２●○○●
	//		３●●●○
	//		４●●●●
	Board4x4 bd(0x49ef, 0x8610);
	cout << bd.text() << "\n";
	auto ev = bd.negaMax();
	cout << "ev = " << ev << "\n";
	//	D1, C1: -> 11 - 5 = 6
	//		　ＡＢＣＤ
	//		１○○○●
	//		２●○●●
	//		３●●●○
	//		４●●●●
	//	C1, D1: -> 9 - 7 = 2
	//		　ＡＢＣＤ
	//		１○○○○
	//		２●○●○
	//		３●●●○
	//		４●●●●
	assert( ev == 6 );
	//	パスがある場合
	//		　ＡＢＣＤ
	//		１●●・・
	//		２●○○●
	//		３●●●○
	//		４●●●●
	bd = Board4x4(0xc9ef, 0x0610);
	cout << bd.text() << "\n";
	ev = bd.negaMax();
	cout << "ev = " << ev << "\n";
	assert( ev == 14 );		//	D1, C1 連打、15 - 1 = 14
}
//	初期化
int main()
{
	test_numOfBits();
	test_getRev();
	//
	Board4x4 bd;
	cout << bd.text() << "\n";
	assert( numSpace(bd.m_black, bd.m_white) == 12 );
	//
	test_negaMax();
	auto ev = bd.negaMax();
	cout << "ev = " << ev << "\n";
	//
	auto evtt = bd.negaMaxTT();		//	トランスポジションテーブル 使用
	cout << "evtt = " << evtt << "\n";
	//
    std::cout << "OK\n";
}
