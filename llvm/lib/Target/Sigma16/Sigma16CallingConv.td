//=- Sigma16CallingConv.td - Calling Conventions for Sigma16 -*- tablegen -===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// This describes the calling conventions for Sigma16 architecture.
//===----------------------------------------------------------------------===//

/// CCIfSubtarget - Match if the current subtarget has a feature F.
class CCIfSubtarget<string F, CCAction A>:
  CCIf<!strconcat("State.getTarget().getSubtarget<Sigma16Subtarget>().", F), A>;

//===----------------------------------------------------------------------===//
// Sigma16 Return Values Calling Convention
//===----------------------------------------------------------------------===//
def RetCC_SIGMA16 : CallingConv<[
    // i16 is returned in R1
    CCIfType<[i16], CCAssignToReg<[R1]>>,

    // Integer values get stored in stack slots that are 2 byte both in size and alignment.
    CCIfType<[i16], CCAssignToStack<2,2>>
]>;

//===----------------------------------------------------------------------===//
// Sigma16 Argument Calling Convention
//===----------------------------------------------------------------------===//
def CC_SIGMA16 : CallingConv<[
    // Promote i8 to i16
    CCIfType<[i8], CCPromoteToType<i16>>,

    // The first 4 arguments are passed in registers R1-R4
    CCIfType<[i16], CCAssignToReg<[R1, R2, R3, R4]>>,

    // Integer values get stored in stack slots that are 2 byte both in size and alignment.
    CCIfType<[i16], CCAssignToStack<2, 2>>,
]>;

// Callee save to registers R1-R11
def CC_Save : CalleeSavedRegs<(add R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11)>;
