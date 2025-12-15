#include "RV.h"

static int opcode_op_imm(void) // OPCODE_OP_IMM = 0x13
{
    uint32_t result = 0,
             rs1 = CPU.REG.x[CPU.instr.itype.rs1];
    int32_t imm = SIGN_EXTEND(CPU.instr.itype.imm11_0, 12); // sign-extended 12-bit immediate

    switch (CPU.instr.itype.f3)
    {
    case 0x0: // ADDI (със знак)
    {
        PRINT_ASM("ADDI %s, %s, %d\n", RN[CPU.instr.itype.rd], RN[CPU.instr.itype.rs1], imm);
        result = (int32_t)(rs1 + imm);
        break;
    }

    case 0x1: // SLLI (логическо изместване наляво)
    {
        if (0 == CPU.instr.rtype.f7)
        {
            uint8_t shamt = CPU.instr.itype.imm11_0 & 0x1F; // Използваме imm[4:0] require(SHAMT < 32);
            PRINT_ASM("SLLI %s, %s, %u\n", RN[CPU.instr.itype.rd], RN[CPU.instr.itype.rs1], shamt);
            result = rs1 << shamt;
        }
        else
        {
            return E_INVALID_INSTRUCTION;
        }
        break;
    }

    case 0x2: // SLTI (сравнение със знак)
    {
        PRINT_ASM("SLTI %s, %s, %d\n", RN[CPU.instr.itype.rd], RN[CPU.instr.itype.rs1], imm);
        result = ((int32_t)rs1 < imm) ? 1 : 0;
        break;
    }

    case 0x3: // SLTIU (сравнение без знак)
    {
        PRINT_ASM("SLTIU %s, %s, %d\n", RN[CPU.instr.itype.rd], RN[CPU.instr.itype.rs1], imm);
        result = (rs1 < (uint32_t)imm) ? 1 : 0;
        break;
    }

    case 0x4: // XORI
    {
        PRINT_ASM("XORI %s, %s, %d\n", RN[CPU.instr.itype.rd], RN[CPU.instr.itype.rs1], imm);
        result = rs1 ^ imm;
        break;
    }

    case 0x5: // SRLI или SRAI
    {
        uint8_t shamt = CPU.instr.itype.imm11_0 & 0x1F;
        if (0x20 == CPU.instr.rtype.f7) // SRAI (funct7 == 0x20)
        {
            PRINT_ASM("SRAI %s, %s, %u\n", RN[CPU.instr.itype.rd], RN[CPU.instr.itype.rs1], shamt);
            result = (int32_t)rs1 >> shamt; // Аритметично изместване
        }
        else if (0 == CPU.instr.rtype.f7) // SRLI (funct7 == 0)
        {
            PRINT_ASM("SRLI %s, %s, %u\n", RN[CPU.instr.itype.rd], RN[CPU.instr.itype.rs1], shamt);
            result = rs1 >> shamt; // Логическо изместване
        }
        else
        {
            return E_INVALID_INSTRUCTION;
        }
        break;
    }

    case 0x6: // ORI
    {
        PRINT_ASM("ORI %s, %s, %d\n", RN[CPU.instr.itype.rd], RN[CPU.instr.itype.rs1], imm);
        result = rs1 | imm;
        break;
    }

    case 0x7: // ANDI
    {
        PRINT_ASM("ANDI %s, %s, %d\n", RN[CPU.instr.itype.rd], RN[CPU.instr.itype.rs1], imm);
        result = rs1 & imm;
        break;
    }

    default:
        return E_INVALID_INSTRUCTION;
    }

    if (CPU.instr.itype.rd)
        CPU.REG.x[CPU.instr.itype.rd] = result;

    return RV_OK;
}

