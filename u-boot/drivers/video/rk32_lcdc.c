/*
 * (C) Copyright 2008-2014 Rockchip Electronics
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#include <common.h>
#include <lcd.h>
#include <asm/arch/rkplat.h>
#include "rockchip_fb.h"

#define lvds_regs			RKIO_LVDS_PHYS

#define LcdReadBit(addr, msk)		((regbak.addr=preg->addr)&(msk))
#define LcdWrReg(addr, val)		preg->addr=regbak.addr=(val)
#define LcdRdReg(addr)			(preg->addr)
#define LcdSetBit(addr, msk)		preg->addr=((regbak.addr) |= (msk))
#define LcdClrBit(addr, msk)		preg->addr=((regbak.addr) &= ~(msk))
#define LcdSetRegBit(addr, msk)		preg->addr=((preg->addr) |= (msk))
#define LcdMskReg(addr, msk, val)	(regbak.addr)&=~(msk);   preg->addr=(regbak.addr|=(val))
#define LCDC_REG_CFG_DONE()		LcdWrReg(REG_CFG_DONE, 0x01); 


/********************************************************************
**                          结构定义                                *
********************************************************************/
/* LCDC的寄存器结构 */
typedef volatile struct tagLCDC_REG
{
	/* offset 0x00~0xc0 */
	unsigned int REG_CFG_DONE;           //0x00 REGISTER CONFIG FINISH
	unsigned int VERSION_INFO;           //0x04
	unsigned int SYS_CTRL;               //0x08 system control register 0
	unsigned int SYS_CTRL1;              //0x0c system control register 1
	unsigned int DSP_CTRL0;			//0x10 display control register 0
	unsigned int DSP_CTRL1;			//0x14 display control register 1
	unsigned int DSP_BG;                 //0x18 backgroundcolor
	unsigned int MCU_CTRL;				 //0x1c MCU mode contol register
	unsigned int INTR_CTRL0;             //0x20 Interruptctrl register0
	unsigned int INTR_CTRL1;             //0x24 Interruptctrl register1
	unsigned int INTR_RESERVED0;         //0x28 
	unsigned int INTR_RESERVED1;         //0x2c
	
	unsigned int WIN0_CTRL0;             //0x30 win0 ctrlregister0
	unsigned int WIN0_CTRL1;             //0x34 win0 ctrlregister1
	unsigned int WIN0_COLOR_KEY;         //0x38 Win0 colorkey register
	unsigned int WIN0_VIR;               //0x3c Win0 virtual stride
	unsigned int WIN0_YRGB_MST;          //0x40 Win0 YRGB memory start address
	unsigned int WIN0_CBR_MST;           //0x44 Win0 Cbr memory start address
	unsigned int WIN0_ACT_INFO;          //0x48 Win0 active window width/height
	unsigned int WIN0_DSP_INFO;          //0x4c Win0 display width/height on panel
	unsigned int WIN0_DSP_ST;            //0x50 Win0 display start point on panel
	unsigned int WIN0_SCL_FACTOR_YRGB;   //0x54 Win0 YRGB scaling factor 
	unsigned int WIN0_SCL_FACTOR_CBR;    //0x58 Win0 Cbr scaling factor 
	unsigned int WIN0_SCL_OFFSET;        //0x5c Win0 scaling start point offset
	unsigned int WIN0_SRC_ALPHA_CTRL;    //0x60
	unsigned int WIN0_DST_ALPHA_CTRL;    //0x64
	unsigned int WIN0_FADING_CTRL;       //0x68
	unsigned int WIN0_RESERVED0;         //0x6c
	
	unsigned int WIN1_CTRL0;             //0x70 win1 ctrlregister0
	unsigned int WIN1_CTRL1;             //0x74 win1 ctrlregister1
	unsigned int WIN1_COLOR_KEY;         //0x78 Win1 colorkey register
	unsigned int WIN1_VIR;               //0x7c Win1 virtualstride
	unsigned int WIN1_YRGB_MST;          //0x80 Win1 YRGB memory start address
	unsigned int WIN1_CBR_MST;           //0x84 Win1 Cbr memory start address
	unsigned int WIN1_ACT_INFO;          //0x88 Win1 active window width/height
	unsigned int WIN1_DSP_INFO;          //0x8c Win1 display width/height on panel
	unsigned int WIN1_DSP_ST;            //0x90 Win1 display start point on panel
	unsigned int WIN1_SCL_FACTOR_YRGB;   //0x94 Win1 YRGB scaling factor 
	unsigned int WIN1_SCL_FACTOR_CBR;    //0x98 Win1 Cbr scaling factor 
	unsigned int WIN1_SCL_OFFSET;        //0x9c Win1 scaling start point offset
	unsigned int WIN1_SRC_ALPHA_CTRL;    //0xa0
	unsigned int WIN1_DST_ALPHA_CTRL;    //0xa4
	unsigned int WIN1_FADING_CTRL;       //0xa8
	unsigned int WIN1_RESERVED0;         //0xac  
	unsigned int RESERVED2[48];          //0xb0-0x16c
	unsigned int POST_DSP_HACT_INFO;     //0x170 posts caler down horizontal start and end
	unsigned int POST_DSP_VACT_INFO;     //0x174 Panel active horizontal scanning start point and end point
	unsigned int POST_SCL_FACTOR_YRGB;   //0x178 posty rgb scaling factor
	unsigned int POST_RESERVED;          //0x17c 
	unsigned int POST_SCL_CTRL;          //0x180 post scaling start point offset
	unsigned int POST_DSP_VACT_INFO_F1;  //0x184 Panel active horizontal scanning start point and end point F1
	unsigned int DSP_HTOTAL_HS_END;       //0x188 Panel scanning horizontal width and hsync pulse end point
	unsigned int DSP_HACT_ST_END;         //0x18c Panel active horizontal scanning start/end point
	unsigned int DSP_VTOTAL_VS_END;       //0x190 Panel scanning vertical height and vsync pulse end point
	unsigned int DSP_VACT_ST_END;         //0x194 Panel active vertical scanning start/end point
	unsigned int DSP_VS_ST_END_F1;        //0x198 Vertical scanning start point and vsync pulse end point of even filed in interlace mode
	unsigned int DSP_VACT_ST_END_F1;      //0x19c Vertical scanning active start/end point of even filed in interlace mode
	unsigned int RESERVED3[4];		//0x1a0-0x1ac
	unsigned int BCSH_COLOR_BAR;		//0x1b0 BCSH CTRL
	unsigned int BCSH_BCS;			//0x1b4	BCSH BCS
	unsigned int BCSH_H;			//0x1b8 BCSH HUE

} LCDC_REG, *pLCDC_REG;

/*******************register definition**********************/
#define m_fpga_version (0xffff<<16)
#define m_rtl_version  (0xffff)

#define m_auto_gating_en (1<<23)
#define m_standby_en     (1<<22)
#define m_dma_stop       (1<<21)
#define m_mmu_en         (1<<20)
#define m_dma_burst_length (0x3<<18)
#define m_mipi_out_en      (1<<15)
#define m_edp_out_en       (1<<14)
#define m_hdmi_out_en      (1<<13)
#define m_rgb_out_en       (1<<12)
#define m_edpi_wms_fs      (1<<10)
#define m_edpi_wms_mode    (1<<9)
#define m_edpi_halt_en     (1<<8)
#define m_doub_ch_overlap_num (0xf<<4)
#define m_doub_channel_en     (1<<3)
#define m_direct_path_layer_sel (0x3<<1)
#define m_direct_path_en       (1)

#define v_auto_gating_en(x) (((x)&1)<<23)
#define v_standby_en(x)     (((x)&1)<<22)
#define v_dma_stop(x)       (((x)&1)<<21)
#define v_mmu_en(x)         (((x)&1)<<20)
#define v_dma_burst_length(x) (((x)&3)<<18)
#define v_mipi_out_en(x)      (((x)&1)<<15)
#define v_edp_out_en(x)       (((x)&1)<<14)
#define v_hdmi_out_en(x)      (((x)&1)<<13)
#define v_rgb_out_en(x)       (((x)&1)<<12)
#define v_edpi_wms_fs(x)      (((x)&1)<<10)
#define v_edpi_wms_mode(x)    (((x)&1)<<9)
#define v_edpi_halt_en(x)     (((x)&1)<<8)
#define v_doub_ch_overlap_num(x) (((x)&0xf)<<4)
#define v_doub_channel_en(x)     (((x)&1)<<3)
#define v_direct_path_layer_sel(x) (((x)&3)<<1)
#define v_direct_path_en(x)       ((x)&1)


#define m_axi_outstanding_max_num (0x1f<<13)
#define m_axi_max_outstanding_en  (1<<12)
#define m_noc_win_qos             (3<<10)
#define m_noc_qos_en              (1<<9)
#define m_noc_hurry_threshold     (0x3f<<3)
#define m_noc_hurry_value         (0x3<<1)
#define m_noc_hurry_en            (1)

