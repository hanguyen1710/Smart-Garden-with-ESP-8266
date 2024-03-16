# Smart-Garden-with-ESP-8266

**1.** **Tổng quan dự án** 
- Smart Garden được hình dung với 3 mô hình hệ thống nhỏ cấu thành:
*1.1. Hệ thống tưới tiêu tự động dựa trên độ ẩm đất*
*1.2. Hệ thống theo dõi, giám sát nhiệt độ độ ẩm trong vườn*
*1.3. Hệ thống điều chỉnh cường độ ánh sáng*


- Mỗi hệ thống đều được điều khiển ở 2 chế độ: **tự động** và **thủ công**
- Mô hình hệ thống:
*+ Thiết bị: Các cảm biến sẽ thu thập dữ liệu từ môi trường: cảm biến độ ẩm đất, cảm biến nhiệt độ độ ẩm, cảm biến ánh sáng*
*+ Vi điều khiển: ESP8266 nhận dữ liệu từ cảm biến thông qua các giao tiếp như SPI, I2C để xử lý và đưa ra các quyết định*
*+ Server sử dụng Blynk.cloud có nhiệm vụ là lưu trữ dữ liệu từ vi điều khiển thông qua giao thức MQTT*
*+ Ứng dụng: Điện thoại điều khiển thông qua các App (App sử dụng là Blynk.iot) với dữ liệu được lấy từ server Blynk.cloud*

**2. Chi tiết**