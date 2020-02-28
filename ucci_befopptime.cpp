/*
ucci.h/ucci.cpp - Source Code for ElephantEye, Part I

ElephantEye - a Chinese Chess Program (UCCI Engine)
Designed by Morning Yellow, Version: 3.0, Last Modified: Nov. 2007
Copyright (C) 2004-2007 www.elephantbase.net

This part (ucci.h/ucci.cpp only) of codes is NOT published under LGPL, and
can be used without restriction.
*/

#include <stdio.h>
#include "base.h"
#include "base2.h"
#include "parse.h"
#include "pipe.h"
#include "ucci.h"  

/* UCCIָ�����ģ��������UCCIָ���������ɡ�
 *
 * ���е�һ��������"BootLine()"��򵥣�ֻ������������������ĵ�һ��ָ��
 * ����"ucci"ʱ�ͷ���"UCCI_COMM_UCCI"������һ�ɷ���"UCCI_COMM_UNKNOWN"
 * ǰ�������������ȴ��Ƿ������룬���û��������ִ�д���ָ��"Idle()"
 * ��������������("BusyLine()"��ֻ��������˼��ʱ)����û������ʱֱ�ӷ���"UCCI_COMM_UNKNOWN"
 */
static PipeStruct pipeStd;

const int MAX_MOVE_NUM = 512; //384; //512; //256;

static char szFen[LINE_INPUT_MAX_CHAR];
static uint32 dwCoordList[MAX_MOVE_NUM];

static Bool ParsePos(UcciCommStruct &UcciComm, char *lp) {
  int i;
  // �����ж��Ƿ�ָ����FEN��
  if (StrEqvSkip(lp, "fen ")) {
    strcpy(szFen, lp);
    UcciComm.szFenStr = szFen;
  // Ȼ���ж��Ƿ���startpos
  } else if (StrEqv(lp, "startpos")) {
    UcciComm.szFenStr = "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR w - - 0 1";
  // ������߶����ǣ�����������
  } else {
    return FALSE;
  }
  // Ȼ��Ѱ���Ƿ�ָ���˺����ŷ������Ƿ���"moves"�ؼ���
  UcciComm.nMoveNum = 0;
  if (StrScanSkip(lp, " moves ")) {
    *(lp - strlen(" moves ")) = '\0';
    UcciComm.nMoveNum = MIN((int) (strlen(lp) + 1) / 5, MAX_MOVE_NUM); // ��ʾ��"moves"�����ÿ���ŷ�����1���ո��4���ַ�
    for (i = 0; i < UcciComm.nMoveNum; i ++) {
      dwCoordList[i] = *(uint32 *) lp; // 4���ַ���ת��Ϊһ��"uint32"���洢�ʹ�����������
      lp += sizeof(uint32) + 1;
    }
    UcciComm.lpdwMovesCoord = dwCoordList;
  }
  return TRUE;
}

UcciCommEnum BootLine(void) {
  char szLineStr[LINE_INPUT_MAX_CHAR];
  pipeStd.Open();
  while (!pipeStd.LineInput(szLineStr)) {
    Idle();
  }
  if (StrEqv(szLineStr, "ucci")) {
    return UCCI_COMM_UCCI;
  } else {
    return UCCI_COMM_UNKNOWN;
  }
}

