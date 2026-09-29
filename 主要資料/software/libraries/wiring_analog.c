//#warning "wiring_analog.c compiled"
#include "wiring_analog.h"

/*
	wiring_analog.c - analog pin input and output functions 
				¥[€WstaticÅýžÓšçŒÆ€£·|³Q¥~³¡Šsšú 
				
*/

//由wiring_digital.c外部呼叫，依傳入的腳位負責設定相應的IOF暫存器為1來啟用PWM，並初始化PWM相關數值
void enableIOF1(uint8_t pin) 
{
    // ®ÚŸÚ³q¹D¿ïŸÜ¥¿œTªº IOF1_MASK
    //**只要有PWM功能的其中一隻接腳被設為ANALOG_OUTPUT，同一組PWM的其他三隻接腳都會被設定成PWM**
    switch (pin) 
    {
    	case PWM_0_GPIO_BASE	:
        case PWM_0_GPIO_BASE + 1:
        case PWM_0_GPIO_BASE + 2:
        case PWM_0_GPIO_BASE + 3:
            GPIO_REG(GPIO_IOF_EN)  |= IOF1_PWM0_MASK;
            GPIO_REG(GPIO_IOF_SEL) |= IOF1_PWM0_MASK;  // IOF1, SEL=1, PWM0
            analogSetting(0, 0, PWM_REG_CMP0_DEFAULT);
            
            GPIO_pinMap[PWM_0_GPIO_BASE].MODE     = ANALOG;
            GPIO_pinMap[PWM_0_GPIO_BASE + 1].MODE = ANALOG;
            GPIO_pinMap[PWM_0_GPIO_BASE + 2].MODE = ANALOG;
            GPIO_pinMap[PWM_0_GPIO_BASE + 3].MODE = ANALOG;
            break;
        case PWM_2_GPIO_BASE    :
        case PWM_2_GPIO_BASE + 1:
        case PWM_2_GPIO_BASE + 2:
        case PWM_2_GPIO_BASE + 3:
            GPIO_REG(GPIO_IOF_EN)  |= IOF1_PWM2_MASK;
	    GPIO_REG(GPIO_IOF_SEL) |= IOF1_PWM2_MASK;  // IOF1, SEL=1, PWM2
            analogSetting(2, 0, PWM_REG_CMP0_DEFAULT);
            
            GPIO_pinMap[PWM_2_GPIO_BASE].MODE     = ANALOG;
            GPIO_pinMap[PWM_2_GPIO_BASE + 1].MODE = ANALOG;
            GPIO_pinMap[PWM_2_GPIO_BASE + 2].MODE = ANALOG;
            GPIO_pinMap[PWM_2_GPIO_BASE + 3].MODE = ANALOG;
            break;
        case PWM_1_GPIO_BASE    :
        case PWM_1_GPIO_BASE + 1:
        case PWM_1_GPIO_BASE + 2:
        case PWM_1_GPIO_BASE + 3:
            GPIO_REG(GPIO_IOF_EN)  |= IOF1_PWM1_MASK;
            GPIO_REG(GPIO_IOF_SEL) |= IOF1_PWM1_MASK;  // IOF1, SEL=1, PWM1
            analogSetting(1, 0, PWM_REG_CMP0_DEFAULT);
            
            GPIO_pinMap[PWM_1_GPIO_BASE].MODE     = ANALOG;
            GPIO_pinMap[PWM_1_GPIO_BASE + 1].MODE = ANALOG;
            GPIO_pinMap[PWM_1_GPIO_BASE + 2].MODE = ANALOG;
            GPIO_pinMap[PWM_1_GPIO_BASE + 3].MODE = ANALOG;
            break;
            
        default:
            return; // µL®Ä³q¹D
    }
    
} 

static void analogSetting(uint8_t PWMchennel, uint8_t count, uint8_t cmp)
{
	//set pwm deglitch, enalways, zerocmp bits
	PWM0_REG(PWM_CFG) = (PWM_CFG_DEGLITCH | PWM_CFG_ENALWAYS | PWM_CFG_ZEROCMP);
	
	//pmws LBS increment at 488.3Hz about 2ms
	//set pwm_count value
	switch (PWMchennel)
	{
		case 0:
			PWM0_REG(PWM_COUNT) = count; 
			PWM0_REG(PWM_CMP0) = cmp; //set pwm0_cmp0 value
			break;
		case 2:
			PWM2_REG(PWM_COUNT) = count;
			PWM2_REG(PWM_CMP0) = cmp; //set pwm2_cmp0 value
			break;
		case 1:
			PWM1_REG(PWM_COUNT) = count;
			PWM1_REG(PWM_CMP0) = cmp; //set pwm1_cmp0 value
			break;
		default:
			return ;//錯誤通道代號
	}
	
}

