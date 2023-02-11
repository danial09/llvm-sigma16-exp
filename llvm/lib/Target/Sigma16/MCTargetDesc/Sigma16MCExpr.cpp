//===-- Sigma16MCExpr.cpp - Sigma16 specific MC expression classes --------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "Sigma16.h"

#include "Sigma16MCExpr.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCAssembler.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCObjectStreamer.h"
#include "llvm/MC/MCSymbolELF.h"

using namespace llvm;

#define DEBUG_TYPE "sigma16mcexpr"

const Sigma16MCExpr *Sigma16MCExpr::create(Sigma16MCExpr::Sigma16ExprKind Kind,
        const MCExpr *Expr, MCContext &Ctx) {
    return new (Ctx) Sigma16MCExpr(Kind, Expr);
}

const Sigma16MCExpr *Sigma16MCExpr::create(const MCSymbol *Symbol, Sigma16MCExpr::Sigma16ExprKind Kind,
        MCContext &Ctx) {
    const MCSymbolRefExpr *MCSym =
        MCSymbolRefExpr::create(Symbol, MCSymbolRefExpr::VK_None, Ctx);
    return new (Ctx) Sigma16MCExpr(Kind, MCSym);
}

const Sigma16MCExpr *Sigma16MCExpr::createGpOff(Sigma16MCExpr::Sigma16ExprKind Kind,
        const MCExpr *Expr, MCContext &Ctx) {
    return create(Kind, create(CEK_None, create(CEK_GlobalAddress, Expr, Ctx), Ctx), Ctx);
}

void Sigma16MCExpr::printImpl(raw_ostream &OS, const MCAsmInfo *MAI) const {
    int64_t AbsVal;

    switch (Kind) {
    case CEK_None:
    case CEK_Special:
        llvm_unreachable("CEK_None and CEK_Special are invalid");
        break;
    case CEK_GlobalAddress:
        OS << "%gp_rel";
        break;
    }

    OS << '(';
    if (Expr->evaluateAsAbsolute(AbsVal))
        OS << AbsVal;
    else
        Expr->print(OS, MAI, true);
    OS << ')';
}

bool
Sigma16MCExpr::evaluateAsRelocatableImpl(MCValue &Res,
        const MCAsmLayout *Layout,
        const MCFixup *Fixup) const {
    return getSubExpr()->evaluateAsRelocatable(Res, Layout, Fixup);
}

void Sigma16MCExpr::visitUsedExpr(MCStreamer &Streamer) const {
    Streamer.visitUsedExpr(*getSubExpr());
}

void Sigma16MCExpr::fixELFSymbolsInTLSFixups(MCAssembler &Asm) const {
    switch ((int)getKind()) {
    case CEK_None:
    case CEK_Special:
        llvm_unreachable("CEK_None and CEK_Special are invalid");
        break;
    default:
        break;
    }
}

bool Sigma16MCExpr::isGpOff(Sigma16ExprKind &Kind) const {
    if (const Sigma16MCExpr *S1 = dyn_cast<const Sigma16MCExpr>(getSubExpr())) {
        if (const Sigma16MCExpr *S2 = dyn_cast<const Sigma16MCExpr>(S1->getSubExpr())) {
            if (S1->getKind() == CEK_None && S2->getKind() == CEK_GlobalAddress) {
                Kind = getKind();
                return true;
            }
        }
    }
    return false;
}