UcciCommEnum IdleLine(UcciCommStruct &UcciComm, Bool bDebug) {
  char szLineStr[LINE_INPUT_MAX_CHAR];
  char *lp;
  int i;
  Bool bGoTime;

  while (!pipeStd.LineInput(szLineStr)) {
    Idle();
  }
  lp = szLineStr;
  if (bDebug) {
    printf("info idleline [%s]\n", lp);
    fflush(stdout);
  }
  if (FALSE) {
  // "IdleLine()"����ӵ�UCCIָ����������������UCCIָ����������ͣ�������

  // 1. "isready"ָ��
  } else if (StrEqv(lp, "isready")) {
    return UCCI_COMM_ISREADY;

  // 2. "setoption <option> [<arguments>]"ָ��
  } else if (StrEqvSkip(lp, "setoption ")) {
    if (FALSE) {

    // (1) "batch"ѡ��
    } else if (StrEqvSkip(lp, "batch ")) {
      UcciComm.Option = UCCI_OPTION_BATCH;
      if (StrEqv(lp, "on")) {
        UcciComm.bCheck = TRUE;
      } else if (StrEqv(lp, "true")) {
        UcciComm.bCheck = TRUE;
      } else {
        UcciComm.bCheck = FALSE;
      } // ����"batch"ѡ��Ĭ���ǹرյģ�����ֻ���趨"on"��"true"ʱ�Ŵ򿪣���ͬ

    // (2) "debug"ѡ��
    } else if (StrEqvSkip(lp, "debug ")) {
      UcciComm.Option = UCCI_OPTION_DEBUG;
      if (StrEqv(lp, "on")) {
        UcciComm.bCheck = TRUE;
      } else if (StrEqv(lp, "true")) {
        UcciComm.bCheck = TRUE;
      } else {
        UcciComm.bCheck = FALSE;
      }

    // (3) "ponder"ѡ��
    } else if (StrEqvSkip(lp, "ponder ")) {
      UcciComm.Option = UCCI_OPTION_PONDER;
      if (StrEqv(lp, "on")) {
        UcciComm.bCheck = TRUE;
      } else if (StrEqv(lp, "true")) {
        UcciComm.bCheck = TRUE;
      } else {
        UcciComm.bCheck = FALSE;
      }

    // (4) "usebook"ѡ��
    } else if (StrEqvSkip(lp, "usebook ")) {
      UcciComm.Option = UCCI_OPTION_USEBOOK;
      if (StrEqv(lp, "off")) {
        UcciComm.bCheck = FALSE;
      } else if (StrEqv(lp, "false")) {
        UcciComm.bCheck = FALSE;
      } else {
        UcciComm.bCheck = TRUE;
      }

    // (5) "useegtb"ѡ��
    } else if (StrEqvSkip(lp, "useegtb ")) {
      UcciComm.Option = UCCI_OPTION_USEEGTB;
      if (StrEqv(lp, "off")) {
        UcciComm.bCheck = FALSE;
      } else if (StrEqv(lp, "false")) {
        UcciComm.bCheck = FALSE;
      } else {
        UcciComm.bCheck = TRUE;
      }

    // (6) "bookfiles"ѡ��
    } else if (StrEqvSkip(lp, "bookfiles ")) {
      UcciComm.Option = UCCI_OPTION_BOOKFILES;
      UcciComm.szOption = lp;

    // (7) "egtbpaths"ѡ��
    } else if (StrEqvSkip(lp, "egtbpaths ")) {
      UcciComm.Option = UCCI_OPTION_EGTBPATHS;
      UcciComm.szOption = lp;

    // (8) "evalapi"ѡ��
    } else if (StrEqvSkip(lp, "evalapi ")) {
      UcciComm.Option = UCCI_OPTION_EVALAPI;
      UcciComm.szOption = lp;

    // (9) "hashsize"ѡ��
    } else if (StrEqvSkip(lp, "hashsize ")) {
      UcciComm.Option = UCCI_OPTION_HASHSIZE;
      UcciComm.nSpin = Str2Digit(lp, 0, 1024);

    // (10) "threads"ѡ��
    } else if (StrEqvSkip(lp, "threads ")) {
      UcciComm.Option = UCCI_OPTION_THREADS;
      UcciComm.nSpin = Str2Digit(lp, 0, 32);

    // (11) "promotion"ѡ��
    } else if (StrEqvSkip(lp, "promotion ")) {
      UcciComm.Option = UCCI_OPTION_PROMOTION;
      if (StrEqv(lp, "on")) {
        UcciComm.bCheck = TRUE;
      } else if (StrEqv(lp, "true")) {
        UcciComm.bCheck = TRUE;
      } else {
        UcciComm.bCheck = FALSE;
      }

    // (12) "idle"ѡ��
    } else if (StrEqvSkip(lp, "idle ")) {
      UcciComm.Option = UCCI_OPTION_IDLE;
      if (FALSE) {
      } else if (StrEqv(lp, "none")) {
        UcciComm.Grade = UCCI_GRADE_NONE;
      } else if (StrEqv(lp, "small")) {
        UcciComm.Grade = UCCI_GRADE_SMALL;
      } else if (StrEqv(lp, "medium")) {
        UcciComm.Grade = UCCI_GRADE_MEDIUM;
      } else if (StrEqv(lp, "large")) {
        UcciComm.Grade = UCCI_GRADE_LARGE;
      } else {
        UcciComm.Grade = UCCI_GRADE_NONE;
      }

    // (13) "pruning"ѡ��
    } else if (StrEqvSkip(lp, "pruning ")) {
      UcciComm.Option = UCCI_OPTION_PRUNING;
      if (FALSE) {
      } else if (StrEqv(lp, "none")) {
        UcciComm.Grade = UCCI_GRADE_NONE;
      } else if (StrEqv(lp, "small")) {
        UcciComm.Grade = UCCI_GRADE_SMALL;
      } else if (StrEqv(lp, "medium")) {
        UcciComm.Grade = UCCI_GRADE_MEDIUM;
      } else if (StrEqv(lp, "large")) {
        UcciComm.Grade = UCCI_GRADE_LARGE;
      } else {
        UcciComm.Grade = UCCI_GRADE_LARGE;
      }

    // (14) "knowledge"ѡ��
    } else if (StrEqvSkip(lp, "knowledge ")) {
      UcciComm.Option = UCCI_OPTION_KNOWLEDGE;
      if (FALSE) {
      } else if (StrEqv(lp, "none")) {
        UcciComm.Grade = UCCI_GRADE_NONE;
      } else if (StrEqv(lp, "small")) {
        UcciComm.Grade = UCCI_GRADE_SMALL;
      } else if (StrEqv(lp, "medium")) {
        UcciComm.Grade = UCCI_GRADE_MEDIUM;
      } else if (StrEqv(lp, "large")) {
        UcciComm.Grade = UCCI_GRADE_LARGE;
      } else {
        UcciComm.Grade = UCCI_GRADE_LARGE;
      }

    // (15) "randomness"ѡ��
    } else if (StrEqvSkip(lp, "randomness ")) {
      UcciComm.Option = UCCI_OPTION_RANDOMNESS;
      if (FALSE) {
      } else if (StrEqv(lp, "none")) {
        UcciComm.Grade = UCCI_GRADE_NONE;
      } else if (StrEqv(lp, "small")) {
        UcciComm.Grade = UCCI_GRADE_SMALL;
      } else if (StrEqv(lp, "medium")) {
        UcciComm.Grade = UCCI_GRADE_MEDIUM;
      } else if (StrEqv(lp, "large")) {
        UcciComm.Grade = UCCI_GRADE_LARGE;
      } else {
        UcciComm.Grade = UCCI_GRADE_NONE;
      }

    // (16) "style"ѡ��
    } else if (StrEqvSkip(lp, "style ")) {
      UcciComm.Option = UCCI_OPTION_STYLE;
      if (FALSE) {
      } else if (StrEqv(lp, "solid")) {
        UcciComm.Style = UCCI_STYLE_SOLID;
      } else if (StrEqv(lp, "normal")) {
        UcciComm.Style = UCCI_STYLE_NORMAL;
      } else if (StrEqv(lp, "risky")) {
        UcciComm.Style = UCCI_STYLE_RISKY;
      } else {
        UcciComm.Style = UCCI_STYLE_NORMAL;
      }

    // (17) "newgame"ѡ��
    } else if (StrEqv(lp, "newgame")) {
      UcciComm.Option = UCCI_OPTION_NEWGAME;

    // (18) �޷�ʶ���ѡ�����������
    } else {
      UcciComm.Option = UCCI_OPTION_UNKNOWN;
    }
    return UCCI_COMM_SETOPTION;

  // 3. "position {<special_position> | fen <fen_string>} [moves <move_list>]"ָ��
  } else if (StrEqvSkip(lp, "position ")) {
    return ParsePos(UcciComm, lp) ? UCCI_COMM_POSITION : UCCI_COMM_UNKNOWN;

  // 4. "banmoves <move_list>"ָ�����������"position ... moves ..."��һ����
  } else if (StrEqvSkip(lp, "banmoves ")) {
    UcciComm.nBanMoveNum = MIN((int) (strlen(lp) + 1) / 5, MAX_MOVE_NUM);
    for (i = 0; i < UcciComm.nBanMoveNum; i ++) {
      dwCoordList[i] = *(uint32 *) lp;
      lp += sizeof(uint32) + 1;
    }
    UcciComm.lpdwBanMovesCoord = dwCoordList;
    return UCCI_COMM_BANMOVES;    
      
  // 5. "go [ponder | draw] <mode>"ָ��
  } else if (StrEqvSkip(lp, "go ")) {
    UcciComm.bPonder = UcciComm.bDraw = FALSE;
    // �����жϵ�����"go"��"go ponder"����"go draw"
    if (StrEqvSkip(lp, "ponder ")) {
      UcciComm.bPonder = TRUE;
    } else if (StrEqvSkip(lp, "draw ")) {
      UcciComm.bDraw = TRUE;
    }
    // Ȼ���ж�˼��ģʽ
    bGoTime = FALSE;
    if (FALSE) {
    } else if (StrEqvSkip(lp, "depth ")) {
      UcciComm.Go = UCCI_GO_DEPTH;
      UcciComm.nDepth = Str2Digit(lp, 0, UCCI_MAX_DEPTH);
    } else if (StrEqvSkip(lp, "nodes ")) {
      UcciComm.Go = UCCI_GO_NODES;
      UcciComm.nDepth = Str2Digit(lp, 0, 2000000000);
    } else if (StrEqvSkip(lp, "time ")) {
      UcciComm.nTime = Str2Digit(lp, 0, 2000000000);
      bGoTime = TRUE;
    // ���û��˵���ǹ̶���Ȼ����趨ʱ�ޣ��͹̶����Ϊ"UCCI_MAX_DEPTH"
    } else {
      UcciComm.Go = UCCI_GO_DEPTH;
      UcciComm.nDepth = UCCI_MAX_DEPTH;
      //1892m - go infinity          
    }
    // ������趨ʱ�ޣ���Ҫ�ж���ʱ���ƻ��Ǽ�ʱ��
    if (bGoTime) {
      if (FALSE) {
      } else if (StrScanSkip(lp, " movestogo ")) {
        UcciComm.Go = UCCI_GO_TIME_MOVESTOGO;
        UcciComm.nMovesToGo = Str2Digit(lp, 1, 999);
      } else if (StrScanSkip(lp, " increment ")) {
        UcciComm.Go = UCCI_GO_TIME_INCREMENT;
        UcciComm.nIncrement = Str2Digit(lp, 0, 999999);
      // ���û��˵����ʱ���ƻ��Ǽ�ʱ�ƣ����趨Ϊ������1��ʱ����
      } else {
        UcciComm.Go = UCCI_GO_TIME_MOVESTOGO;
        //1892m - go infinity 
        if (UcciComm.nDepth == UCCI_MAX_DEPTH)
        	UcciComm.nMovesToGo = 999;
        else	
        UcciComm.nMovesToGo = 1;
      }
    }
    return UCCI_COMM_GO;

  // 6. "stop"ָ��
  } else if (StrEqv(lp, "stop")) {
    return UCCI_COMM_STOP;

  // 7. "probe {<special_position> | fen <fen_string>} [moves <move_list>]"ָ��
  } else if (StrEqvSkip(lp, "probe ")) {
    return ParsePos(UcciComm, lp) ? UCCI_COMM_PROBE : UCCI_COMM_UNKNOWN;

  // 8. "quit"ָ��
  } else if (StrEqv(lp, "quit")) {
    return UCCI_COMM_QUIT;

  // 9. �޷�ʶ���ָ��
  } else {
    return UCCI_COMM_UNKNOWN;
  }
}

