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

def SDT_Sigma16DivRem : SDTypeProfile<0, 2, [SDTCisInt<0>, SDTCisSameAs<0, 1>]>;
def Sigma16DivRem     : SDNode<"Sigma16ISD::Sigma16DivRem", SDT_Sigma16DivRem,
                                [SDNPOutGlue]>;

//===----------------------------------------------------------------------===//
// Instruction Formats
//===----------------------------------------------------------------------===//

include "Sigma16InstrFormats.td"

//===----------------------------------------------------------------------===//
// Instruction operand types
//===----------------------------------------------------------------------===//

def memsrc : Operand<i16> {
  let MIOperandInfo = (ops CPURegs, i16imm);
  let PrintMethod = "printAddrModeMemSrc";
  let EncoderMethod = "getMemSrcValue";
}

//===----------------------------------------------------------------------===//
// Instructions specific formats
//===----------------------------------------------------------------------===//

let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in
class ALU_rr<bits<4> op, string instr_asm, SDNode OpNode,
                  InstrItinClass itin, RegisterClass RC, bit isComm = 0>:
    FRRR<op, (outs CPURegs:$rd), (ins RC:$ra, RC:$rb),
    !strconcat(instr_asm, "\t$rd,$ra,$rb"),
    [(set CPURegs:$rd, (OpNode RC:$ra, RC:$rb))], itin> {
    let isCommutable = isComm;	// e.g. add ra rb =  add rb ra
}

// Memory instructions
class Mem<bits<4> opB, string instr_asm, list<dag> pattern,
                  InstrItinClass itin>:
    FRX<(outs CPURegs:$rd), (ins memsrc:$imm16),
    !strconcat(instr_asm, "\t$rd,$imm16"),
    [(set CPURegs:$rd, (OpNode RC:$imm16))], itin> {
    let rb = opB;
    let ra = 0;

    let DecoderMethod = "decodeMem";
}

class MemDisp<bits<4> opB, string instr_asm, list<dag> pattern,
                  InstrItinClass itin>:
    FRX<(outs CPURegs:$rd), (ins memsrc:$imm16, CPURegs:$ra),
    !strconcat(instr_asm, "\t$rd,$imm16[$ra]"),
    pattern, itin> {
    let rb = opB;

    let DecoderMethod = "decodeMem";
}



// Memory instructions with 2 register operands and displacement
//  (i.e. constant zero register as offset).
class FMem2<bits<4> opB, dag outs, dag ins, string asmstr, list<dag> patterns,
           InstrItinClass itin> :
    FRX<outs, ins, asmstr, patterns, itin> {
    let rb = opB;
    let ra = 0; // 0 since no register offset

    let DecoderMethod = "DecodeMem";
}

// Memory instruction with 3 register operands and displacement
//  (i.e. register offset).
class FMem3<bits<4> opB, bits<4> ir_sa, dag outs, dag ins, string asmstr,
           list<dag> patterns, InstrItinClass itin> :
    FRX<outs, ins, asmstr, patterns, itin> {
    let rb = opB;
    let ra = ir_sa;

    let DecoderMethod = "DecodeMem";
}



// // Memory Load/Store
// let canFoldAsLoad = 1 in
// class LoadM<bits<4> opB, string instr_asm, PatFrag OpNode, RegisterClass RC,
//             Operand MemOpnd, bit Pseudo>:
//   FMem<opB, (outs RC:$rd), (ins MemOpnd:$disp),
//      !strconcat(instr_asm, "\t$rd,$disp"),
//      [(set RC:$rd, (OpNode addr:$disp))], IILoad> {
//   let isPseudo = Pseudo;
// }

// class StoreM<bits<4> opB, string instr_asm, PatFrag OpNode, RegisterClass RC,
//              Operand MemOpnd, bit Pseudo>:
//   FMem<opB, (outs), (ins RC:$rd, MemOpnd:$disp),
//      !strconcat(instr_asm, "\t$rd,$disp"),
//      [(OpNode RC:$rd, addr:$disp)], IIStore> {
//   let isPseudo = Pseudo;
// }

// //@ 16-bit load.
// class LoadM16<bits<4> opB, string    instr_asm, PatFrag OpNode,
//                    bit Pseudo = 0>
//   : LoadM<op, instr_asm, OpNode, CPURegs, mem, Pseudo> {
// }

// // 16-bit store.
// class StoreM16<bits<4> op, string instr_asm, PatFrag OpNode,
//                     bit Pseudo = 0>
//   : StoreM<op, instr_asm, OpNode, CPURegs, mem, Pseudo> {
// }

/// Arithmetic Instructions (3-Operand, RRR-Type)

def ADD     : ALU_rr<0x0, "add", add, IIAlu, CPURegs, /*isComm=*/ 1>;
def SUB     : ALU_rr<0x1, "sub", sub, IIAlu, CPURegs>;
def MUL     : ALU_rr<0x2, "mul", mul, IIMulDiv, CPURegs, /*isComm=*/ 1>;
def DIV     : ALU_rr<0x3, "div", sdiv, IIMulDiv, CPURegs>;

/// Load/Store Instructions (2-Operand, RX-Type)
def LD      : Mem<0x0, "load", load, IILoad, CPURegs>;

/// Load and Store Instructions
///  aligned
// def LD     : LoadM32<0x01,  "load",  load_a>;
// def ST     : StoreM32<0x02, "store",  store_a>;