static int opcode_op(void) // OPCODE_OP = 0x33
{
    uint32_t result = 0,
             rs1 = CPU.REG.x[CPU.instr.rtype.rs1],
             rs2 = CPU.REG.x[CPU.instr.rtype.rs2];

    if (0x00 == CPU.instr.rtype.f7)
    {
        switch (CPU.instr.rtype.f3)
        {
        case 0: // ADD
        {
            PRINT_ASM("ADD %s, %s, %s\n", RN[CPU.instr.rtype.rd], RN[CPU.instr.rtype.rs1], RN[CPU.instr.rtype.rs2]);
            result = rs1 + rs2;
            break;
        }
        case 1: // SLL
        {
            if (CPU.instr.rtype.f7 == 0x00)
            {
                PRINT_ASM("SLL %s, %s, %s\n", RN[CPU.instr.rtype.rd], RN[CPU.instr.rtype.rs1], RN[CPU.instr.rtype.rs2]);
                result = rs1 << (rs2 & 0x1F);
            }
            else
            {
                return E_INVALID_INSTRUCTION;
            }
            break;
        }
        case 2: // SLT
        {
            if (CPU.instr.rtype.f7 == 0x00)
            {
                PRINT_ASM("SLT %s, %s, %s\n", RN[CPU.instr.rtype.rd], RN[CPU.instr.rtype.rs1], RN[CPU.instr.rtype.rs2]);
                result = ((int32_t)rs1 < (int32_t)rs2) ? 1 : 0;
            }
            else
            {
                return E_INVALID_INSTRUCTION;
            }
            break;
        }
        case 3: // SLTU
        {
            if (CPU.instr.rtype.f7 == 0x00)
            {
                PRINT_ASM("SLTU %s, %s, %s\n", RN[CPU.instr.rtype.rd], RN[CPU.instr.rtype.rs1], RN[CPU.instr.rtype.rs2]);
                result = (rs1 < rs2) ? 1 : 0;
            }
            else
            {
                return E_INVALID_INSTRUCTION;
            }
            break;
        }
        case 4: // XOR
        {
            if (CPU.instr.rtype.f7 == 0x00)
            {
                PRINT_ASM("XOR %s, %s, %s\n", RN[CPU.instr.rtype.rd], RN[CPU.instr.rtype.rs1], RN[CPU.instr.rtype.rs2]);
                result = rs1 ^ rs2;
            }
            else
            {
                return E_INVALID_INSTRUCTION;
            }
            break;
        }
        case 5: // SRL
        {
            PRINT_ASM("SRL %s, %s, %s\n", RN[CPU.instr.rtype.rd], RN[CPU.instr.rtype.rs1], RN[CPU.instr.rtype.rs2]);
            result = rs1 >> (rs2 & 0x1F);
            break;
        }
        case 6: // OR
        {
            if (CPU.instr.rtype.f7 == 0x00)
            {
                PRINT_ASM("OR %s, %s, %s\n", RN[CPU.instr.rtype.rd], RN[CPU.instr.rtype.rs1], RN[CPU.instr.rtype.rs2]);
                result = rs1 | rs2;
            }
            else
            {
                return E_INVALID_INSTRUCTION;
            }
            break;
        }
        case 7: // AND
        {
            if (CPU.instr.rtype.f7 == 0x00)
            {
                PRINT_ASM("AND %s, %s, %s\n", RN[CPU.instr.rtype.rd], RN[CPU.instr.rtype.rs1], RN[CPU.instr.rtype.rs2]);
                result = rs1 & rs2;
            }
            else
            {
                return E_INVALID_INSTRUCTION;
            }
            break;
        }
        default:
            return E_INVALID_INSTRUCTION;
        }
    }
    else if (0x20 == CPU.instr.rtype.f7)
    {
        switch (CPU.instr.rtype.f3)
        {
        case 0: // SUB
        {
            PRINT_ASM("SUB %s, %s, %s\n", RN[CPU.instr.rtype.rd], RN[CPU.instr.rtype.rs1], RN[CPU.instr.rtype.rs2]);
            result = rs1 - rs2;
            break;
        }
        case 5: // SRA
        {
            PRINT_ASM("SRA %s, %s, %s\n", RN[CPU.instr.rtype.rd], RN[CPU.instr.rtype.rs1], RN[CPU.instr.rtype.rs2]);
            result = ((int32_t)rs1) >> (rs2 & 0x1F);
            break;
        }
        default:
            return E_INVALID_INSTRUCTION;
        }
    }
    else if (0x01 == CPU.instr.rtype.f7)
    {
        switch (CPU.instr.rtype.f3)
        {
        case 0: // MUL
        {
            result = (int32_t)((int32_t)rs1 * (int32_t)rs2);
            break;
        }
        case 1: // MULH
        {
            int64_t prod = (int64_t)(int32_t)rs1 * (int64_t)(int32_t)rs2;
            result = (uint32_t)(prod >> 32);
            break;
        }
        case 2: // MULHSU
        {
            int64_t prod = (int64_t)(int32_t)rs1 * (uint64_t)rs2;
            result = (uint32_t)(prod >> 32);
            break;
        }
        case 3: // MULHU
        {
            uint64_t prod = (uint64_t)rs1 * (uint64_t)rs2;
            result = (uint32_t)(prod >> 32);
            break;
        }
        case 4: // DIV
        {
            if ((int32_t)rs2 == 0)
                result = E_DIVIDE;
            else if ((int32_t)rs1 == INT32_MIN && (int32_t)rs2 == -1)
                result = INT32_MIN; // overflow case
            else
                result = (int32_t)rs1 / (int32_t)rs2;
            break;
        }
        case 5: // DIVU
        {
            result = (rs2 == 0) ? UINT32_MAX : rs1 / rs2;
            break;
        }
        case 6: // REM
        {
            if ((int32_t)rs2 == 0)
                result = (int32_t)rs1;
            else if ((int32_t)rs1 == INT32_MIN && (int32_t)rs2 == -1)
                result = 0;
            else
                result = (int32_t)rs1 % (int32_t)rs2;
            break;
        }
        case 7: // REMU
        {
            result = (rs2 == 0) ? rs1 : rs1 % rs2;
            break;
        }
        default:
            return E_INVALID_INSTRUCTION;
        }
    }
    else
    {
        return E_INVALID_INSTRUCTION;
    }

    if (CPU.instr.rtype.rd)
        CPU.REG.x[CPU.instr.rtype.rd] = result;

    return RV_OK;
}

