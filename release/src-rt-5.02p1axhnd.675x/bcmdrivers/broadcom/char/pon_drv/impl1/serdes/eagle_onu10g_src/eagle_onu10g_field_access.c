/*
  <:copyright-BRCM:2015:proprietary:standard
  
     Copyright (c) 2015 Broadcom 
     All Rights Reserved
  
   This program is the proprietary software of Broadcom and/or its
   licensors, and may only be used, duplicated, modified or distributed pursuant
   to the terms and conditions of a separate, written license agreement executed
   between you and Broadcom (an "Authorized License").  Except as set forth in
   an Authorized License, Broadcom grants no license (express or implied), right
   to use, or waiver of any kind with respect to the Software, and Broadcom
   expressly reserves all rights in and to the Software and all intellectual
   property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE
   NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY
   BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
  
   Except as expressly set forth in the Authorized License,
  
   1. This program, including its structure, sequence and organization,
      constitutes the valuable trade secrets of Broadcom, and you shall use
      all reasonable efforts to protect the confidentiality thereof, and to
      use this information only in connection with your use of Broadcom
      integrated circuit products.
  
   2. TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
      AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
      WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
      RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND
      ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT,
      FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR
      COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE
      TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF USE OR
      PERFORMANCE OF THE SOFTWARE.
  
   3. TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR
      ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
      INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY
      WAY RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN
      IF BROADCOM HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES;
      OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE
      SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS
      SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF ANY
      LIMITED REMEDY.
  :>
*/


/** @file eagle_onu10g_field_access.c
 * Registers and field access
 */

#include "eagle_onu10g_field_access.h"
#include "eagle_onu10g_dependencies.h"
#include "eagle_onu10g_ipconfig.h"
#include "eagle_onu10g_internal.h"

err_code_t _eagle_onu10g_pmd_rdt_field(uint16_t addr, uint8_t shift_left, uint8_t shift_right, uint16_t *val_p) {
#ifdef ATE_LOG
    EFUN_PRINTF(("// ATE_LOG Addr x%04x : MASK x%04x : Right Shift %d\n",addr,(((uint16_t)(0xFFFF<<shift_left))>>shift_right)<<(shift_right-shift_left),shift_right-shift_left));
#endif

  EFUN(eagle_onu10g_pmd_rdt_reg(addr,val_p));
  *val_p <<= shift_left;                   /* Move the MSB to the left most      [shift_left  = (15-msb)]     */
  *val_p >>= shift_right;                  /* Right shift entire field to bit 0  [shift_right = (15-msb+lsb)] */

  return(ERR_CODE_NONE);
}

err_code_t _eagle_onu10g_pmd_rdt_field_signed(uint16_t addr, uint8_t shift_left, uint8_t shift_right, int16_t *val_p) {
#ifdef ATE_LOG
    EFUN_PRINTF(("// ATE_LOG Addr x%04x : Signed MASK x%04x : Right Shift %d\n",addr,(((uint16_t)(0xFFFF<<shift_left))>>shift_right)<<(shift_right-shift_left),shift_right-shift_left));
#endif

  EFUN(eagle_onu10g_pmd_rdt_reg(addr,(uint16_t *)val_p));
  *val_p <<= shift_left;                   /* Move the sign bit to the left most [shift_left  = (15-msb)]     */
  *val_p >>= shift_right;                  /* Right shift entire field to bit 0  [shift_right = (15-msb+lsb)] */

  return(ERR_CODE_NONE);
}

/*-------------------------------*/
/* Byte Write and Read Functions */
/*-------------------------------*/

err_code_t _eagle_onu10g_pmd_mwr_reg_byte(uint16_t addr, uint16_t mask, uint8_t lsb, uint8_t val) {

  EFUN(eagle_onu10g_pmd_mwr_reg(addr, mask, lsb, (uint16_t) val));

  return(ERR_CODE_NONE);
}

  err_code_t _eagle_onu10g_pmd_rdt_field_byte(uint16_t addr, uint8_t shift_left, uint8_t shift_right, uint8_t *val8_p) {

  uint16_t val;
#ifdef ATE_LOG
    EFUN_PRINTF(("// ATE_LOG Addr x%04x : MASK x%04x : Right Shift %d\n",addr,(((uint16_t)(0xFFFF<<shift_left))>>shift_right)<<(shift_right-shift_left),shift_right-shift_left));
#endif

  EFUN(eagle_onu10g_pmd_rdt_reg(addr,&val));
  val <<= shift_left;                   /* Move the MSB to the left most      [shift_left  = (15-msb)]     */
  val >>= shift_right;                  /* Right shift entire field to bit 0  [shift_right = (15-msb+lsb)] */

  *val8_p = (uint8_t) val;

  return(ERR_CODE_NONE);
}

