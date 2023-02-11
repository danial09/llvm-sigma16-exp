//==- Sigma16InstrFormats.td - Sigma16 Instruction Formats -*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//  Describe Sigma16 instructions format
//
//  CPU INSTRUCTION FORMATS
//
//
//  op              4-bit major operation code
//
//  rd              4-bit destination register
//
//  ra              4-bit source register
//
//  rb              4-bit source register
//
//  displacement    16-bit immediate, branch displacement, or
//  or disp         address displacement
//===----------------------------------------------------------------------===//

// Format specifies the encoding used by the instruction. To be used by
// code emitter to determine which form of fixup to use.
class InstFormat<bits<5> val> {
  bits<5> Value = val;
}
def InstFormatPseudo : InstFormat<0>;
def InstFormatRRR    : InstFormat<1>;
def InstFormatRX     : InstFormat<2>;
def InstFormatRRRK   : InstFormat<3>;
def InstFormatOther  : InstFormat<4>;

// Generic Sigma16 Format
class Sigma16Inst<dag outs, dag ins, int size, string asmstr,
    list<dag> pattern, InstrItinClass itin, InstFormat f>
    : Instruction {

    // Inst and Size for tablegen emitter. Note that although the largest
    // possible size is currently 32 bits, there are additional Sigma16
    // formats currently not implemented which can be of a larger size.
    field bits<32> Inst;
    // SoftFail is not used, but is required to build the tablegen decode table.
    field bits<32> SoftFail = 0;
    InstFormat Format = f;
    let Size = size;

    let Namespace = "Sigma16";

    bits<4> Opcode;

    dag OutOperandList = outs;
    dag InOperandList  = ins;

    let AsmString = asmstr;
	let Pattern = pattern;
	let Itinerary = itin;
}


class Sigma16Pseudo<dag outs, dag ins, string asmstr, list<dag> pattern,
    InstrItinClass itin> : Sigma16Inst<outs, ins, 0, asmstr, pattern, itin,
    InstFormatPseudo> {
    let isPseudo = 1;
    let isCodeGenOnly = 1;
}

//===----------------------------------------------------------------------===//
// Format RRR Instruction : <|op|d|a|b|>
//===----------------------------------------------------------------------===//

class FRRR<bits<4> op, dag outs, dag ins, string asmstr, list<dag> pattern,
	InstrItinClass itin>:
		Sigma16Inst<outs, ins, 2, asmstr, pattern, itin, InstFormatRRR> {

    bits<4> rd;
    bits<4> ra;
    bits<4> rb;

    let Opcode = op;

    let Inst{15-12} = op;
    let Inst{11-8} = rd;
    let Inst{7-4} = ra;
    let Inst{3-0} = rb;
}

//===----------------------------------------------------------------------===//
// Format RX Instruction : <|'f'|d|a|b|disp|>
//===----------------------------------------------------------------------===//

class FRX<dag outs, dag ins, string asmstr, list<dag> pattern,
	InstrItinClass itin>:
    	Sigma16Inst<outs, ins, 2, asmstr, pattern, itin, InstFormatRX> {

	bits<4> rd;
	bits<4> ra;
	bits<4> rb;
	bits<16> imm16;

	let Inst{31-28} = 0b1111;
    let Inst{27-24} = rd;
    let Inst{23-20} = ra;
    let Inst{19-16} = rb;
	let Inst{15-0} = imm16;
}

//===----------------------------------------------------------------------===//
// Format RRRK Instruction : <|'e'|d|b|a|b|k|>
//===----------------------------------------------------------------------===//

class FRRRK<dag outs, dag ins, string asmstr, list<dag> pattern,
    InstrItinClass itin>:
        Sigma16Inst<outs, ins, 2, asmstr, pattern, itin, InstFormatRRRK> {

    bits<4> rd;
    bits<4> ra;
    bits<4> rb;
    bits<8> b;
    bits<8> k;

    let Inst{31-28} = 0b1110;
    let Inst{27-24} = rd;
    let Inst{23-16} = b;
    let Inst{15-12} = ra;
    let Inst{11-8} = rb;
    let Inst{7-0} = k;
}
