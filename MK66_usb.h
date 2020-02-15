#ifndef _MK66_USB_H_
#define _MK66_USB_H_

#include "USB.h"


 // USB  ģ��ʱ�ӱ��� ���� Ϊ 48M

 // USB ģ��ʱ��Դ ѡ��
#define USB_CLOCK_PLL       //PLL����Ϊʱ��Դ
//#define USB_CLOCK_FLL     //FLL  ��Ϊʱ��Դ
//#define USB_CLOCK_CLKIN   //USB_CLKIN (�� PTE26) ��Ϊʱ��Դ

//USB clk = ʱ��ԴƵ�� * (1 + USB_CLK_FRAC) / (USB_CLK_DIV + 1)
//USB_CLK_FRAC = 0 �� 1
//USB_CLK_DIV 0 ~ 7

//PLL  = 120M
#define USB_CLK_DIV         4       // ʱ��Դ��Ƶ���� �� 
#define USB_CLK_FRAC        1





void    usb_init(void);                         // USB ģ���ʼ��

void    usb_com_init(void);                     // USB ���⴮�� ��ʼ��
void    usb_enum_wait(void);                    // USB �ȴ�ö��
uint8   usb_com_rx(uint8 *rx_buf);              // USB ���⴮�� ����
void    usb_com_tx(uint8 *tx_buf, uint8 len);   // USB ���⴮�� ����


#endif  //_MK60_USB_H_
