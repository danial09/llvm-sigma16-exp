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

def CSR_O16 : CalleeSavedRegs<(add R1, R13,
                                   (sequence "R%u", 1, 0))>;

