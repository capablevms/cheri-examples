#include<stdint.h>


 uint32_t add(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 51; // 33 00 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t addi(uint32_t rd, uint32_t rs1, uint32_t imm) {
	uint32_t i; i = 19; // 13 00 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( imm >> 0 ) & 0b111111111111) << 20);
	return i;
}

uint32_t addiw(uint32_t rd, uint32_t rs1, uint32_t imm) {
	uint32_t i; i = 27; // 1B 00 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( imm >> 0 ) & 0b111111111111) << 20);
	return i;
}

uint32_t addw(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 59; // 3B 00 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t and(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 28723; // 33 70 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t andi(uint32_t rd, uint32_t rs1, uint32_t imm) {
	uint32_t i; i = 28691; // 13 70 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( imm >> 0 ) & 0b111111111111) << 20);
	return i;
}

uint32_t auipc(uint32_t rd, uint32_t imm) {
	uint32_t i; i = 23; // 17 00 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( imm >> 0 ) & 0b11111111111111111111) << 12);
	return i;
}

uint32_t auipcc(uint32_t rd, uint32_t imm) {
	uint32_t i; i = 23; // 17 00 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( imm >> 0 ) & 0b11111111111111111111) << 12);
	return i;
}

uint32_t beq(uint32_t rs1, uint32_t rs2, uint32_t imm) {
	uint32_t i; i = 99; // 63 00 00 00  
	i |= ((( imm >> 10 ) & 0b1) << 7);
	i |= ((( imm >> 0 ) & 0b1111) << 8);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	i |= ((( imm >> 4 ) & 0b111111) << 25);
	i |= ((( imm >> 11 ) & 0b1) << 31);
	return i;
}

uint32_t bge(uint32_t rs1, uint32_t rs2, uint32_t imm) {
	uint32_t i; i = 20579; // 63 50 00 00  
	i |= ((( imm >> 10 ) & 0b1) << 7);
	i |= ((( imm >> 0 ) & 0b1111) << 8);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	i |= ((( imm >> 4 ) & 0b111111) << 25);
	i |= ((( imm >> 11 ) & 0b1) << 31);
	return i;
}

uint32_t bgeu(uint32_t rs1, uint32_t rs2, uint32_t imm) {
	uint32_t i; i = 28771; // 63 70 00 00  
	i |= ((( imm >> 10 ) & 0b1) << 7);
	i |= ((( imm >> 0 ) & 0b1111) << 8);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	i |= ((( imm >> 4 ) & 0b111111) << 25);
	i |= ((( imm >> 11 ) & 0b1) << 31);
	return i;
}

uint32_t blt(uint32_t rs1, uint32_t rs2, uint32_t imm) {
	uint32_t i; i = 16483; // 63 40 00 00  
	i |= ((( imm >> 10 ) & 0b1) << 7);
	i |= ((( imm >> 0 ) & 0b1111) << 8);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	i |= ((( imm >> 4 ) & 0b111111) << 25);
	i |= ((( imm >> 11 ) & 0b1) << 31);
	return i;
}

uint32_t bltu(uint32_t rs1, uint32_t rs2, uint32_t imm) {
	uint32_t i; i = 24675; // 63 60 00 00  
	i |= ((( imm >> 10 ) & 0b1) << 7);
	i |= ((( imm >> 0 ) & 0b1111) << 8);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	i |= ((( imm >> 4 ) & 0b111111) << 25);
	i |= ((( imm >> 11 ) & 0b1) << 31);
	return i;
}

uint32_t bne(uint32_t rs1, uint32_t rs2, uint32_t imm) {
	uint32_t i; i = 4195; // 63 10 00 00  
	i |= ((( imm >> 10 ) & 0b1) << 7);
	i |= ((( imm >> 0 ) & 0b1111) << 8);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	i |= ((( imm >> 4 ) & 0b111111) << 25);
	i |= ((( imm >> 11 ) & 0b1) << 31);
	return i;
}

