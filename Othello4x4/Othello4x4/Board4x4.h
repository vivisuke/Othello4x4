#pragma once

#include <string>

#if	0
typedef unsigned short bitboard_t;
#else
using bitboard_t = unsigned short;
#endif
typedef unsigned int uint32;

#define		MSB		0x8000			//	Most Significant Bit、最上位ビット

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

//	4x4 盤面クラス
class Board4x4 {
public:
	Board4x4() {
		init();
	}
public:
	std::string	text() const;
	bitboard_t	b_genPutPos() const;		//	黒が着手可能な箇所を取得
	bitboard_t	b_getRev(bitboard_t p);	//	黒を p に打った場合に、反転する白のパターンを取得
public:
	void	init();
	void	b_set(bitboard_t p) { m_black |= p; }
	void	w_set(bitboard_t p) { m_white |= p; }
public:
	bitboard_t	m_black;		//	黒石
	bitboard_t	m_white;		//	白石
};