#define v_axi_outstanding_max_num(x) (((x)&0x1f)<<13)
#define v_axi_max_outstanding_en(x)  (((x)&1)<<12)
#define v_noc_win_qos(x)             (((x)&3)<<10)
#define v_noc_qos_en(x)              (((x)&1)<<9)
#define v_noc_hurry_threshold(x)     (((x)&0x3f)<<3)
#define v_noc_hurry_value(x)         (((x)&3)<<1)
#define v_noc_hurry_en(x)            ((x)&1)


#define m_dsp_y_mir_en              (1<<23)
#define m_dsp_x_mir_en              (1<<22)
#define m_dsp_yuv_clip              (1<<21) 
#define m_dsp_ccir656_avg           (1<<20)
#define m_dsp_black_en              (1<<19)
#define m_dsp_blank_en              (1<<18) 
#define m_dsp_out_zero              (1<<17)
#define m_dsp_dummy_swap            (1<<16)
#define m_dsp_delta_swap            (1<<15)
#define m_dsp_rg_swap               (1<<14)
#define m_dsp_rb_swap               (1<<13)
#define m_dsp_bg_swap               (1<<12)
#define m_dsp_field_pol             (1<<11)  
#define m_dsp_interlace             (1<<10)
#define m_dsp_ddr_phase             (1<<9)
#define m_dsp_dclk_ddr              (1<<8)
#define m_dsp_dclk_pol              (1<<7)
#define m_dsp_den_pol               (1<<6)
#define m_dsp_vsync_pol             (1<<5)
#define m_dsp_hsync_pol             (1<<4)
#define m_dsp_out_mode              (0xf)

#define v_dsp_y_mir_en(x)              (((x)&1)<<23)
#define v_dsp_x_mir_en(x)              (((x)&1)<<22)
#define v_dsp_yuv_clip(x)              (((x)&1)<<21) 
#define v_dsp_ccir656_avg(x)           (((x)&1)<<20)
#define v_dsp_black_en(x)              (((x)&1)<<19)
#define v_dsp_blank_en(x)              (((x)&1)<<18) 
#define v_dsp_out_zero(x)              (((x)&1)<<17)
#define v_dsp_dummy_swap(x)            (((x)&1)<<16)
#define v_dsp_delta_swap(x)            (((x)&1)<<15)
#define v_dsp_rg_swap(x)               (((x)&1)<<14)
#define v_dsp_rb_swap(x)               (((x)&1)<<13)
#define v_dsp_bg_swap(x)               (((x)&1)<<12)
#define v_dsp_field_pol(x)             (((x)&1)<<11)  
#define v_dsp_interlace(x)             (((x)&1)<<10)
#define v_dsp_ddr_phase(x)             (((x)&1)<<9)
#define v_dsp_dclk_ddr(x)              (((x)&1)<<8)
#define v_dsp_dclk_pol(x)              (((x)&1)<<7)
#define v_dsp_den_pol(x)               (((x)&1)<<6)
#define v_dsp_vsync_pol(x)             (((x)&1)<<5)
#define v_dsp_hsync_pol(x)             (((x)&1)<<4)
#define v_dsp_out_mode(x)              ((x)&0xf)


#define m_dsp_layer3_sel               (3<<14)
#define m_dsp_layer2_sel               (3<<12)
#define m_dsp_layer1_sel               (3<<10)
#define m_dsp_layer0_sel               (3<<8)
#define m_dither_up_en                 (1<<6)
#define m_dither_down_sel              (1<<4)
#define m_dither_down_mode             (1<<3)
#define m_dither_down_en               (1<<2)
#define m_pre_dither_down_en           (1<<1)
#define m_dsp_lut_en                   (1)

#define v_dsp_layer3_sel(x)                (((x)&3)<<14)
#define v_dsp_layer2_sel(x)                (((x)&3)<<12)
#define v_dsp_layer1_sel(x)                (((x)&3)<<10)
#define v_dsp_layer0_sel(x)                (((x)&3)<<8)
#define v_dither_up_en(x)                  (((x)&1)<<6)
#define v_dither_down_sel(x)               (((x)&1)<<4)
#define v_dither_down_mode(x)              (((x)&1)<<3)
#define v_dither_down_en(x)                (((x)&1)<<2)
#define v_pre_dither_down_en(x)            (((x)&1)<<1)
#define v_dsp_lut_en(x)                    ((x)&1)


#define m_dsp_bg_red     (0x3f<<20)
#define m_dsp_bg_green   (0x3f<<10)
#define m_dsp_bg_blue    (0x3f<<0)

#define v_dsp_bg_red(x)     (((x)&0x3f)<<20)
#define v_dsp_bg_green(x)   (((x)&0x3f)<<10)
#define v_dsp_bg_blue(x)    (((x)&0x3f)<<0)


#define m_win0_yuv_clip     (1<<20)
#define m_win0_cbr_deflick  (1<<19)
#define m_win0_yrgb_deflick  (1<<18)
#define m_win0_ppas_zero_en  (1<<16)
#define m_win0_uv_swap       (1<<15)
#define m_win0_mid_swap      (1<<14)
#define m_win0_alpha_swap    (1<<13)
#define m_win0_rb_swap       (1<<12)
#define m_win0_csc_mode      (3<<10)
#define m_win0_no_outstanding (1<<9)
#define m_win0_interlace_read  (1<<8)
#define m_win0_lb_mode         (7<<5)
#define m_win0_fmt_10          (1<<4)
#define m_win0_data_fmt        (7<<1)
#define m_win0_en              (1)

#define v_win0_yuv_clip(x)       (((x)&1)<<20)
#define v_win0_cbr_deflick(x)    (((x)&1)<<19)
#define v_win0_yrgb_deflick(x)   (((x)&1)<<18)
#define v_win0_ppas_zero_en(x)   (((x)&1)<<16)
#define v_win0_uv_swap(x)        (((x)&1)<<15)
#define v_win0_mid_swap(x)       (((x)&1)<<14)
#define v_win0_alpha_swap(x)     (((x)&1)<<13)
#define v_win0_rb_swap(x)        (((x)&1)<<12)
#define v_win0_csc_mode(x)       (((x)&3)<<10)
#define v_win0_no_outstanding(x) (((x)&1)<<9)
#define v_win0_interlace_read(x)  (((x)&1)<<8)
#define v_win0_lb_mode(x)         (((x)&7)<<5)
#define v_win0_fmt_10(x)          (((x)&1)<<4)
#define v_win0_data_fmt(x)        (((x)&7)<<1)
#define v_win0_en(x)              ((x)&1)


#define m_win0_cbr_vsd_mode        (1<<31)
#define m_win0_cbr_vsu_mode        (1<<30)
#define m_win0_cbr_hsd_mode        (3<<28)
#define m_win0_cbr_ver_scl_mode    (3<<26)
#define m_win0_cbr_hor_scl_mode    (3<<24)
#define m_win0_yrgb_vsd_mode       (1<<23)
#define m_win0_yrgb_vsu_mode       (1<<22)
#define m_win0_yrgb_hsd_mode       (3<<20)
#define m_win0_yrgb_ver_scl_mode   (3<<18)
#define m_win0_yrgb_hor_scl_mode   (3<<16)
#define m_win0_line_load_mode      (1<<15)
#define m_win0_cbr_axi_gather_num  (7<<12)
#define m_win0_yrgb_axi_gather_num (0xf<<8)
#define m_win0_vsd_cbr_gt2         (1<<7)
#define m_win0_vsd_cbr_gt4         (1<<6)
#define m_win0_vsd_yrgb_gt2        (1<<5)
#define m_win0_vsd_yrgb_gt4        (1<<4)
#define m_win0_bic_coe_sel         (3<<2)
#define m_win0_cbr_axi_gather_en   (1<<1)
#define m_win0_yrgb_axi_gather_en  (1)

#define v_win0_cbr_vsd_mode(x)        (((x)&1)<<31)
#define v_win0_cbr_vsu_mode(x)        (((x)&1)<<30)
#define v_win0_cbr_hsd_mode(x)        (((x)&3)<<28)
#define v_win0_cbr_ver_scl_mode(x)    (((x)&3)<<26)
#define v_win0_cbr_hor_scl_mode(x)    (((x)&3)<<24)
#define v_win0_yrgb_vsd_mode(x)       (((x)&1)<<23)
#define v_win0_yrgb_vsu_mode(x)       (((x)&1)<<22)
#define v_win0_yrgb_hsd_mode(x)       (((x)&3)<<20)
#define v_win0_yrgb_ver_scl_mode(x)   (((x)&3)<<18)
#define v_win0_yrgb_hor_scl_mode(x)   (((x)&3)<<16)
#define v_win0_line_load_mode(x)      (((x)&1)<<15)
#define v_win0_cbr_axi_gather_num(x)  (((x)&7)<<12)
#define v_win0_yrgb_axi_gather_num(x) (((x)&0xf)<<8)
#define v_win0_vsd_cbr_gt2(x)         (((x)&1)<<7)
#define v_win0_vsd_cbr_gt4(x)         (((x)&1)<<6)
#define v_win0_vsd_yrgb_gt2(x)        (((x)&1)<<5)
#define v_win0_vsd_yrgb_gt4(x)        (((x)&1)<<4)
#define v_win0_bic_coe_sel(x)         (((x)&3)<<2)
#define v_win0_cbr_axi_gather_en(x)   (((x)&1)<<1)
#define v_win0_yrgb_axi_gather_en(x)  ((x)&1)


