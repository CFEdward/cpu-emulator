#include <stdio.h>
#include <stdlib.h>

using Byte = unsigned char;
using Word = unsigned short;

using u32 = unsigned int;

struct Mem
{
    static constexpr u32 MAX_MEM = 1024 * 64;
    Byte Data[MAX_MEM];

    void Initialize()
    {
        for (u32 i = 0; i < MAX_MEM; i++)
        {
            Data[i] = 0;
        }
    }

    /** read 1 byte */
    Byte operator[](u32 address) const
    {
        // assert here address is < MAX_MEM
        return Data[address];
    }

    /** write 1 byte */
    Byte& operator[](u32 address)
    {
        // assert here address is < MAX_MEM
        return Data[address];
    }
};

struct CPU
{
    Word PC;    // program counter
    Word SP;    // stack pointer

    Byte A, X, Y;   // registers

    Byte C : 1; // status flag
    Byte Z : 1; // status flag
    Byte I : 1; // status flag
    Byte D : 1; // status flag
    Byte B : 1; // status flag
    Byte V : 1; // status flag
    Byte N : 1; // status flag

    void Reset(Mem& memory)
    {
        PC = 0xFFFC;
        SP = 0x0100;
        C = Z = I = D = B = V = N = 0;
        A = X = Y = 0;
        memory.Initialize();
    }

    Byte FetchByte(u32& cycles, Mem& memory)
    {
        Byte Data = memory[PC];
        PC++;
        cycles--;
        return Data;
    }

    // opcodes
    static constexpr Byte INS_LDA_IM = 0xA9;

    void Execute(u32 cycles, Mem& memory)
    {
        while (cycles > 0)
        {
            Byte Ins = FetchByte(cycles, memory);
            switch (Ins)
            {
            case INS_LDA_IM:
            {
                Byte Value = FetchByte(cycles, memory);
                A = Value;
                Z = (A == 0);
                N = (A & 0b10000000) > 0;
            } break;
            default:
            {
                printf("Instruction not handled %d", Ins);
            } break;
            }
        }
    }
};

int main()
{
    Mem mem;
    CPU cpu;
    cpu.Reset(mem);
    //
    mem[0xFFFC] = CPU::INS_LDA_IM;
    mem[0xFFFD] = 0x42;
    //
    cpu.Execute(2, mem);

    return 0;
}