UcciCommEnum BusyLine(UcciCommStruct &UcciComm, Bool bDebug) {
  char szLineStr[LINE_INPUT_MAX_CHAR];
  char *lp;
  if (pipeStd.LineInput(szLineStr)) {
    if (bDebug) {
      printf("info busyline [%s]\n", szLineStr);
      fflush(stdout);
    }
    // "BusyLine"ֻ�ܽ���"isready"��"ponderhit"��"stop"������ָ��
    if (FALSE) {
    } else if (StrEqv(szLineStr, "isready")) {
      return UCCI_COMM_ISREADY;
    } else if (StrEqv(szLineStr, "ponderhit draw")) {
      return UCCI_COMM_PONDERHIT_DRAW;
    // ע�⣺���������ж�"ponderhit draw"�����ж�"ponderhit"
    } else if (StrEqv(szLineStr, "ponderhit")) {
      return UCCI_COMM_PONDERHIT;
    } else if (StrEqv(szLineStr, "stop")) {
      return UCCI_COMM_STOP;
    } else if (StrEqv(szLineStr, "quit")) {
      return UCCI_COMM_QUIT;
    } else {
      lp = szLineStr;
      if (StrEqvSkip(lp, "probe ")) {
        return ParsePos(UcciComm, lp) ? UCCI_COMM_PROBE : UCCI_COMM_UNKNOWN;
      } else {
        return UCCI_COMM_UNKNOWN;
      }
    }
  } else {
    return UCCI_COMM_UNKNOWN;
  }
}
