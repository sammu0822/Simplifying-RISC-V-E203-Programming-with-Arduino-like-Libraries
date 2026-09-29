/*
	wiring_digital.c - digital pin input and output functions 
				¥[€WstaticÅýžÓšçŒÆ€£·|³Q¥~³¡Šsšú 
				digital done
				
*/

#include "wiring_digital.h" //©wžq isPWMSupported¡BgetPWMChannel¡BdisableIOF  
#include "wiring_analog.h"  // €Þ€J enableIOF1 šçŒÆ


//GPIO_PinDescription GPIO_pinMap[];

//static inline bool isPWMSupported(uint8_t pin) 
//{
//    return GPIO_pinMap[pin].support_pwm;
//}

//static inline bool getPWMChannel(uint8_t pin) 
//{
//    return GPIO_pinMap[pin].support_pwm ? GPIO_pinMap[pin].pwm_channel : NO_PWM;
//}


static void defaultIOF(uint8_t pin) 
{
	//**若要將當成類比輸出的接腳重設為一般GPIO，需要重設同一組PWM的其他接腳**
	if(GPIO_pinMap[pin].MODE == ANALOG)
	{
		switch (pin)
		{
			case PWM_0_GPIO_BASE	:
			case PWM_0_GPIO_BASE + 1:
			case PWM_0_GPIO_BASE + 2:
			case PWM_0_GPIO_BASE + 3:
			    GPIO_REG(GPIO_IOF_EN)  &= ~IOF1_PWM0_MASK;
			    GPIO_REG(GPIO_IOF_SEL) &= ~IOF1_PWM0_MASK;  // 禁用IOF, PWM0
			    
			    //關閉GPIO輸入輸出
			    GPIO_REG(GPIO_INPUT_VAL) &= ~GPIO_BIT(pin);
			    GPIO_REG(GPIO_OUTPUT_VAL) &= ~GPIO_BIT(pin);
			    
			    GPIO_pinMap[PWM_0_GPIO_BASE].MODE     = UNDEFINED;
			    GPIO_pinMap[PWM_0_GPIO_BASE + 1].MODE = UNDEFINED;
			    GPIO_pinMap[PWM_0_GPIO_BASE + 2].MODE = UNDEFINED;
			    GPIO_pinMap[PWM_0_GPIO_BASE + 3].MODE = UNDEFINED;
			    break;
				
			case PWM_2_GPIO_BASE	:
			case PWM_2_GPIO_BASE + 1:
			case PWM_2_GPIO_BASE + 2:
			case PWM_2_GPIO_BASE + 3:
			    GPIO_REG(GPIO_IOF_EN)  &= ~IOF1_PWM2_MASK;
			    GPIO_REG(GPIO_IOF_SEL) &= ~IOF1_PWM2_MASK;  // 禁用IOF, PWM2
			    
			    //關閉GPIO輸入輸出
			    GPIO_REG(GPIO_INPUT_VAL) &= ~GPIO_BIT(pin);
			    GPIO_REG(GPIO_OUTPUT_VAL) &= ~GPIO_BIT(pin);
			    
			    GPIO_pinMap[PWM_2_GPIO_BASE].MODE     = UNDEFINED;
			    GPIO_pinMap[PWM_2_GPIO_BASE + 1].MODE = UNDEFINED;
			    GPIO_pinMap[PWM_2_GPIO_BASE + 2].MODE = UNDEFINED;
			    GPIO_pinMap[PWM_2_GPIO_BASE + 3].MODE = UNDEFINED;
			    break;
				
			case PWM_1_GPIO_BASE	:
			case PWM_1_GPIO_BASE + 1:
			case PWM_1_GPIO_BASE + 2:
			case PWM_1_GPIO_BASE + 3:
			    GPIO_REG(GPIO_IOF_EN)  &= ~IOF1_PWM1_MASK;
			    GPIO_REG(GPIO_IOF_SEL) &= ~IOF1_PWM1_MASK;  // 禁用IOF, PWM1
			    
			    //關閉GPIO輸入輸出
			    GPIO_REG(GPIO_INPUT_VAL) &= ~GPIO_BIT(pin);
			    GPIO_REG(GPIO_OUTPUT_VAL) &= ~GPIO_BIT(pin);
			    
			    GPIO_pinMap[PWM_1_GPIO_BASE].MODE     = UNDEFINED;
			    GPIO_pinMap[PWM_1_GPIO_BASE + 1].MODE = UNDEFINED;
			    GPIO_pinMap[PWM_1_GPIO_BASE + 2].MODE = UNDEFINED;
			    GPIO_pinMap[PWM_1_GPIO_BASE + 3].MODE = UNDEFINED;
			    break;
				
			default :
				return ;
		}
		
	}
	else if (GPIO_pinMap[pin].MODE == DIGITAL || GPIO_pinMap[pin].MODE == UNDEFINED)
	{
		// 禁用IOF
		//GPIO_REG(GPIO_IOF_EN)  &= ~GPIO_BIT(pin);
		//GPIO_REG(GPIO_IOF_SEL) &= ~GPIO_BIT(pin);
		
		// 關閉GPIO輸入輸出
		GPIO_REG(GPIO_INPUT_VAL) &= ~GPIO_BIT(pin);
		GPIO_REG(GPIO_OUTPUT_VAL) &= ~GPIO_BIT(pin);
		
		GPIO_pinMap[pin].MODE = UNDEFINED;
	}
	else 
		return ;
	
}

