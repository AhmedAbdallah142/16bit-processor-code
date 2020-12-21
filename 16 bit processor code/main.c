#include <stdio.h>
#include <stdlib.h>

#define MOVE 0
#define ADD 1
#define SUB 2
#define BRA 3
#define CMP 4
#define BEQ 5
#define BNE 6
#define EXG 7
#define STOP 15

int main()
{
    unsigned short int pc = 0;
    unsigned short int D0 = 0;
    unsigned short int A0 = 0;
    unsigned short int CCR = 0;
    unsigned short int MAR;
    unsigned short int MBR;
    unsigned short int IR;
    unsigned short int operand;
    unsigned short int source;
    unsigned short int destination;
    unsigned short int opcode;
    unsigned short int amode;
    unsigned short int direction;
    unsigned short int memory[256];
    unsigned short int run = 1;

    unsigned short int choosedOperation;
    //fill the memory with instructions and operands
    for(int i = 0 ; i<256;i+=2)
    {
        printf("\nCHOOSE THE OPERATION :\n0-MOVE\t1-ADD\t2-SUB\t3-BRA\t4-CMP\t5-BEQ\t6-BNE\t7-EXG\t15-Stop\n");
        scanf("%hu",&choosedOperation);
        memory[i]=choosedOperation<<4;
        if (choosedOperation == 15)
            break;
        printf("\nCHOOSE Direction:\n0-register to memory\t1-memory to register\n");
        scanf("%hu",&choosedOperation);
        memory[i] = (memory[i]&0xF0)+(choosedOperation<<2);
        printf("\nCHOOSE ADDRESSING MODE:\n0-absolute\t1-literal(immediate)\t2-indexed\t3-PC relative\n");
        scanf("%hu",&choosedOperation);
        memory[i] = (memory[i]&0xFC)+(choosedOperation);
        printf("\nEnter the operand :\n");
        scanf("%hu",&memory[i+1]);
    }

    while (run){

        //*****************************************************************\\*
        MAR = pc ;
        pc++;
        MBR = memory[MAR];
        IR = MBR;
        opcode = IR;
        MAR = pc;
        pc++;
        MBR = memory[MAR];
        IR = MBR;
        operand = IR;
        amode = opcode & 0x03;
        direction = (opcode & 0x04) >> 2;
        opcode = opcode >> 4;

        switch(amode)
        {
        case 0 :
            {
                source = memory[operand];
                break;
            }
        case 1 :
            {
                source = operand;
                break;
            }
        case 2 :
            {
                source = memory[A0 + operand];
                break;
            }
        case 3 :
            {
                source = memory[pc + operand];
            }
        }


        switch(opcode)
        {
        case MOVE:
            {
                if (direction == 0)
                    destination = D0;
                else
                    D0 = source;
                if (D0 == 0)
                    CCR = 1;
                else
                    CCR = 0;
                break;
            }
        case ADD:
            {
                if (direction == 0)
                {
                    destination = D0 + source;
                    if (destination == 0)
                        CCR = 1;
                    else
                        CCR = 0;
                }
                else
                {
                    D0 = D0 +source;
                    if (D0 == 0)
                        CCR = 1;
                    else
                        CCR = 0;
                }
                break;
            }
        case SUB:
            {
                if (direction == 0)
                {
                    destination = D0 - source;
                    if (destination == 0)
                        CCR = 1;
                    else
                        CCR = 0;
                }
                else
                {
                    D0 = D0 - source;
                    if(D0 == 0)
                        CCR = 1;
                    else
                        CCR = 0;
                }
                break;
            }
        case BRA :
            {
                if (amode == 0)
                    pc = operand;
                if (amode == 1)
                    pc = pc + operand;
                break;
            }
        case CMP:
            {
                MBR = D0 - source;
                if (MBR == 0)
                    CCR = 1 ;
                else
                    CCR = 0;
                break;
            }
        case BEQ:
            {
                if (CCR == 1)
                {
                    if (amode == 0)
                        pc  = operand;
                    if (amode == 1)
                        pc = pc + operand;
                }
                break;
            }
        case BNE:
            {
                if (CCR != 1)
                {
                    if (amode == 0)
                        pc  = operand;
                    if (amode == 1)
                        pc = pc + operand;
                }
                break;
            }
        case EXG :
            {
                MBR = D0;
                D0 = A0;
                A0 = MBR;
                break;
            }
        case STOP:
            {
                run = 0;
                break;
            }
        }
    }


        if (direction == 0)
        {
            printf("\nthe value at the specified memory location will be :");
            printf ("%hu",destination);
             switch (amode)
             {
             case 0 :
                {
                    memory[operand] = destination;
                    break;
                }
             case 2:
                {
                   memory[A0 + operand] = destination;
                   break;
                }
             case 3:
                {
                    memory[pc + operand] = destination;
                    break;
                }
             }

        }
        else
        {
            printf("\nthe value at D0 regigster :");
            printf ("%hu",D0);
            printf("\n");
        }

    return 0;
}
