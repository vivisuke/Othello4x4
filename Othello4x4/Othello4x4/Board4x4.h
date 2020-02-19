#pragma once

#include <string>

#if	0
typedef unsigned short bitboard_t;
#else
using bitboard_t = unsigned short;
#endif
typedef unsigned int uint32;

#define		MSB		0x8000			//	Most Significant Bit、最上位ビット

//	4x4 盤面クラス
class Board4x4 {
public:
	Board4x4() {
		init();
	}
public:
	std::string	text() const;
public:
	void	init();
public:
	bitboard_t	m_black;		//	黒石
	bitboard_t	m_white;		//	白石
};