#define m_win0_key_en                 (1<<31)
#define m_win0_key_color              (0x3fffffff)

#define v_win0_key_en(x)                 (((x)&1)<<31)
#define v_win0_key_color(x)              ((x)&0x3fffffff)

#define v_ARGB888_VIRWIDTH(x) 	(((x)&0x3fff)<<0)
#define v_RGB888_VIRWIDTH(x) 	(((((x*3)>>2)+((x)%3))&0x3fff)<<0)
#define v_RGB565_VIRWIDTH(x) 	 ((DIV_ROUND_UP(x,2)&0x3fff)<<0)
#define v_YUV_VIRWIDTH(x)    	 ((DIV_ROUND_UP(x,4)&0x3fff)<<0)

#define v_act_height(x)             (((x)&0x1fff)<<16)
#define v_act_width(x)              ((x)&0x1fff)

#define v_dsp_height(x)             (((x)&0xfff)<<16)
#define v_dsp_width(x)              ((x)&0xfff)

#define v_dsp_yst(x)                (((x)&0x1fff)<<16)
#define v_dsp_xst(x)                ((x)&0x1fff)


#define v_X_SCL_FACTOR(x)  (((x)&0xffff)<<0)
#define v_Y_SCL_FACTOR(x)  (((x)&0xffff)<<16)

#define v_win0_vs_offset_cbr(x)     (((x)&0xff)<<24)
#define v_win0_vs_offset_yrgb(x)     (((x)&0xff)<<16)
#define v_win0_hs_offset_cbr(x)     (((x)&0xff)<<8)
#define v_win0_hs_offset_yrgb(x)     ((x)&0xff)


#define m_win0_fading_value         (0xff<<24)
#define m_win0_src_global_alpha     (0xff<<16)
#define m_win0_src_factor_m0        (7<<6)
#define m_win0_src_alpha_cal_m0     (1<<5)
#define m_win0_src_blend_m0         (3<<3)
#define m_win0_src_alpha_m0         (1<<2)
#define m_win0_src_color_m0         (1<<1)
#define m_win0_src_alpha_en         (1)

#define v_win0_fading_value(x)         (((x)&0xff)<<24)
#define v_win0_src_global_alpha(x)     (((x)&0xff)<<16)
#define v_win0_src_factor_m0(x)        (((x)&7)<<6)
#define v_win0_src_alpha_cal_m0(x)     (((x)&1)<<5)
#define v_win0_src_blend_m0(x)         (((x)&3)<<3)
#define v_win0_src_alpha_m0(x)         (((x)&1)<<2)
#define v_win0_src_color_m0(x)         (((x)&1)<<1)
#define v_win0_src_alpha_en(x)         ((x)&1)


#define m_win0_dst_factor_m0          (7<<6)
#define v_win0_dst_factor_m0(x)          (((x)&7)<<6)

#define m_layer0_fading_en             (1<<24)
#define m_layer0_fading_offset_b       (0xff<<16)
#define m_layer0_fading_offset_g       (0xff<<8)
#define m_layer0_fading_offset_r       0xff

#define v_layer0_fading_en(x)             (((x)&1)<<24)
#define v_layer0_fading_offset_b(x)       (((x)&0xff)<<16)
#define v_layer0_fading_offset_g(x)       (((x)&0xff)<<8)
#define v_layer0_fading_offset_r(x)       ((x)&0xff)

#define v_HSYNC(x)  		(((x)&0x1fff)<<0)   //hsync pulse width
#define v_HORPRD(x) 		(((x)&0x1fff)<<16)   //horizontal period
#define v_VSYNC(x) 		(((x)&0x1fff)<<0)
#define v_VERPRD(x) 		(((x)&0x1fff)<<16)

#define v_HAEP(x) 		(((x)&0x1fff)<<0)  //horizontal active end point
#define v_HASP(x) 		(((x)&0x1fff)<<16) //horizontal active start point
#define v_VAEP(x) 		(((x)&0x1fff)<<0)
#define v_VASP(x) 		(((x)&0x1fff)<<16)

#define LVDS_CH0_REG_0			0x00
#define LVDS_CH0_REG_1			0x04
#define LVDS_CH0_REG_2			0x08
#define LVDS_CH0_REG_3			0x0c
#define LVDS_CH0_REG_4			0x10
#define LVDS_CH0_REG_5			0x14
#define LVDS_CH0_REG_9			0x24
#define LVDS_CFG_REG_c			0x30
#define LVDS_CH0_REG_d			0x34
#define LVDS_CH0_REG_f			0x3c
#define LVDS_CH0_REG_20			0x80
#define LVDS_CFG_REG_21			0x84

#define LVDS_SEL_VOP_LIT		(1 << 3)

#define LVDS_FMT_MASK			(0x07 << 16)
#define LVDS_MSB			(0x01 << 3)
#define LVDS_DUAL			(0x01 << 4)
#define LVDS_FMT_1			(0x01 << 5)
#define LVDS_TTL_EN			(0x01 << 6)
#define LVDS_START_PHASE_RST_1		(0x01 << 7)
#define LVDS_DCLK_INV			(0x01 << 8)
#define LVDS_CH0_EN			(0x01 << 11)
#define LVDS_CH1_EN			(0x01 << 12)
#define LVDS_PWRDN			(0x01 << 15)

/*****************************************************************************************************/
#define SCALE_FACTOR_BILI_DN_FIXPOINT_SHIFT   12   /* 4.12*/
#define SCALE_FACTOR_BILI_DN_FIXPOINT(x)      ((INT32)((x)*(1 << SCALE_FACTOR_BILI_DN_FIXPOINT_SHIFT)))

#define SCALE_FACTOR_BILI_UP_FIXPOINT_SHIFT   16   /* 0.16*/

#define SCALE_FACTOR_AVRG_FIXPOINT_SHIFT   16   /*0.16*/
#define SCALE_FACTOR_AVRG_FIXPOINT(x)      ((INT32)((x)*(1 << SCALE_FACTOR_AVRG_FIXPOINT_SHIFT)))

#define SCALE_FACTOR_BIC_FIXPOINT_SHIFT    16   /* 0.16*/
#define SCALE_FACTOR_BIC_FIXPOINT(x)       ((INT32)((x)*(1 << SCALE_FACTOR_BIC_FIXPOINT_SHIFT)))

#define SCALE_FACTOR_DEFAULT_FIXPOINT_SHIFT    12  /*NONE SCALE,vsd_bil*/
#define SCALE_FACTOR_VSDBIL_FIXPOINT_SHIFT     12  /*VER SCALE DOWN BIL*/

/*****************************************************************************************************/

/*#define GET_SCALE_FACTOR_BILI(src, dst) ((((src) - 1) << SCALE_FACTOR_BILI_FIXPOINT_SHIFT) / ((dst) - 1))*/
/*#define GET_SCALE_FACTOR_BIC(src, dst)  ((((src) - 1) << SCALE_FACTOR_BIC_FIXPOINT_SHIFT) / ((dst) - 1))*/
/*modified by hpz*/
#define GET_SCALE_FACTOR_BILI_DN(src, dst)  ((((src)*2 - 3) << (SCALE_FACTOR_BILI_DN_FIXPOINT_SHIFT-1)) / ((dst) - 1))
#define GET_SCALE_FACTOR_BILI_UP(src, dst)  ((((src)*2 - 3) << (SCALE_FACTOR_BILI_UP_FIXPOINT_SHIFT-1)) / ((dst) - 1))
#define GET_SCALE_FACTOR_BIC(src, dst)      ((((src)*2 - 3) << (SCALE_FACTOR_BIC_FIXPOINT_SHIFT-1)) / ((dst) - 1))
/*****************************************************************/
/*NOTE: hardware为节省开销, srcH先抽行得到 (srcH+vScaleDnMult-1)/vScaleDnMult; 然后缩放*/
#define GET_SCALE_DN_ACT_HEIGHT(srcH, vScaleDnMult) (((srcH)+(vScaleDnMult)-1)/(vScaleDnMult))
/*#define VSKIP_MORE_PRECISE*/

#ifdef VSKIP_MORE_PRECISE
#define MIN_SCALE_FACTOR_AFTER_VSKIP        1.5f
#define GET_SCALE_FACTOR_BILI_DN_VSKIP(srcH, dstH, vScaleDnMult) \
            (GET_SCALE_FACTOR_BILI_DN(GET_SCALE_DN_ACT_HEIGHT((srcH), (vScaleDnMult)), (dstH)))
