/******************** (C) COPYRIGHT 2011 Ұ��Ƕ��ʽ���������� ********************
 * �ļ���       ��main.c
 * ����         ��PIT��ʱ�ж�ʵ��
 *
 * ʵ��ƽ̨     ��Ұ��kinetis������
 * ��汾       ��
 * Ƕ��ϵͳ     ��
 *
 * ����         ��Ұ��Ƕ��ʽ����������
 * �Ա���       ��http://firestm32.taobao.com
 * ����֧����̳ ��http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/


#include "common.h"
#include "include.h"


/*************************************************************************
*                             Ұ��Ƕ��ʽ����������
*                               PIT��ʱ�жϲ���
*
*  ʵ��˵����Ұ�� PIT ��ʱ�ж�ʵ�飬���жϺ������� LED0 ��ʾ�������жϺ�����
*
*  ʵ���������
*
*  ʵ��Ч����LED0���1s��˸
*
*  ʵ��Ŀ�ģ����� PIT �Ƿ�ʱ�����ж�
*
*  �޸�ʱ�䣺2012-2-29     �Ѳ���
*
*  ��    ע��Ұ��Kinetis������� LED0~3 ���ֱ��PTD15~PTD12 ���͵�ƽ����
*
*************************************************************************/
void main()
{

    DisableInterrupts;                                //��ֹ���ж�

    LED_INIT();                                       //��ʼ��LED��PIT0�ж��õ�LED
    pit_init_ms(PIT0, 1000);                          //��ʼ��PIT0����ʱʱ��Ϊ�� 1000ms

    EnableInterrupts;			              //�����ж�
    while(1)
    {
    }
}