/*
	測試程式碼:
*/
//#include "../../libraries/wiring_digital.h"
#include "wiring_digital.h"  // 這裡定義了 digitalRead, digitalWrite, pinMode
#include "wiring_analog.h"
#include <stdint.h>
#include "time_utils.h"  // 包含 millis() 函式


// #include "serial.h"  //如果你只是要使用底層的 UART 函式
#include "T-core_serial.hpp"  // 含有 class SerialHardware
#include "serial.h"  // 包含 TcoreSerial 的定義

#include "ros.h" //rosserial_client 生成的 ros_lib 沒有這個函式
#include <std_msgs/String.h>


extern TcoreSerial Serial; // 如果你使用 TcoreSerial，這行可以放在檔案最上方

ros::NodeHandle_<SerialHardware> nh; // 使用 SerialHardware 作為硬體介面

std_msgs::String str_msg;

ros::Publisher chatter("chatter_topic",&str_msg);

// 指定 ros::NodeHandle 使用 SerialHardware
/*非rosserial_arduino版本的ros_lib，不須自行定義命名空間 */
// typedef ros::NodeHandle_<SerialHardware, 25, 25, 512, 2048> NodeHandle;
// ros::NodeHandle_<SerialHardware, 25, 25, 512, 2048> NodeHandle;
// NodeHandle nh;
// ros::Publisher chatter("chatter", &str_msg);//20250718
// nh.advertise(chatter); //20250718
// ros::Publisher<std_msgs::String> chatter("chatter", &str_msg); //20250729

void delay(int ms) //（僅限測試用途，請視時脈微調）
{
    volatile int count = ms * 100000;
    while(count--);
}

// static void uart_print(const char *s)
// {
// 	int i=0;
	
// 	while (*s != '\0') // the string is end of '\0'
// 	{
// 		uart_tx(*s++);
// 	}
// }

int main(void)
{
    // Serial.begin(115200); // 初始化序列埠，設定鮑率為 115200
    // delay(10);  // 穩定硬體，等待序列埠穩定
    // nh.getHardware()->setPort(&TcoreSerial); //設定 UART 的埠（port）
    // nh.getHardware()->setPort(&Serial); //設定 UART 的埠（port）
    // nh.getHardware()->setBaud(115200); //設定 UART 鮑率（baud rate）

    /*初始化 ROS 節點，開始 rosserial 的交握 (handshake) 過程*/
    nh.initNode(); // 由ros::NodeHandle_<Hardware>使用，此函數內部會呼叫 SerialHardware::init()

    /*宣告 (Advertise) 您要發布的話題，讓 NodeHandle 知道這個 'chatter' Publisher 的存在*/
    nh.advertise(chatter); // 宣告一個 Publisher，並將其與 NodeHandle 綁定
    // nh.advertise(chatter);
    // nh.advertise(&chatter); // 修改，advertise() 修改成接受 PublisherBase& 或 PublisherBase* //20250729
    // nh.advertise(chatter); //20250718

    char hello[] = "hello world";  // 非 const 可修改
    str_msg.data = hello;


    while (1) {
        /*發布訊息*/
        chatter.publish(&str_msg);
        // for (int i = 0; i < 100; i++) nh.spinOnce(); // 等待主機發送同步封包
        /*處理ROS通訊事件，這裡處理序列阜收發，並觸發訂閱者的回應呼叫函式*/
        nh.spinOnce();
        // uart_print();
        delay(10);  // 根據你的時脈調整數值
    }

    // while (1) { // 先測試發佈訊息
    // nh.spinOnce();  // 不 publish
    // delay(10);
    // }
}



// int main(void)
// {
//     nh.initNode();
//     nh.advertise(chatter);

//     str_msg.data = hello;
//     chatter.publish(&str_msg);
//     nh.spinOnce();
//     delay(1000);  // 用軟體延遲或替代機制
// }

// void setup() {
//   nh.initNode();
//   nh.advertise(chatter);
// }

// void loop() {
//   str_msg.data = "hello world";
//   chatter.publish(&str_msg);
//   nh.spinOnce();
//   delay(1000);  // 用軟體延遲或替代機制
// }