#else
#define MIN_SCALE_FACTOR_AFTER_VSKIP        1
#define GET_SCALE_FACTOR_BILI_DN_VSKIP(srcH, dstH, vScaleDnMult) \
            ((GET_SCALE_DN_ACT_HEIGHT((srcH), (vScaleDnMult)) == (dstH))\
                            ? (GET_SCALE_FACTOR_BILI_DN((srcH), (dstH))/(vScaleDnMult))\
                            : GET_SCALE_FACTOR_BILI_DN(GET_SCALE_DN_ACT_HEIGHT((srcH), (vScaleDnMult)), (dstH)))
#endif
/*****************************************************************/


/*ScaleFactor must >= dst/src, or pixels at end of line may be unused*/
/*ScaleFactor must < dst/(src-1), or dst buffer may overflow*/
/*avrg old code:       ((((dst) << SCALE_FACTOR_AVRG_FIXPOINT_SHIFT))/((src) - 1)) hxx_chgsrc*/
/*modified by hpz:*/
#define GET_SCALE_FACTOR_AVRG(src, dst)  ((((dst) << (SCALE_FACTOR_AVRG_FIXPOINT_SHIFT+1)))/(2*(src) - 1))

enum {
	LB_YUV_3840X5 = 0x0,
	LB_YUV_2560X8 = 0x1,
	LB_RGB_3840X2 = 0x2,
	LB_RGB_2560X4 = 0x3,
	LB_RGB_1920X5 = 0x4,
	LB_RGB_1280X8 = 0x5 
};


LCDC_REG *preg = NULL;  
LCDC_REG regbak;

extern int rk32_edp_enable(vidinfo_t * vid);
extern int rk32_mipi_enable(vidinfo_t * vid);
extern int rk32_dsi_enable(void);
extern int rk32_dsi_sync(void);

static void inline writel_relaxed(uint32 val, uint32 addr)
{
	*(int*)addr = val;
}

#define CPU_AXI_QOS_PRIORITY    0x08
#define CPU_AXI_QOS_PRIORITY_LEVEL(h, l)        ((((h) & 3) << 2) | ((l) & 3))
#define CPU_AXI_SET_QOS_PRIORITY(h, l, base) \
	writel_relaxed(CPU_AXI_QOS_PRIORITY_LEVEL(h, l), base + CPU_AXI_QOS_PRIORITY)

static int inline lvds_writel(uint32 offset, uint32 val)
{
	writel_relaxed(val, lvds_regs + offset);
	writel_relaxed(val, lvds_regs + offset + 0x100);

	return 0;
}

static int rk32_lvds_disable(void)
{
	grf_writel(0x80008000, GRF_SOC_CON7);

	writel_relaxed(0x00, lvds_regs + LVDS_CFG_REG_21); /*disable tx*/
	writel_relaxed(0xff, lvds_regs + LVDS_CFG_REG_c); /*disable pll*/
	return 0;
}

// lvds_type : 0 ttl, use lvds transmit ic;  1 lvds in rk3288
static int rk32_lvds_en(vidinfo_t *vid)
{
	u32 h_bp = vid->vl_hspw + vid->vl_hbpd;
	u32 i,j, val ;

	int screen_type = SCREEN_RGB;
    
	if (vid->lcdc_id == 1) /*lcdc1 = vop little,lcdc0 = vop big*/
		val = LVDS_SEL_VOP_LIT | (LVDS_SEL_VOP_LIT << 16);
	else
		val = LVDS_SEL_VOP_LIT << 16;  // video source from vop0 = vop big

	grf_writel(val, GRF_SOC_CON6);

	val = vid->lvds_format;
	if (screen_type == SCREEN_DUAL_LVDS)
		val |= LVDS_DUAL | LVDS_CH0_EN | LVDS_CH1_EN;
	else if(screen_type == SCREEN_LVDS)
		val |= LVDS_CH0_EN;
		//val |= LVDS_MSB;
	else if (screen_type == SCREEN_RGB)
		val |= LVDS_TTL_EN | LVDS_CH0_EN | LVDS_CH1_EN;

	if (h_bp & 0x01)
		val |= LVDS_START_PHASE_RST_1;

	val |= (vid->vl_clkp << 8) | (vid->vl_hsp << 9) |
		(vid->vl_oep << 10);
	val |= 0xffff << 16;

	grf_writel(val, GRF_SOC_CON7);
	grf_writel(0x0f000f00, GRF_GPIO1H_SR);
	grf_writel(0x00ff00ff, GRF_GPIO1D_E);
	if (screen_type == SCREEN_LVDS)
		val = 0xbf;
	else
		val = 0x7f;

	if(vid->lvds_ttl_en) {//  1 lvds
		grf_writel(0x00550055, GRF_GPIO1D_IOMUX); //lcdc iomux
	
	    	lvds_writel( LVDS_CH0_REG_0, 0x7f);
	    	lvds_writel( LVDS_CH0_REG_1, 0x40);
	    	lvds_writel( LVDS_CH0_REG_2, 0x00);
	
	    	if (screen_type == SCREEN_RGB)
	    		val = 0x1f;
	    	else
	    		val = 0x00;
	    	lvds_writel( LVDS_CH0_REG_4, 0x3f);
	    	lvds_writel( LVDS_CH0_REG_5, 0x3f);
	    	lvds_writel( LVDS_CH0_REG_3, 0x46);
	    	lvds_writel( LVDS_CH0_REG_d, 0x0a);
	    	lvds_writel( LVDS_CH0_REG_20,0x44);/* 44:LSB  45:MSB*/
	    	writel_relaxed(0x00, lvds_regs + LVDS_CFG_REG_c); /*eanble pll*/
	    	writel_relaxed(0x92, lvds_regs + LVDS_CFG_REG_21); /*enable tx*/
	
	    	lvds_writel( 0x100, 0x7f);
	    	lvds_writel( 0x104, 0x40);
	    	lvds_writel( 0x108, 0x00);
	    	lvds_writel( 0x10c, 0x46);
	    	lvds_writel( 0x110, 0x3f);
	    	lvds_writel( 0x114, 0x3f);
	    	lvds_writel( 0x134, 0x0a);
	} else {
	    	val  = *(int*)(lvds_regs + 0x88);
	    	printf("0x88:0x%x\n",val);
	
	    	lvds_writel( LVDS_CH0_REG_0, 0xbf);
	    	lvds_writel( LVDS_CH0_REG_1, 0x3f);//  3f
	    	lvds_writel( LVDS_CH0_REG_2, 0xfe);
	    	lvds_writel( LVDS_CH0_REG_3, 0x46);//0x46
	    	lvds_writel( LVDS_CH0_REG_4, 0x00);
	    	//lvds_writel( LVDS_CH0_REG_9, 0x20);
	    	//lvds_writel( LVDS_CH0_REG_d, 0x4b);
	    	//lvds_writel( LVDS_CH0_REG_f, 0x0d);
	    	lvds_writel( LVDS_CH0_REG_d, 0x0a);//0a
	    	lvds_writel( LVDS_CH0_REG_20,0x44);/* 44:LSB  45:MSB*/
	    	//lvds_writel( 0x24,0x20);
	    	//writel_relaxed(0x23, lvds_regs + 0x88);
	    	writel_relaxed(0x00, lvds_regs + LVDS_CFG_REG_c); /*eanble pll*/
	    	writel_relaxed(0x92, lvds_regs + LVDS_CFG_REG_21); /*enable tx*/
	
	    	//lvds_writel( 0x100, 0xbf);
	    	//lvds_writel( 0x104, 0x3f);
	    	//lvds_writel( 0x108, 0xfe);
	    	//lvds_writel( 0x10c, 0x46); //0x46
	    	//lvds_writel( 0x110, 0x00);
	    	//lvds_writel( 0x114, 0x00);
	    	//lvds_writel( 0x134, 0x0a);
	}

	return 0;
}

enum
{
	SCALE_NONE = 0x0,
	SCALE_UP   = 0x1,
	SCALE_DOWN = 0x2
};

enum sacle_up_mode {
    SCALE_UP_BIL = 0x0,
    SCALE_UP_BIC = 0x1
};

enum scale_down_mode {
    SCALE_DOWN_BIL = 0x0,
    SCALE_DOWN_AVG = 0x1
};

struct rk3288_scale_info {
	u8 win_lb_mode;
	u32 scale_yrgb_x;
	u32 scale_yrgb_y;
	u32 scale_cbcr_x;
	u32 scale_cbcr_y;
	u8 bic_coe_el;
	u8 yrgb_hor_scl_mode;//h 01:scale up ;10:down
	u8 yrgb_ver_scl_mode;//v 01:scale up ;10:down
	u8 yrgb_hsd_mode;//h scale down mode
	u8 yrgb_vsu_mode;//v scale up mode
	u8 yrgb_vsd_mode;//v scale down mode
	u8 cbr_hor_scl_mode;
	u8 cbr_ver_scl_mode;
	u8 cbr_hsd_mode;
	u8 cbr_vsu_mode;
	u8 cbr_vsd_mode;
	u8 vsd_yrgb_gt4;
	u8 vsd_yrgb_gt2;
	u8 vsd_cbr_gt4;
	u8 vsd_cbr_gt2;
};