err_code_t _eagle_onu10g_pmd_rdt_field_signed_byte(uint16_t addr, uint8_t shift_left, uint8_t shift_right, int8_t *val8_p) {

  int16_t val;
#ifdef ATE_LOG
    EFUN_PRINTF(("// ATE_LOG Addr x%04x : Signed MASK x%04x : Right Shift %d\n",addr,(((uint16_t)(0xFFFF<<shift_left))>>shift_right)<<(shift_right-shift_left),shift_right-shift_left));
#endif

  EFUN(eagle_onu10g_pmd_rdt_reg(addr,(uint16_t *) &val));
  val <<= shift_left;                   /* Move the sign bit to the left most [shift_left  = (15-msb)]     */
  val >>= shift_right;                  /* Right shift entire field to bit 0  [shift_right = (15-msb+lsb)] */

  *val8_p = (int8_t) val;

  return(ERR_CODE_NONE);
}

/*-------------------------------*/
/* MDIO specific Misc functions  */
/*-------------------------------*/

err_code_t eagle_onu10g_pmd_mdio_mwr_reg(uint16_t addr, uint16_t mask, uint8_t lsb, uint16_t val) {

  /* Can't use "wrc_mdio_maskdata(~mask)" to avoid circular function calls */
#if              defined(MDIO_MMDSEL_AER_COM_MDIO_MASKDATA)
  EFUN(eagle_onu10g_pmd_wr_reg(MDIO_MMDSEL_AER_COM_MDIO_MASKDATA, ~mask));
#elif            defined(DIG_COM_MASKDATA_REG)
  EFUN(eagle_onu10g_pmd_wr_reg(DIG_COM_MASKDATA_REG, ~mask));
#else
#error "SERDES_MDIO_MASKED_WRITE enabled without mask register specified."
#endif
  EFUN(eagle_onu10g_pmd_wr_reg(addr, val<<lsb));


  return(ERR_CODE_NONE);
}


/*--------------------------*/
/* EVAL specific functions  */
/*--------------------------*/

uint16_t _eagle_onu10g_pmd_rde_reg(uint16_t addr, err_code_t *err_code_p)
{
  uint16_t data;
  EPFUN(eagle_onu10g_pmd_rdt_reg(addr, &data));
  return data;
}

uint16_t _eagle_onu10g_pmd_rde_field(uint16_t addr, uint8_t shift_left, uint8_t shift_right, err_code_t *err_code_p)
{
  uint16_t data;
#ifdef ATE_LOG
    EFUN_PRINTF(("// ATE_LOG Addr x%04x : MASK x%04x : Right Shift %d\n",addr,(((uint16_t)(0xFFFF<<shift_left))>>shift_right)<<(shift_right-shift_left),shift_right-shift_left));
#endif
  EPSTM(data = _eagle_onu10g_pmd_rde_reg(addr, err_code_p));

  data <<= shift_left;                 /* Move the sign bit to the left most [shift_left  = (15-msb)] */
  data >>= shift_right;                /* Right shift entire field to bit 0  [shift_right = (15-msb+lsb)] */

  return data;
}

int16_t _eagle_onu10g_pmd_rde_field_signed(uint16_t addr, uint8_t shift_left, uint8_t shift_right, err_code_t *err_code_p)
{
  int16_t  data;
#ifdef ATE_LOG
    EFUN_PRINTF(("// ATE_LOG Addr x%04x : Signed MASK x%04x : Right Shift %d\n",addr,(((uint16_t)(0xFFFF<<shift_left))>>shift_right)<<(shift_right-shift_left),shift_right-shift_left));
#endif
  EPSTM(data = (int16_t) _eagle_onu10g_pmd_rde_reg(addr, err_code_p));  /* convert it to signed */

  data <<= shift_left;             /* Move the sign bit to the left most    [shift_left  = (15-msb)] */
  data >>= shift_right;            /* Move to the right with sign extension [shift_right = (15-msb+lsb)] */

  return data;
}

uint8_t _eagle_onu10g_pmd_rde_field_byte(uint16_t addr, uint8_t shift_left, uint8_t shift_right, err_code_t *err_code_p) {
   return ((uint8_t) _eagle_onu10g_pmd_rde_field(addr, shift_left, shift_right, err_code_p));
}

int8_t _eagle_onu10g_pmd_rde_field_signed_byte(uint16_t addr, uint8_t shift_left, uint8_t shift_right, err_code_t *err_code_p) {
  return ((int8_t) _eagle_onu10g_pmd_rde_field_signed(addr, shift_left, shift_right, err_code_p));
}
