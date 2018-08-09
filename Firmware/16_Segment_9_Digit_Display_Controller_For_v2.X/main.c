//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//	main.c
//	16 segment 9 digit display Controller 実体プログラム
//
//	Created by thotgamma. (https://gammalab.net)
//
//	16Segment9DigitDisplay公式:
//		https://htlab.net/products/electronics/16-segment-9-digit-display-board-1/
//		https://htlab.net/products/electronics/16-segment-9-digit-display-controller-1/
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


#include <xc.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "segFonts.h"
#include "system.h"
#include "utilities.h"

#include "i2c.h"
#include "usb.h"
#include "usb_device_hid.h"
#include "app_device_custom_hid.h"
#include "app_led_usb_status.h"


void main(void) {

    ADCON1 = 0b00001111; //All Digital
    CMCON  = 0b00000111; //No Comparator
    TRISA  = 0b00001111;
    TRISB  = 0b00000011;
    TRISC  = 0b10000000; //Rxのみ入力
    TRISD  = 0b11111111;
    TRISE  = 0b00000111;
    
    PORTEbits.RDPU = 1;

    LATCbits.LATC0 = 0; // OUTPUT ENABLE
    LATCbits.LATC1 = 1; // CLEAR

    LATCbits.LATC2 = 0; // PWM

    // 12VDCジャックアリのときはどっちも1
    LATAbits.LATA4 = 0; // USB_VCC -> VCC
    LATAbits.LATA5 = 0; // VCC -> LED
    
    /* MEMO
    A/D Result High Register (ADRESH)   
    A/D Result Low Register (ADRESL)    
    A/D Control Register 0 (ADCON0)     (-,-,CHS3,CHS2,CHS1,CHS0,GO/Done,ADON)
    A/D Control Register 1 (ADCON1)     (-,-,VCFG1,VCFG0,PCFG3,PCFG2,PCFG1,PCFG0)
    A/D Control Register 2 (ADCON2)     (ADFM,-,ACQT2,ACQT1,ADCS2,ADCS1,ADCS0)
    */

    //タイマー設定。比較機が使えるTimer2を使う
    //TMR2 = 0;
    //PR2 = 125;
    //T2CON = 0b01111101;
    
    // LED制御のタイマー設定
    T0CON = 0b11000101; //Timer0 ON, 8Bit Mode, InternalClock, 1/64 Prescale
    TMR0 = 0xFF - 125+1;

    //各種割り込み許可
    //PIE1bits.TMR2IE = 1;
    INTCONbits.TMR0IE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;

    // UART設定
    RCSTA   = 0b10010000;
    BAUDCON = 0b00001000;
    SPBRGH  = 0;
    BRGH = 0;
    SPBRG   = 129;

    uint8_t RxData;            // 受信データ用バッファ
    uint8_t digitSelector;    // 書き換え桁数
    uint32_t dotflag;  // ドットをつけるかどうか
    
    while (PORTDbits.RD7);
    showDemoMessage = PORTDbits.RD0;
    
    USBDeviceInit();
    USBDeviceAttach();
    
    I2C_Init();
    while (1){
        
        
        if (PIR1bits.RCIF) {
            PIR1bits.RCIF = 0;           //フラグを下げる
            RxData = RCREG;              // 受信データを取り込む
        
            //もし、先頭ビットが111であれば
            if ((RxData & 0b11100000) == 0b11100000) {
                digitSelector = (RxData & 0b00001111);
                dotflag = (RxData & 0b00010000) >> 4;
                while (!PIR1bits.RCIF);      // 受信するまで待つ
                PIR1bits.RCIF = 0;
                RxData = RCREG;               // 受信データを取り込む
                if (digitSelector > 8) continue;  // 無効な入力の処理
                if (RxData > 0b01111111) RxData = ~RxData;
                segMap[digitSelector] = ~(fontList[RxData] | (dotflag << 16)); // 値を実際にセット
            }
        }
        
        APP_DeviceCustomHIDTasks();
        
    }
}