﻿#pragma once

#include <algorithm>
#include <string>
#include <unordered_map>

#if	0
typedef unsigned short bitboard_t;
#else
using bitboard_t = unsigned short;
#endif
typedef unsigned int uint32;
typedef char int8;

#define		MSB			0x8000			//	Most Significant Bit、最上位ビット
#define		BB_MASK	0xffff

//		　ＡＢＣＤ
//		１・・・・
//		２・○●・
//		３・●○・
//		４・・・・
#define		A1			0x8000
#define		B1			0x4000
#define		C1			0x2000
#define		D1		0x1000
#define		A2			0x0800
#define		B2			0x0400
#define		C2			0x0200
#define		D2		0x0100
#define		A3			0x0080
#define		B3			0x0040
#define		C3			0x0020
#define		D3		0x0010
#define		A4			0x0008
#define		B4			0x0004
#define		C4			0x0002
#define		D4		0x0001

extern std::unordered_map<uint32, int8> g_tt;

//  立っているビット数を数える。分割統治法を用いる版（処理時間は O(logN)）
int numOfBits(bitboard_t bits);
int numSpace(bitboard_t black, bitboard_t white);		//	空欄数を返す
bitboard_t getRev(bitboard_t black, bitboard_t white, bitboard_t p);		//	黒を p に打った場合に、反転する白のパターンを取得
void	b_put(bitboard_t& black, bitboard_t& white, bitboard_t p, bitboard_t rev);		//	黒を p に打つ
void	w_put(bitboard_t& black, bitboard_t& white, bitboard_t p, bitboard_t rev);		//	白を p に打つ
int negaMax(bitboard_t black, bitboard_t white, int nspc, bool=false);			//	黒番深さ優先探索
int negaMaxTT(bitboard_t black, bitboard_t white, int nspc, bool=false);			//	黒番深さ優先探索、トランスポジションテーブル使用版
std::string boardText(bitboard_t black, bitboard_t white);

//	4x4 盤面クラス
class Board4x4 {
public:
	Board4x4() {
		init();
	}
	Board4x4(bitboard_t black, bitboard_t white)
		: m_black(black), m_white(white)
	{
	}
public:
	std::string	text() const;
	//bitboard_t	b_genPutPos() const;		//	黒が着手可能な箇所を取得
	bitboard_t	b_getRev(bitboard_t p) const;	//	黒を p に打った場合に、反転する白のパターンを取得
	bitboard_t	w_getRev(bitboard_t p) const;	//	白を p に打った場合に、反転する黒のパターンを取得
	int	negaMax() const;
	int	b_negaMaxTT() const;			//	黒番評価、プラスの値：黒有利
	int	w_negaMaxTT() const;			//	白番評価、プラスの値：白有利
public:
	void	init();
	void	swapBW() { std::swap(m_black, m_white); }
	void	clear(bitboard_t p) { m_black &= ~p; m_white &= ~p; }
	void	b_set(bitboard_t p) { m_black |= p; m_white &= ~p; }
	void	w_set(bitboard_t p) { m_white |= p; m_black &= ~p; }
	void	b_put(bitboard_t p, bitboard_t rev) { ::b_put(m_black, m_white, p, rev); }
	void	w_put(bitboard_t p, bitboard_t rev) { ::w_put(m_black, m_white, p, rev); }
public:
	bitboard_t	m_black;		//	黒石
	bitboard_t	m_white;		//	白石
};