static int opcode_amo(void) // OPCODE_AMO = 0x2F
{
    uint32_t addr;
    switch (CPU.instr.rtype.f3)
    {
    case 2:
    {
        addr = CPU.REG.x[CPU.instr.rtype.rs1];
        switch (CPU.instr.rtype.f7)
        {
        case 0x2:  /* LR.W */
        case 0x3:  /* SC.W */
        case 0x1:  /* AMOSWAP.W */
        case 0x0:  /* AMOADD.W */
        case 0x4:  /* AMOXOR.W */
        case 0xC:  /* AMOAND.W */
        case 0x8:  /* AMOOR.W */
        case 0x10: /* AMOMIN.W */
        case 0x14: /* aMOMAX.W */
        case 0x18: /* AMOMINU.W */
        case 0x1C: /* AMOMAXU.W */
        default:
            return E_INVALID_INSTRUCTION;
        }
        break;
    }
    default:
        return E_INVALID_INSTRUCTION;
    }
}

static int opcode_load(void) // OPCODE_LOAD = 0x03
{
    int32_t value = 0,
            imm = SIGN_EXTEND(CPU.instr.itype.imm11_0, 12);

    uint32_t addr = CPU.REG.x[CPU.instr.itype.rs1] + imm;

    int err;

    switch (CPU.instr.itype.f3)
    {
    case 0x0: // LB
    {
        PRINT_ASM("LB %s, %s, %d\n", RN[CPU.instr.itype.rd], RN[CPU.instr.itype.rs1], imm);
        if ((err = READ_MEM_8(addr, (uint32_t *)&value)))
            return err;
        value = SIGN_EXTEND_8(value);
        break;
    }

    case 0x1: // LH
    {
        PRINT_ASM("LH %s, %s, %d\n", RN[CPU.instr.itype.rd], RN[CPU.instr.itype.rs1], imm);
        if ((err = READ_MEM_16(addr, (uint32_t *)&value)))
            return err;
        value = SIGN_EXTEND_16(value);
        break;
    }

    case 0x2: // LW
        PRINT_ASM("LW %s, %s, %d\n", RN[CPU.instr.itype.rd], RN[CPU.instr.itype.rs1], imm);
        if ((err = READ_MEM_32(addr, (uint32_t *)&value)))
            return err;
        break;

    case 0x4: // LBU
        if ((err = READ_MEM_8(addr, (uint32_t *)&value)))
            return err;
        PRINT_ASM("LBU %s, %s, %d\n", RN[CPU.instr.itype.rd], RN[CPU.instr.itype.rs1], imm);
        break;

    case 0x5: // LHU
        PRINT_ASM("LHU %s, %s, %d\n", RN[CPU.instr.itype.rd], RN[CPU.instr.itype.rs1], imm);
        if ((err = READ_MEM_16(addr, (uint32_t *)&value)))
            return err;
        break;

    default:
        return E_INVALID_INSTRUCTION;
    }

    if (CPU.instr.itype.rd)
        CPU.REG.x[CPU.instr.itype.rd] = value;

    return RV_OK;
}