u32 getHardWareVSkipLines(u32 srcH, u32 dstH)
{
    u32 vScaleDnMult;

    if(srcH >= (u32)(4*dstH*MIN_SCALE_FACTOR_AFTER_VSKIP))
    {
        vScaleDnMult = 4;
    }
    else if(srcH >= (u32)(2*dstH*MIN_SCALE_FACTOR_AFTER_VSKIP))
    {
        vScaleDnMult = 2;
    }
    else
    {
        vScaleDnMult = 1;
    }

    return vScaleDnMult;
}

static int rk3288_lcdc_cal_scl_fac(struct fb_dsp_info *disp_info,
		struct rk3288_scale_info *win)
{
	u16 srcW;
	u16 srcH;
	u16 dstW;
	u16 dstH;
	u16 yrgb_srcW;
	u16 yrgb_srcH;
	u16 yrgb_dstW;
	u16 yrgb_dstH;
	u32 yrgb_vScaleDnMult;
	u32 yrgb_xscl_factor;
	u32 yrgb_yscl_factor;
	u8  yrgb_vsd_bil_gt2=0;
	u8  yrgb_vsd_bil_gt4=0;
	
	u16 cbcr_srcW;
	u16 cbcr_srcH;
	u16 cbcr_dstW;
	u16 cbcr_dstH;	  
	u32 cbcr_vScaleDnMult;
	u32 cbcr_xscl_factor;
	u32 cbcr_yscl_factor;
	u8  cbcr_vsd_bil_gt2=0;
	u8  cbcr_vsd_bil_gt4=0;
	u8  yuv_fmt=0;


	srcW = disp_info->xact;
	srcH = disp_info->yact;
	dstW = disp_info->xsize;
	dstH = disp_info->ysize;

	/*yrgb scl mode*/
	yrgb_srcW = srcW;
	yrgb_srcH = srcH;
	yrgb_dstW = dstW;
	yrgb_dstH = dstH;
	if ((yrgb_dstW >= yrgb_srcW*8) || (yrgb_dstH >= yrgb_srcH*8) ||
		(yrgb_dstW*8 <= yrgb_srcW) || (yrgb_dstH*8 <= yrgb_srcH)) {
		printf("ERROR: yrgb scale exceed 8,"
		       "srcW=%d,srcH=%d,dstW=%d,dstH=%d\n",
		       yrgb_srcW,yrgb_srcH,yrgb_dstW,yrgb_dstH);
		return -1;
	}
	if (yrgb_srcW < yrgb_dstW)
		win->yrgb_hor_scl_mode = SCALE_UP;
    	else if (yrgb_srcW > yrgb_dstW)
        	win->yrgb_hor_scl_mode = SCALE_DOWN;
    	else
        	win->yrgb_hor_scl_mode = SCALE_NONE;

    	if (yrgb_srcH < yrgb_dstH)
        	win->yrgb_ver_scl_mode = SCALE_UP;
    	else if (yrgb_srcH  > yrgb_dstH)
        	win->yrgb_ver_scl_mode = SCALE_DOWN;
    	else
        	win->yrgb_ver_scl_mode = SCALE_NONE;

	/*cbcr scl mode*/
	switch (disp_info->format) {
	case YUV422:
//	case YUV422_A:	
		cbcr_srcW = srcW/2;
		cbcr_dstW = dstW;
		cbcr_srcH = srcH;
		cbcr_dstH = dstH;
		yuv_fmt = 1;
		break;
	case YUV420:
//	case YUV420_A:	
		cbcr_srcW = srcW/2;
		cbcr_dstW = dstW;
		cbcr_srcH = srcH/2;
		cbcr_dstH = dstH;
		yuv_fmt = 1;
		break;
	case YUV444:
//	case YUV444_A:	
		cbcr_srcW = srcW;
		cbcr_dstW = dstW;
		cbcr_srcH = srcH;
		cbcr_dstH = dstH;
		yuv_fmt = 1;
		break;
	default:
		cbcr_srcW = 0;
	        cbcr_dstW = 0;
	        cbcr_srcH = 0;
	        cbcr_dstH = 0;
		yuv_fmt = 0;
		break;
	}		
	if (yuv_fmt) {
		if ((cbcr_dstW >= cbcr_srcW*8) || (cbcr_dstH >= cbcr_srcH*8) ||
			(cbcr_dstW*8 <= cbcr_srcW)||(cbcr_dstH*8 <= cbcr_srcH)) {
			printf("ERROR: cbcr scale exceed 8,"
		       "srcW=%d,srcH=%d,dstW=%d,dstH=%d\n",
		       cbcr_srcW,cbcr_srcH,cbcr_dstW,cbcr_dstH);
		}
	}
	
	if(cbcr_srcW < cbcr_dstW){
		win->cbr_hor_scl_mode = SCALE_UP;
	}else if(cbcr_srcW > cbcr_dstW){
		win->cbr_hor_scl_mode = SCALE_DOWN;
	}else{
		win->cbr_hor_scl_mode = SCALE_NONE;
	}
	
	if(cbcr_srcH < cbcr_dstH){
		win->cbr_ver_scl_mode = SCALE_UP;
	}else if(cbcr_srcH > cbcr_dstH){
		win->cbr_ver_scl_mode = SCALE_DOWN;
	}else{
		win->cbr_ver_scl_mode = SCALE_NONE;
	}

    /*line buffer mode*/
    	if((disp_info->format == YUV422) ||
    	   (disp_info->format == YUV420) /*||
    	   (disp_info->format == YUV422_A) ||
    	   (disp_info->format == YUV420_A)*/) {
        	if(win->cbr_hor_scl_mode == SCALE_DOWN){
            		if ((cbcr_dstW > 3840) || (cbcr_dstW == 0)) {
                		printf("ERROR cbcr_dstW = %d\n",cbcr_dstW);                
            		}else if(cbcr_dstW > 2560){
                		win->win_lb_mode = LB_RGB_3840X2;
            		}else if(cbcr_dstW > 1920){
                		if(win->yrgb_hor_scl_mode == SCALE_DOWN){
                    			if(yrgb_dstW > 3840){
                        			printf("ERROR yrgb_dst_width exceeds 3840\n");
		                    	}else if(yrgb_dstW > 2560){
		                        	win->win_lb_mode = LB_RGB_3840X2;
		                    	}else if(yrgb_dstW > 1920){
		                        	win->win_lb_mode = LB_RGB_2560X4;
		                    	}else{
		                        	printf("ERROR never run here!yrgb_dstW<1920 ==> cbcr_dstW>1920\n");
		                    	}
		        	}
            		}else if(cbcr_dstW > 1280){
                		win->win_lb_mode = LB_YUV_3840X5;
            		}else{
                		win->win_lb_mode = LB_YUV_2560X8;
            		}            
        	} else { /*SCALE_UP or SCALE_NONE*/
            		if ((cbcr_srcW > 3840) || (cbcr_srcW == 0)) {
                		printf("ERROR cbcr_srcW = %d\n",cbcr_srcW);
            		}else if(cbcr_srcW > 2560){                
                		win->win_lb_mode = LB_RGB_3840X2;
            		}else if(cbcr_srcW > 1920){
                		if(win->yrgb_hor_scl_mode == SCALE_DOWN){
                    			if(yrgb_dstW > 3840){
                        			printf("ERROR yrgb_dst_width exceeds 3840\n");
                    			}else if(yrgb_dstW > 2560){
                        			win->win_lb_mode = LB_RGB_3840X2;
                    			}else if(yrgb_dstW > 1920){
                        			win->win_lb_mode = LB_RGB_2560X4;
                    			}else{
                        			printf("ERROR never run here!yrgb_dstW<1920 ==> cbcr_dstW>1920\n");
                    			}
                		}  
            		}else if(cbcr_srcW > 1280){
               			 win->win_lb_mode = LB_YUV_3840X5;
            		}else{
                		win->win_lb_mode = LB_YUV_2560X8;
            		}            
        	}
    	}else {
        	if(win->yrgb_hor_scl_mode == SCALE_DOWN){
            		if ((yrgb_dstW > 3840) || (yrgb_dstW == 0)) {
                		printf("ERROR yrgb_dstW = %d\n",yrgb_dstW);
            		}else if(yrgb_dstW > 2560){
                		win->win_lb_mode = LB_RGB_3840X2;
            		}else if(yrgb_dstW > 1920){
                		win->win_lb_mode = LB_RGB_2560X4;
            		}else if(yrgb_dstW > 1280){
                		win->win_lb_mode = LB_RGB_1920X5;
            		}else{
                		win->win_lb_mode = LB_RGB_1280X8;
            		}            
        	}else{ /*SCALE_UP or SCALE_NONE*/
            		if ((yrgb_srcW > 3840) || (yrgb_srcW == 0)) {
                		printf("ERROR yrgb_srcW = %d\n",yrgb_srcW);
            		}else if(yrgb_srcW > 2560){
                		win->win_lb_mode = LB_RGB_3840X2;
            		}else if(yrgb_srcW > 1920){
                		win->win_lb_mode = LB_RGB_2560X4;
            		}else if(yrgb_srcW > 1280){
                		win->win_lb_mode = LB_RGB_1920X5;
            		}else{
                		win->win_lb_mode = LB_RGB_1280X8;
            		}            
        	}
    	}

