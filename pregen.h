/*
pregen.h/pregen.cpp - Source Code for ElephantEye, Part II

ElephantEye - a Chinese Chess Program (UCCI Engine)
Designed by Morning Yellow, Version: 1.2, Last Modified: Feb. 2006
Copyright (C) 2004-2006 www.elephantbase.net

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//#include "../utility/bool.h"

#ifndef PREGEN_H
#define PREGEN_H

#ifndef _WIN32
#define __int64 long long
#endif

/*
extern const BoolChar cbcInBoard[256]; // ���������
extern const BoolChar cbcInFort[256];  // �Ź����б�
extern const char ccLegalMoveTab[512]; // �����ŷ������Ա�
extern const char ccHorseLegTab[512];  // ���ȱ�
*/

// ������λ�С��͡�λ�С����ɳ����ŷ���Ԥ�ýṹ
struct SlideMoveStruct {
    unsigned char NonCap[2];    // ���������ߵ������һ��/��Сһ��
    unsigned char RookCap[2];   // ���������ߵ������һ��/��Сһ��
    unsigned char CannonCap[2]; // �ڳ������ߵ������һ��/��Сһ��
    unsigned char SuperCap[2];  // ������(�����ӳ���)���ߵ������һ��/��Сһ��
    unsigned char NonCapCannonCnt[2];  // space moveable by cannon in two directions
    //unsigned char NonCapCnt;    // NonCapCannonCnt[0] + NonCapCannonCnt[1]
}; // smv

// ������λ�С��͡�λ�С��жϳ����ŷ������Ե�Ԥ�ýṹ
struct SlideMaskStruct {
    unsigned short NonCap, RookCap, CannonCap, SuperCap;
}; // sms

extern struct PreGenStruct {
        /*
          // Zobrist��ֵ����Zobrist��ֵ��ZobristУ����������
          unsigned long dwZobristKeyPlayer;
          unsigned long dwZobristKeyTable[14][256];
          unsigned __int64 qwZobristLockPlayer;
          unsigned __int64 qwZobristLockTable[14][256];
        */
        unsigned short wBitRankMask[160-7]; // ÿ�����ӵ�λ�е�����λ
        unsigned short wBitFileMask[160-7]; // ÿ�����ӵ�λ�е�����λ

        /* ������λ�С��͡�λ�С����ɳ����ŷ����жϳ����ŷ������Ե�Ԥ������
         *
         * ��λ�С��͡�λ�С�������ElephantEye�ĺ��ļ����������������ڵ��ŷ����ɡ������жϺ;��������
         * �Գ�ʼ����췽�ұߵ����ڸ��е��ж�Ϊ�������ȱ���֪�����еġ�λ�С�����"1010000101b"��
         * ElephantEye������Ԥ�����飬��"...MoveTab"��"...MaskTab"���÷��ֱ��ǣ�
         * һ�����Ҫ֪��������ǰ���ӵ�Ŀ���(��ʼ����2��Ŀ�����9)����ôϣ��������֪��������ӣ�
         * Ԥ������һ������"FileMoveTab_CannonCap[10][1024]"��ʹ��"FileMoveTab_CannonCap[2][1010000101b] == 9"�Ϳ����ˡ�
         * �������Ҫ�жϸ����ܷ�Ե�Ŀ���(ͬ������ʼ����2��Ŀ�����9Ϊ��)����ô��Ҫ֪��Ŀ����λ�У���"0000000001b"��
         * ֻҪ��"...MoveTab"�ĸ����ԡ�����λ������ʽ���¼�������"...MaskTab"�Ϳ����ˣ��á��롱�������ж��ܷ�Ե�Ŀ���
         * ͨ��һ��"...MaskTab"��Ԫ������������λ���ж��ܷ�Ե�ͬ�л�ͬ�е�ĳ������ʱ��ֻ��Ҫ��һ���жϾͿ����ˡ�
         */
        SlideMoveStruct smvRankMoveTab[9][512];   // 36,864 �ֽ�
        SlideMoveStruct smvFileMoveTab[10][1024]; // 81,920 �ֽ�
        SlideMaskStruct smsRankMaskTab[9][512];   // 36,864 �ֽ�
        SlideMaskStruct smsFileMaskTab[10][1024]; // 81,920 �ֽ�
        // ����:  237,568 �ֽ�

        /* ��������(���ʺ��á�λ�С��͡�λ�С�)���ŷ�Ԥ��������
         *
         * �ⲿ�����������������ϵġ��ŷ�Ԥ���ɡ����飬���Ը���ĳ�����ӵ���ʼ��ֱ�Ӳ����飬�õ����е�Ŀ���
         * ʹ������ʱ�����Ը�����ʼ����ȷ��һ��ָ��"g_...Moves[Square]"�����ָ��ָ��һϵ��Ŀ�����0������
         * Ϊ�˶����ַ������[256][n]��n����4�ı��������ұ������n(��Ϊ��������˽�����ʶ��0)���������ۺ��������顣
         */
        /*
          unsigned char ucsqKingMoves[256][8];
          unsigned char ucsqAdvisorMoves[256][8];
          unsigned char ucsqBishopMoves[256][8];
          unsigned char ucsqElephantEyes[256][4];
          unsigned char ucsqKnightMoves[256][12];
          unsigned char ucsqHorseLegs[256][8];
          unsigned char ucsqPawnMoves[2][256][4];
        */
    } PreGen;

void PreGenInit(void);

#endif
