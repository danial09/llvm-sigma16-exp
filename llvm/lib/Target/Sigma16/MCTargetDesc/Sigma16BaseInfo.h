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
// Target Operand Flag enum.
enum TOF {
    //===------------------------------------------------------------------===//
    // Sigma16 Specific MachineOperand flags.

    MO_NO_FLAG,
    //===------------------------------------------------------------------===//

    /// MO_GOT_CALL - Represents the offset into the global offset table at
    /// which the address of a call site relocation entry symbol resides
    /// during execution. This is different from the above since this flag
    /// can only be present in call instructions.
    MO_GOT_CALL,

    /// MO_GPREL - Represents the offset from the current gp value to be used
    /// for the relocatable object file being produced.
    MO_GPREL,

};
enum {
    InstFormatPseudo = 0,
    InstFormatRRR = 1,
    InstFormatRX = 2,
    InstFormatROther = 3,

    InstFormatMask = 15
};
} // end namespace Sigma16II
} // end namespace llvm
