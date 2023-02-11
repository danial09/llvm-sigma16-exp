//===-- Sigma16MCExpr.h - Sigma16 specific MC expression classes ----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SIGMA16_MCTARGETDESC_SIGMA16MCEXPR_H
#define LLVM_LIB_TARGET_SIGMA16_MCTARGETDESC_SIGMA16MCEXPR_H

#include "llvm/MC/MCAsmLayout.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCValue.h"

namespace llvm {

class Sigma16MCExpr : public MCTargetExpr {
public:
  enum Sigma16ExprKind {
    CEK_None,
    CEK_GlobalAddress,
    CEK_Special,
  };

private:
  const Sigma16ExprKind Kind;
  const MCExpr *Expr;

  explicit Sigma16MCExpr(Sigma16ExprKind Kind, const MCExpr *Expr)
      : Kind(Kind), Expr(Expr) {}

public:
  static const Sigma16MCExpr *create(Sigma16ExprKind Kind, const MCExpr *Expr,
                                     MCContext &Ctx);
  static const Sigma16MCExpr *create(const MCSymbol *Symbol,
                                     Sigma16MCExpr::Sigma16ExprKind Kind,
                                     MCContext &Ctx);
  static const Sigma16MCExpr *createGpOff(Sigma16ExprKind Kind,
                                          const MCExpr *Expr, MCContext &Ctx);

  /// Get the kind of this expression.
  Sigma16ExprKind getKind() const { return Kind; }

  /// Get the child of this expression.
  const MCExpr *getSubExpr() const { return Expr; }

  void printImpl(raw_ostream &OS, const MCAsmInfo *MAI) const override;
  bool evaluateAsRelocatableImpl(MCValue &Res, const MCAsmLayout *Layout,
                                 const MCFixup *Fixup) const override;
  void visitUsedExpr(MCStreamer &Streamer) const override;
  MCFragment *findAssociatedFragment() const override {
    return getSubExpr()->findAssociatedFragment();
  }

  void fixELFSymbolsInTLSFixups(MCAssembler &Asm) const override;

  static bool classof(const MCExpr *E) {
    return E->getKind() == MCExpr::Target;
  }

  bool isGpOff(Sigma16ExprKind &Kind) const;
  bool isGpOff() const {
    Sigma16ExprKind Kind;
    return isGpOff(Kind);
  }
};
} // end namespace llvm

#endif