	/*vsd/vsu scale ALGORITHM*/
	win->yrgb_hsd_mode = SCALE_DOWN_BIL;/*not to specify*/
	win->cbr_hsd_mode  = SCALE_DOWN_BIL;/*not to specify*/
	win->yrgb_vsd_mode = SCALE_DOWN_BIL;/*not to specify*/
	win->cbr_vsd_mode  = SCALE_DOWN_BIL;/*not to specify*/
	switch(win->win_lb_mode){
	    case LB_YUV_3840X5:
	    case LB_YUV_2560X8:
	    case LB_RGB_1920X5:
	    case LB_RGB_1280X8: 	
		win->yrgb_vsu_mode = SCALE_UP_BIC; 
		win->cbr_vsu_mode  = SCALE_UP_BIC; 
		break;
	    case LB_RGB_3840X2:
		if(win->yrgb_ver_scl_mode != SCALE_NONE) {
		    printf("ERROR : not allow yrgb ver scale\n");
		}
		if(win->cbr_ver_scl_mode != SCALE_NONE) {
		    printf("ERROR : not allow cbcr ver scale\n");
		}	     	  
		break;
	    case LB_RGB_2560X4:
		win->yrgb_vsu_mode = SCALE_UP_BIL; 
		win->cbr_vsu_mode  = SCALE_UP_BIL; 	    
		break;
	    default:
	    	printf("%s:un supported win_lb_mode:%d\n",
			__func__,win->win_lb_mode);	
		break;
	}
	
    	/*SCALE FACTOR*/
    
    	/*(1.1)YRGB HOR SCALE FACTOR*/
    	switch(win->yrgb_hor_scl_mode){
        case SCALE_NONE:
        	yrgb_xscl_factor = (1<<SCALE_FACTOR_DEFAULT_FIXPOINT_SHIFT);
            	break;
        case SCALE_UP  :
            	yrgb_xscl_factor = GET_SCALE_FACTOR_BIC(yrgb_srcW, yrgb_dstW);
            	break;
        case SCALE_DOWN:
            	switch(win->yrgb_hsd_mode)
            	{
                case SCALE_DOWN_BIL:
                	yrgb_xscl_factor = GET_SCALE_FACTOR_BILI_DN(yrgb_srcW, yrgb_dstW);
                    	break;
                case SCALE_DOWN_AVG:
                    	yrgb_xscl_factor = GET_SCALE_FACTOR_AVRG(yrgb_srcW, yrgb_dstW);
                    	break;
                default :
			printf("%s:un supported yrgb_hsd_mode:%d\n",
				__func__,win->yrgb_hsd_mode);		
                    	break;
            	} 
            	break;
        default :
		printf("%s:un supported yrgb_hor_scl_mode:%d\n",
				__func__,win->yrgb_hor_scl_mode);	
            break;
    	} /*win->yrgb_hor_scl_mode*/

    	/*(1.2)YRGB VER SCALE FACTOR*/
    	switch(win->yrgb_ver_scl_mode)
    	{
        case SCALE_NONE:
            	yrgb_yscl_factor = (1<<SCALE_FACTOR_DEFAULT_FIXPOINT_SHIFT);
           	 break;
        case SCALE_UP  :
            	switch(win->yrgb_vsu_mode)
            	{
                case SCALE_UP_BIL:
                    	yrgb_yscl_factor = GET_SCALE_FACTOR_BILI_UP(yrgb_srcH, yrgb_dstH);
                    	break;
                case SCALE_UP_BIC:
                    	if(yrgb_srcH < 3){
                        	printf("yrgb_srcH should be greater than 3 !!!\n");
                    	}                    
                    	yrgb_yscl_factor = GET_SCALE_FACTOR_BIC(yrgb_srcH, yrgb_dstH);
                   	break;
                default :
			printf("%s:un supported yrgb_vsu_mode:%d\n",
				__func__,win->yrgb_vsu_mode);			
                    	break;
            }
            break;
        case SCALE_DOWN:
            	switch(win->yrgb_vsd_mode)
            	{
                case SCALE_DOWN_BIL:
                    	yrgb_vScaleDnMult = getHardWareVSkipLines(yrgb_srcH, yrgb_dstH);
                    	yrgb_yscl_factor  = GET_SCALE_FACTOR_BILI_DN_VSKIP(yrgb_srcH, yrgb_dstH, yrgb_vScaleDnMult);                                 
                    	if(yrgb_vScaleDnMult == 4){
                        	yrgb_vsd_bil_gt4 = 1;
                        	yrgb_vsd_bil_gt2 = 0;
                    	}else if(yrgb_vScaleDnMult == 2){
                        	yrgb_vsd_bil_gt4 = 0;
                        	yrgb_vsd_bil_gt2 = 1;
                    	}else{
                        	yrgb_vsd_bil_gt4 = 0;
                        	yrgb_vsd_bil_gt2 = 0;
                    	}
                    	break;
                case SCALE_DOWN_AVG:
                    	yrgb_yscl_factor = GET_SCALE_FACTOR_AVRG(yrgb_srcH, yrgb_dstH);
                    	break;
                default:
			pr_warn("%s:un supported yrgb_vsd_mode:%d\n",
				__func__,win->yrgb_vsd_mode);		
                    	break;
            	} /*win->yrgb_vsd_mode*/
            	break;
	default :
		pr_warn("%s:un supported yrgb_ver_scl_mode:%d\n",
			__func__,win->yrgb_ver_scl_mode);		
            	break;
    	}
    	win->scale_yrgb_x = yrgb_xscl_factor;
    	win->scale_yrgb_y = yrgb_yscl_factor;
    	win->vsd_yrgb_gt4 = yrgb_vsd_bil_gt4;
    	win->vsd_yrgb_gt2 = yrgb_vsd_bil_gt2;

    	/*(2.1)CBCR HOR SCALE FACTOR*/
    	switch(win->cbr_hor_scl_mode) {
        case SCALE_NONE:
            	cbcr_xscl_factor = (1<<SCALE_FACTOR_DEFAULT_FIXPOINT_SHIFT);
            	break;
        case SCALE_UP  :
            	cbcr_xscl_factor = GET_SCALE_FACTOR_BIC(cbcr_srcW, cbcr_dstW);
            	break;
        case SCALE_DOWN:
            	switch(win->cbr_hsd_mode)
            	{
                case SCALE_DOWN_BIL:
                    	cbcr_xscl_factor = GET_SCALE_FACTOR_BILI_DN(cbcr_srcW, cbcr_dstW);
                    	break;
                case SCALE_DOWN_AVG:
                    	cbcr_xscl_factor = GET_SCALE_FACTOR_AVRG(cbcr_srcW, cbcr_dstW);
                    	break;
                default :
			pr_warn("%s:un supported cbr_hsd_mode:%d\n",
				__func__,win->cbr_hsd_mode);	
                    	break;
            	}
            	break;
        default :
		pr_warn("%s:un supported cbr_hor_scl_mode:%d\n",
			__func__,win->cbr_hor_scl_mode);	
            	break;
    	} /*win->cbr_hor_scl_mode*/

    	/*(2.2)CBCR VER SCALE FACTOR*/
    	switch(win->cbr_ver_scl_mode)
    	{
        case SCALE_NONE:
            	cbcr_yscl_factor = (1<<SCALE_FACTOR_DEFAULT_FIXPOINT_SHIFT);
            	break;
        case SCALE_UP  :
            	switch(win->cbr_vsu_mode)
            	{
                case SCALE_UP_BIL:
                    	cbcr_yscl_factor = GET_SCALE_FACTOR_BILI_UP(cbcr_srcH, cbcr_dstH);
                    	break;
                case SCALE_UP_BIC:
                    	if(cbcr_srcH < 3) {
                        	printf("cbcr_srcH should be greater than 3 !!!\n");
                    	}                    
                    	cbcr_yscl_factor = GET_SCALE_FACTOR_BIC(cbcr_srcH, cbcr_dstH);
                    	break;
                default :
			pr_warn("%s:un supported cbr_vsu_mode:%d\n",
				__func__,win->cbr_vsu_mode);		
                    	break;
            	}
            	break;
        case SCALE_DOWN:
            	switch(win->cbr_vsd_mode)
            	{
                case SCALE_DOWN_BIL:
                    	cbcr_vScaleDnMult = getHardWareVSkipLines(cbcr_srcH, cbcr_dstH);
                    	cbcr_yscl_factor  = GET_SCALE_FACTOR_BILI_DN_VSKIP(cbcr_srcH, cbcr_dstH, cbcr_vScaleDnMult);                    
                    	if(cbcr_vScaleDnMult == 4){
                        	cbcr_vsd_bil_gt4 = 1;
                        	cbcr_vsd_bil_gt2 = 0;
                    	}else if(cbcr_vScaleDnMult == 2){
                        	cbcr_vsd_bil_gt4 = 0;
                        	cbcr_vsd_bil_gt2 = 1;
                    	}else{
                        	cbcr_vsd_bil_gt4 = 0;
                        	cbcr_vsd_bil_gt2 = 0;
                    	}
                    	break;
                case SCALE_DOWN_AVG:
                    	cbcr_yscl_factor = GET_SCALE_FACTOR_AVRG(cbcr_srcH, cbcr_dstH);
                    	break;
                default :
			pr_warn("%s:un supported cbr_vsd_mode:%d\n",
				__func__,win->cbr_vsd_mode);		
                    break;
            	}
            	break;
        default :
		pr_warn("%s:un supported cbr_ver_scl_mode:%d\n",
			__func__,win->cbr_ver_scl_mode);			
            	break;
    	}
    	win->scale_cbcr_x = cbcr_xscl_factor;
    	win->scale_cbcr_y = cbcr_yscl_factor;
   	win->vsd_cbr_gt4  = cbcr_vsd_bil_gt4;
    	win->vsd_cbr_gt2  = cbcr_vsd_bil_gt2;	
	printf("yrgb:h_fac=%d,v_fac=%d,gt4=%d,gt2=%d\n",yrgb_xscl_factor,
		yrgb_yscl_factor,yrgb_vsd_bil_gt4,yrgb_vsd_bil_gt2);
	return 0;
}

