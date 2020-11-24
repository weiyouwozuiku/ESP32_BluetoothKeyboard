# ESP32_BluetoothKeyboard
---
**这个项目采用ESP32作为主控，利用其内置的蓝牙模块将其模拟成一个蓝牙键盘。使其在采集了as608传来的数据后自动输入相应的密码。**
## 电路连接

OLED：

- VCC->3.3V
- GND->GND
- SCL->接D22
- SDA->D21

AS608:

- VCC->3.3V(5V会烧坏元件)
- GND->GND
- TX->RX2
- RX->TX2

![](https://cdn.jsdelivr.net/gh/weiyouwozuiku/buerlog_img/BlogImage/ESP32%E6%8C%87%E7%BA%B9%E8%93%9D%E7%89%99%E9%94%AE%E7%9B%98%E5%BC%80%E5%8F%91_%E8%BF%9E%E7%BA%BF%E5%9B%BE.png)

详情请见[博客](https://buerlog.top/2020/11/18/esp32-zhi-wen-lan-ya-jian-pan-kai-fa)。
