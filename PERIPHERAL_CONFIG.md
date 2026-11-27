# Çevre birimi ayar özeti

Başlangıç varsayımı NUCLEO-F401RE / STM32F401RE, HSI 16 MHz ve STM32 HAL'dir.
Bu klasör CubeIDE tarafından üretilmiş bir proje değildir; `.ioc`, HAL/CMSIS,
startup dosyası ve linker betiği ayrıca gerekir.

| İşlev | STM32 kaynağı | Ayar |
|---|---|---|
| Duman analog giriş | PA0 / ADC1_IN0 | 12 bit, rank 1, 84 çevrim örnekleme |
| Sıcaklık analog giriş | PA1 / ADC1_IN1 | 12 bit, rank 2, 84 çevrim örnekleme |
| Ölçüm isteği | TIM2 | PSC 15999, ARR 999; 1 Hz, 16 MHz timer clock varsayımı |
| UART | USART2, PA2 TX / PA3 RX | 115200 baud, 8N1 |
| LED | PA5 | Push-pull çıkış |
| Buzzer sürücüsü | PB0 | Push-pull çıkış; buzzer'ı doğrudan pine bağlamayın |
| Reset butonu | PC13 | Aktif yüksek ve pull-down varsayımı; kart devresiyle doğrulayın |

USART2 için PA2/PA3 alternatif fonksiyonu ve TIM2 kesme/NVIC etkinliği CubeMX'te
açılmalıdır. ADC girişi GPIO'ları analog modda, dijital pull olmadan ayarlanır.

Eşikler 12 bit ham ADC sayımıdır: duman 2600, sıcaklık 2400. Bunlar örnek değerlerdir;
sensör modeli, analog ön uç ve kalibrasyon belirlenmeden gerçek eşik sayılmaz.
