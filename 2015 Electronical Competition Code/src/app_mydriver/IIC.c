#include "include.h"
/******************** (C) COPYRIGHT 2011 Ұ��Ƕ��ʽ���������� ********************
* �ļ���       ��IIC.c
* ����         �����ٶȼƺ�����������ģ��IIC��������
* ʵ��ƽ̨     ������ӡ�󿪷���
* ��汾       ������Ұ���
* Ƕ��ϵͳ     ��
* ����         ��xuxu
**********************************************************************************/




/************************************************
*  �������ƣ�IIC_start
*  ����˵����IIC start
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2014-1-14    �Ѿ�����
*************************************************/
void IIC_start()
{
    SCLout;
    SDAout;
    SCL_L;
    asm("nop");
    SDA_H;
    nop5();
    SCL_H;
    nops();
    SDA_L;
    nops();
    SCL_L;
}



/************************************************
*  �������ƣ�IIC_stop
*  ����˵����IIC end//��ֹͣλ SDA=0->1
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2014-1-14    �Ѿ�����
*************************************************/
void IIC_stop()
{
    SCLout;
    SDAout;
    SCL_L;nop5();
    SDA_L;nop5();
    SCL_H;nops();
    SDA_H;nops();
    SCL_L;
}




/************************************************
*  �������ƣ�IIC_send_byte
*  ����˵����IIC end�ֽڷ��ͳ���
*  ����˵����cΪ�ֽ�
*  �������أ��ޣ������Ǵ�Ӧ��λ
*  �޸�ʱ�䣺2014-1-14    �Ѿ�����
*************************************************/
void send_byte(unsigned char c)
{
    unsigned char i;
    SCLout;
    SDAout;asm("nop");
    for(i=0;i<8;i++)
    {
        SCL_L;
        if((c<<i) & 0x80)
            SDA_H; //�жϷ���λ
        else 
            SDA_L;
        nop5();
        SCL_H;
        nops();
        SCL_L;
    }
    nops();
    SDA_H; //������8bit���ͷ�����׼������Ӧ��λ
    nop5();
    SCL_H;
    nops(); //sda�����ݼ��Ǵ�Ӧ��λ
    SCL_L; //�����Ǵ�Ӧ��λ|��Ҫ���ƺ�ʱ��
}



/************************************************
*  �������ƣ�IIC_read_byte
*  ����˵�����ֽڽ��ճ���,������������������
*  ����˵������
*  �������أ�return: uchar��1�ֽ�
*  �޸�ʱ�䣺2014-1-14    �Ѿ�����
*************************************************/
unsigned char read_byte(void)
{
    unsigned char i;
    unsigned char c;
    SDAin;
    SCLout;
    c=0;
    SCL_L;
    nop5();
    for(i=0;i<8;i++)
    {
        nop5();
        SCL_L; //��ʱ����Ϊ�ͣ�׼����������λ
        nops();
        SCL_H; //��ʱ����Ϊ�ߣ�ʹ��������������Ч
        nop5();
        c<<=1;
        if(SDA_read)
            c+=1; //������λ�������յ����ݴ�c
    }
    SCL_L;
    return c;
}




/************************************************
*  �������ƣ�IIC_I2C_Single_Write
*  ����˵����//д��Ĵ���
*  ����˵����SlaveAddress�豸ID���Ĵ�����ַaddress��thedataΪд������
*  �������أ�return: uchar��1�ֽ�
*  �޸�ʱ�䣺2014-1-14    �Ѿ�����
*************************************************/
void I2C_Single_Write(unsigned char SlaveAddress,unsigned char address, unsigned char thedata)
{
    IIC_start();		//����
    send_byte(SlaveAddress);	//д���豸ID��д�ź�
    send_byte(address);	//X��ַ
    send_byte(thedata);	//д���豸ID������
    IIC_stop();
}



/************************************************
*  �������ƣ�IIC_I2C_Single_Read
*  ����˵�������Ĵ��� T = 46us(debugģʽ)
*  ����˵����SlaveAddress�豸ID���Ĵ�����ַaddress
*  �������أ�return1���ֽڣ�retΪ��������
*  �޸�ʱ�䣺2014-1-14    �Ѿ�����
*************************************************/
unsigned char I2C_Single_Read(unsigned char SlaveAddress,unsigned char address)
{
    unsigned char ret = 100;
    IIC_start();		//����
    send_byte(SlaveAddress);	//д���豸ID��д�ź�
    send_byte(address);	//X��ַ
    IIC_start();		//���·��Ϳ�ʼ
    send_byte(SlaveAddress+1);	//д���豸ID������
    ret = read_byte();	//��ȡһ�ֽ�
    IIC_stop();
    return ret;
}


