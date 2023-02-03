#include <stdint.h>

uint32_t add(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x00000033; // 33 00 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t addi(uint32_t rd, uint32_t rs1, uint32_t imm)
{
    uint32_t i = 0x00000013; // 13 00 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((imm >> 0) & 0b111111111111) << 20);
    return i;
}

uint32_t addiw(uint32_t rd, uint32_t rs1, uint32_t imm)
{
    uint32_t i = 0x0000001B; // 1B 00 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((imm >> 0) & 0b111111111111) << 20);
    return i;
}

uint32_t addw(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x0000003B; // 3B 00 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t and (uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x00007033; // 33 70 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t andi(uint32_t rd, uint32_t rs1, uint32_t imm)
{
    uint32_t i = 0x00007013; // 13 70 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((imm >> 0) & 0b111111111111) << 20);
    return i;
}

uint32_t auipc(uint32_t rd, uint32_t imm)
{
    uint32_t i = 0x00000017; // 17 00 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((imm >> 0) & 0b11111111111111111111) << 12);
    return i;
}

uint32_t auipcc(uint32_t rd, uint32_t imm)
{
    uint32_t i = 0x00000017; // 17 00 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((imm >> 0) & 0b11111111111111111111) << 12);
    return i;
}

uint32_t beq(uint32_t rs1, uint32_t rs2, uint32_t imm)
{
    uint32_t i = 0x00000063; // 63 00 00 00
    i |= (((imm >> 10) & 0b1) << 7);
    i |= (((imm >> 0) & 0b1111) << 8);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    i |= (((imm >> 4) & 0b111111) << 25);
    i |= (((imm >> 11) & 0b1) << 31);
    return i;
}

uint32_t bge(uint32_t rs1, uint32_t rs2, uint32_t imm)
{
    uint32_t i = 0x00005063; // 63 50 00 00
    i |= (((imm >> 10) & 0b1) << 7);
    i |= (((imm >> 0) & 0b1111) << 8);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    i |= (((imm >> 4) & 0b111111) << 25);
    i |= (((imm >> 11) & 0b1) << 31);
    return i;
}

uint32_t bgeu(uint32_t rs1, uint32_t rs2, uint32_t imm)
{
    uint32_t i = 0x00007063; // 63 70 00 00
    i |= (((imm >> 10) & 0b1) << 7);
    i |= (((imm >> 0) & 0b1111) << 8);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    i |= (((imm >> 4) & 0b111111) << 25);
    i |= (((imm >> 11) & 0b1) << 31);
    return i;
}

uint32_t blt(uint32_t rs1, uint32_t rs2, uint32_t imm)
{
    uint32_t i = 0x00004063; // 63 40 00 00
    i |= (((imm >> 10) & 0b1) << 7);
    i |= (((imm >> 0) & 0b1111) << 8);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    i |= (((imm >> 4) & 0b111111) << 25);
    i |= (((imm >> 11) & 0b1) << 31);
    return i;
}

uint32_t bltu(uint32_t rs1, uint32_t rs2, uint32_t imm)
{
    uint32_t i = 0x00006063; // 63 60 00 00
    i |= (((imm >> 10) & 0b1) << 7);
    i |= (((imm >> 0) & 0b1111) << 8);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    i |= (((imm >> 4) & 0b111111) << 25);
    i |= (((imm >> 11) & 0b1) << 31);
    return i;
}

uint32_t bne(uint32_t rs1, uint32_t rs2, uint32_t imm)
{
    uint32_t i = 0x00001063; // 63 10 00 00
    i |= (((imm >> 10) & 0b1) << 7);
    i |= (((imm >> 0) & 0b1111) << 8);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    i |= (((imm >> 4) & 0b111111) << 25);
    i |= (((imm >> 11) & 0b1) << 31);
    return i;
}

uint32_t candperm(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x1A00005B; // 5B 00 00 1A
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t cbuildcap(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x3A00005B; // 5B 00 00 3A
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t ccseal(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x3E00005B; // 5B 00 00 3E
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t ccall(uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0xFC0000DB; // DB 00 00 FC
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t ccleartag(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFEB0005B; // 5B 00 B0 FE
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t ccopytype(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x3C00005B; // 5B 00 00 3C
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t cfld(uint32_t rd, uint32_t rs1, uint32_t imm)
{
    uint32_t i = 0x00003007; // 07 30 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((imm >> 0) & 0b111111111111) << 20);
    return i;
}

uint32_t cflw(uint32_t rd, uint32_t rs1, uint32_t imm)
{
    uint32_t i = 0x00002007; // 07 20 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((imm >> 0) & 0b111111111111) << 20);
    return i;
}

uint32_t cfsd(uint32_t rs1, uint32_t rs2, uint32_t imm)
{
    uint32_t i = 0x00003027; // 27 30 00 00
    i |= (((imm >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    i |= (((imm >> 5) & 0b1111111) << 25);
    return i;
}

uint32_t cfsw(uint32_t rs1, uint32_t rs2, uint32_t imm)
{
    uint32_t i = 0x00002027; // 27 20 00 00
    i |= (((imm >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    i |= (((imm >> 5) & 0b1111111) << 25);
    return i;
}

uint32_t cfromptr(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x2600005B; // 5B 00 00 26
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t cgetaddr(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFEF0005B; // 5B 00 F0 FE
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t cgetbase(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFE20005B; // 5B 00 20 FE
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t cgetflags(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFE70005B; // 5B 00 70 FE
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t cgetlen(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFE30005B; // 5B 00 30 FE
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t cgetoffset(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFE60005B; // 5B 00 60 FE
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t cgetperm(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFE00005B; // 5B 00 00 FE
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t cgetsealed(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFE50005B; // 5B 00 50 FE
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t cgettag(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFE40005B; // 5B 00 40 FE
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t cgettype(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFE10005B; // 5B 00 10 FE
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t cincoffset(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x2200005B; // 5B 00 00 22
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t cincoffsetimm(uint32_t rd, uint32_t rs1, uint32_t imm)
{
    uint32_t i = 0x0000105B; // 5B 10 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((imm >> 0) & 0b111111111111) << 20);
    return i;
}

uint32_t cjalr(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFEC0005B; // 5B 00 C0 FE
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t clb(uint32_t rd, uint32_t rs1, uint32_t imm)
{
    uint32_t i = 0x00000003; // 03 00 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((imm >> 0) & 0b111111111111) << 20);
    return i;
}

uint32_t clbu(uint32_t rd, uint32_t rs1, uint32_t imm)
{
    uint32_t i = 0x00004003; // 03 40 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((imm >> 0) & 0b111111111111) << 20);
    return i;
}

uint32_t clc_128(uint32_t rd, uint32_t rs1, uint32_t imm)
{
    uint32_t i = 0x0000200F; // 0F 20 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((imm >> 0) & 0b111111111111) << 20);
    return i;
}

uint32_t clc_64(uint32_t rd, uint32_t rs1, uint32_t imm)
{
    uint32_t i = 0x00003003; // 03 30 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((imm >> 0) & 0b111111111111) << 20);
    return i;
}

uint32_t cld(uint32_t rd, uint32_t rs1, uint32_t imm)
{
    uint32_t i = 0x00003003; // 03 30 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((imm >> 0) & 0b111111111111) << 20);
    return i;
}

uint32_t clh(uint32_t rd, uint32_t rs1, uint32_t imm)
{
    uint32_t i = 0x00001003; // 03 10 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((imm >> 0) & 0b111111111111) << 20);
    return i;
}

uint32_t clhu(uint32_t rd, uint32_t rs1, uint32_t imm)
{
    uint32_t i = 0x00005003; // 03 50 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((imm >> 0) & 0b111111111111) << 20);
    return i;
}

uint32_t clw(uint32_t rd, uint32_t rs1, uint32_t imm)
{
    uint32_t i = 0x00002003; // 03 20 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((imm >> 0) & 0b111111111111) << 20);
    return i;
}

uint32_t clwu(uint32_t rd, uint32_t rs1, uint32_t imm)
{
    uint32_t i = 0x00006003; // 03 60 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((imm >> 0) & 0b111111111111) << 20);
    return i;
}

uint32_t cmove(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFEA0005B; // 5B 00 A0 FE
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t cram(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFE90005B; // 5B 00 90 FE
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t crrl(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFE80005B; // 5B 00 80 FE
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t csb(uint32_t rs1, uint32_t rs2, uint32_t imm)
{
    uint32_t i = 0x00000023; // 23 00 00 00
    i |= (((imm >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    i |= (((imm >> 5) & 0b1111111) << 25);
    return i;
}

uint32_t csc_128(uint32_t rs1, uint32_t rs2, uint32_t imm)
{
    uint32_t i = 0x00004023; // 23 40 00 00
    i |= (((imm >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    i |= (((imm >> 5) & 0b1111111) << 25);
    return i;
}

uint32_t csc_64(uint32_t rs1, uint32_t rs2, uint32_t imm)
{
    uint32_t i = 0x00003023; // 23 30 00 00
    i |= (((imm >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    i |= (((imm >> 5) & 0b1111111) << 25);
    return i;
}

uint32_t csd(uint32_t rs1, uint32_t rs2, uint32_t imm)
{
    uint32_t i = 0x00003023; // 23 30 00 00
    i |= (((imm >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    i |= (((imm >> 5) & 0b1111111) << 25);
    return i;
}

uint32_t cseqx(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x4200005B; // 5B 00 00 42
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t csh(uint32_t rs1, uint32_t rs2, uint32_t imm)
{
    uint32_t i = 0x00001023; // 23 10 00 00
    i |= (((imm >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    i |= (((imm >> 5) & 0b1111111) << 25);
    return i;
}

uint32_t csrrc(uint32_t rd, uint32_t rs1, uint32_t imm)
{
    uint32_t i = 0x00003073; // 73 30 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((imm >> 0) & 0b111111111111) << 20);
    return i;
}

uint32_t csrrci(uint32_t rd, uint32_t rs1, uint32_t imm)
{
    uint32_t i = 0x00007073; // 73 70 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((imm >> 0) & 0b111111111111) << 20);
    return i;
}

uint32_t csrrs(uint32_t rd, uint32_t rs1, uint32_t imm)
{
    uint32_t i = 0x00002073; // 73 20 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((imm >> 0) & 0b111111111111) << 20);
    return i;
}

uint32_t csrrsi(uint32_t rd, uint32_t rs1, uint32_t imm)
{
    uint32_t i = 0x00006073; // 73 60 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((imm >> 0) & 0b111111111111) << 20);
    return i;
}

uint32_t csrrw(uint32_t rd, uint32_t rs1, uint32_t imm)
{
    uint32_t i = 0x00001073; // 73 10 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((imm >> 0) & 0b111111111111) << 20);
    return i;
}

uint32_t csrrwi(uint32_t rd, uint32_t rs1, uint32_t imm)
{
    uint32_t i = 0x00005073; // 73 50 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((imm >> 0) & 0b111111111111) << 20);
    return i;
}

uint32_t csw(uint32_t rs1, uint32_t rs2, uint32_t imm)
{
    uint32_t i = 0x00002023; // 23 20 00 00
    i |= (((imm >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    i |= (((imm >> 5) & 0b1111111) << 25);
    return i;
}

uint32_t cseal(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x1600005B; // 5B 00 00 16
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t csealentry(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFF10005B; // 5B 00 10 FF
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t csetaddr(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x2000005B; // 5B 00 00 20
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t csetbounds(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x1000005B; // 5B 00 00 10
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t csetboundsexact(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x1200005B; // 5B 00 00 12
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t csetboundsimm(uint32_t rd, uint32_t rs1, uint32_t imm)
{
    uint32_t i = 0x0000205B; // 5B 20 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((imm >> 0) & 0b111111111111) << 20);
    return i;
}

uint32_t csetflags(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x1C00005B; // 5B 00 00 1C
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t csetoffset(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x1E00005B; // 5B 00 00 1E
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t cspecialrw(uint32_t rd, uint32_t rs1, uint32_t imm)
{
    uint32_t i = 0x0200005B; // 5B 00 00 02
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((imm >> 0) & 0b11111) << 20);
    return i;
}

uint32_t csub(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x2800005B; // 5B 00 00 28
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t ctestsubset(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x4000005B; // 5B 00 00 40
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t ctoptr(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x2400005B; // 5B 00 00 24
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t cunseal(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x1800005B; // 5B 00 00 18
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t clear(uint32_t quarter, uint32_t mask)
{
    uint32_t i = 0xFED0005B; // 5B 00 D0 FE
    i |= (((mask >> 0) & 0b11111) << 7);
    i |= (((mask >> 5) & 0b111) << 15);
    i |= (((quarter >> 0) & 0b11) << 18);
    return i;
}

uint32_t asm_div(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x02004033; // 33 40 00 02
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t divu(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x02005033; // 33 50 00 02
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t divuw(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x0200503B; // 3B 50 00 02
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t divw(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x0200403B; // 3B 40 00 02
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t dret()
{
    uint32_t i = 0x7B200073; // 73 00 20 7B

    return i;
}

uint32_t ebreak()
{
    uint32_t i = 0x00100073; // 73 00 10 00

    return i;
}

uint32_t ecall()
{
    uint32_t i = 0x00000073; // 73 00 00 00

    return i;
}

uint32_t fence(uint32_t succ, uint32_t pred)
{
    uint32_t i = 0x0000000F; // 0F 00 00 00
    i |= (((succ >> 0) & 0b1111) << 20);
    i |= (((pred >> 0) & 0b1111) << 24);
    return i;
}

uint32_t fence_i()
{
    uint32_t i = 0x0000100F; // 0F 10 00 00

    return i;
}

uint32_t fence_tso()
{
    uint32_t i = 0x8330000F; // 0F 00 30 83

    return i;
}

uint32_t fpclear(uint32_t quarter, uint32_t mask)
{
    uint32_t i = 0xFF00005B; // 5B 00 00 FF
    i |= (((mask >> 0) & 0b11111) << 7);
    i |= (((mask >> 5) & 0b111) << 15);
    i |= (((quarter >> 0) & 0b11) << 18);
    return i;
}

uint32_t jal(uint32_t rd, uint32_t imm)
{
    uint32_t i = 0x0000006F; // 6F 00 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((imm >> 11) & 0b11111111) << 12);
    i |= (((imm >> 10) & 0b1) << 20);
    i |= (((imm >> 0) & 0b1111111111) << 21);
    i |= (((imm >> 19) & 0b1) << 31);
    return i;
}

uint32_t jalr(uint32_t rd, uint32_t rs1, uint32_t imm)
{
    uint32_t i = 0x00000067; // 67 00 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((imm >> 0) & 0b111111111111) << 20);
    return i;
}

uint32_t lb(uint32_t rd, uint32_t rs1, uint32_t imm)
{
    uint32_t i = 0x00000003; // 03 00 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((imm >> 0) & 0b111111111111) << 20);
    return i;
}

uint32_t lbu(uint32_t rd, uint32_t rs1, uint32_t imm)
{
    uint32_t i = 0x00004003; // 03 40 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((imm >> 0) & 0b111111111111) << 20);
    return i;
}

uint32_t lbu_cap(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFAC0005B; // 5B 00 C0 FA
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t lbu_ddc(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFA40005B; // 5B 00 40 FA
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t lb_cap(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFA80005B; // 5B 00 80 FA
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t lb_ddc(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFA00005B; // 5B 00 00 FA
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t lc_128(uint32_t rd, uint32_t rs1, uint32_t imm)
{
    uint32_t i = 0x0000200F; // 0F 20 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((imm >> 0) & 0b111111111111) << 20);
    return i;
}

uint32_t lc_64(uint32_t rd, uint32_t rs1, uint32_t imm)
{
    uint32_t i = 0x00003003; // 03 30 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((imm >> 0) & 0b111111111111) << 20);
    return i;
}

uint32_t lc_cap_128(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFBF0005B; // 5B 00 F0 FB
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t lc_cap_64(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFAB0005B; // 5B 00 B0 FA
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t lc_ddc_128(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFB70005B; // 5B 00 70 FB
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t lc_ddc_64(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFA30005B; // 5B 00 30 FA
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t ld(uint32_t rd, uint32_t rs1, uint32_t imm)
{
    uint32_t i = 0x00003003; // 03 30 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((imm >> 0) & 0b111111111111) << 20);
    return i;
}

uint32_t ld_cap(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFAB0005B; // 5B 00 B0 FA
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t ld_ddc(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFA30005B; // 5B 00 30 FA
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t lh(uint32_t rd, uint32_t rs1, uint32_t imm)
{
    uint32_t i = 0x00001003; // 03 10 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((imm >> 0) & 0b111111111111) << 20);
    return i;
}

uint32_t lhu(uint32_t rd, uint32_t rs1, uint32_t imm)
{
    uint32_t i = 0x00005003; // 03 50 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((imm >> 0) & 0b111111111111) << 20);
    return i;
}

uint32_t lhu_cap(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFAD0005B; // 5B 00 D0 FA
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t lhu_ddc(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFA50005B; // 5B 00 50 FA
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t lh_cap(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFA90005B; // 5B 00 90 FA
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t lh_ddc(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFA10005B; // 5B 00 10 FA
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t lr_b_cap(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFB80005B; // 5B 00 80 FB
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t lr_b_ddc(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFB00005B; // 5B 00 00 FB
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t lr_c_cap_128(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFBC0005B; // 5B 00 C0 FB
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t lr_c_cap_64(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFBB0005B; // 5B 00 B0 FB
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t lr_c_ddc_128(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFB40005B; // 5B 00 40 FB
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t lr_c_ddc_64(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFB30005B; // 5B 00 30 FB
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t lr_d_cap(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFBB0005B; // 5B 00 B0 FB
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t lr_d_ddc(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFB30005B; // 5B 00 30 FB
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t lr_h_cap(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFB90005B; // 5B 00 90 FB
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t lr_h_ddc(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFB10005B; // 5B 00 10 FB
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t lr_w_cap(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFBA0005B; // 5B 00 A0 FB
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t lr_w_ddc(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFB20005B; // 5B 00 20 FB
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t lui(uint32_t rd, uint32_t imm)
{
    uint32_t i = 0x00000037; // 37 00 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((imm >> 0) & 0b11111111111111111111) << 12);
    return i;
}

uint32_t lw(uint32_t rd, uint32_t rs1, uint32_t imm)
{
    uint32_t i = 0x00002003; // 03 20 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((imm >> 0) & 0b111111111111) << 20);
    return i;
}

uint32_t lwu(uint32_t rd, uint32_t rs1, uint32_t imm)
{
    uint32_t i = 0x00006003; // 03 60 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((imm >> 0) & 0b111111111111) << 20);
    return i;
}

uint32_t lwu_cap(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFAE0005B; // 5B 00 E0 FA
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t lwu_ddc(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFA60005B; // 5B 00 60 FA
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t lw_cap(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFAA0005B; // 5B 00 A0 FA
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t lw_ddc(uint32_t rd, uint32_t rs1)
{
    uint32_t i = 0xFA20005B; // 5B 00 20 FA
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    return i;
}

uint32_t mret()
{
    uint32_t i = 0x30200073; // 73 00 20 30

    return i;
}

uint32_t mul(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x02000033; // 33 00 00 02
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t mulh(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x02001033; // 33 10 00 02
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t mulhsu(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x02002033; // 33 20 00 02
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t mulhu(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x02003033; // 33 30 00 02
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t mulw(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x0200003B; // 3B 00 00 02
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t or (uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x00006033; // 33 60 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t ori(uint32_t rd, uint32_t rs1, uint32_t imm)
{
    uint32_t i = 0x00006013; // 13 60 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((imm >> 0) & 0b111111111111) << 20);
    return i;
}

uint32_t rem(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x02006033; // 33 60 00 02
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t remu(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x02007033; // 33 70 00 02
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t remuw(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x0200703B; // 3B 70 00 02
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t remw(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x0200603B; // 3B 60 00 02
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t sb(uint32_t rs1, uint32_t rs2, uint32_t imm)
{
    uint32_t i = 0x00000023; // 23 00 00 00
    i |= (((imm >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    i |= (((imm >> 5) & 0b1111111) << 25);
    return i;
}

uint32_t sb_cap(uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0xF800045B; // 5B 04 00 F8
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t sb_ddc(uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0xF800005B; // 5B 00 00 F8
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t sc_128(uint32_t rs1, uint32_t rs2, uint32_t imm)
{
    uint32_t i = 0x00004023; // 23 40 00 00
    i |= (((imm >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    i |= (((imm >> 5) & 0b1111111) << 25);
    return i;
}

uint32_t sc_64(uint32_t rs1, uint32_t rs2, uint32_t imm)
{
    uint32_t i = 0x00003023; // 23 30 00 00
    i |= (((imm >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    i |= (((imm >> 5) & 0b1111111) << 25);
    return i;
}

uint32_t sc_b_cap(uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0xF8000C5B; // 5B 0C 00 F8
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t sc_b_ddc(uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0xF800085B; // 5B 08 00 F8
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t sc_cap_128(uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0xF800065B; // 5B 06 00 F8
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t sc_cap_64(uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0xF80005DB; // DB 05 00 F8
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t sc_c_cap_128(uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0xF8000E5B; // 5B 0E 00 F8
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t sc_c_cap_64(uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0xF8000DDB; // DB 0D 00 F8
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t sc_c_ddc_128(uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0xF8000A5B; // 5B 0A 00 F8
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t sc_c_ddc_64(uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0xF80009DB; // DB 09 00 F8
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t sc_ddc_128(uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0xF800025B; // 5B 02 00 F8
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t sc_ddc_64(uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0xF80001DB; // DB 01 00 F8
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t sc_d_cap(uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0xF8000DDB; // DB 0D 00 F8
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t sc_d_ddc(uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0xF80009DB; // DB 09 00 F8
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t sc_h_cap(uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0xF8000CDB; // DB 0C 00 F8
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t sc_h_ddc(uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0xF80008DB; // DB 08 00 F8
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t sc_w_cap(uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0xF8000D5B; // 5B 0D 00 F8
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t sc_w_ddc(uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0xF800095B; // 5B 09 00 F8
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t sd(uint32_t rs1, uint32_t rs2, uint32_t imm)
{
    uint32_t i = 0x00003023; // 23 30 00 00
    i |= (((imm >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    i |= (((imm >> 5) & 0b1111111) << 25);
    return i;
}

uint32_t sd_cap(uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0xF80005DB; // DB 05 00 F8
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t sd_ddc(uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0xF80001DB; // DB 01 00 F8
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t sfence_vma(uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x12000073; // 73 00 00 12
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t sh(uint32_t rs1, uint32_t rs2, uint32_t imm)
{
    uint32_t i = 0x00001023; // 23 10 00 00
    i |= (((imm >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    i |= (((imm >> 5) & 0b1111111) << 25);
    return i;
}

uint32_t sh_cap(uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0xF80004DB; // DB 04 00 F8
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t sh_ddc(uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0xF80000DB; // DB 00 00 F8
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t sll(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x00001033; // 33 10 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t sllw(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x0000103B; // 3B 10 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t slt(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x00002033; // 33 20 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t slti(uint32_t rd, uint32_t rs1, uint32_t imm)
{
    uint32_t i = 0x00002013; // 13 20 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((imm >> 0) & 0b111111111111) << 20);
    return i;
}

uint32_t sltiu(uint32_t rd, uint32_t rs1, uint32_t imm)
{
    uint32_t i = 0x00003013; // 13 30 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((imm >> 0) & 0b111111111111) << 20);
    return i;
}

uint32_t sltu(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x00003033; // 33 30 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t sra(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x40005033; // 33 50 00 40
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t sraw(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x4000503B; // 3B 50 00 40
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t sret()
{
    uint32_t i = 0x10200073; // 73 00 20 10

    return i;
}

uint32_t srl(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x00005033; // 33 50 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t srlw(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x0000503B; // 3B 50 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t sub(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x40000033; // 33 00 00 40
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t subw(uint32_t rd, uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0x4000003B; // 3B 00 00 40
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t sw(uint32_t rs1, uint32_t rs2, uint32_t imm)
{
    uint32_t i = 0x00002023; // 23 20 00 00
    i |= (((imm >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    i |= (((imm >> 5) & 0b1111111) << 25);
    return i;
}

uint32_t sw_cap(uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0xF800055B; // 5B 05 00 F8
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t sw_ddc(uint32_t rs1, uint32_t rs2)
{
    uint32_t i = 0xF800015B; // 5B 01 00 F8
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((rs2 >> 0) & 0b11111) << 20);
    return i;
}

uint32_t unimp()
{
    uint32_t i = 0xC0001073; // 73 10 00 C0

    return i;
}

uint32_t uret()
{
    uint32_t i = 0x00200073; // 73 00 20 00

    return i;
}

uint32_t wfi()
{
    uint32_t i = 0x10500073; // 73 00 50 10

    return i;
}

uint32_t xor
    (uint32_t rd, uint32_t rs1, uint32_t rs2) {
        uint32_t i = 0x00004033; // 33 40 00 00
        i |= (((rd >> 0) & 0b11111) << 7);
        i |= (((rs1 >> 0) & 0b11111) << 15);
        i |= (((rs2 >> 0) & 0b11111) << 20);
        return i;
    }

    uint32_t xori(uint32_t rd, uint32_t rs1, uint32_t imm)
{
    uint32_t i = 0x00004013; // 13 40 00 00
    i |= (((rd >> 0) & 0b11111) << 7);
    i |= (((rs1 >> 0) & 0b11111) << 15);
    i |= (((imm >> 0) & 0b111111111111) << 20);
    return i;
}