void analogWrite(uint8_t pin, uint8_t val) 
{
	if (val <0 || val > PWM_REG_CMP0_DEFAULT ) return; // Á×§K«Dªk­È //檢查不合法狀態輸入
	if (pin >= GPIO_PIN_COUNT) return; //檢查腳位是否存在
	if (!GPIO_pinMap[pin].support_gpio) return; //檢查是否支援gpio
	if (GPIO_pinMap[pin].digital_available && !GPIO_pinMap[pin].analog_available) return; //檢查目前這個腳位是否可作為analog腳位使用
	

    	//if (!desc.support_pwm || desc.pwm_channel == NO_PWM) return; 
    	//檢查是否支援pwm（因目前可作為analog腳位皆為pwm可用接腳，先停用）
    	

    	// ³]©wŠU PWM ³q¹Dªº€ñžû­È¡]¥eªÅ€ñ¡^
    	switch(pin)
    	{
    		//**設定PWM_CMP0值是設定計數器最大值**
    		//**每個PWM接腳暫存器(PWM_CMP[1..3])可以單獨設定類比輸出值，唯獨注意該接腳的那組PWM之最大值(PWM_CMP0)**
    		//**佔空比與類比輸出數值相反 (類比最大值 - 類比輸出值)**
    		case PWM_0_GPIO_BASE    :
    			PWM0_REG(PWM_CMP0) = val;
    			break; 
    			
    		case PWM_0_GPIO_BASE + 1:
    			PWM0_REG(PWM_CMP1) = val;
    			break;
		case PWM_0_GPIO_BASE + 2:
			PWM0_REG(PWM_CMP2) = val;
			break;
		case PWM_0_GPIO_BASE + 3:
			PWM0_REG(PWM_CMP3) = val;
			break;
			
		case PWM_2_GPIO_BASE    :
    			PWM2_REG(PWM_CMP0) = val;
    			break; 
    			
		case PWM_2_GPIO_BASE + 1:
			PWM2_REG(PWM_CMP1) = val;
    			break;
		case PWM_2_GPIO_BASE + 2:
			PWM2_REG(PWM_CMP2) = val;
    			break;
		case PWM_2_GPIO_BASE + 3:
		    	PWM2_REG(PWM_CMP3) = val;
    			break;
    			
    		case PWM_1_GPIO_BASE    :
    			PWM1_REG(PWM_CMP0) = val;
    			break; 
    			
		case PWM_1_GPIO_BASE + 1:
			PWM1_REG(PWM_CMP1) = val;
    			break;
		case PWM_1_GPIO_BASE + 2:
			PWM1_REG(PWM_CMP2) = val;
    			break;
		case PWM_1_GPIO_BASE + 3:
			PWM1_REG(PWM_CMP3) = val;
    			break;
		    
		default:
		    return; // µL®Ä³q¹D
	}
	    	
}

//沒有使用

//GPIO_REG(GPIO_IOF_EN)  |= IOF1_PWM0_MASK; //IOF EN=1
//GPIO_REG(GPIO_IOF_SEL) |= IOF1_PWM0_MASK; //IOF SEL=1

//static void setupPWM(uint8_t pin, uint16_t value)
//{
//	// ±Ò¥Î PWM ¥\¯à»PšŸ€òšë¡A³]©w¬°¶gŽÁ©Ê»PŠÛ°ÊÂk¹s
//    	PWM0_REG(PWM_CFG) = (PWM_CFG_DEGLITCH | PWM_CFG_ENALWAYS | PWM_CFG_ZEROCMP);
//
//    	// ³]©w­pŒÆŸ¹ªì©l­È
//    	//pmws LBS increment at 488.3Hz about 2ms
//	//set pwm_count value
//    	PWM0_REG(PWM_COUNT) = 0;
//
//    	// ³]©w€@­Ó¶gŽÁªºªø«×¡GPWM_CMP0 ³q±`¬° TOP ­È
//    	PWM0_REG(PWM_CMP0) = 255;  // ¥iœÕŸã¬°§óªøªº¶gŽÁšÓ­°§CÀW²v
//
//    	// ³]©wŠU PWM ³q¹Dªº€ñžû­È¡]¥eªÅ€ñ¡^
//    	switch (pwm_channel) 
//	{
//    	    case 0:
//     	       PWM0_REG(PWM_CMP1) = duty_cycle;
//     	       break;
//     	   case 1:
//     	       PWM0_REG(PWM_CMP2) = duty_cycle;
//      	      break;
//      	  case 2:
//      	      PWM0_REG(PWM_CMP3) = duty_cycle;
//      	      break;
//      	  default:
//      	      break;
//	}
//	PWM0_REG(PWM_CMP1) = 0;
//	PWM0_REG(PWM_CMP2) = 0;
//	PWM0_REG(PWM_CMP3) = 0;
//}


