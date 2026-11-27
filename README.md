# STM32 Yangın Alarm Demo

STM32 HAL ve C ile analog duman/sıcaklık örneklerini izleyip Normal, Alarm ve Fault
durumlarını gösteren teknik değerlendirme demosu. Fiziksel kart olmadan alarm mantığını
masaüstünde göstermek için ayrıca bir simülatör hedefi bulunur. Sertifikalı bir yangın
güvenlik ürünü değildir.

## Davranış

- TIM2 kesmesi saniyede bir ölçüm bayrağı kurar; ADC/UART işlemleri ana döngüde yürür.
- Her iki sensör de eşik altındaysa mevcut tehlike yoktur. Herhangi bir eşik üç
  ardışık geçerli örnekte aşılırsa alarm kilitlenir.
- Geçersiz ADC okuması FAULT gösterir ve ardışık örnek sayacını sıfırlar. Önceden
  kilitlenen alarm Fault sırasında da çıkışları açık tutar.
- PC13 reseti yalnızca son okuma geçerliyse ve tehlike koşulu bitmişse alarmı siler.
- Alarm durumunda PA5 LED ve PB0 buzzer sürücü çıkışı etkinleşir.
- USART2 her ölçümde ham değerleri ve durumu bildirir.

## Simülasyon

Masaüstü simülatörü, firmware ile aynı `fire_alarm.c` durum makinesini derler ve
normal, eşik aşımı, kilitli alarm, Fault ve güvenli reset akışını terminalde gösterir.
GCC ve GNU Make ile tek komutta derleyip çalıştırın:

```sh
make demo
```

CMake alternatifi repo kökünde `cmake -S . -B build && cmake --build build`
komutlarıyla kullanılabilir.

STM32 firmware tarafındaki `FIRE_ALARM_SIMULATION` varsayılan olarak `1`'dir; cihaz
üzerinde simülasyon örnekleri üretir. Gerçek ADC okumaları için
`Core/Inc/fire_alarm.h` içindeki makroyu `0` yapın.

## Proje varsayımları ve kurulum

Örnek firmware hedefi NUCLEO-F401RE / STM32F401RE, HSI 16 MHz ve
`PERIPHERAL_CONFIG.md` içindeki pin eşlemesidir; bu kartın mevcut olduğu varsayılmaz.
`host_demo` fiziksel kart gerektirmez. STM32 firmware kaynakları tek başına CubeIDE
proje kabuğunun yerine geçmez. Firmware'i hedefte derlemek için CubeMX ile proje
oluşturun; HAL, CMSIS, startup ve linker dosyalarını ekleyin ve ayarları özet
dosyasıyla eşleştirin. Detaylar ve açık kararlar için `PROJECT_LOG.md`'ye bakın.

## Depo düzeni

- `Core/`: STM32 HAL firmware uygulaması.
- `host_demo/`: aynı alarm durum makinesini gösteren masaüstü demo ve GPIO shim'i.
- `PERIPHERAL_CONFIG.md`: örnek pin ve saat ayarları.
- `PROJECT_LOG.md`: ilerleme ve çözülmemiş varsayımlar.
- `TESLIM_RAPORU.md`: teslim kapsamı, çalıştırma ve doğrulama durumu.

Sensör kopukluğu yalnızca 0..4095 ADC aralığıyla her devrede teşhis edilemez.
Gerçek sensör modeli/analog devre belirlenince açık-devre teşhisi ve eşik kalibrasyonu
yeniden tasarlanmalıdır. Buzzer için uygun transistör/sürücü ve koruma elemanları
kullanın.
