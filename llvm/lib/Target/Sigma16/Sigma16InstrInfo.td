//===- Sigma16InstrInfo.td - Target Description for Sigma16  -*- tablegen -*-=//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//



//===----------------------------------------------------------------------===//
//
// This file describes Sigma16 instructions.
//
//===----------------------------------------------------------------------===//

include "Sigma16InstrFormats.td"

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

// // Arithmetic and logical instructions with 2 register operands.
// class ArithLogicI<bits<8> op, string instr_asm, SDNode OpNode,
//                   Operand Od, PatLeaf imm_type, RegisterClass RC> :
//   FRX<op, (outs CPURegs:$rd), (ins RC:$ra, Od:$disp),
//      !strconcat(instr_asm, "\t$rd,$ra[$disp]"),
//      [(set GPROut:$ra, (OpNode RC:$rb, imm_type:$imm16))], IIAlu> {
//   let isReMaterializable = 1;
// }

class FMem<bits<4> opB, dag outs, dag ins, string asmstr, list<dag> pattern,
          InstrItinClass itin>: FL<op, outs, ins, asmstr, pattern, itin>
    : FRX<outs, ins, asmstr, pattern, itin> {
    let b = opB;
    let a = 0;

    let DecoderMethod = "DecodeMem";
}

// Memory Load/Store
let canFoldAsLoad = 1 in
class LoadM<bits<4> opB, string instr_asm, PatFrag OpNode, RegisterClass RC,
            Operand MemOpnd, bit Pseudo>:
  FMem<opB, (outs RC:$rd), (ins MemOpnd:$disp),
     !strconcat(instr_asm, "\t$rd,$disp"),
     [(set RC:$rd, (OpNode addr:$disp))], IILoad> {
  let isPseudo = Pseudo;
}

class StoreM<bits<4> opB, string instr_asm, PatFrag OpNode, RegisterClass RC,
             Operand MemOpnd, bit Pseudo>:
  FMem<opB, (outs), (ins RC:$rd, MemOpnd:$disp),
     !strconcat(instr_asm, "\t$rd,$disp"),
     [(OpNode RC:$rd, addr:$disp)], IIStore> {
  let isPseudo = Pseudo;
}

//@ 16-bit load.
class LoadM16<bits<4> opB, string instr_asm, PatFrag OpNode,
                   bit Pseudo = 0>
  : LoadM<op, instr_asm, OpNode, CPURegs, mem, Pseudo> {
}

// 16-bit store.
class StoreM16<bits<4> op, string instr_asm, PatFrag OpNode,
                    bit Pseudo = 0>
  : StoreM<op, instr_asm, OpNode, CPURegs, mem, Pseudo> {
}

/// Arithmetic Instructions (3-Operand, RRR-Type)

def ADD     : ArithLogicR<0x0, "add", add, IIAlu, CPURegs, 1>;
def SUB     : ArithLogicR<0x1, "sub", sub, IIAlu, CPURegs>;
def MUL     : ArithLogicR<0x2, "mul", mul, IIAlu, CPURegs, /*isComm=*/ 1>;
// def DIV     : ArithLogicR<0x3, "div", div, IIAlu, CPURegs>

/// Load and Store Instructions
///  aligned
def LD     : LoadM32<0x01,  "load",  load_a>;
def ST     : StoreM32<0x02, "store",  store_a>;