//���ٶȼƺ�������������ƫ�Ƿ�ɼ���ɵı�־λ
unsigned char Acc_CALIBRATED=1;
unsigned char Gyro_CALIBRATED=1;


//MPU6050��ʼ������������������ʣ���ͨ�˲�Ƶ��
void MPU6050_Init(unsigned short sample_rate, unsigned short lpf)
{
    unsigned char default_filter;
    gpio_init(PORTC,19,GPO,LOW);
    gpio_init(PORTD, 0,GPO,LOW);
    //InitMPU6050()
    switch (lpf)
    {
    case 5:
        default_filter = MPU6050_LPF_5HZ;
        break;
    case 10:
        default_filter = MPU6050_LPF_10HZ;
        break;
    case 20:
        default_filter = MPU6050_LPF_20HZ;
        break;
    case 42:
        default_filter = MPU6050_LPF_42HZ;
        break;
    case 98:
        default_filter = MPU6050_LPF_98HZ;
        break;
    case 188:
        default_filter = MPU6050_LPF_188HZ;
        break;
    case 256:
        default_filter = MPU6050_LPF_256HZ;
        break;
    default:
        default_filter = MPU6050_LPF_98HZ;
        break;
    }	
    
    //�豸��λ
    I2C_Single_Write(MPU6050_ADDRESS,MPU_RA_PWR_MGMT_1, 0x80);	
    
    delayms(5);
    
    //�����ǲ����ʣ�0x00(1000Hz)   ������ = �����ǵ������ / (1 + SMPLRT_DIV)
    I2C_Single_Write(MPU6050_ADDRESS,MPU_RA_SMPLRT_DIV, (1000/sample_rate - 1));	
    //�����豸ʱ��Դ��������Z��
    I2C_Single_Write(MPU6050_ADDRESS, MPU_RA_PWR_MGMT_1, 0x03);	
    //i2c��·ģʽ
    I2C_Single_Write(MPU6050_ADDRESS, MPU_RA_INT_PIN_CFG, 0 << 7 | 0 << 6 | 0 << 5 | 0 << 4 | 0 << 3 | 0 << 2 | 1 << 1 | 0 << 0); 
    // INT_PIN_CFG   -- INT_LEVEL_HIGH, INT_OPEN_DIS, LATCH_INT_DIS, INT_RD_CLEAR_DIS, FSYNC_INT_LEVEL_HIGH, FSYNC_INT_DIS, I2C_BYPASS_EN, CLOCK_DIS
    //��ͨ�˲�Ƶ�ʣ�0x03(42Hz)
    I2C_Single_Write(MPU6050_ADDRESS,MPU_RA_CONFIG, default_filter);	
    //�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
    I2C_Single_Write(MPU6050_ADDRESS, MPU_RA_GYRO_CONFIG, 0x18); 
    //���ټ��Լ졢������Χ(���Լ죬+-8G)			
    I2C_Single_Write(MPU6050_ADDRESS,MPU_RA_ACCEL_CONFIG, 2 << 3);	
}

//��ȡ���ٶ�
unsigned char mpu6050_buffer[12];short acc_temp[3];
void MPU6050_Read_Acc_Data(void)
{
    mpu6050_buffer[0] = I2C_Single_Read(MPU6050_ADDRESS,MPU_RA_ACCEL_XOUT_L); 
    mpu6050_buffer[1] = I2C_Single_Read(MPU6050_ADDRESS,MPU_RA_ACCEL_XOUT_H);
    acc_temp[0] = ((short)((mpu6050_buffer[1]<<8u) | mpu6050_buffer[0])) - ((short)(Acc_Offset.x));  //���ٶ�X��
    
    mpu6050_buffer[2] = I2C_Single_Read(MPU6050_ADDRESS,MPU_RA_ACCEL_YOUT_L);
    mpu6050_buffer[3] = I2C_Single_Read(MPU6050_ADDRESS,MPU_RA_ACCEL_YOUT_H);
    acc_temp[1] = ((short)((mpu6050_buffer[3]<<8u) | mpu6050_buffer[2]))- ((short)(Acc_Offset.y));  //���ٶ�Y��
    
    mpu6050_buffer[4] = I2C_Single_Read(MPU6050_ADDRESS,MPU_RA_ACCEL_ZOUT_L);
    mpu6050_buffer[5] = I2C_Single_Read(MPU6050_ADDRESS,MPU_RA_ACCEL_ZOUT_H);
    acc_temp[2] = ((short)((mpu6050_buffer[5]<<8u) | mpu6050_buffer[4]))- ((short)(Acc_Offset.z));  //���ٶ�Z��
    
    Acc_ADC.x = (float)acc_temp[0];
    Acc_ADC.y = (float)acc_temp[1];
    Acc_ADC.z = (float)acc_temp[2];
    
    MPU6050_CalOffset_Acc();//���ٶ���ƫ����,�����ʼ�Ĳɼ���ƫ��ʱ���Acc_CALIBRATED����0�����뺯���ͷ�����
}