uint32_t candperm(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 436207707; // 5B 00 00 1A  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t cbuildcap(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 973078619; // 5B 00 00 3A  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t ccseal(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 1040187483; // 5B 00 00 3E  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t ccall(uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 4227858651; // DB 00 00 FC  
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t ccleartag(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4272947291; // 5B 00 B0 FE  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t ccopytype(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 1006633051; // 5B 00 00 3C  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t cfld(uint32_t rd, uint32_t rs1, uint32_t imm) {
	uint32_t i; i = 12295; // 07 30 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( imm >> 0 ) & 0b111111111111) << 20);
	return i;
}

uint32_t cflw(uint32_t rd, uint32_t rs1, uint32_t imm) {
	uint32_t i; i = 8199; // 07 20 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( imm >> 0 ) & 0b111111111111) << 20);
	return i;
}

uint32_t cfsd(uint32_t rs1, uint32_t rs2, uint32_t imm) {
	uint32_t i; i = 12327; // 27 30 00 00  
	i |= ((( imm >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	i |= ((( imm >> 5 ) & 0b1111111) << 25);
	return i;
}

uint32_t cfsw(uint32_t rs1, uint32_t rs2, uint32_t imm) {
	uint32_t i; i = 8231; // 27 20 00 00  
	i |= ((( imm >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	i |= ((( imm >> 5 ) & 0b1111111) << 25);
	return i;
}

uint32_t cfromptr(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 637534299; // 5B 00 00 26  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t cgetaddr(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4277141595; // 5B 00 F0 FE  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t cgetbase(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4263510107; // 5B 00 20 FE  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t cgetflags(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4268752987; // 5B 00 70 FE  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t cgetlen(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4264558683; // 5B 00 30 FE  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t cgetoffset(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4267704411; // 5B 00 60 FE  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t cgetperm(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4261412955; // 5B 00 00 FE  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t cgetsealed(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4266655835; // 5B 00 50 FE  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t cgettag(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4265607259; // 5B 00 40 FE  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t cgettype(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4262461531; // 5B 00 10 FE  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t cincoffset(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 570425435; // 5B 00 00 22  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t cincoffsetimm(uint32_t rd, uint32_t rs1, uint32_t imm) {
	uint32_t i; i = 4187; // 5B 10 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( imm >> 0 ) & 0b111111111111) << 20);
	return i;
}

uint32_t cjalr(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4273995867; // 5B 00 C0 FE  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t clb(uint32_t rd, uint32_t rs1, uint32_t imm) {
	uint32_t i; i = 3; // 03 00 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( imm >> 0 ) & 0b111111111111) << 20);
	return i;
}

uint32_t clbu(uint32_t rd, uint32_t rs1, uint32_t imm) {
	uint32_t i; i = 16387; // 03 40 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( imm >> 0 ) & 0b111111111111) << 20);
	return i;
}

uint32_t clc_128(uint32_t rd, uint32_t rs1, uint32_t imm) {
	uint32_t i; i = 8207; // 0F 20 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( imm >> 0 ) & 0b111111111111) << 20);
	return i;
}

uint32_t clc_64(uint32_t rd, uint32_t rs1, uint32_t imm) {
	uint32_t i; i = 12291; // 03 30 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( imm >> 0 ) & 0b111111111111) << 20);
	return i;
}

uint32_t cld(uint32_t rd, uint32_t rs1, uint32_t imm) {
	uint32_t i; i = 12291; // 03 30 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( imm >> 0 ) & 0b111111111111) << 20);
	return i;
}

uint32_t clh(uint32_t rd, uint32_t rs1, uint32_t imm) {
	uint32_t i; i = 4099; // 03 10 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( imm >> 0 ) & 0b111111111111) << 20);
	return i;
}

uint32_t clhu(uint32_t rd, uint32_t rs1, uint32_t imm) {
	uint32_t i; i = 20483; // 03 50 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( imm >> 0 ) & 0b111111111111) << 20);
	return i;
}

uint32_t clw(uint32_t rd, uint32_t rs1, uint32_t imm) {
	uint32_t i; i = 8195; // 03 20 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( imm >> 0 ) & 0b111111111111) << 20);
	return i;
}

uint32_t clwu(uint32_t rd, uint32_t rs1, uint32_t imm) {
	uint32_t i; i = 24579; // 03 60 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( imm >> 0 ) & 0b111111111111) << 20);
	return i;
}

uint32_t cmove(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4271898715; // 5B 00 A0 FE  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t cram(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4270850139; // 5B 00 90 FE  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t crrl(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4269801563; // 5B 00 80 FE  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t csb(uint32_t rs1, uint32_t rs2, uint32_t imm) {
	uint32_t i; i = 35; // 23 00 00 00  
	i |= ((( imm >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	i |= ((( imm >> 5 ) & 0b1111111) << 25);
	return i;
}

uint32_t csc_128(uint32_t rs1, uint32_t rs2, uint32_t imm) {
	uint32_t i; i = 16419; // 23 40 00 00  
	i |= ((( imm >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	i |= ((( imm >> 5 ) & 0b1111111) << 25);
	return i;
}

uint32_t csc_64(uint32_t rs1, uint32_t rs2, uint32_t imm) {
	uint32_t i; i = 12323; // 23 30 00 00  
	i |= ((( imm >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	i |= ((( imm >> 5 ) & 0b1111111) << 25);
	return i;
}

uint32_t csd(uint32_t rs1, uint32_t rs2, uint32_t imm) {
	uint32_t i; i = 12323; // 23 30 00 00  
	i |= ((( imm >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	i |= ((( imm >> 5 ) & 0b1111111) << 25);
	return i;
}

uint32_t cseqx(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 1107296347; // 5B 00 00 42  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t csh(uint32_t rs1, uint32_t rs2, uint32_t imm) {
	uint32_t i; i = 4131; // 23 10 00 00  
	i |= ((( imm >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	i |= ((( imm >> 5 ) & 0b1111111) << 25);
	return i;
}

uint32_t csrrc(uint32_t rd, uint32_t rs1, uint32_t imm) {
	uint32_t i; i = 12403; // 73 30 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( imm >> 0 ) & 0b111111111111) << 20);
	return i;
}

uint32_t csrrci(uint32_t rd, uint32_t rs1, uint32_t imm) {
	uint32_t i; i = 28787; // 73 70 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( imm >> 0 ) & 0b111111111111) << 20);
	return i;
}

uint32_t csrrs(uint32_t rd, uint32_t rs1, uint32_t imm) {
	uint32_t i; i = 8307; // 73 20 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( imm >> 0 ) & 0b111111111111) << 20);
	return i;
}

uint32_t csrrsi(uint32_t rd, uint32_t rs1, uint32_t imm) {
	uint32_t i; i = 24691; // 73 60 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( imm >> 0 ) & 0b111111111111) << 20);
	return i;
}

uint32_t csrrw(uint32_t rd, uint32_t rs1, uint32_t imm) {
	uint32_t i; i = 4211; // 73 10 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( imm >> 0 ) & 0b111111111111) << 20);
	return i;
}

uint32_t csrrwi(uint32_t rd, uint32_t rs1, uint32_t imm) {
	uint32_t i; i = 20595; // 73 50 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( imm >> 0 ) & 0b111111111111) << 20);
	return i;
}

uint32_t csw(uint32_t rs1, uint32_t rs2, uint32_t imm) {
	uint32_t i; i = 8227; // 23 20 00 00  
	i |= ((( imm >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	i |= ((( imm >> 5 ) & 0b1111111) << 25);
	return i;
}

uint32_t cseal(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 369098843; // 5B 00 00 16  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t csealentry(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4279238747; // 5B 00 10 FF  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t csetaddr(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 536871003; // 5B 00 00 20  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t csetbounds(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 268435547; // 5B 00 00 10  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t csetboundsexact(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 301989979; // 5B 00 00 12  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t csetboundsimm(uint32_t rd, uint32_t rs1, uint32_t imm) {
	uint32_t i; i = 8283; // 5B 20 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( imm >> 0 ) & 0b111111111111) << 20);
	return i;
}

uint32_t csetflags(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 469762139; // 5B 00 00 1C  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t csetoffset(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 503316571; // 5B 00 00 1E  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t cspecialrw(uint32_t rd, uint32_t rs1, uint32_t imm) {
	uint32_t i; i = 33554523; // 5B 00 00 02  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( imm >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t csub(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 671088731; // 5B 00 00 28  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t ctestsubset(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 1073741915; // 5B 00 00 40  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t ctoptr(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 603979867; // 5B 00 00 24  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t cunseal(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 402653275; // 5B 00 00 18  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t clear(uint32_t quarter, uint32_t mask) {
	uint32_t i; i = 4275044443; // 5B 00 D0 FE  
	i |= ((( mask >> 0 ) & 0b11111) << 7);
	i |= ((( mask >> 5 ) & 0b111) << 15);
	i |= ((( quarter >> 0 ) & 0b11) << 18);
	return i;
}

uint32_t inst_div(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 33570867; // 33 40 00 02  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t divu(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 33574963; // 33 50 00 02  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t divuw(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 33574971; // 3B 50 00 02  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t divw(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 33570875; // 3B 40 00 02  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t dret() {
	uint32_t i; i = 2065694835; // 73 00 20 7B  
	
	return i;
}

uint32_t ebreak() {
	uint32_t i; i = 1048691; // 73 00 10 00  
	
	return i;
}

uint32_t ecall() {
	uint32_t i; i = 115; // 73 00 00 00  
	
	return i;
}

uint32_t fpclear(uint32_t quarter, uint32_t mask) {
	uint32_t i; i = 4278190171; // 5B 00 00 FF  
	i |= ((( mask >> 0 ) & 0b11111) << 7);
	i |= ((( mask >> 5 ) & 0b111) << 15);
	i |= ((( quarter >> 0 ) & 0b11) << 18);
	return i;
}

uint32_t jal(uint32_t rd, uint32_t imm) {
	uint32_t i; i = 111; // 6F 00 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( imm >> 11 ) & 0b11111111) << 12);
	i |= ((( imm >> 10 ) & 0b1) << 20);
	i |= ((( imm >> 0 ) & 0b1111111111) << 21);
	i |= ((( imm >> 19 ) & 0b1) << 31);
	return i;
}

uint32_t jalr(uint32_t rd, uint32_t rs1, uint32_t imm) {
	uint32_t i; i = 103; // 67 00 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( imm >> 0 ) & 0b111111111111) << 20);
	return i;
}

uint32_t lb(uint32_t rd, uint32_t rs1, uint32_t imm) {
	uint32_t i; i = 3; // 03 00 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( imm >> 0 ) & 0b111111111111) << 20);
	return i;
}

uint32_t lbu(uint32_t rd, uint32_t rs1, uint32_t imm) {
	uint32_t i; i = 16387; // 03 40 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( imm >> 0 ) & 0b111111111111) << 20);
	return i;
}

uint32_t lbu_cap(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4206887003; // 5B 00 C0 FA  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t lbu_ddc(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4198498395; // 5B 00 40 FA  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t lb_cap(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4202692699; // 5B 00 80 FA  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t lb_ddc(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4194304091; // 5B 00 00 FA  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t lc_128(uint32_t rd, uint32_t rs1, uint32_t imm) {
	uint32_t i; i = 8207; // 0F 20 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( imm >> 0 ) & 0b111111111111) << 20);
	return i;
}

uint32_t lc_64(uint32_t rd, uint32_t rs1, uint32_t imm) {
	uint32_t i; i = 12291; // 03 30 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( imm >> 0 ) & 0b111111111111) << 20);
	return i;
}

uint32_t lc_cap_128(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4226809947; // 5B 00 F0 FB  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t lc_cap_64(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4205838427; // 5B 00 B0 FA  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t lc_ddc_128(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4218421339; // 5B 00 70 FB  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t lc_ddc_64(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4197449819; // 5B 00 30 FA  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t ld(uint32_t rd, uint32_t rs1, uint32_t imm) {
	uint32_t i; i = 12291; // 03 30 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( imm >> 0 ) & 0b111111111111) << 20);
	return i;
}

uint32_t ld_cap(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4205838427; // 5B 00 B0 FA  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t ld_ddc(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4197449819; // 5B 00 30 FA  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t lh(uint32_t rd, uint32_t rs1, uint32_t imm) {
	uint32_t i; i = 4099; // 03 10 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( imm >> 0 ) & 0b111111111111) << 20);
	return i;
}

uint32_t lhu(uint32_t rd, uint32_t rs1, uint32_t imm) {
	uint32_t i; i = 20483; // 03 50 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( imm >> 0 ) & 0b111111111111) << 20);
	return i;
}

uint32_t lhu_cap(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4207935579; // 5B 00 D0 FA  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t lhu_ddc(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4199546971; // 5B 00 50 FA  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t lh_cap(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4203741275; // 5B 00 90 FA  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t lh_ddc(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4195352667; // 5B 00 10 FA  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t lr_b_cap(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4219469915; // 5B 00 80 FB  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t lr_b_ddc(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4211081307; // 5B 00 00 FB  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t lr_c_cap_128(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4223664219; // 5B 00 C0 FB  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t lr_c_cap_64(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4222615643; // 5B 00 B0 FB  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t lr_c_ddc_128(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4215275611; // 5B 00 40 FB  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t lr_c_ddc_64(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4214227035; // 5B 00 30 FB  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t lr_d_cap(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4222615643; // 5B 00 B0 FB  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t lr_d_ddc(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4214227035; // 5B 00 30 FB  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t lr_h_cap(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4220518491; // 5B 00 90 FB  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t lr_h_ddc(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4212129883; // 5B 00 10 FB  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t lr_w_cap(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4221567067; // 5B 00 A0 FB  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t lr_w_ddc(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4213178459; // 5B 00 20 FB  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t lui(uint32_t rd, uint32_t imm) {
	uint32_t i; i = 55; // 37 00 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( imm >> 0 ) & 0b11111111111111111111) << 12);
	return i;
}

uint32_t lw(uint32_t rd, uint32_t rs1, uint32_t imm) {
	uint32_t i; i = 8195; // 03 20 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( imm >> 0 ) & 0b111111111111) << 20);
	return i;
}

uint32_t lwu(uint32_t rd, uint32_t rs1, uint32_t imm) {
	uint32_t i; i = 24579; // 03 60 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( imm >> 0 ) & 0b111111111111) << 20);
	return i;
}

uint32_t lwu_cap(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4208984155; // 5B 00 E0 FA  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t lwu_ddc(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4200595547; // 5B 00 60 FA  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t lw_cap(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4204789851; // 5B 00 A0 FA  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t lw_ddc(uint32_t rd, uint32_t rs1) {
	uint32_t i; i = 4196401243; // 5B 00 20 FA  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	return i;
}

uint32_t mret() {
	uint32_t i; i = 807403635; // 73 00 20 30  
	
	return i;
}

uint32_t mul(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 33554483; // 33 00 00 02  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t mulh(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 33558579; // 33 10 00 02  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t mulhsu(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 33562675; // 33 20 00 02  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t mulhu(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 33566771; // 33 30 00 02  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t mulw(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 33554491; // 3B 00 00 02  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t or(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 24627; // 33 60 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t ori(uint32_t rd, uint32_t rs1, uint32_t imm) {
	uint32_t i; i = 24595; // 13 60 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( imm >> 0 ) & 0b111111111111) << 20);
	return i;
}

uint32_t rem(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 33579059; // 33 60 00 02  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t remu(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 33583155; // 33 70 00 02  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t remuw(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 33583163; // 3B 70 00 02  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t remw(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 33579067; // 3B 60 00 02  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t sb(uint32_t rs1, uint32_t rs2, uint32_t imm) {
	uint32_t i; i = 35; // 23 00 00 00  
	i |= ((( imm >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	i |= ((( imm >> 5 ) & 0b1111111) << 25);
	return i;
}

uint32_t sb_cap(uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 4160750683; // 5B 04 00 F8  
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t sb_ddc(uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 4160749659; // 5B 00 00 F8  
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t sc_128(uint32_t rs1, uint32_t rs2, uint32_t imm) {
	uint32_t i; i = 16419; // 23 40 00 00  
	i |= ((( imm >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	i |= ((( imm >> 5 ) & 0b1111111) << 25);
	return i;
}

uint32_t sc_64(uint32_t rs1, uint32_t rs2, uint32_t imm) {
	uint32_t i; i = 12323; // 23 30 00 00  
	i |= ((( imm >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	i |= ((( imm >> 5 ) & 0b1111111) << 25);
	return i;
}

uint32_t sc_b_cap(uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 4160752731; // 5B 0C 00 F8  
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t sc_b_ddc(uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 4160751707; // 5B 08 00 F8  
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t sc_cap_128(uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 4160751195; // 5B 06 00 F8  
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t sc_cap_64(uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 4160751067; // DB 05 00 F8  
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t sc_c_cap_128(uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 4160753243; // 5B 0E 00 F8  
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t sc_c_cap_64(uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 4160753115; // DB 0D 00 F8  
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t sc_c_ddc_128(uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 4160752219; // 5B 0A 00 F8  
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t sc_c_ddc_64(uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 4160752091; // DB 09 00 F8  
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t sc_ddc_128(uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 4160750171; // 5B 02 00 F8  
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t sc_ddc_64(uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 4160750043; // DB 01 00 F8  
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t sc_d_cap(uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 4160753115; // DB 0D 00 F8  
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t sc_d_ddc(uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 4160752091; // DB 09 00 F8  
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t sc_h_cap(uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 4160752859; // DB 0C 00 F8  
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t sc_h_ddc(uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 4160751835; // DB 08 00 F8  
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t sc_w_cap(uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 4160752987; // 5B 0D 00 F8  
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t sc_w_ddc(uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 4160751963; // 5B 09 00 F8  
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t sd(uint32_t rs1, uint32_t rs2, uint32_t imm) {
	uint32_t i; i = 12323; // 23 30 00 00  
	i |= ((( imm >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	i |= ((( imm >> 5 ) & 0b1111111) << 25);
	return i;
}

uint32_t sd_cap(uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 4160751067; // DB 05 00 F8  
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t sd_ddc(uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 4160750043; // DB 01 00 F8  
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t sh(uint32_t rs1, uint32_t rs2, uint32_t imm) {
	uint32_t i; i = 4131; // 23 10 00 00  
	i |= ((( imm >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	i |= ((( imm >> 5 ) & 0b1111111) << 25);
	return i;
}

uint32_t sh_cap(uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 4160750811; // DB 04 00 F8  
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t sh_ddc(uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 4160749787; // DB 00 00 F8  
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t sll(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 4147; // 33 10 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t sllw(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 4155; // 3B 10 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t slt(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 8243; // 33 20 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t slti(uint32_t rd, uint32_t rs1, uint32_t imm) {
	uint32_t i; i = 8211; // 13 20 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( imm >> 0 ) & 0b111111111111) << 20);
	return i;
}

uint32_t sltiu(uint32_t rd, uint32_t rs1, uint32_t imm) {
	uint32_t i; i = 12307; // 13 30 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( imm >> 0 ) & 0b111111111111) << 20);
	return i;
}

uint32_t sltu(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 12339; // 33 30 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t sra(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 1073762355; // 33 50 00 40  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t sraw(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 1073762363; // 3B 50 00 40  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t sret() {
	uint32_t i; i = 270532723; // 73 00 20 10  
	
	return i;
}

uint32_t srl(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 20531; // 33 50 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t srlw(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 20539; // 3B 50 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t sub(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 1073741875; // 33 00 00 40  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t subw(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 1073741883; // 3B 00 00 40  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t sw(uint32_t rs1, uint32_t rs2, uint32_t imm) {
	uint32_t i; i = 8227; // 23 20 00 00  
	i |= ((( imm >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	i |= ((( imm >> 5 ) & 0b1111111) << 25);
	return i;
}

uint32_t sw_cap(uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 4160750939; // 5B 05 00 F8  
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t sw_ddc(uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 4160749915; // 5B 01 00 F8  
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t unimp() {
	uint32_t i; i = 3221229683; // 73 10 00 C0  
	
	return i;
}

uint32_t uret() {
	uint32_t i; i = 2097267; // 73 00 20 00  
	
	return i;
}

uint32_t wfi() {
	uint32_t i; i = 273678451; // 73 00 50 10  
	
	return i;
}

uint32_t inst_xor(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	uint32_t i; i = 16435; // 33 40 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( rs2 >> 0 ) & 0b11111) << 20);
	return i;
}

uint32_t xori(uint32_t rd, uint32_t rs1, uint32_t imm) {
	uint32_t i; i = 16403; // 13 40 00 00  
	i |= ((( rd >> 0 ) & 0b11111) << 7);
	i |= ((( rs1 >> 0 ) & 0b11111) << 15);
	i |= ((( imm >> 0 ) & 0b111111111111) << 20);
	return i;
}


