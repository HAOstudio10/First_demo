#ifndef     __MK66_I2C_H__
#define     __MK66_I2C_H__
/**
 *  @brief I2Cģ����
 */
typedef enum
{
    I2C0  = 0,
    I2C1  = 1
} I2Cn_e;

/**
 *  @brief ������дģʽѡ��
 */
typedef enum MSmode
{
    MWSR =   0x00,  /* ����дģʽ  */
    MRSW =   0x01   /* ������ģʽ  */
} MSmode;


//Ŀǰ�����֧�� I2C����ģʽ
extern uint32  i2c_init(I2Cn_e i2cn, uint32 baud);                               //��ʼ��I2C
extern void    i2c_write_reg(I2Cn_e, uint8 SlaveID, uint8 reg, uint8 Data);      //д�����ݵ��Ĵ���
extern uint8   i2c_read_reg (I2Cn_e, uint8 SlaveID, uint8 reg);                  //��ȡ�Ĵ���������

#endif  //__MK66_I2C_H__
