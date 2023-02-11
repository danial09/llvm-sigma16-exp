//===- Sigma16InstrInfo.td - Target Description for Sigma16  -*- tablegen -*-=//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file describes Sigma16 instructions.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Sigma16 specific DAG nodes
//===----------------------------------------------------------------------===//


//===----------------------------------------------------------------------===//
// Type profiles
//===----------------------------------------------------------------------===//

// def MoveImm32Ty : SDTypeProfile<1, 1, [
//  SDTCisSameAs<0, 1>, SDTCisInt<0>
// ]>;

def SDT_Sigma16Ret          : SDTypeProfile<0, 1, [SDTCisInt<0>]>;
def SDT_Sigma16JmpLink      : SDTypeProfile<0, 1, [SDTCisVT<0, iPTR>]>;
def SDT_Sigma16CallSeqStart : SDCallSeqStart<[ SDTCisVT<0, i16> ]>;
def Sigma16GlobalAddr : SDNode<"Sigma16ISD::GlobalAddress", SDTIntUnaryOp>;
// def SDT_Sigma16Wrapper      : SDTypeProfile<1, 1, [SDTCisSameAs<0, 1>,
//                                                 SDTCisPtrTy<0>]>;

def SDT_Sigma16CallSeqEnd   : SDCallSeqEnd<[
  SDTCisVT<0, i16>, SDTCisVT<1, i16>
]>;

def SDT_Sigma16DivRem : SDTypeProfile<0, 2, [SDTCisInt<0>, SDTCisSameAs<0, 1>]>;
def Sigma16DivRem     : SDNode<"Sigma16ISD::Sigma16DivRem", SDT_Sigma16DivRem,
                                [SDNPOutGlue]>;

def Sigma16Wrapper    : SDNode<"Sigma16ISD::Wrapper", SDTIntBinOp>;

def Sigma16JmpLink : SDNode<"Sigma16ISD::JmpLink",SDT_Sigma16JmpLink,
                         [SDNPHasChain, SDNPOutGlue, SDNPOptInGlue,
                          SDNPVariadic]>;

