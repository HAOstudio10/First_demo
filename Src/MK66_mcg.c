#include "common.h"
#include "MK66_mcg.h"
#include "MK66_lptmr.h"

/*!
 *  @brief      PLL超频
 *  @param      PLL_e       频率设置参数
 *  @return     超频频率（MHz）
 *  @since      v5.0
 *  @warning    此函数只能在 复位后没进行任何频率设置情况下调用，即MCG在FEI模式下才可调用
 *  Sample usage:       uint8 clk = pll_init(PLL100);        //超频
 */
uint8 pll_init(u16 pll)
{

    //上电复位后，单片机会自动进入 FEI 模式，使用 内部参考时钟
	//MCG_CLK_MHZ = 50u*(VDIV+16)/(PRDIV+1)/2
	char pll_prdiv;
	char pll_vdiv;
	u32 temp_reg;
	
	SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK
		| SIM_SCGC5_PORTB_MASK
		| SIM_SCGC5_PORTC_MASK
		| SIM_SCGC5_PORTD_MASK
		| SIM_SCGC5_PORTE_MASK);
	pll_prdiv = 4;
	pll_vdiv = pll / 5 - 16;
    //FEI -> FBE
    MCG_C2 &= ~MCG_C2_LP_MASK;
    MCG_C2 |= MCG_C2_RANGE(1);

    MCG_C1 = MCG_C1_CLKS(2) | MCG_C1_FRDIV(3);

    while (MCG_S & MCG_S_IREFST_MASK) {};                       //等待FLL参考时钟 为 外部参考时钟（S[IREFST]=0，表示使用外部参考时钟，）

    while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2) {}; //等待选择外部参考时钟
	temp_reg = FMC_PFAPR;

	//通过M&PFD置位M0PFD来禁止预取功能
	FMC_PFAPR |= FMC_PFAPR_M7PFD_MASK | FMC_PFAPR_M6PFD_MASK | FMC_PFAPR_M5PFD_MASK
		| FMC_PFAPR_M4PFD_MASK | FMC_PFAPR_M3PFD_MASK | FMC_PFAPR_M2PFD_MASK
		| FMC_PFAPR_M1PFD_MASK | FMC_PFAPR_M0PFD_MASK;
   
	//设置系统分频器
	// bus 、 flex bus 、 flash 频率 都是 core 整数倍分频而来最大100,50,28
	SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0)    //core = MCG/(0+1)
		| SIM_CLKDIV1_OUTDIV2(1)    //bus  = MCG/(0+1)修改总线频率后记得把bus_clk_M的分频也修改了！
		| SIM_CLKDIV1_OUTDIV3(3)    //FlexBus    = MCG/(2+1)
		| SIM_CLKDIV1_OUTDIV4(6);   //Flash clock= MCG/(n+1)

    //FBE -> PBE
	//从新存FMC_PFAPR的原始值
	FMC_PFAPR = temp_reg;

    MCG_C5 = MCG_C5_PRDIV(pll_prdiv);                      //分频， EXTAL_IN_MHz/( PRDIV+1)
    MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV(pll_vdiv) ;    //倍频， EXTAL_IN_MHz/( PRDIV+1)  * (VDIV+24)

    while (!(MCG_S & MCG_S_PLLST_MASK)) {};                         //等待时钟源选择PLL

    while (!(MCG_S & MCG_S_LOCK_MASK)) {};                          //等待 PLL锁了（锁相环）

    // 现在已经进入了 PBE 模式

    // PBE -> PEE
    MCG_C1 &= ~MCG_C1_CLKS_MASK;

    while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3) {};//等待选择输出PLL

    // 现在已经进入了 PEE 模式

    return pll;
} //pll_init