//��ȡ���ٶ�
void MPU6050_Read_Gyro_Data(void)
{
    short gyro_temp[3];
    
    mpu6050_buffer[6] = I2C_Single_Read(MPU6050_ADDRESS,MPU_RA_GYRO_XOUT_L); 
    mpu6050_buffer[7] = I2C_Single_Read(MPU6050_ADDRESS,MPU_RA_GYRO_XOUT_H);
    gyro_temp[0] = ((short)((mpu6050_buffer[7]<<8u) | mpu6050_buffer[6]))  - ((short)(Gyro_Offset.x));	//������X��
    
    mpu6050_buffer[8] = I2C_Single_Read(MPU6050_ADDRESS,MPU_RA_GYRO_YOUT_L);
    mpu6050_buffer[9] = I2C_Single_Read(MPU6050_ADDRESS,MPU_RA_GYRO_YOUT_H);
    gyro_temp[1] = ((short)((mpu6050_buffer[9]<<8u) | mpu6050_buffer[8]))  -  ((short)(Gyro_Offset.y));	//������Y��
    
    mpu6050_buffer[10] = I2C_Single_Read(MPU6050_ADDRESS,MPU_RA_GYRO_ZOUT_L);
    mpu6050_buffer[11] = I2C_Single_Read(MPU6050_ADDRESS,MPU_RA_GYRO_ZOUT_H);
    gyro_temp[2] = ((short)((mpu6050_buffer[11]<<8u) | mpu6050_buffer[10])) - ((short)(Gyro_Offset.z));	  //������Z��		
    
    Gyro_ADC.x = (float)gyro_temp[0];
    Gyro_ADC.y = (float)gyro_temp[1];
    Gyro_ADC.z = (float)gyro_temp[2];
    
    MPU6050_CalOffset_Gyro();
    
    //	if(_fabsf(Gyro_ADC.x) < GyroAD_Limit) Gyro_ADC.x = 0;
    //	if(_fabsf(Gyro_ADC.y) < GyroAD_Limit) Gyro_ADC.y = 0;
    //	if(_fabsf(Gyro_ADC.z) < GyroAD_Limit) Gyro_ADC.z = 0;
    
    Gyro_dps.x = radians(Gyro_ADC.x * MPU6050G_s2000dps);   // dps
    Gyro_dps.y = radians(Gyro_ADC.y * MPU6050G_s2000dps);   // dps
    Gyro_dps.z = radians(Gyro_ADC.z * MPU6050G_s2000dps);   // dps	
}

//���ٶ���ƫ����
void MPU6050_CalOffset_Acc(void)
{
    if(Acc_CALIBRATED)
    {
        static struct CarThreeNum	tempAcc={0,0,0};
        static unsigned short cnt_a=0;
        
        if(cnt_a==0)
        {
            Acc_Offset.x = 0;
            Acc_Offset.y = 0;
            Acc_Offset.z = 0;
            tempAcc.x = 0;
            tempAcc.y = 0;
            tempAcc.z = 0;
            cnt_a = 1;
            return;
        }			
        tempAcc.x = (tempAcc.x + Acc_ADC.x) / 2.0f;
        tempAcc.y = (tempAcc.y + Acc_ADC.y) / 2.0f;
        tempAcc.z = (tempAcc.z + Acc_ADC.z) / 2.0f;
        if(cnt_a == CALIBRATING_ACC_CYCLES)
        {
            Acc_Offset.x = tempAcc.x;
            Acc_Offset.y = tempAcc.y;
            Acc_Offset.z = tempAcc.z - ADC_1G;
            cnt_a = 0;
            Acc_CALIBRATED = 0;
            //param.SAVE_ACC_OFFSET();//��������
            return;
        }
        cnt_a++;		
    }	
}