static int opcode_store(void) // OPCODE_STORE = 0x23
{
    int err;
    uint32_t value = CPU.REG.x[CPU.instr.stype.rs2];

    int32_t imm = (CPU.instr.stype.imm11_5 << 5) | CPU.instr.stype.imm4_0;
    imm = SIGN_EXTEND(imm, 12);

    uint32_t addr = CPU.REG.x[CPU.instr.stype.rs1] + imm;

    switch (CPU.instr.stype.f3)
    {
    case 0x0: // SB
        PRINT_ASM("SB %s, %s, %d)\n", RN[CPU.instr.stype.rs2], RN[CPU.instr.stype.rs1], imm);
        err = WRITE_MEM_8(addr, value);
        break;

    case 0x1: // SH
        PRINT_ASM("SH %s, %s, %d\n", RN[CPU.instr.stype.rs2], RN[CPU.instr.stype.rs1], imm);
        err = WRITE_MEM_16(addr, value);
        break;

    case 0x2: // SW
        PRINT_ASM("SW %s, %s, %d\n", RN[CPU.instr.stype.rs2], RN[CPU.instr.stype.rs1], imm);
        err = WRITE_MEM_32(addr, value);
        break;

    default:
        return E_INVALID_INSTRUCTION;
    }

    return err;
}

static int opcode_jal(void) // OPCODE_JAL = 0x6F
{
    // imm = {imm20, imm19_12, imm11, imm10_1, 0}
    int32_t imm = (int32_t)((CPU.instr.jtype.imm20 << 20) |
                            (CPU.instr.jtype.imm19_12 << 12) |
                            (CPU.instr.jtype.imm11 << 11) |
                            (CPU.instr.jtype.imm10_1 << 1));
    imm = SIGN_EXTEND(imm, 21);

    if (CPU.instr.jtype.rd) // Запазване на адреса за връщане (PC + 4) в rd, ако rd != 0
    {
        PRINT_ASM("JAL %s, %d\n", RN[CPU.instr.jtype.rd], imm);
        CPU.REG.x[CPU.instr.jtype.rd] = CPU.pc + 4;
    }
    else
    {
        PRINT_ASM("JAL %d\n", imm);
    }

    return SET_PC(CPU.pc + imm);
}

