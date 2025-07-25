/*
 * RISC-V translation routines for the Zb[abcs] Standard Extension.
 *
 * Copyright (c) 2020 Kito Cheng, kito.cheng@sifive.com
 * Copyright (c) 2020 Frank Chang, frank.chang@sifive.com
 * Copyright (c) 2021 Philipp Tomsich, philipp.tomsich@vrull.eu
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2 or later, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

static void gen_clz(TCGContext *tcg_ctx, TCGv ret, TCGv arg1)
{
    tcg_gen_clzi_tl(tcg_ctx, ret, arg1, TARGET_LONG_BITS);
}

static bool trans_clz(DisasContext *ctx, arg_clz *a)
{
    return gen_unary(ctx, a, gen_clz);
}

static void gen_ctz(TCGContext *tcg_ctx, TCGv ret, TCGv arg1)
{
    tcg_gen_ctzi_tl(tcg_ctx, ret, arg1, TARGET_LONG_BITS);
}

static bool trans_ctz(DisasContext *ctx, arg_ctz *a)
{
    return gen_unary(ctx, a, gen_ctz);
}

static bool trans_cpop(DisasContext *ctx, arg_cpop *a)
{
    return gen_unary(ctx, a, tcg_gen_ctpop_tl);
}

static bool trans_andn(DisasContext *ctx, arg_andn *a)
{
    TCGContext *tcg_ctx = ctx->uc->tcg_ctx;
    return gen_arith(tcg_ctx, a, tcg_gen_andc_tl);
}

static bool trans_orn(DisasContext *ctx, arg_orn *a)
{
    TCGContext *tcg_ctx = ctx->uc->tcg_ctx;
    return gen_arith(tcg_ctx, a, tcg_gen_orc_tl);
}

static bool trans_xnor(DisasContext *ctx, arg_xnor *a)
{
    TCGContext *tcg_ctx = ctx->uc->tcg_ctx;
    return gen_arith(tcg_ctx, a, tcg_gen_eqv_tl);
}

static bool trans_min(DisasContext *ctx, arg_min *a)
{
    TCGContext *tcg_ctx = ctx->uc->tcg_ctx;
    return gen_arith(tcg_ctx, a, tcg_gen_smin_tl);
}

static bool trans_max(DisasContext *ctx, arg_max *a)
{
    TCGContext *tcg_ctx = ctx->uc->tcg_ctx;
    return gen_arith(tcg_ctx, a, tcg_gen_smax_tl);
}

static bool trans_minu(DisasContext *ctx, arg_minu *a)
{
    TCGContext *tcg_ctx = ctx->uc->tcg_ctx;
    return gen_arith(tcg_ctx, a, tcg_gen_umin_tl);
}

static bool trans_maxu(DisasContext *ctx, arg_maxu *a)
{
    TCGContext *tcg_ctx = ctx->uc->tcg_ctx;
    return gen_arith(tcg_ctx, a, tcg_gen_umax_tl);
}

static bool trans_sext_b(DisasContext *ctx, arg_sext_b *a)
{
    return gen_unary(ctx, a, tcg_gen_ext8s_tl);
}

static bool trans_sext_h(DisasContext *ctx, arg_sext_h *a)
{
    return gen_unary(ctx, a, tcg_gen_ext16s_tl);
}

static void gen_sbop_mask(TCGContext *tcg_ctx, TCGv ret, TCGv shamt)
{
    tcg_gen_movi_tl(tcg_ctx, ret, 1);
    tcg_gen_shl_tl(tcg_ctx, ret, ret, shamt);
}

static void gen_bset(TCGContext *tcg_ctx, TCGv ret, TCGv arg1, TCGv shamt)
{
    TCGv t = tcg_temp_new(tcg_ctx);

    gen_sbop_mask(tcg_ctx, t, shamt);
    tcg_gen_or_tl(tcg_ctx, ret, arg1, t);

    tcg_temp_free(tcg_ctx, t);
}

static bool trans_bset(DisasContext *ctx, arg_bset *a)
{
    return gen_shift(ctx, a, gen_bset);
}

static bool trans_bseti(DisasContext *ctx, arg_bseti *a)
{
    return gen_shifti(ctx, a, gen_bset);
}

static void gen_bclr(TCGContext *tcg_ctx, TCGv ret, TCGv arg1, TCGv shamt)
{
    TCGv t = tcg_temp_new(tcg_ctx);

    gen_sbop_mask(tcg_ctx, t, shamt);
    tcg_gen_andc_tl(tcg_ctx, ret, arg1, t);

    tcg_temp_free(tcg_ctx, t);
}

static bool trans_bclr(DisasContext *ctx, arg_bclr *a)
{
    return gen_shift(ctx, a, gen_bclr);
}

static bool trans_bclri(DisasContext *ctx, arg_bclri *a)
{
    return gen_shifti(ctx, a, gen_bclr);
}

static void gen_binv(TCGContext *tcg_ctx, TCGv ret, TCGv arg1, TCGv shamt)
{
    TCGv t = tcg_temp_new(tcg_ctx);

    gen_sbop_mask(tcg_ctx, t, shamt);
    tcg_gen_xor_tl(tcg_ctx, ret, arg1, t);

    tcg_temp_free(tcg_ctx, t);
}

static bool trans_binv(DisasContext *ctx, arg_binv *a)
{
    return gen_shift(ctx, a, gen_binv);
}

static bool trans_binvi(DisasContext *ctx, arg_binvi *a)
{
    return gen_shifti(ctx, a, gen_binv);
}

static void gen_bext(TCGContext *tcg_ctx, TCGv ret, TCGv arg1, TCGv shamt)
{
    tcg_gen_shr_tl(tcg_ctx, ret, arg1, shamt);
    tcg_gen_andi_tl(tcg_ctx, ret, ret, 1);
}

static bool trans_bext(DisasContext *ctx, arg_bext *a)
{
    return gen_shift(ctx, a, gen_bext);
}

static bool trans_bexti(DisasContext *ctx, arg_bexti *a)
{
    return gen_shifti(ctx, a, gen_bext);
}

static bool trans_ror(DisasContext *ctx, arg_ror *a)
{
    return gen_shift(ctx, a, tcg_gen_rotr_tl);
}

static bool trans_rori(DisasContext *ctx, arg_rori *a)
{
    return gen_shifti(ctx, a, tcg_gen_rotr_tl);
}

static bool trans_rol(DisasContext *ctx, arg_rol *a)
{
    return gen_shift(ctx, a, tcg_gen_rotl_tl);
}

static bool trans_rev8_32(DisasContext *ctx, arg_rev8_32 *a)
{
    REQUIRE_32BIT(ctx);
    return gen_unary(ctx, a, tcg_gen_bswap32_tl);
}

#if defined(TARGET_RISCV64)
static bool trans_rev8_64(DisasContext *ctx, arg_rev8_64 *a)
{
    return gen_unary(ctx, a, tcg_gen_bswap_tl);
}
#endif

static void gen_orc_b(TCGContext *tcg_ctx, TCGv ret, TCGv source1)
{
    TCGv  tmp = tcg_temp_new(tcg_ctx);
    TCGv  low7 = tcg_const_tl(tcg_ctx, dup_const_tl(MO_8, 0x7f));

    /* Set msb in each byte if the byte was non-zero. */
    tcg_gen_and_tl(tcg_ctx, tmp, source1, low7);
    tcg_gen_add_tl(tcg_ctx, tmp, tmp, low7);
    tcg_gen_or_tl(tcg_ctx, tmp, tmp, source1);

    /* Extract the msb to the lsb in each byte */
    tcg_gen_andc_tl(tcg_ctx, tmp, tmp, low7);
    tcg_gen_shri_tl(tcg_ctx, tmp, tmp, 7);

    /* Replicate the lsb of each byte across the byte. */
    tcg_gen_muli_tl(tcg_ctx, ret, tmp, 0xff);

    tcg_temp_free(tcg_ctx, low7);
    tcg_temp_free(tcg_ctx, tmp);
}

