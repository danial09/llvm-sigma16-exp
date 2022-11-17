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
//  d               4-bit destination register
//
//  a               4-bit source register
//
//  b              4-bit source or destination register
//
//  displacement    16-bit immediate, branch displacement, or
//  or disp         address displacement
//===----------------------------------------------------------------------===//

// Format specifies the encoding used by the instruction. To be used by
// code emitter to determine which form of fixup to use.
class InstFormat<bits<3> val> {
  bits<5> Value = val;
}
def InstFormatPseudo : InstFormat<0>;
def InstFormatRRR    : InstFormat<1>;
def InstFormatRX     : InstFormat<2>
def InstFormatOther  : InstFormat<3>

// Generic Sigma16 Format
class Sigma16Inst<dag outs, dag ins, int size, string asmstr, 
    list<dag> pattern, InstrItinClass itin, InstFormat f> 
    : Instruction {
    
    // Inst and Size for tablegen emitter. Note that although the largest 
    // possible size is currently 32 bits, there are additional Sigma16 
    // formats currently not implemented which can be of a larger size.
    field bits<32> Inst;
    field bits<32> SoftFail = 0;
    InstFormat Format = f;
    let Size = size;

    let Namespace = "Sigma16";

    dag OutOperandList = outs;
    dag InOperandList  = ins;

    let AsmString = asmstr;
	let Pattern = pattern;
	let Itinerary = itin;
}


//===----------------------------------------------------------------------===//
// Format RRR Instruction : <|op|d|a|b|>
//===----------------------------------------------------------------------===//

class FRRR<bits<4> op, dag outs, dag ins, string asmstr, list<dag> pattern
	InstrItinClass itin>: 
		Sigma16Inst<outs, ins, 2, asmstr, pattern, itin, InstFormatRRR> {

    bits<4> = d;
    bits<4> = a;
    bits<4> = b;

    let Opcode<15-12> = op; 
    let Inst<11-8> = d;
    let Inst <7-4> = a;
    let Inst<3-0> = b;
}

//===----------------------------------------------------------------------===//
// Format RX Instruction : <|op|d|a|b|disp|>
//===----------------------------------------------------------------------===//

class FRX<dag outs, dag ins, string asmstr, list<dag> pattern
	InstrItinClass itin>:
    	Sigma16Inst<outs, ins, 2, asmstr, pattern, itin, InstFormatRX> {
	
	bits<4> = f;
	bits<4> = a;
	bits<4> = b;
	bits<16> = disp;

	let Opcode<31-28> = 0b1111;
    let Inst<27-24> = d;
    let Inst <23-20> = a;
    let Inst<19-16> = b;
	let Int<15-0> = disp;
}

