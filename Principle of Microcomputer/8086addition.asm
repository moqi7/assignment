DATA segment
    DATA1 DW 1234H
    DATA2 DW 5678H
    SUM DW ?
    DATA ends
STACK segment para STACK 'STACK'
    DW 100 DUP (?)
    top label word
    STACK ends
CODE segment
    ASSUME CS:CODE,DS:DATA,SS:STACK
START:MOV AX,DATA
      MOV DS,AX
      MOV AX,STACK
      MOV SS,AX;
      MOV SP,OFFSET TOP
      MOV AX,DATA1
      ADD AX,DATA2
      MOV SUM,AX
      MOV AH,4CH
      INT 21H;
      CODE ENDS
END START