static bool trans_orc_b(DisasContext *ctx, arg_orc_b *a)
{
    return gen_unary(ctx, a, gen_orc_b);
}

#define GEN_SHADD(SHAMT)                                       \
static void gen_sh##SHAMT##add(TCGContext *tcg_ctx, TCGv ret, TCGv arg1, TCGv arg2) \
{                                                              \
    TCGv t = tcg_temp_new(tcg_ctx);                                   \
                                                               \
    tcg_gen_shli_tl(tcg_ctx, t, arg1, SHAMT);                           \
    tcg_gen_add_tl(tcg_ctx, ret, t, arg2);                              \
                                                               \
    tcg_temp_free(tcg_ctx, t);                                          \
}

GEN_SHADD(1)
GEN_SHADD(2)
GEN_SHADD(3)

#define GEN_TRANS_SHADD(SHAMT)                                             \
static bool trans_sh##SHAMT##add(DisasContext *ctx, arg_sh##SHAMT##add *a) \
{                                                                          \
    TCGContext *tcg_ctx = ctx->uc->tcg_ctx;                                \
    return gen_arith(tcg_ctx, a, gen_sh##SHAMT##add);                      \
}

GEN_TRANS_SHADD(1)
GEN_TRANS_SHADD(2)
GEN_TRANS_SHADD(3)

static bool trans_zext_h_32(DisasContext *ctx, arg_zext_h_32 *a)
{
    REQUIRE_32BIT(ctx);
    return gen_unary(ctx, a, tcg_gen_ext16u_tl);
}

#if defined(TARGET_RISCV64)
static bool trans_zext_h_64(DisasContext *ctx, arg_zext_h_64 *a)
{
    return gen_unary(ctx, a, tcg_gen_ext16u_tl);
}

static void gen_clzw(TCGContext *tcg_ctx, TCGv ret, TCGv arg1)
{
    TCGv t = tcg_temp_new(tcg_ctx);
    tcg_gen_shli_tl(tcg_ctx, t, arg1, 32);
    tcg_gen_clzi_tl(tcg_ctx, ret, t, 32);
    tcg_temp_free(tcg_ctx, t);
}

static bool trans_clzw(DisasContext *ctx, arg_clzw *a)
{
    return gen_unary(ctx, a, gen_clzw);
}

static void gen_ctzw(TCGContext *tcg_ctx, TCGv ret, TCGv arg1)
{
    tcg_gen_ctzi_tl(tcg_ctx, ret, arg1, 32);
}

static bool trans_ctzw(DisasContext *ctx, arg_ctzw *a)
{
    return gen_unary(ctx, a, gen_ctzw);
}

static void gen_cpopw(TCGContext *tcg_ctx, TCGv ret, TCGv arg1)
{
    tcg_gen_ext32u_tl(tcg_ctx, arg1, arg1);
    tcg_gen_ctpop_tl(tcg_ctx, ret, arg1);
}

static bool trans_cpopw(DisasContext *ctx, arg_cpopw *a)
{
    return gen_unary(ctx, a, gen_cpopw);
}

static bool gen_shiftw(DisasContext *ctx, arg_r *a,
                       void(*func)(TCGContext *, TCGv, TCGv, TCGv))
{
    TCGContext *tcg_ctx = ctx->uc->tcg_ctx;

    TCGv source1 = tcg_temp_new(tcg_ctx);
    TCGv source2 = tcg_temp_new(tcg_ctx);

    gen_get_gpr(tcg_ctx, source1, a->rs1);
    gen_get_gpr(tcg_ctx, source2, a->rs2);

    tcg_gen_andi_tl(tcg_ctx, source2, source2, 31);
    (*func)(tcg_ctx, source1, source1, source2);
    tcg_gen_ext32s_tl(tcg_ctx, source1, source1);

    gen_set_gpr(tcg_ctx, a->rd, source1);
    tcg_temp_free(tcg_ctx, source1);
    tcg_temp_free(tcg_ctx, source2);
    return true;
}

static void gen_rorw(TCGContext *tcg_ctx, TCGv ret, TCGv arg1, TCGv arg2)
{
    TCGv_i32 t1 = tcg_temp_new_i32(tcg_ctx);
    TCGv_i32 t2 = tcg_temp_new_i32(tcg_ctx);

    /* truncate to 32-bits */
    tcg_gen_trunc_tl_i32(tcg_ctx, t1, arg1);
    tcg_gen_trunc_tl_i32(tcg_ctx, t2, arg2);

    tcg_gen_rotr_i32(tcg_ctx, t1, t1, t2);

    /* sign-extend 64-bits */
    tcg_gen_ext_i32_tl(tcg_ctx, ret, t1);

    tcg_temp_free_i32(tcg_ctx, t1);
    tcg_temp_free_i32(tcg_ctx, t2);
}

static bool trans_rorw(DisasContext *ctx, arg_rorw *a)
{
    return gen_shiftw(ctx, a, gen_rorw);
}

static bool gen_shiftiw(DisasContext *ctx, arg_shift *a,
                        void(*func)(TCGContext *, TCGv, TCGv, TCGv))
{
    TCGContext *tcg_ctx = ctx->uc->tcg_ctx;

    TCGv source1 = tcg_temp_new(tcg_ctx);
    TCGv source2 = tcg_temp_new(tcg_ctx);

    gen_get_gpr(tcg_ctx, source1, a->rs1);
    tcg_gen_movi_tl(tcg_ctx, source2, a->shamt);

    (*func)(tcg_ctx, source1, source1, source2);
    tcg_gen_ext32s_tl(tcg_ctx, source1, source1);

    gen_set_gpr(tcg_ctx, a->rd, source1);
    tcg_temp_free(tcg_ctx, source1);
    tcg_temp_free(tcg_ctx, source2);
    return true;
}

static bool trans_roriw(DisasContext *ctx, arg_roriw *a)
{
    return gen_shiftiw(ctx, a, gen_rorw);
}

static void gen_rolw(TCGContext *tcg_ctx, TCGv ret, TCGv arg1, TCGv arg2)
{
    TCGv_i32 t1 = tcg_temp_new_i32(tcg_ctx);
    TCGv_i32 t2 = tcg_temp_new_i32(tcg_ctx);

    /* truncate to 32-bits */
    tcg_gen_trunc_tl_i32(tcg_ctx, t1, arg1);
    tcg_gen_trunc_tl_i32(tcg_ctx, t2, arg2);

    tcg_gen_rotl_i32(tcg_ctx, t1, t1, t2);

    /* sign-extend 64-bits */
    tcg_gen_ext_i32_tl(tcg_ctx, ret, t1);

    tcg_temp_free_i32(tcg_ctx, t1);
    tcg_temp_free_i32(tcg_ctx, t2);
}

static bool trans_rolw(DisasContext *ctx, arg_rolw *a)
{
    return gen_shiftw(ctx, a, gen_rolw);
}

#define GEN_SHADD_UW(SHAMT)                                       \
static void gen_sh##SHAMT##add_uw(TCGContext *tcg_ctx, TCGv ret, TCGv arg1, TCGv arg2) \
{                                                                 \
    TCGv t = tcg_temp_new(tcg_ctx);                                      \
                                                                  \
    tcg_gen_ext32u_tl(tcg_ctx, t, arg1);                                   \
                                                                  \
    tcg_gen_shli_tl(tcg_ctx, t, t, SHAMT);                                 \
    tcg_gen_add_tl(tcg_ctx, ret, t, arg2);                                 \
                                                                  \
    tcg_temp_free(tcg_ctx, t);                                             \
}

GEN_SHADD_UW(1)
GEN_SHADD_UW(2)
GEN_SHADD_UW(3)

#define GEN_TRANS_SHADD_UW(SHAMT)                             \
static bool trans_sh##SHAMT##add_uw(DisasContext *ctx,        \
                                    arg_sh##SHAMT##add_uw *a) \
{                                                             \
    TCGContext *tcg_ctx = ctx->uc->tcg_ctx;                   \
    return gen_arith(tcg_ctx, a, gen_sh##SHAMT##add_uw);      \
}

GEN_TRANS_SHADD_UW(1)
GEN_TRANS_SHADD_UW(2)
GEN_TRANS_SHADD_UW(3)

static void gen_add_uw(TCGContext *tcg_ctx, TCGv ret, TCGv arg1, TCGv arg2)
{
    TCGv t = tcg_temp_new(tcg_ctx);
    tcg_gen_ext32u_tl(tcg_ctx, t, arg1);
    tcg_gen_add_tl(tcg_ctx, ret, t, arg2);
    tcg_temp_free(tcg_ctx, t);
}

static bool trans_add_uw(DisasContext *ctx, arg_add_uw *a)
{
    TCGContext *tcg_ctx = ctx->uc->tcg_ctx;
    return gen_arith(tcg_ctx, a, gen_add_uw);
}

static bool trans_slli_uw(DisasContext *ctx, arg_slli_uw *a)
{
    TCGContext *tcg_ctx = ctx->uc->tcg_ctx;

    TCGv source1 = tcg_temp_new(tcg_ctx);
    gen_get_gpr(tcg_ctx, source1, a->rs1);

    if (a->shamt < 32) {
        tcg_gen_deposit_z_tl(tcg_ctx, source1, source1, a->shamt, 32);
    } else {
        tcg_gen_shli_tl(tcg_ctx, source1, source1, a->shamt);
    }

    gen_set_gpr(tcg_ctx, a->rd, source1);
    tcg_temp_free(tcg_ctx, source1);
    return true;
}
#endif

static bool trans_clmul(DisasContext *ctx, arg_clmul *a)
{
    TCGContext *tcg_ctx = ctx->uc->tcg_ctx;
    return gen_arith(tcg_ctx, a, gen_helper_clmul);
}

static void gen_clmulh(TCGContext *tcg_ctx, TCGv dst, TCGv src1, TCGv src2)
{
     gen_helper_clmulr(tcg_ctx, dst, src1, src2);
     tcg_gen_shri_tl(tcg_ctx, dst, dst, 1);
}

static bool trans_clmulh(DisasContext *ctx, arg_clmulr *a)
{
    TCGContext *tcg_ctx = ctx->uc->tcg_ctx;
    return gen_arith(tcg_ctx, a, gen_clmulh);
}

static bool trans_clmulr(DisasContext *ctx, arg_clmulh *a)
{
    TCGContext *tcg_ctx = ctx->uc->tcg_ctx;
    return gen_arith(tcg_ctx, a, gen_helper_clmulr);
}