/* Configure VENC for a given Mode (NTSC / PAL) */
void rk_lcdc_set_par(struct fb_dsp_info *fb_info, vidinfo_t *vid)
{
	struct layer_par *par = &vid->par[fb_info->layer_id];
	struct rk3288_scale_info scale_info;
	int mask, val;

	if(par == NULL)
		printf("%s lay_par==NULL,id=%d\n",fb_info->layer_id);
	
	if(fb_info != &par->fb_info)
		memcpy(&par->fb_info,fb_info,sizeof(struct fb_dsp_info *));

	fb_info->xpos = 0;
	fb_info->ypos = 0;
	fb_info->xsize = vid->vl_col;
	fb_info->ysize = vid->vl_row;
	rk3288_lcdc_cal_scl_fac(fb_info, &scale_info);
	
	switch(fb_info->layer_id) {
		case WIN0:
			mask =	m_win0_bic_coe_sel |
			m_win0_vsd_yrgb_gt4 | m_win0_vsd_yrgb_gt2 |
			m_win0_vsd_cbr_gt4 | m_win0_vsd_cbr_gt2 |
			m_win0_yrgb_hor_scl_mode | m_win0_yrgb_ver_scl_mode |
			m_win0_yrgb_hsd_mode | m_win0_yrgb_vsu_mode |
			m_win0_yrgb_vsd_mode | m_win0_cbr_hor_scl_mode |
			m_win0_cbr_ver_scl_mode | m_win0_cbr_hsd_mode |
			m_win0_cbr_vsu_mode | m_win0_cbr_vsd_mode;
			val =	v_win0_bic_coe_sel(scale_info.bic_coe_el) |
			v_win0_vsd_yrgb_gt4(scale_info.vsd_yrgb_gt4) |
			v_win0_vsd_yrgb_gt2(scale_info.vsd_yrgb_gt2) |
			v_win0_vsd_cbr_gt4(scale_info.vsd_cbr_gt4) |
			v_win0_vsd_cbr_gt2(scale_info.vsd_cbr_gt2) |
			v_win0_yrgb_hor_scl_mode(scale_info.yrgb_hor_scl_mode) |
			v_win0_yrgb_ver_scl_mode(scale_info.yrgb_ver_scl_mode) |
			v_win0_yrgb_hsd_mode(scale_info.yrgb_hsd_mode) |
			v_win0_yrgb_vsu_mode(scale_info.yrgb_vsu_mode) |
			v_win0_yrgb_vsd_mode(scale_info.yrgb_vsd_mode) |
			v_win0_cbr_hor_scl_mode(scale_info.cbr_hor_scl_mode) |
			v_win0_cbr_ver_scl_mode(scale_info.cbr_ver_scl_mode) |
			v_win0_cbr_hsd_mode(scale_info.cbr_hsd_mode) |
			v_win0_cbr_vsu_mode(scale_info.cbr_vsu_mode) |
			v_win0_cbr_vsd_mode(scale_info.cbr_vsd_mode);
			LcdMskReg(WIN0_CTRL1, mask, val);
			printf("scale_yrgb_x %d scale_yrgb_y %d", scale_info.scale_yrgb_x, scale_info.scale_yrgb_y);
			LcdWrReg(WIN0_SCL_FACTOR_YRGB, v_X_SCL_FACTOR(scale_info.scale_yrgb_x) | v_Y_SCL_FACTOR(scale_info.scale_yrgb_y));
			LcdWrReg(WIN0_SCL_FACTOR_CBR,v_X_SCL_FACTOR(scale_info.scale_cbcr_x)| v_Y_SCL_FACTOR(scale_info.scale_cbcr_y));
//			LcdWrReg(WIN0_SCL_FACTOR_YRGB, v_X_SCL_FACTOR(0x1000) | v_Y_SCL_FACTOR(0x1000));
//			LcdWrReg(WIN0_SCL_FACTOR_CBR,v_X_SCL_FACTOR(0x1000)| v_Y_SCL_FACTOR(0x1000));
			LcdMskReg(WIN0_CTRL0, m_win0_rb_swap | m_win0_alpha_swap | m_win0_lb_mode | m_win0_data_fmt | m_win0_en,
				v_win0_rb_swap(0) | v_win0_alpha_swap(0) | v_win0_lb_mode(5) | v_win0_data_fmt(vid->logo_rgb_mode) | v_win0_en(1));
//			LcdMskReg(WIN0_CTRL1, m_win0_cbr_vsu_mode | m_win0_yrgb_vsu_mode, v_win0_cbr_vsu_mode(1)| v_win0_yrgb_vsu_mode(1));
			LcdWrReg(WIN0_ACT_INFO, v_act_width(fb_info->xact-1) | v_act_height(fb_info->yact-1));
			LcdWrReg(WIN0_DSP_ST,
				v_dsp_xst(fb_info->xpos + vid->vl_hspw + vid->vl_hbpd) | v_dsp_yst(fb_info->ypos + vid->vl_vspw + vid->vl_vbpd)); 
			LcdWrReg(WIN0_DSP_INFO, v_dsp_width(fb_info->xsize-1) | v_dsp_height(fb_info->ysize-1));
			LcdMskReg(WIN0_COLOR_KEY, m_win0_key_en | m_win0_key_color,
				v_win0_key_en(0) | v_win0_key_color(0));
			LcdMskReg(WIN0_CTRL0, m_win0_lb_mode, v_win0_lb_mode(scale_info.win_lb_mode));
//			if(fb_info->xsize > 2560) {                
//				LcdMskReg(WIN0_CTRL0, m_win0_lb_mode, v_win0_lb_mode(LB_RGB_3840X2));
//			} else if(fb_info->xsize > 1920) {
//				LcdMskReg(WIN0_CTRL0, m_win0_lb_mode, v_win0_lb_mode(LB_RGB_2560X4));
//			} else if(fb_info->xsize > 1280){
//				LcdMskReg(WIN0_CTRL0, m_win0_lb_mode, v_win0_lb_mode(LB_RGB_1920X5));
//			} else {
//				LcdMskReg(WIN0_CTRL0, m_win0_lb_mode, v_win0_lb_mode(LB_RGB_1280X8));
//			}
			switch(vid->logo_rgb_mode) 
			{
				case ARGB888:
					LcdWrReg(WIN0_VIR,v_ARGB888_VIRWIDTH(fb_info->xvir));  //zyw
					break;
				case RGB888:  //rgb888
					LcdWrReg(WIN0_VIR,v_RGB888_VIRWIDTH(fb_info->xvir));
					break;
				case RGB565:  //rgb565
					LcdWrReg(WIN0_VIR,v_RGB565_VIRWIDTH(fb_info->xvir));
					break;
				case YUV422:
				case YUV420:   
					LcdWrReg(WIN0_VIR,v_YUV_VIRWIDTH(fb_info->xvir));
					if(fb_info->xsize > 1280) {
						LcdMskReg(WIN0_CTRL0, m_win0_lb_mode, v_win0_lb_mode(LB_YUV_3840X5));
					} else {
						LcdMskReg(WIN0_CTRL0, m_win0_lb_mode, v_win0_lb_mode(LB_YUV_2560X8));
					}          
					break;
				default:
					LcdWrReg(WIN0_VIR,v_RGB888_VIRWIDTH(fb_info->xvir));
					break;
			}
			LcdWrReg(WIN0_YRGB_MST, fb_info->yaddr);
			break;
		case WIN1:
			printf("%s --->WIN1 not support \n");
			break;
		default:
			printf("%s --->unknow lay_id \n");
			break;
	}

	LcdWrReg(BCSH_BCS, 0xd0010000);
	LcdWrReg(BCSH_H, 0x01000000);
	LcdWrReg(BCSH_COLOR_BAR, 1);
	LCDC_REG_CFG_DONE();
}