static uint8_t DigitalModeAvailable(uint8_t pin, uint8_t mode) 
{
	//檢驗不合法存取
	//if(!GPIO_pinMap[pin].digital_available == YES || GPIO_pinMap[pin].analog_available == NO) return 1;//未定義錯誤 
	/* ->這個條件有問題，暫時停用。使用test_wiring_digital.c測試時會讓開關無效， GPIO[13]始終維持高電位（函式回傳錯誤訊號）*/
	
	switch (mode)
	{
		case INPUT:   
			GPIO_REG(GPIO_INPUT_EN) |= GPIO_BIT(pin);  
			break;
		case OUTPUT:  
			GPIO_REG(GPIO_OUTPUT_EN) |= GPIO_BIT(pin); 
			break;
		default:
			return 0;//未定義錯誤
	}
	GPIO_pinMap[pin].MODE = DIGITAL;
	
	return 0;
}

static uint8_t AnalogModeAvailable(uint8_t pin, uint8_t mode) 
{
	//檢驗不合法存取
	if( GPIO_pinMap[pin].digital_available == NO || !GPIO_pinMap[pin].analog_available == YES) return 1;
	
	switch (mode)
	{
		case ANALOG_OUTPUT:
			enableIOF1(pin) ;//位於wiring_analog.c 負責啟用IOF1、PWM設定、將pin_MODE設定為ANALOG
			break;
		default:
			return 1;//未定義錯誤
	}
	
	return 0;
}

//**********************************************************

void pinMode(uint8_t pin, uint8_t mode) {
	
	//檢驗不合法存取
	//ÀË¬d¿é€J­È¬O§_¬°ŠXªkž}ŠìžòŒÒŠ¡¡AÀË¬d¬O§_€äŽ©°ò¥»GPIO 
	if ((pin >= GPIO_PIN_COUNT) || (GPIO_pinMap[pin].support_gpio == NO)) return;


	//¹w³]°±¥ÎIOF (IOF = 0, MODE_UNDEFINED) 
	defaultIOF(pin);//重設IOF = 0, 關閉輸入輸出

	//®ÚŸÚŒÒŠ¡€ÁŽ«ž}Šì¥\¯à 
	switch (mode) 
	{
        	case INPUT:
		case OUTPUT:
			DigitalModeAvailable(pin, mode);
		    //if() return ;//檢查可否被設成digital
		    break;
		    
		case ANALOG_INPUT:
		case ANALOG_OUTPUT:
		    if(AnalogModeAvailable(pin, mode)) return ;//檢查可否被設成analog 
		    break;
		    
		default:
		    //return ;//不合法模式輸入
		    break;
    	}
}

void digitalWrite(uint8_t pin, uint8_t val)
{
	//檢驗不合法存取
	if (val != LOW && val != HIGH) return; // Á×§K«Dªk­È //檢查不合法狀態輸入
	if (pin >= GPIO_PIN_COUNT) return; //檢查腳位是否存在
	if (!GPIO_pinMap[pin].support_gpio) return; //檢查是否支援gpio
	//if (!GPIO_pinMap[pin].digital_available || GPIO_pinMap[pin].analog_available) return; //檢查目前這個腳位是否為digital使用
	if (GPIO_pinMap[pin].MODE != DIGITAL) return ;//檢查目前這個腳位是否為digital使用



	if (val == LOW)
        	GPIO_REG(GPIO_OUTPUT_VAL) &= ~GPIO_BIT(pin); 
    	else 
        	GPIO_REG(GPIO_OUTPUT_VAL) |= GPIO_BIT(pin);
}