def Sigma16Ret : SDNode<"Sigma16ISD::Ret", SDTNone,
                     [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;
def callseq_start : SDNode<"ISD::CALLSEQ_START", SDT_Sigma16CallSeqStart,
                           [SDNPHasChain, SDNPOutGlue]>;
def callseq_end   : SDNode<"ISD::CALLSEQ_END",   SDT_Sigma16CallSeqEnd,
                           [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue]>;

def SDT_Sigma16Call    : SDTypeProfile<0, -1, [SDTCisPtrTy<0>]>;

class IsTailCall {
  bit isCall = 1;
  bit isTerminator = 1;
  bit isReturn = 1;
  bit isBarrier = 1;
  bit hasExtraSrcRegAllocReq = 1;
  bit isCodeGenOnly = 1;
}

//===----------------------------------------------------------------------===//
// Instruction Formats
//===----------------------------------------------------------------------===//

include "Sigma16InstrFormats.td"

//===----------------------------------------------------------------------===//
// Instruction operand types
//===----------------------------------------------------------------------===//

// def memsrc : Operand<i16> {
//   let MIOperandInfo = (ops CPURegs, i16imm);
//   let PrintMethod = "printAddrModeMemSrc";
//   let EncoderMethod = "getMemSrcValue";
// }


def addr :
  ComplexPattern<iPTR, 2, "SelectAddr", [frameindex], []>;

def simm16      : Operand<i16> {
  let DecoderMethod= "DecodeSimm16";
}

// JEQ, JNE, ...
def brtarget    : Operand<OtherVT> {
  let EncoderMethod = "getBranch24TargetOpValue";
  let OperandType = "OPERAND_PCREL";
  let DecoderMethod = "DecodeBranch24Target";
}

// JMP
def jmptarget    : Operand<OtherVT> {
  let EncoderMethod = "getJumpTargetOpValue";
  let OperandType = "OPERAND_PCREL";
}

def calltarget  : Operand<iPTR> {
  let EncoderMethod = "getJumpTargetOpValue";
  let OperandType = "OPERAND_PCREL";
}

// Node immediate fits as 16-bit sign extended on target immediate.
def immSExt16  : PatLeaf<(imm), [{ return isInt<16>(N->getSExtValue()); }]>;

def mem : Operand<iPTR> {
  let PrintMethod = "printMemOperand";
  let MIOperandInfo = (ops CPURegs, simm16);
  let EncoderMethod = "getMemEncoding";
//  let ParserMatchClass = Sigma16MemAsmOperand;
}

class Sigma16InstAlias<string Asm, dag Result, bit Emit = 0b1> :
  InstAlias<Asm, Result, Emit>;

//===----------------------------------------------------------------------===//
// Instructions specific formats
//===----------------------------------------------------------------------===//

let isReturn=1, isTerminator=1, hasDelaySlot=1, isBarrier=1, hasCtrlDep=1 in
  def RetLR : Sigma16Pseudo<(outs), (ins), "", [(Sigma16Ret)], IIPseudo> {
}

let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in
class ALU_rr<bits<4> op, string instr_asm, SDNode OpNode,
                  InstrItinClass itin, RegisterClass RC, bit isComm = 0>:
    FRRR<op, (outs CPURegs:$rd), (ins RC:$ra, RC:$rb),
    !strconcat(instr_asm, "\t$rd,$ra,$rb"),
    [(set CPURegs:$rd, (OpNode RC:$ra, RC:$rb))], itin> {
    let isCommutable = isComm;	// e.g. add ra rb =  add rb ra
}

class ALU_ri<bits<4> opB, string instr_asm, SDNode OpNode,
                  Operand Od, PatLeaf imm_type, RegisterClass RC> :
  FRX<(outs CPURegs:$rd), (ins RC:$ra, Od:$imm16),
     !strconcat(instr_asm, "\t$rd,$imm16[$ra]"),
     [(set CPURegs:$rd, (OpNode RC:$ra, imm_type:$imm16))], IIAlu> {
  let rb = opB;
  let isReMaterializable = 1;
}
class CmpInstr<bits<4> op, string instr_asm,
               InstrItinClass itin, RegisterClass RC, RegisterClass RD,
               bit isComm = 0>:
  FRRR<op, (outs RD:$rd), (ins RC:$ra, RC:$rb),
     !strconcat(instr_asm, "\t$rd,$ra,$rb"), [], itin> {
  let isCommutable = isComm;
}

// LogicInstr - Used to implement and, xor, or, nor, nand
class LogicInstr<bits<8> opB, bits<8> logicCode, string instr_asm, SDNode OpNode,
                InstrItinClass itin, RegisterClass RC>:
  FRRRK<(outs CPURegs:$rd), (ins RC:$ra, RC:$rb),
     !strconcat(instr_asm, "\t$rd,$ra,$rb,", !cast<string>(logicCode)),
     [(set CPURegs:$rd, (OpNode RC:$ra, RC:$rb))], itin> {
     let isCommutable = 1;
     let b = opB;
     let k = logicCode;
}

class LogicInstrNot<bits<8> opB, bits<8> logicCode, string instr_asm, SDNode OpNode,
                InstrItinClass itin, RegisterClass RC>:
  FRRRK<(outs CPURegs:$rd), (ins RC:$ra, RC:$rb),
     !strconcat(instr_asm, "\t$rd,$ra,", !cast<string>(logicCode)),
     [(set CPURegs:$rd, (not (OpNode RC:$ra, RC:$rb)))], itin> {
     let isCommutable = 1;
     let b = opB;
     let k = logicCode;
}

class LogicW<SDNode OpNode, bits<8> logicCode, RegisterClass RC>:
                LogicInstr<0x12, logicCode, "logicw", OpNode, IIAlu, RC> {
}

class LogicWNot<SDNode OpNode, bits<8> logicCode, RegisterClass RC>:
                LogicInstrNot<0x12, logicCode, "logicw", OpNode, IIAlu, RC> {
}

class UncondBranch<bits<4> opB, string instr_asm>:
  FRX<(outs), (ins jmptarget:$imm16),
             !strconcat(instr_asm, "\t$imm16"), [(br bb:$imm16)], IIBranch> {
  let rd = 0;
  let ra = 0;
  let rb = opB;
  let isBranch = 1;
  let isTerminator = 1;
  let isBarrier = 1;
  let hasDelaySlot = 0;
}

// Memory instructions
class FMem<bits<4> opB, dag outs, dag ins, string asmstr, list<dag> pattern,
            InstrItinClass itin>: FRX<outs, ins, asmstr, pattern, itin> {
    let rb = opB;
    let ra = 0;

    let DecoderMethod = "decodeMem";
}

// FIXME: Pointers to local variables do not work properly.

class EffectiveAddress<bits<4> opB, string instr_asm, RegisterClass RC, Operand Mem> :
  FMem<opB, (outs RC:$rd), (ins Mem:$imm16),
     instr_asm, [(set RC:$rd, addr:$imm16)], IIAlu> {
}

// Memory instructions with 16-bit displacement stored in ra
class MemDisp<bits<4> opB, dag outs, dag ins, string asmstr, list<dag> pattern,
            InstrItinClass itin>: FRX<outs, ins, asmstr, pattern, itin> {
    let rb = opB;
    let DecoderMethod = "decodeMemDisp";
}

// Memory instruction with 16-bit immediate with a 16-bit displacement stored in ra
class MemImmDisp<bits<4> opB, dag outs, dag ins, string asmstr, list<dag> pattern,
            InstrItinClass itin>: FRX<outs, ins, asmstr, pattern, itin> {
    let rb = opB;
    let DecoderMethod = "decodeMemDispImm";
}

let canFoldAsLoad = 1 in
class LoadM<bits<4> opB, string instr_asm, RegisterClass RC,
            Operand MemOpnd, bit Pseudo>:
  FMem<opB, (outs RC:$rd), (ins MemOpnd:$imm16),
     !strconcat(instr_asm, "\t$rd,$imm16"),
     [(set RC:$rd, (load addr:$imm16))], IILoad> {
  let isPseudo = Pseudo;
}

let canFoldAsLoad = 1 in
class LoadDispM<bits<4> opB, string instr_asm, RegisterClass RC, Operand MemOpnd, bit Pseudo>:
    MemDisp<opB, (outs RC:$rd), (ins MemOpnd:$imm16, RC:$ra),
    !strconcat(instr_asm, "\t$rd,$imm16[$ra]"),
    [(set RC:$rd, (load (add RC:$ra, addr:$imm16)))], IILoad> {
    let isPseudo = Pseudo;
}

let canFoldAsLoad = 1 in
class LoadImmDispM<bits<4> opB, string instr_asm, RegisterClass RC, Operand Od, PatLeaf imm_type, bit Pseudo>:
    MemImmDisp<opB, (outs RC:$rd), (ins Od:$imm16, RC:$ra),
    !strconcat(instr_asm, "\t$rd,$imm16[$ra]"),
    [(set RC:$rd, (load (add RC:$ra, imm_type:$imm16)))], IILoad> {
    let isPseudo = Pseudo;
}

class StoreM<bits<4> opB, string instr_asm, RegisterClass RC,
             Operand MemOpnd, bit Pseudo>:
  FMem<opB, (outs), (ins RC:$rd, MemOpnd:$imm16),
     !strconcat(instr_asm, "\t$rd,$imm16"),
     [(store RC:$rd, addr:$imm16)], IIStore> {
  let isPseudo = Pseudo;
}

// Store with 16-bit displacement(offset) stored in ra.
class StoreDispM<bits<4> opB, string asmstr, RegisterClass RC, Operand MemOpnd, bit Pseudo>:
    MemDisp<opB, (outs), (ins RC:$rd, MemOpnd:$imm16, CPURegs:$ra),
    !strconcat(asmstr, "\t$rd,$imm16[$ra]"),
    [(store RC:$rd, (add CPURegs:$ra, addr:$imm16))], IIStore> {
    let isPseudo = Pseudo;
}

// Store 16-bit immediate with 16-bit displacement stored in ra
class StoreImmDispM<bits<4> opB, string asmstr, RegisterClass RC, Operand Od, PatLeaf imm_type, bit Pseudo>:
    MemImmDisp<opB, (outs), (ins RC:$rd, Od:$imm16, CPURegs:$ra),
    !strconcat(asmstr, "\t$rd,$imm16[$ra]"),
    [(store RC:$rd, (add CPURegs:$ra, imm_type:$imm16))], IIStore> {
    let isPseudo = Pseudo;
}

// 16-bit load.
class LoadM16<bits<4> opB, string instr_asm,
                   bit Pseudo = 0>
  : LoadM<opB, instr_asm, CPURegs, mem, Pseudo> {
}

class LoadDispM16<bits<4> opB, string instr_asm,
                   bit Pseudo = 0>
  : LoadDispM<opB, instr_asm, CPURegs, mem, Pseudo> {
}

class LoadImmDispM16<bits<4> opB, string instr_asm,
                   bit Pseudo = 0>
  : LoadImmDispM<opB, instr_asm, CPURegs, simm16, immSExt16, Pseudo> {
}

// 16-bit store.
class StoreM16<bits<4> opB, string instr_asm,
                    bit Pseudo = 0>
  : StoreM<opB, instr_asm, CPURegs, mem, Pseudo> {
}

class StoreDispM16<bits<4> opB, string instr_asm,
                    bit Pseudo = 0>
  : StoreDispM<opB, instr_asm, CPURegs, mem, Pseudo> {
}

class StoreImmDispM16<bits<4> opB, string instr_asm,
                    bit Pseudo = 0>
  : StoreImmDispM<opB, instr_asm, CPURegs, simm16, immSExt16, Pseudo> {
}

let isBranch=1, isTerminator=1, isBarrier=1, hasDelaySlot = 1,
    isIndirectBranch = 1 in
class JumpFR<bits<4> opB, string instr_asm, RegisterClass RC>:
  FRX<(outs), (ins RC:$ra),
     !strconcat(instr_asm, "\t0[$ra]"), [(brind RC:$ra)], IIBranch> {
  let rb = opB;
  let rd = 0;
  let imm16 = 0;
}

class CBranch<bits<4> opB, string instr_asm, RegisterClass RC>:
  FRX<(outs), (ins RC:$ra, brtarget:$imm16),
             !strconcat(instr_asm, "\t$ra, $imm16"),
             [], IIBranch> {
  let rb = opB;
  let rd = 0;
  let isBranch = 1;
  let isTerminator = 1;
  let hasDelaySlot = 1;
}

class RetBase<RegisterClass RC>: JumpFR<3, "jump", RC> {
  let isReturn = 1;
  let isCodeGenOnly = 1;
  let hasCtrlDep = 1;
  let hasExtraSrcRegAllocReq = 1;
}

// TODO: Check registers
let isCall=1, hasDelaySlot=1 in {
  //@JumpLink {
  class JumpLink<bits<4> opB, string instr_asm>:
    FRX<(outs), (ins calltarget:$target, variable_ops),
       !strconcat(instr_asm, "\t$target"), [(Sigma16JmpLink imm:$target)],
       IIBranch> {
    let rd = 0;
    let ra = 14;
    let rb = opB;
    let imm16 = 0;

       }
  //@JumpLink }

  class JumpLinkReg<bits<4> opB, string instr_asm,
                    RegisterClass RC>:
    FRX<(outs), (ins RC:$rd, variable_ops),
       !strconcat(instr_asm, "\t0[$rd]"), [(Sigma16JmpLink RC:$rd)], IIBranch> {
    let rb = opB;
    let ra = 14;
    let imm16 = 0;
  }
}

//===----------------------------------------------------------------------===//
/// Arithmetic Instructions (3-Operand, RRR-Type)
def ADD     : ALU_rr<0x0, "add", add, IIAlu, CPURegs, /*isComm=*/ 1>;
def SUB     : ALU_rr<0x1, "sub", sub, IIAlu, CPURegs>;
def MUL     : ALU_rr<0x2, "mul", mul, IIMulDiv, CPURegs, /*isComm=*/ 1>;
def DIV     : ALU_rr<0x3, "div", sdiv, IIMulDiv, CPURegs>;

// Arithmetic Logic Instructions (3-Operand, RRR-Type)
// FIXME: Only work for constants.
def AND     : LogicW<and, 0x1, CPURegs>;
def XOR     : LogicW<xor, 0x6, CPURegs>;
def OR      : LogicW<or, 0x7, CPURegs>;
def NOR     : LogicWNot<or, 0x8, CPURegs>;
def NAND    : LogicWNot<and, 0x14, CPURegs>;

/// Load/Store Instructions (2-Operand, RX-Type)
def LEA     : ALU_ri<0x0, "lea", add, simm16, immSExt16, CPURegs>;
def LEA_SET : EffectiveAddress<0x0, "lea", CPURegs, mem>;
def LD      : LoadM16<0x1, "load">;
def ST      : StoreM16<0x2, "store">;

// Load/Store Instructions with 16-bit displacement (3-Operand, RX-Type)
// LEADISP : LeaMDisp<0x0, "lea", CPURegs, simm16>;
def LDDISP  : LoadDispM16<0x1, "load">;
def STDISP  : StoreDispM16<0x2, "store">;

// Load/Store Instructions with 16-bit immediate with 16-bit displacement (3-Operand, RX-Type)
def LDIMMDISP     : LoadImmDispM16<0x1, "load">;
def STIMMDISP     : StoreImmDispM16<0x2, "store">;

// Comparison Instructions
def CMP     : CmpInstr<0x4, "cmp", IIAlu, CPURegs, SR, 0>;

// TODO: These are pseudo-instructions.
//def ANDW     : ALU_rr<0x0, "andw", and, IIAlu, CPURegs, 1>;
//def ORW     : ALU_rr<0x1, "orw", or, IIAlu, CPURegs, 1>;
//def XORW     : ALU_rr<0x2, "xorw", xor, IIAlu, CPURegs, 1>;

def JR      : JumpFR<3, "jump", CPURegs>;
def RET     : RetBase<CPURegs>;

// Control Flow

def JMP     : UncondBranch<0x3, "jmp">;

def JEQ     : CBranch<0xa, "jumpeq", CPURegs>;
def JNE     : CBranch<0xa, "jumpne", CPURegs>;
def JLT     : CBranch<0xa, "jumplt", CPURegs>;
def JGT     : CBranch<0xa, "jumpgt", CPURegs>;
def JLE     : CBranch<0xa, "jumple", CPURegs>;
def JGE     : CBranch<0xa, "jumpge", CPURegs>;

def JAL    : JumpLinkReg<0xf, "jal", CPURegs>;

//===----------------------------------------------------------------------===//
//  Arbitrary patterns that map to one or more instructions
//===----------------------------------------------------------------------===//

def : Pat<(i16 immSExt16:$in),
          (LEA R0, imm:$in)>;


def : Pat<(i16 (Sigma16GlobalAddr tglobaladdr:$dst)), (LEA R0, tglobaladdr:$dst)>;
def : Pat<(add CPURegs:$src, (Sigma16GlobalAddr tglobaladdr:$src2)),
          (LEA CPURegs:$src, tglobaladdr:$src2)>;

// brcond for cmp instruction
multiclass BrcondPatsCmp<RegisterClass RC, Instruction JEQOp, Instruction JNEOp,
Instruction JLTOp, Instruction JGTOp, Instruction JLEOp, Instruction JGEOp,
Instruction CMPOp, Instruction CMPuOp, Register ZEROReg> {
def : Pat<(brcond (i16 (seteq RC:$lhs, RC:$rhs)), bb:$dst),
          (JEQOp (CMPOp RC:$lhs, RC:$rhs), bb:$dst)>;
def : Pat<(brcond (i16 (setueq RC:$lhs, RC:$rhs)), bb:$dst),
          (JEQOp (CMPuOp RC:$lhs, RC:$rhs), bb:$dst)>;
def : Pat<(brcond (i16 (setne RC:$lhs, RC:$rhs)), bb:$dst),
          (JNEOp (CMPOp RC:$lhs, RC:$rhs), bb:$dst)>;
def : Pat<(brcond (i16 (setune RC:$lhs, RC:$rhs)), bb:$dst),
          (JNEOp (CMPuOp RC:$lhs, RC:$rhs), bb:$dst)>;
def : Pat<(brcond (i16 (setlt RC:$lhs, RC:$rhs)), bb:$dst),
          (JLTOp (CMPOp RC:$lhs, RC:$rhs), bb:$dst)>;
def : Pat<(brcond (i16 (setult RC:$lhs, RC:$rhs)), bb:$dst),
          (JLTOp (CMPuOp RC:$lhs, RC:$rhs), bb:$dst)>;
def : Pat<(brcond (i16 (setgt RC:$lhs, RC:$rhs)), bb:$dst),
          (JGTOp (CMPOp RC:$lhs, RC:$rhs), bb:$dst)>;
def : Pat<(brcond (i16 (setugt RC:$lhs, RC:$rhs)), bb:$dst),
          (JGTOp (CMPuOp RC:$lhs, RC:$rhs), bb:$dst)>;
def : Pat<(brcond (i16 (setle RC:$lhs, RC:$rhs)), bb:$dst),
          (JLEOp (CMPOp RC:$lhs, RC:$rhs), bb:$dst)>;
def : Pat<(brcond (i16 (setule RC:$lhs, RC:$rhs)), bb:$dst),
          (JLEOp (CMPuOp RC:$lhs, RC:$rhs), bb:$dst)>;
def : Pat<(brcond (i16 (setge RC:$lhs, RC:$rhs)), bb:$dst),
          (JGEOp (CMPOp RC:$lhs, RC:$rhs), bb:$dst)>;
def : Pat<(brcond (i16 (setuge RC:$lhs, RC:$rhs)), bb:$dst),
          (JGEOp (CMPuOp RC:$lhs, RC:$rhs), bb:$dst)>;

def : Pat<(brcond RC:$cond, bb:$dst),
          (JNEOp (CMPOp RC:$cond, ZEROReg), bb:$dst)>;
}