//��������ƫ����
void MPU6050_CalOffset_Gyro(void)
{
    if(Gyro_CALIBRATED)
    {
        static struct CarThreeNum	tempGyro={0,0,0};
        static unsigned short cnt_g=0;
        if(cnt_g==0)
        {
            Gyro_Offset.x = 0;
            Gyro_Offset.y = 0;
            Gyro_Offset.z = 0;
            tempGyro.x = 0;
            tempGyro.y = 0;
            tempGyro.z = 0;
            cnt_g = 1;
            return;
        }
        tempGyro.x = (tempGyro.x + Gyro_ADC.x) / 2.0f;
        tempGyro.y = (tempGyro.y + Gyro_ADC.y) / 2.0f;
        tempGyro.z = (tempGyro.z + Gyro_ADC.z) / 2.0f;
        
        if(cnt_g == CALIBRATING_GYRO_CYCLES)
        {
            Gyro_Offset.x = tempGyro.x;
            Gyro_Offset.y = tempGyro.y;
            Gyro_Offset.z = tempGyro.z;
            cnt_g = 0;
            Gyro_CALIBRATED = 0;
            //param.SAVE_GYRO_OFFSET();//��������
            return;
        }
        cnt_g++;
    }
}

//���´���������
void MPU6050_updateSensor(void)
{
    //��ȡ���ٶ�
    MPU6050_Read_Acc_Data();
    //��ȡ���ٶ�
    MPU6050_Read_Gyro_Data();	
}








/*
�����˲����Ļ�����ַ���
*/
//��ֹƵ��:30Hz ����Ƶ��:500Hz
#define b0 0.1883633f
#define b1 0
#define a1 1.023694f
#define a2 0.2120577f

struct CarThreeNum LastIn={0,0,0};
struct CarThreeNum PreOut={0,0,0};
struct CarThreeNum LastOut={0,0,0};

void Filter_2nd_LPF2ndFilter()//����������ڼ��ٶȼ������˲���ʹ��ֱ�����˲�ҲЧ��һ��
{
    Acc_ADC_Data.x = b0 * Acc_ADC.x + b1 * LastIn.x -  a1 * LastOut.x - a2 * PreOut.x ;
    Acc_ADC_Data.y = b0 * Acc_ADC.y + b1 * LastIn.y -  a1 * LastOut.y - a2 * PreOut.y;
    Acc_ADC_Data.z = b0 * Acc_ADC.z + b1 * LastIn.z -  a1 * LastOut.z - a2 * PreOut.z;
    
    LastIn.x = Acc_ADC.x;
    LastIn.y = Acc_ADC.y;
    LastIn.z = Acc_ADC.z;
    
    PreOut.x = LastOut.x;
    PreOut.y = LastOut.y;
    PreOut.z = LastOut.z;
    
    LastOut.x = Acc_ADC_Data.x;
    LastOut.y = Acc_ADC_Data.y;
    LastOut.z = Acc_ADC_Data.z;
}



/*
������Ԫ��
*/
#define Kp 1.0f
#define Ki 0.0f 
float norm;
float halfT;
float vx,vy,vz;
float ex,ey,ez;
float gx,gy,gz;
float ax,ay,az;	
static float exInt = 0, eyInt = 0, ezInt = 0;//������̬�������Ļ���

    
    