int digitalRead(uint8_t pin)
{
	//檢驗不合法存取
	if (pin >= GPIO_PIN_COUNT) return -1; //檢查腳位是否存在
	if (GPIO_pinMap[pin].support_gpio != YES) return -1; //檢查是否支援gpio
	//if (!GPIO_pinMap[pin].digital_available || GPIO_pinMap[pin].analog_available) return 0; //檢查目前這個腳位是否可作為digital腳位使用
	if (GPIO_pinMap[pin].MODE != DIGITAL) return -1;//檢查目前這個腳位是否為digital使用
	
	
	return (GPIO_REG(GPIO_INPUT_VAL) & GPIO_BIT(pin)) ? 1 : 0;
}


// 沒有使用 --------------------------------------------------------------------------------------
//void pinMode(uint8_t pin, uint8_t mode) {
//    switch (mode) {
//        case INPUT:
////            GPIO_DIR_REG &= ~(1 << pin);
//			GPIO_REG(GPIO_INPUT_EN) |= pin;
//            break;
//
////        case INPUT_PULLUP:
////            GPIO_DIR_REG &= ~(1 << pin);     // ³]¬°¿é€J
////            GPIO_OUT_REG |=  (1 << pin);     // ©Ô°ªšÏ€º³¡€W©Ô¹qªý¥Í®Ä¡]­Y€äŽ©¡^
////            break;
//
//        case OUTPUT:
////            GPIO_DIR_REG |= (1 << pin);      // ³]¬°¿é¥X
//			GPIO_REG(GPIO_OUTPUT_EN) |= pin;
//            break;
//
//        case ANALOG_INPUT:
//            GPIO_DIR_REG &= ~(1 << pin);     // ž}Šì¬°¿é€J
//            disableDigitalInput(pin);        // Ãö³¬ŒÆŠì¿é€J¡]­YµwÅé€äŽ©¡^
//            enableADCChannel(pin);           // ¶}±Ò ADC šÃ³]©w³q¹D
//            break;
//
//        case PWM_OUTPUT:
//            configurePWMOutput(pin);         // ªì©l€Æ Timer šÃ³]©w¬° PWM ŒÒŠ¡
//            break;
//
//        default:
//            // €£€äŽ©ªºŒÒŠ¡
//            break;
//    }
//}
//
//
////ÀË¬dž}Šì¬O§_€äŽ© PWM
//bool isPWMSupported(uint8_t pin) {
//    return pinMap[pin].support_pwm;
//}
//
////šú±o PWM ³q¹D
//uint8_t getPWMChannel(uint8_t pin) {
//    return pinMap[pin].support_pwm ? pinMap[pin].pwm_channel : NO_PWM;
//}
//
////³]©w IOF multiplexer
//void enableIOF1(uint8_t pin) {
//    // °²³] iof_en, iof_sel ¬°°OŸÐÅéŒÈŠsŸ¹
//    IOF_EN_REG |= (1 << pin);
//    IOF_SEL_REG |= (1 << pin);  // 1 ªí IOF1¡]PWM µ¥¡^
//}


//#define GPIO_BASE_ADDR (0x10012000) // gpio base address
//#define GPIO_INPUT_VAL_ADDR (GPIO_BASE_ADDR+0x00) // gpio input valueregister addr
//#define GPIO_INPUT_EN_ADDR (GPIO_BASE_ADDR+0x04) // gpio input enableregister addr
//#define GPIO_OUTPUT_EN_ADDR (GPIO_BASE_ADDR+0x08) // gpio outputenable register addr
//#define GPIO_OUTPUT_VAL_ADDR (GPIO_BASE_ADDR+0x0C) // gpio output valueregister addr
//
//void pinMode(uint8_t pin, uint8_t mode)
//{
//	
//}

//// Set LED0-3 output
//	GPIO_REG(GPIO_OUTPUT_EN) |= TERASIC_LED_MASK;
//	
//	// Set SW0-3 input
//	GPIO_REG(GPIO_INPUT_EN) |= TERASIC_SW_MASK;