static int opcode_branch(void) // OPCODE_BRANCH = 0x63
{
    int32_t imm = (CPU.instr.btype.imm12 << 12) |
                  (CPU.instr.btype.imm11 << 11) |
                  (CPU.instr.btype.imm10_5 << 5) |
                  (CPU.instr.btype.imm4_1 << 1);
    imm = SIGN_EXTEND(imm, 13);

    uint32_t rs1_val = CPU.REG.x[CPU.instr.btype.rs1];
    uint32_t rs2_val = CPU.REG.x[CPU.instr.btype.rs2];
    bool jump = false;

    switch (CPU.instr.btype.f3)
    {
    case 0x0: // BEQ
        jump = (rs1_val == rs2_val);
        PRINT_ASM("BEQ %s, %s, %d\n", RN[CPU.instr.btype.rs1], RN[CPU.instr.btype.rs2], imm);
        break;

    case 0x1: // BNE
        jump = (rs1_val != rs2_val);
        PRINT_ASM("BNE %s, %s, %d\n", RN[CPU.instr.btype.rs1], RN[CPU.instr.btype.rs2], imm);
        break;

    case 0x4: // BLT
        jump = ((int32_t)rs1_val < (int32_t)rs2_val);
        PRINT_ASM("BLT %s, %s, %d\n", RN[CPU.instr.btype.rs1], RN[CPU.instr.btype.rs2], imm);
        break;

    case 0x5: // BGE
        jump = ((int32_t)rs1_val >= (int32_t)rs2_val);
        PRINT_ASM("BGE %s, %s, %d\n", RN[CPU.instr.btype.rs1], RN[CPU.instr.btype.rs2], imm);
        break;

    case 0x6: // BLTU (беззнаково)
        jump = (rs1_val < rs2_val);
        PRINT_ASM("BLTU %s, %s, %d\n", RN[CPU.instr.btype.rs1], RN[CPU.instr.btype.rs2], imm);
        break;

    case 0x7: // BGEU (беззнаково)
        jump = (rs1_val >= rs2_val);
        PRINT_ASM("BGEU %s, %s, %d\n", RN[CPU.instr.btype.rs1], RN[CPU.instr.btype.rs2], imm);
        break;

    default:
        return E_INVALID_INSTRUCTION;
    }

    if (jump)
        return SET_PC(CPU.pc + imm);

    return RV_OK;
}

int rv_execute(void)
{
    int err;

    if ((int32_t)CPU.pc < 0)
        return RV_DONE;

    if ((err = READ_MEM_32(CPU.pc, (uint32_t *)&CPU.instr)))
        return err;

    //PRINT_REG_16();
    //PRINT("[A] 0x%08X: 0x%08X ( OP = 0x%X )\n", CPU.pc, CPU.instr.raw, CPU.instr.any.opcode);

    int32_t imm;

    switch (CPU.instr.any.opcode)
    {
    case OPCODE_AUIPC:
    {
        if (CPU.instr.utype.rd)
        {
            imm = (int32_t)(CPU.instr.raw & 0xFFFFF000); // imm20 signed
            CPU.REG.x[CPU.instr.utype.rd] = CPU.pc + imm;
            PRINT_ASM("AUIPC %s, 0x%X\n", RN[CPU.instr.utype.rd], imm >> 12); // оригиналния imm
        }
        break;
    }
    case OPCODE_LUI:
    {
        if (CPU.instr.utype.rd)
        {
            imm = (int32_t)(CPU.instr.raw & 0xFFFFF000); // imm20 signed
            CPU.REG.x[CPU.instr.utype.rd] = imm;
            PRINT_ASM("LUI %s, %d\n", RN[CPU.instr.utype.rd], imm >> 12); // оригиналния imm
        }
        break;
    }
    case OPCODE_JALR:
    {
        imm = (int32_t)CPU.instr.raw >> 20; // signed
        PRINT_ASM("JALR %s, %s, %d\n", RN[CPU.instr.itype.rd], RN[CPU.instr.itype.rs1], imm);
        if (CPU.instr.itype.rd)
            CPU.REG.x[CPU.instr.itype.rd] = CPU.pc + 4; // RA
        err = SET_PC((CPU.REG.x[CPU.instr.itype.rs1] + imm) & ~1U);
        break;
    }

    case OPCODE_OP:
    {
        err = opcode_op();
        break;
    }
    case OPCODE_LOAD:
    {
        err = opcode_load();
        break;
    }
    case OPCODE_STORE:
    {
        err = opcode_store();
        break;
    }
    case OPCODE_OP_IMM:
    {
        err = opcode_op_imm();
        break;
    }
    case OPCODE_JAL:
    {
        err = opcode_jal();
        break;
    }
    case OPCODE_BRANCH:
    {
        err = opcode_branch();
        break;
    }

    // case OPCODE_AMO:
    // case OPCODE_MISC_MEM:
    // case OPCODE_SYSTEM:
    default:
        PRINT_ERROR("Invalid Instruction: 0x%X at 0x%X\n", CPU.instr.any.opcode, CPU.pc);
        return E_INVALID_INSTRUCTION;
    }

    if (err == RV_JUMP)
        return RV_OK;
    else if (err < RV_OK)
        return err;

    CPU.pc += 4;
    return err;
}