/***************************************************
�Լ��ٶȼƽ��ж��׵�ͨ�˲�
����gx��gy��gz�ֱ��Ӧ������Ľ��ٶȣ���λ�ǻ���/��
������ax��ay��az�ֱ��Ӧ������ļ��ٶ�ԭʼ����
***************************************************/
void Attitude_Updata_Quaternion(float deltaT)//û�õ�
{
    halfT = deltaT / 2.0f;
    ax = Acc_ADC_Data.x;
    ay = Acc_ADC_Data.y;
    az = Acc_ADC_Data.z;
    
    gx = Gyro_dps.x;
    gy = Gyro_dps.y;
    gz = Gyro_dps.z;
    
    //�����ٶȵ�ԭʼ���ݣ���һ�����õ���λ���ٶ�
    norm = (double)sqrt((double)(ax * ax + ay * ay + az * az));
    if(norm == 0) 
        return;
    ax = ax / norm;
    ay = ay / norm;
    az = az / norm;
    
    /**************************************************
    ����Ԫ������ɡ��������Ҿ����еĵ����е�����Ԫ�ء�
    �������Ҿ����ŷ���ǵĶ��壬��������ϵ������������
    ת����������ϵ��������������Ԫ�ء����������vx��vy��vz��
    ��ʵ���ǵ�ǰ�Ļ����������ϵ�ϣ����������������λ������
    (�ñ�ʾ������̬����Ԫ�����л���)
    ***************************************************/
    vx = 2.0f * (Q.q2 * Q.q4 - Q.q1 * Q.q3);
    vy = 2.0f * (Q.q1 * Q.q2 + Q.q3 * Q.q4);
    vz = 1.0f - 2.0f * ( Q.q2 * Q.q2 + Q.q3 * Q.q3);//Q.w * Q.w + Q.z * Q.z;
    
    /***************************************************
    ���������������������(Ҳ����������)����ʾ��	ex��
    ey��ez�����������������Ĳ���������������Ծ���λ�ڻ���
    ����ϵ�ϵģ������ݻ������Ҳ���ڻ�������ϵ�����Ҳ���Ĵ�
    С�����ݻ����������ȣ����������������ݡ����������ǶԻ�
    ��ֱ�ӻ��֣����Զ����ݵľ�������ֱ�������ڶԻ�������ϵ��
    ������
    ***************************************************/
    ex = (ay * vz - az * vy);
    ey = (az * vx - ax * vz);
    ez = (ax * vy - ay * vx);
    
    /***************************************************
    �ò���������PI����������ƫ��ͨ������Kp��Ki������������
    �Կ��Ƽ��ٶȼ����������ǻ�����̬���ٶ�
    ***************************************************/
    if(Ki > 0)
    {
        exInt = exInt + ex * Ki;
        eyInt = eyInt + ey * Ki;
        ezInt = ezInt + ez * Ki;
        gx = gx + Kp * ex + exInt;
        gy = gy + Kp * ey + eyInt;
        gz = gz + Kp * ez + ezInt;
    }
    else
    {
        gx = gx + Kp * ex;
        gy = gy + Kp * ey;
        gz = gz + Kp * ez;
    }
    
    //��Ԫ��΢�ַ��� 
    Q.q1 += (-Q.q2 * gx - Q.q3 * gy - Q.q4 * gz) * halfT;
    Q.q2 += ( Q.q1 * gx + Q.q3 * gz - Q.q4 * gy) * halfT;
    Q.q3 += ( Q.q1 * gy - Q.q2 * gz + Q.q4 * gx) * halfT;
    Q.q4 += ( Q.q1 * gz + Q.q2 * gy - Q.q3 * gx) * halfT;
    
    //��Ԫ����λ��
    norm = sqrt(Q.q1 * Q.q1 + Q.q2 * Q.q2 + Q.q3 * Q.q3 + Q.q4 * Q.q4);
    
    if(norm == 0) return;
    
    Q.q1 = Q.q1 / norm;
    Q.q2 = Q.q2 / norm;
    Q.q3 = Q.q3 / norm;
    Q.q4 = Q.q4 / norm;
    
    Quaternion_to_euler();
    
    //��ת�Ƕ�
    if(Q.q3!=0)
        Angle.rotation = atan2f(Q.q2,Q.q3) * 57.29577951f;
    
    // ���ظ���Ԫ���ĵ�Ч��ת�����е���������   
    Gravity.x = 2*(Q.q2*Q.q4 - Q.q1*Q.q3);								
    Gravity.y = 2*(Q.q1*Q.q2 + Q.q3*Q.q4);						  
    Gravity.z = 1-2*(Q.q2*Q.q2 + Q.q3*Q.q3);
    
    //����ֱ����ĽǶ�
    float normal = sqrt(Gravity.x * Gravity.x + Gravity.y * Gravity.y);
    Angle.vertical = 90.0f - atan2f(Gravity.z , normal) * 57.29577951f; 
}


//��Ԫ��ת����ŷ����
void Quaternion_to_euler()
{
    Angle.roll = degrees(atan2f(2.0f*(Q.q1*Q.q2 + Q.q3*Q.q4),1 - 2.0f*(Q.q2*Q.q2 + Q.q3*Q.q3)));
    //ʹ��safe_asin()������pitch�ӽ�90/-90ʱ�����
    Angle.pitch = degrees(safe_asin(2.0f*(Q.q1*Q.q3 - Q.q2*Q.q4)));
    Angle.yaw = degrees(atan2f(2.0f*(Q.q2*Q.q3 - Q.q1*Q.q4), 2.0f*(Q.q1*Q.q1 + Q.q2*Q.q2) - 1));
}






//��֤����ֵ����Ч��
float safe_asin(float v)
{
    if (isnan(v)) {
        return 0.0f;
    }
    if (v >= 1.0f) {
        return M_PI/2;
    }
    if (v <= -1.0f) {
        return -M_PI/2;
    }
    return asinf(v);
}
//�Ƕ�ת����
float radians(float deg) 
{
    return deg * DEG_TO_RAD;
}

//����ת�Ƕ�
float degrees(float rad) 
{
    return rad * RAD_TO_DEG;
}

//32λ�������޷�
int constrain_int32(int amt, int low, int high) 
{
    return ((amt)<(low)?(low):((amt)>(high)?(high):(amt)));
}