int rk_lcdc_load_screen(vidinfo_t *vid)
{
	int ret = -1;
	int face = 0;
	
	vid->dp_enabled = 0;
	vid->mipi_enabled = 0;
	if ((vid->screen_type == SCREEN_MIPI)||(vid->screen_type == SCREEN_DUAL_MIPI))
		vid->mipi_enabled = 1;
	else if(vid->screen_type == SCREEN_EDP)
		vid->dp_enabled = 1;
	
	if (vid->mipi_enabled) {
		rk32_mipi_enable(vid);
		if (vid->screen_type == SCREEN_MIPI) {
			LcdMskReg(SYS_CTRL, m_mipi_out_en|m_edp_out_en|m_hdmi_out_en|m_rgb_out_en, v_mipi_out_en(1));
		} else {
			LcdMskReg(SYS_CTRL, m_mipi_out_en|m_doub_channel_en|m_edp_out_en|m_hdmi_out_en|m_rgb_out_en,
				v_mipi_out_en(1)|v_doub_channel_en(1));
		}
	} else if (vid->dp_enabled) {
		LcdMskReg(SYS_CTRL,m_mipi_out_en|m_edp_out_en|m_hdmi_out_en|m_rgb_out_en,v_edp_out_en(1));
	} else if(vid->screen_type == SCREEN_HDMI){
		LcdMskReg(SYS_CTRL,m_mipi_out_en|m_edp_out_en|m_hdmi_out_en|m_rgb_out_en,v_hdmi_out_en(1));
	} else {
		LcdMskReg(SYS_CTRL,m_mipi_out_en|m_edp_out_en|m_hdmi_out_en|m_rgb_out_en,v_rgb_out_en(1));
	}
	LcdMskReg(DSP_CTRL0, m_dsp_black_en|m_dsp_blank_en|m_dsp_out_zero|m_dsp_dclk_pol|m_dsp_den_pol|m_dsp_vsync_pol|m_dsp_hsync_pol, 
	                 v_dsp_black_en(0)|v_dsp_blank_en(0)|v_dsp_out_zero(0)|v_dsp_dclk_pol(vid->vl_clkp)
	                 |v_dsp_den_pol(vid->vl_oep)|v_dsp_vsync_pol(vid->vl_vsp)|v_dsp_hsync_pol(vid->vl_hsp));
	
	switch (vid->lcd_face)
	{
	case OUT_P565:
			face = OUT_P565;
			LcdMskReg(DSP_CTRL1, m_dither_down_en | m_dither_down_mode, v_dither_down_en(1) | v_dither_down_mode(0));
			break;
	case OUT_P666:
			face = OUT_P666;
			LcdMskReg(DSP_CTRL1, m_dither_down_en | m_dither_down_mode|m_dither_down_sel,
				v_dither_down_en(1) | v_dither_down_mode(1))|v_dither_down_sel(1);
			break;
	case OUT_D888_P565:
			face = OUT_P888;
			LcdMskReg(DSP_CTRL1, m_dither_down_en | m_dither_down_mode, v_dither_down_en(1) | v_dither_down_mode(0));
			break;
	case OUT_D888_P666:
			face = OUT_P888;
			LcdMskReg(DSP_CTRL1, m_dither_down_en | m_dither_down_mode|m_dither_down_sel,
				v_dither_down_en(1) | v_dither_down_mode(1)|v_dither_down_sel(1));
			break; 
	case OUT_P888:
			face = OUT_P888;
			LcdMskReg(DSP_CTRL1, m_dither_down_en | m_dither_down_mode | m_dither_up_en,
				v_dither_down_en(0) | v_dither_down_mode(0) | v_dither_up_en(1));
			break;
	default:
			LcdMskReg(DSP_CTRL1, m_dither_down_en | m_dither_down_mode|m_dither_up_en,
				v_dither_down_en(0) | v_dither_down_mode(0) | v_dither_up_en(0));
			face = vid->lcd_face;
			break;
	}
	
	if (vid->screen_type == SCREEN_EDP || vid->screen_type == SCREEN_HDMI)
		face = OUT_RGB_AAA;
	//set background color to black,set swap according to the screen panel,disable blank mode
	LcdMskReg(DSP_CTRL0, m_dsp_rg_swap | m_dsp_rb_swap | m_dsp_delta_swap | m_dsp_field_pol  | 
	 	m_dsp_dummy_swap | m_dsp_bg_swap|m_dsp_out_mode,v_dsp_rg_swap(0) | v_dsp_rb_swap(vid->vl_swap_rb) | 
	 	v_dsp_delta_swap(0) | v_dsp_dummy_swap(0) | v_dsp_field_pol(0) | v_dsp_bg_swap(0) | v_dsp_out_mode(face));
	LcdWrReg(DSP_BG,0);
	LcdWrReg(DSP_HTOTAL_HS_END,v_HSYNC(vid->vl_hspw) |
	     v_HORPRD(vid->vl_hspw + vid->vl_hbpd + vid->vl_col + vid->vl_hfpd));
	LcdWrReg(DSP_HACT_ST_END, v_HAEP(vid->vl_hspw + vid->vl_hbpd + vid->vl_col) |
	     v_HASP(vid->vl_hspw + vid->vl_hbpd));
	
	LcdWrReg(DSP_VTOTAL_VS_END, v_VSYNC(vid->vl_vspw) |
	      v_VERPRD(vid->vl_vspw + vid->vl_vbpd + vid->vl_row + vid->vl_vfpd));
	LcdWrReg(DSP_VACT_ST_END,  v_VAEP(vid->vl_vspw + vid->vl_vbpd + vid->vl_row)|
	      v_VASP(vid->vl_vspw + vid->vl_vbpd));
	//  LcdWrReg(DSP_VACT_ST_END_F1,  v_VAEP(vid->vl_vspw + vid->vl_vbpd + vid->vl_row)|
	//           v_VASP(vid->vl_vspw + vid->vl_vbpd));
	
	LcdWrReg(POST_DSP_HACT_INFO,v_HAEP(vid->vl_hspw + vid->vl_hbpd + vid->vl_col) |
	     v_HASP(vid->vl_hspw + vid->vl_hbpd));
	LcdWrReg(POST_DSP_VACT_INFO,  v_VAEP(vid->vl_vspw + vid->vl_vbpd + vid->vl_row)|
	      v_VASP(vid->vl_vspw + vid->vl_vbpd));
	LcdWrReg(POST_DSP_VACT_INFO_F1, 0);
	LcdWrReg(POST_RESERVED, 0x10001000);
	LcdWrReg(MCU_CTRL, 0);
	
	// let above to take effect
	LCDC_REG_CFG_DONE();
	
	if ((vid->screen_type == SCREEN_LVDS) ||
		(vid->screen_type == SCREEN_DUAL_LVDS)) {
		rk32_lvds_en(vid);
	} else if (vid->screen_type == SCREEN_EDP) {
		rk32_edp_enable(vid);
	} else if ((vid->screen_type == SCREEN_MIPI)
		||(vid->screen_type == SCREEN_DUAL_MIPI)) {
		//rk32_mipi_enable(vid);
		//rk32_dsi_enable();
		rk32_dsi_sync();
	}

	return 0;
}


/* Enable LCD and DIGITAL OUT in DSS */
void rk_lcdc_standby(enable)
{
	LcdMskReg(SYS_CTRL, m_standby_en, v_standby_en(enable ? 1 : 0));
	LCDC_REG_CFG_DONE();  
}


int rk_lcdc_init(int lcdc_id)
{
	CPU_AXI_SET_QOS_PRIORITY(2, 2, 0xffad0400);
	
	preg = (lcdc_id == 1) ? RKIO_VOP_LIT_PHYS : RKIO_VOP_BIG_PHYS;  
	grf_writel(1<<16, GRF_IO_VSEL); //LCDCIOdomain 3.3 Vvoltageselectio
	
	// printf(" %s vop_version = 0x%x\n",__func__,LcdRdReg(VERSION_INFO));
	LcdMskReg(SYS_CTRL, m_auto_gating_en | m_standby_en | m_dma_stop | m_mmu_en, 
	                v_auto_gating_en(1)|v_standby_en(0)|v_dma_stop(0)|v_mmu_en(0));	      //zyw
	LcdMskReg(DSP_CTRL1, m_dsp_layer3_sel | m_dsp_layer2_sel | m_dsp_layer1_sel | m_dsp_layer0_sel,
	             v_dsp_layer3_sel(3) | v_dsp_layer2_sel(2) | v_dsp_layer1_sel(1) | v_dsp_layer0_sel(0)); 

//	LcdMskReg(INT_STATUS, m_FS_INT_EN, v_FS_INT_EN(1));  
	LCDC_REG_CFG_DONE();  // write any value to  REG_CFG_DONE let config become effective

	return 0;
}

