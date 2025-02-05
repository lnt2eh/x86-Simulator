#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <windows.h>

#define MEMORY_SIZE 1024

typedef struct {
    uint16_t AX, BX, CX, DX;
    uint16_t SI, DI;
    uint16_t SP, BP, IP;
    uint16_t FLAGS;
} Cpu_Registers;

uint8_t memory[MEMORY_SIZE];
Cpu_Registers Register;

typedef enum {
    MOV_IMM = 0xB8,
    ADD = 0x83,
    SUB = 0x84,
    MOV_REG = 0x89,
    RET = 0xC3,
    TEST = 0x85,
} Instructions;

uint16_t *regs[] = { &Register.AX, &Register.BX, &Register.CX, &Register.DX, &Register.SI, &Register.DI };

uint8_t fetch() {
    return memory[Register.IP++];
}

void reset_registers() {
    memset(&Register, 0, sizeof(Register));
    Register.SP = MEMORY_SIZE - 1;
    Register.FLAGS = 0;
}

void load_program(uint8_t *program, size_t size) {
    memcpy(memory, program, size);
    Register.IP = 0;
}

void execute() {
    while (1) {
        uint8_t opcode = fetch();
        switch (opcode) {
            case MOV_IMM: {
                uint8_t reg = fetch();
                uint16_t value = fetch() | (fetch() << 8);
                *regs[reg] = value;
                break;
            }
            case ADD: {
                uint8_t reg = fetch();
                uint16_t value = fetch() | (fetch() << 8);
                *regs[reg] += value;
                break;
            }
            case SUB: {
                uint8_t reg = fetch();
                uint16_t value = fetch() | (fetch() << 8);
                *regs[reg] -= value;
                break;
            }
            case MOV_REG: {
                uint8_t reg_dest = fetch();
                uint8_t reg_src = fetch();
                *regs[reg_dest] = *regs[reg_src];
                break;
            }
            case TEST: {
                uint8_t reg_dest = fetch();
                uint8_t reg_src = fetch();
                if (*regs[reg_dest] == *regs[reg_src]) {
                    Register.FLAGS |= 0x01;
                } else {
                    Register.FLAGS |= 0x02;
                }
                break;
            }
            case RET: {
                return;
            }
            default:
                printf("Erro: Instrução inválida 0x%02X na posição %d\n", opcode, Register.IP - 1);
                return;
        }
    }
}

int main(void) {
    SetConsoleOutputCP(65001);
    reset_registers();

    uint8_t programa[] = {
        MOV_IMM, 0, 0x32, 0x12,
        ADD, 0, 0x45, 0x15,
        MOV_IMM, 1, 0x20, 0x00,
        SUB, 1, 0x14, 0x00,
        MOV_REG, 2, 0,
        TEST, 2, 0,
        SUB, 1, 0x15, 0x23,
        RET
    };

    printf("Instruções carregadas:\n");
    for (size_t i = 0; i < sizeof(programa); i++) {
        printf("%02X ", programa[i]);
        if ((i + 1) % 4 == 0) {
            printf("\n");
        }
    }
    printf("\n");

    load_program(programa, sizeof(programa));
    execute();

    printf("\nRegistradores: \n");
    printf("AX: 0x%04X\n", Register.AX);
    printf("BX: 0x%04X\n", Register.BX);
    printf("CX: 0x%04X\n", Register.CX);
    printf("DX: 0x%04X\n", Register.DX);
    printf("SP: 0x%04X\n", Register.SP);
    printf("BP: 0x%04X\n", Register.BP);

    printf("\nFLAGS: 0x%04X\n", Register.FLAGS);
    printf("ZF (Zero Flag): %s\n", (Register.FLAGS & 0x01) ? "Set" : "Clear");
    printf("CF (Carry Flag): %s\n", (Register.FLAGS & 0x02) ? "Set" : "Clear");

    return 0;
}
