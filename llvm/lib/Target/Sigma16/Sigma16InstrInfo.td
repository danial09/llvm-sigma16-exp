//===- Sigma16InstrInfo.td - Target Description for Sigma16  -*- tablegen -*-=//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//



//===----------------------------------------------------------------------===//
// Type profiles
//===----------------------------------------------------------------------===//

def MoveImm32Ty : SDTypeProfile<1, 1, [
  SDTCisSameAs<0, 1>, SDTCisInt<0>
]>;

def SDT_Sigma16CallSeqStart : SDCallSeqStart<[ SDTCisVT<0, i16> ]>;

def SDT_Sigma16CallSeqEnd   : SDCallSeqEnd<[
  SDTCisVT<0, i16>, SDTCisVT<1, i16>
]>;

def Sigma16RetFlag    : SDNode<"Sigma16ISD::RET_FLAG", SDTNone,
                           [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;
def callseq_start : SDNode<"ISD::CALLSEQ_START", SDT_Sigma16CallSeqStart,
                           [SDNPHasChain, SDNPOutGlue]>;
def callseq_end   : SDNode<"ISD::CALLSEQ_END",   SDT_Sigma16CallSeqEnd,
                           [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue]>;

def SDT_Sigma16Call    : SDTypeProfile<0, -1, [SDTCisPtrTy<0>]>;

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
  ComplexPattern<iPTR, 2, "SelectAddr", [frameindex], [SDNPWantParent]>;

def mem : Operand<iPTR> {
  let PrintMethod = "printMemOperand";
  let MIOperandInfo = (ops CPURegs, i16imm);
  let EncoderMethod = "getMemEncoding";
//  let ParserMatchClass = Cpu0MemAsmOperand;
}

//===----------------------------------------------------------------------===//
// Instructions specific formats
//===----------------------------------------------------------------------===//

// Arithmetic and logical instructions with 3 register operands.
class ArithLogicR<bits<4> op, string instr_asm, SDNode OpNode,
                  InstrItinClass itin, RegisterClass RC, bit isComm = 0>:
    FRRR<op, (outs CPURegs:$rd), (ins RC:$ra, RC:$rb),
    !strconcat(instr_asm, "\t$rd,$ra,$rb"),
    [(set CPURegs:$rd, (OpNode RC:$ra, RC:$rb))], itin> {
    let isCommutable = isComm;	// e.g. add ra rb =  add rb ra
    let isReMaterializable = 1;
}

// Memory instructions
class Mem<bits<4> opB, dag outs, dag ins, string asmstr, list<dag> pattern,
            InstrItinClass itin>: FRX<outs, ins, asmstr, pattern, itin> {
    let rb = opB;
    let ra = 0;

class FMem<bits<4> opB, dag outs, dag ins, string asmstr, list<dag> pattern,
          InstrItinClass itin>: FL<op, outs, ins, asmstr, pattern, itin>
    : FRX<outs, ins, asmstr, pattern, itin> {
    let b = opB;
    let a = 0;

    let DecoderMethod = "DecodeMem";
}

// Memory instructions with 16-bit displacement stored in ra
class MemDisp<bits<4> opB, dag outs, dag ins, string asmstr, list<dag> pattern,
            InstrItinClass itin>: FRX<outs, ins, asmstr, pattern, itin> {
    let rb = opB;
    let DecoderMethod = "decodeMem";
}

// Lea with no displacement is equivalent to mov
let isMoveImm = 1 in
class LeaM<bits<4> opB, string asmstr, RegisterClass RC, Operand MemOpnd>:
    Mem<opB, (outs RC:$rd), (ins MemOpnd:$imm16),
    !strconcat(asmstr, "\t$rd,$imm16"),
    [(set CPURegs:$rd, (addr:$imm16))], IILea> {

}

let canFoldAsLoad = 1 in
class LoadM<bits<4> opB, string asmstr, RegisterClass RC, Operand MemOpnd>:
    Mem<opB, (outs RC:$rd), (ins MemOpnd:$imm16),
    !strconcat(asmstr, "\t$rd,$imm16"),
   [(set i16:$rd, (load addr:$imm16))], IILoad> {

}

// Load with 16-bit displacement(offset) stored in ra, similar to "lw $rd, 0x1234($ra)" in MIPS
//let canFoldAsLoad = 1 in
//class LoadDispM<bits<4> opB, string asmstr, RegisterClass RC, Operand MemOpnd>:
//    MemDisp<opB, (outs RC:$rd), (ins MemOpnd:$imm16, CPURegs:$ra),
//    !strconcat(asmstr, "\t$rd,$imm16[$ra]"),



class StoreM<bits<4> opB, string asmstr, RegisterClass RC, Operand MemOpnd>:
    Mem<opB, (outs), (ins RC:$rd, MemOpnd:$imm16),
    !strconcat(asmstr, "\t$rd,$imm16"),
    [(store i16:$rd, addr:$imm16)], IIStore> {
}

//===----------------------------------------------------------------------===//
/// Arithmetic Instructions (3-Operand, RRR-Type)

def ADD     : ArithLogicR<0x0, "add", add, IIAlu, CPURegs, 1>;
def SUB     : ArithLogicR<0x1, "sub", sub, IIAlu, CPURegs>;
def MUL     : ArithLogicR<0x2, "mul", mul, IIAlu, CPURegs, /*isComm=*/ 1>;
// def DIV     : ArithLogicR<0x3, "div", div, IIAlu, CPURegs>

/// Load/Store Instructions (2-Operand, RX-Type)
def LEA     : LeaM<0x0, "lea", CPURegs, mem>;
def LD      : LoadM<0x1, "load", CPURegs, mem>;
def ST      : StoreM<0x2, "store", CPURegs, mem>;

/// Load/Store Instructions with 16-bit displacement (3-Operand, RX-Type)

//TODO: implement this
