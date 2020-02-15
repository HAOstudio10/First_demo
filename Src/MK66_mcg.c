#include "common.h"
#include "MK66_mcg.h"
#include "MK66_lptmr.h"

/*!
 *  @brief      PLL��Ƶ
 *  @param      PLL_e       Ƶ�����ò���
 *  @return     ��ƵƵ�ʣ�MHz��
 *  @since      v5.0
 *  @warning    �˺���ֻ���� ��λ��û�����κ�Ƶ����������µ��ã���MCG��FEIģʽ�²ſɵ���
 *  Sample usage:       uint8 clk = pll_init(PLL100);        //��Ƶ
 */
uint8 pll_init(u16 pll)
{

    //�ϵ縴λ�󣬵�Ƭ�����Զ����� FEI ģʽ��ʹ�� �ڲ��ο�ʱ��
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

    while (MCG_S & MCG_S_IREFST_MASK) {};                       //�ȴ�FLL�ο�ʱ�� Ϊ �ⲿ�ο�ʱ�ӣ�S[IREFST]=0����ʾʹ���ⲿ�ο�ʱ�ӣ���

    while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2) {}; //�ȴ�ѡ���ⲿ�ο�ʱ��
	temp_reg = FMC_PFAPR;

	//ͨ��M&PFD��λM0PFD����ֹԤȡ����
	FMC_PFAPR |= FMC_PFAPR_M7PFD_MASK | FMC_PFAPR_M6PFD_MASK | FMC_PFAPR_M5PFD_MASK
		| FMC_PFAPR_M4PFD_MASK | FMC_PFAPR_M3PFD_MASK | FMC_PFAPR_M2PFD_MASK
		| FMC_PFAPR_M1PFD_MASK | FMC_PFAPR_M0PFD_MASK;
   
	//����ϵͳ��Ƶ��
	// bus �� flex bus �� flash Ƶ�� ���� core ��������Ƶ�������100,50,28
	SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0)    //core = MCG/(0+1)
		| SIM_CLKDIV1_OUTDIV2(1)    //bus  = MCG/(0+1)�޸�����Ƶ�ʺ�ǵð�bus_clk_M�ķ�ƵҲ�޸��ˣ�
		| SIM_CLKDIV1_OUTDIV3(3)    //FlexBus    = MCG/(2+1)
		| SIM_CLKDIV1_OUTDIV4(6);   //Flash clock= MCG/(n+1)

    //FBE -> PBE
	//���´�FMC_PFAPR��ԭʼֵ
	FMC_PFAPR = temp_reg;

    MCG_C5 = MCG_C5_PRDIV(pll_prdiv);                      //��Ƶ�� EXTAL_IN_MHz/( PRDIV+1)
    MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV(pll_vdiv) ;    //��Ƶ�� EXTAL_IN_MHz/( PRDIV+1)  * (VDIV+24)

    while (!(MCG_S & MCG_S_PLLST_MASK)) {};                         //�ȴ�ʱ��Դѡ��PLL

    while (!(MCG_S & MCG_S_LOCK_MASK)) {};                          //�ȴ� PLL���ˣ����໷��

    // �����Ѿ������� PBE ģʽ

    // PBE -> PEE
    MCG_C1 &= ~MCG_C1_CLKS_MASK;

    while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3) {};//�ȴ�ѡ�����PLL

    // �����Ѿ������� PEE ģʽ

    return pll;
} //pll_init

