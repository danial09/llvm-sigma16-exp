//===-- Sigma16BaseInfo.h - Top level definitions for Sigma16 MC -*- C++ -*-==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains small standalone helper functions and enum definitions for
// the Sigma16 target useful for the compiler back-end and the MC libraries.
//
//===----------------------------------------------------------------------===//

#include "Sigma16MCTargetDesc.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/Support/DataTypes.h"
#include "llvm/Support/ErrorHandling.h"

namespace llvm {
/// Sigma16II - This namespace holds all of the target specific flags
/// that instruction info tracks. All definitions must match
/// Sigma16InstrFormats.td.
///@Sigma16II {
namespace Sigma16II {
enum {
  InstFormatPseudo = 0,
  InstFormatRRR = 1,
  InstFormatRX = 2,
  InstFormatROther = 3,

  InstFormatMask = 15
};

// Sigma16 Specific MachineOperand flags.
enum {
  MO_NO_FLAG,
};
} // end namespace Sigma16II
} // end namespace llvm
