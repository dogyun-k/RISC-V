#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "rv_format.h"
#include "utils.h"

uint32_t 	x_reg[32];
uint8_t 	memory[4096];
uint32_t	pc = 0;
uint32_t 	clock = 0;


int read_program(char*);

int main(int argc, char* argv[]){

	char* program_file;
	rv_format a_instr;

	if(argc < 2){
		printf("Usage: rv_iss_[file name]\n");
		return 1;
	}
	else{
		int ret;

		program_file = argv[1];
		printf("Program file name : %s\n", program_file);

		ret = read_program(program_file);
	
		if(ret != 0){
			printf("Error: read program\n");
			return 1;
		}
	}


	memory[1024] = 10;
	memory[1028] = 20;



	printf("Start RICS-V Simulator...\n");


	while(1){
		
		printf("\n");
		a_instr.instr = byte_to_word(memory + pc);
		uint32_t opcode = a_instr.r.opcode;

		printf("%x\n", opcode);

		
		if(opcode == 0x33){			// R type
			uint32_t funct3 = a_instr.r.funct3;
			uint32_t funct7 = a_instr.r.funct7;
			uint32_t rd = a_instr.r.rd;
			uint32_t rs1 = a_instr.r.rs1;
			uint32_t rs2 = a_instr.r.rs2;

			
			if(funct3 == 0x0){			//# add, sub

				if(funct7 == 0x0){	// add
					x_reg[rd] = x_reg[rs1] + x_reg[rs2];
				}
				else if(funct7 == 0x20){	// sub
					x_reg[rd] = x_reg[rs1] - x_reg[rs2];
				}
				else{
					printf("Error: Unknown funct7 = 0x%x\n", funct7);
					exit(1);
				}

			}
			else if(funct3 == 0x1){		//# sll
				
				if(funct7 != 0x0){
					printf("Error: Unknown funct7 = 0x%x\n", funct7);
					exit(1);
				}

				x_reg[rd] = x_reg[rs1] << x_reg[rs2];

			}
			else if(funct3 == 0x2){		//# slt
				
				if(funct7 != 0x0){
					printf("Error: Unknown funct7 = 0x%x\n", funct7);
					exit(1);
				}

				if(x_reg[rs1] < x_reg[rs2]){
					x_reg[rd] = 1;
				}
				else{
					x_reg[rd] = 0;
				}

			}
			else if(funct3 == 0x3){		// sltu
				
				if(funct7 != 0x0){
					printf("Error: Unknown funct7 = 0x%x\n", funct7);
					exit(1);
				}



			}
			else if(funct3 == 0x4){		//# xor

				if(funct7 != 0x0){
					printf("Error: Unknown funct7 = 0x%x\n", funct7);
					exit(1);
				}

				x_reg[rd] = x_reg[rs1] ^ x_reg[rs2];

			}
			else if(funct3 == 0x5){		// srl, sra
				
				if(funct7 == 0x0){		//# srl
					x_reg[rd] = x_reg[rs1] >> x_reg[rs2];
					
				}
				else if(funct7 == 0x20){	// sra
					x_reg[rd] = x_reg[rs1] >> x_reg[rs2];
					int32_t temp = 0x1;
					temp <<= x_reg[rs2];
					x_reg[rd] |= temp;
				}
				else{
					printf("Error: Unknown funct7 = 0x%x\n", funct7);
					exit(1);
				}

			}
			else if(funct3 == 0x6){		//# or
				
				if(funct7 != 0x0){
					printf("Error: Unknown funct7 = 0x%x\n", funct7);
					exit(1);
				}

				x_reg[rd] = x_reg[rs1] | x_reg[rs2];

			}
			else if(funct3 == 0x7){		//# and
				
				if(funct7 != 0x0){
					printf("Error: Unknown funct7 = 0x%x\n", funct7);
					exit(1);
				}

				x_reg[rd] = x_reg[rs1] & x_reg[rs2];

			}
			else{
				printf("Error: Unknown funct3 = 0x%x\n", funct3);
				exit(1);
			}

		}

		else if(opcode == 0x13){	// I type
			uint32_t funct3 = a_instr.i.funct3;
			uint32_t rd = a_instr.i.rd;
			uint32_t rs1 = a_instr.i.rs1;
			uint32_t imm = a_instr.i.imm;

		
			if(funct3 == 0x0){			//# addi
				printf("PC:%3d addi x%d, x%d, %d\n", pc, rd, rs1, imm);
				x_reg[rd] = x_reg[rs1] + imm;
			}
			else if(funct3 == 0x1){		//# slli
				if(imm != 0x0){
					printf("Error: Unknown Immediate = %d\n", imm);
					exit(1);
				}

				x_reg[rd] = x_reg[rs1] << imm;
			}
			else if(funct3 == 0x2){		//# slti
				
				if(x_reg[rs1] < imm){
					x_reg[rd] = 1;
				}
				else{
					x_reg[rd] = 0;
				}

			}
			else if(funct3 == 0x3){		// sltiu
				
			}
			else if(funct3 == 0x4){		//# xori
				x_reg[rd] = x_reg[rs1] ^ imm;
			}
			else if(funct3 == 0x5){		// srli, srai

				if(imm == 0x0){			//# srli
					x_reg[rd] = x_reg[rs1] >> imm;
				}
				else if(imm == 0x20){	// srai
					x_reg[rd] = x_reg[rs1] >> imm;
 
				}
				else{
					printf("Error: Unknown Immediate = %d\n", imm);
					exit(1);
				}
			
			}
			else if(funct3 == 0x6){		//# ori
				x_reg[rd] = x_reg[rs1] | imm;
			}
			else if(funct3 == 0x7){		//# andi
				x_reg[rd] = x_reg[rs1] & imm;
			}
			else{
				printf("Error: Unknown funct3 = 0x%x\n", funct3);
				exit(1);
			}
		
		}

		else if(opcode == 0x03){	//# I type : memory load
			uint32_t funct3 = a_instr.i.funct3;
			uint32_t rd = a_instr.i.rd;
			uint32_t rs1 = a_instr.i.rs1;
			uint32_t imm = a_instr.i.imm;

			if(funct3 == 0x0){		//# lb
				x_reg[rd] = memory[x_reg[rs1] + imm];
				if((x_reg[rd] >> 7) == 1){	// 음수면 부호확장
					x_reg[rd] |= 0xffffff00;
				}
			}
			else if(funct3 == 0x1){	//# lh
				x_reg[rd] = memory[x_reg[rs1] + imm];
				x_reg[rd] |= memory[x_reg[rs1] + imm + 1] << 8;

				if((x_reg[rd] >> 15) == 1){
					x_reg[rd] |= 0xffff0000;
				}
			}
			else if(funct3 == 0x2){	//# lw
				x_reg[rd] = memory[x_reg[rs1] + imm];
				x_reg[rd] |= memory[x_reg[rs1] + imm + 1] << 8;
				x_reg[rd] |= memory[x_reg[rs1] + imm + 2] << 16;
				x_reg[rd] |= memory[x_reg[rs1] + imm + 3] << 24;
			}
			else if(funct3 == 0x3){	//# ld
				printf("64-bit instruction\n");
			}
			else if(funct3 == 0x4){	//# lbu
				x_reg[rd] = memory[x_reg[rs1] + imm];
			}
			else if(funct3 == 0x5){	//# lhu
				x_reg[rd] = memory[x_reg[rs1] + imm];
				x_reg[rd] |= memory[x_reg[rs1] + imm + 1] << 8;
			}
			else if(funct3 == 0x6){	//# lwu
				printf("64-bit instruction\n");
			}
			else{
				printf("Error: Unknown LOAD funct3 = 0x%x\n", funct3);
				exit(1);
			}

		}

		else if(opcode == 0x23){	//# I type : memory store
			uint32_t funct3 = a_instr.sb.funct3;
			uint32_t rs1 = a_instr.sb.rs1;
			uint32_t rs2 = a_instr.sb.rs2;
			uint32_t imm = 0;
			imm = a_instr.sb.imm1 | (a_instr.sb.imm2 << 5);
		
			if(funct3 == 0x0){		// sb
				memory[x_reg[rs1] + imm] = (uint8_t)(x_reg[rs2] & 0xff);
			}	
			else if(funct3 == 0x1){	// sh
				memory[x_reg[rs1] + imm] = (uint8_t)(x_reg[rs2] >> 8 & 0xff);
				memory[x_reg[rs1] + imm + 1] = (uint8_t)((x_reg[rs2] >> 8) & 0xff);
			}
			else if(funct3 == 0x2){	// sw
				memory[x_reg[rs1] + imm] = (uint8_t)(x_reg[rs2] & 0xff);
				memory[x_reg[rs1] + imm + 1] = (uint8_t)((x_reg[rs2] >> 8) & 0xff);
				memory[x_reg[rs1] + imm + 2] = (uint8_t)((x_reg[rs2] >> 16) & 0xff);
				memory[x_reg[rs1] + imm + 3] = (uint8_t)((x_reg[rs2] >> 24) & 0xff);
			}
			else{
				printf("Error: Unknown STORE funct3 = 0x%x\n", funct3);
				exit(1);
			}
		}

		else if(opcode == 0x63){	//# Branch
			uint32_t funct3 = a_instr.sb.funct3;
			uint32_t rs1 = a_instr.sb.rs1;
			uint32_t rs2 = a_instr.sb.rs2;
			uint32_t imm = 0;

			imm = a_instr.sb.imm1 & 0x1e;
			imm |= (a_instr.sb.imm2 & 0x3f) << 5;
			imm |= (a_instr.sb.imm1 & 0x1) << 11;
			imm |= (a_instr.sb.imm2 >> 7) << 12;

			if(funct3 == 0x0){	// beq
				if(x_reg[rs1] == x_reg[rs2]){
					pc += imm - 4;
				}
			}
			else if(funct3 == 0x1){	// bne
				printf("PC:%3d bne x%d, x%d, %d\n", pc, rs1, rs2, imm);
				if(x_reg[rs1] != x_reg[rs2]){
					pc += imm - 4;
				}
			}
			else if(funct3 == 0x4){	// blt
				if((int32_t)x_reg[rs1] < (int32_t)x_reg[rs2]){
					pc += imm - 4;
				}
			}
			else if(funct3 == 0x5){	// bge
				if((int32_t)x_reg[rs1] >= (int32_t)x_reg[rs2]){
					pc += imm - 4;
				}			
			}
			else if(funct3 == 0x6){	// bltu
				if(x_reg[rs1] < x_reg[rs2]){
					pc += imm - 4;
				}
			}
			else if(funct3 == 0x7){	// bgeu
				if(x_reg[rs1] >= x_reg[rs2]){
					pc += imm - 4;
				}
			}
			else{
				printf("Error: Unknown BRANCH funct3 = 0x%x\n", funct3);
				exit(1);
			}
		}
		
		else if(opcode == 0x6f){	//# jal
			uint32_t rd = a_instr.uj.rd;
			uint32_t imm = 0;
			uint32_t temp = a_instr.uj.imm;

			imm = ((temp >> 9) & 0x3ff) << 1;
			imm |= ((temp >> 8) & 0x1) << 11;
			imm |= (temp & 0xff) << 12;
			imm |= (temp >> 19) << 20;
		
			printf("PC:%3d jal x%d, %d\n", pc, rd, imm);

			x_reg[rd] = pc + 4;
			pc += imm;

		}
		
		else if(opcode == 0x0f){	// fence, fencei
			
		}
		
		else if(opcode == 0x17){	// auipc
			
		}
		
		else if(opcode == 0x37){	// lui
			
		}
		
		else if(opcode == 0x67){	// jalr
			
		}
		
		else if(opcode == 0x73){	// ecall, ebreak, CSR...
			
		}
		
		else{
			if(opcode == 0x0){
				printf("Read Complete!\n", opcode);
			}
			else{
				printf("Error: Unknown OPCODE 0x%x\n", opcode);
			}
			break;
		}

		// 명령어 하나 수행 후 동작
		pc += 4;
		clock++;


	}

	display_reg();

	printf("Memory[1032] = %d\n", memory[1032]);
	printf("End RICS-V Simulator. Clock = %d\n", clock - 1);
	return 0;
}
