# STM32 Yangın Alarm Demo — Teslim Raporu

## Projenin amacı

STM32 HAL ve C kullanarak duman/sıcaklık sensör okumalarını değerlendiren, Normal,
Alarm ve Fault durumlarını yöneten bir teknik görüşme demosu hazırlamak. Fiziksel
STM32 kartı olmadığı için aynı alarm mantığının masaüstü gösterimi de eklenmiştir.

## Tamamlananlar

- HAL tabanlı firmware taslağı: ADC1, TIM2, USART2, GPIO ve reset butonu.
- Üç ardışık yüksek geçerli örnekte kilitlenen alarm durum makinesi.
- Hatalı örneklemede Fault durumu; Fault, ardışık yüksek örnek sayacını sıfırlar.
- Alarm önceden kilitlenmişse sensör Fault durumuna geçse de LED ve buzzer çıkışları
  alarm koşulu sıfırlanana kadar etkin kalır.
- Buton için kararlı seviye tabanlı 30 ms debounce; reset yalnızca son örnek geçerli
  ve güvenliyse kabul edilir.
- Firmware ile aynı durum mantığını kullanan masaüstü demo.
- `make demo` ile tek komutta derleme ve senaryo çıktısı.
- Pin/saat varsayımları, kurulum ve sınırlamalar için README ve çevre birimi belgesi.

## Masaüstü demoyu çalıştırma

GCC ve GNU Make kurulu bir bilgisayarda proje kökünde:

```sh
make demo
```

Program, Normal → eşik aşımı → Alarm → Fault → güvenli reset senaryosunu terminalde
gösterir. Derleme çıktısı `build/` klasörüne yazılır; bu klasör Git tarafından yok
sayılır.

## Firmware hedefi

Başlangıç hedefi yalnızca örnek olarak NUCLEO-F401RE / STM32F401RE kabul edilmiştir.
Örnek pin eşlemesi ve saat varsayımları `PERIPHERAL_CONFIG.md` dosyasındadır.
Bu depoda henüz STM32CubeIDE/CubeMX `.ioc` dosyası, HAL/CMSIS paketleri, startup
kodu veya linker betiği yoktur. Masaüstü demonun derlenmesi STM32 firmware'inin hedef
üzerinde derlendiği anlamına gelmez.

## Doğrulama durumu

- Masaüstü demo GCC ile derlenip çalıştırıldı; örnek durum akışı gözlemlendi.
- Bu teslim turundaki debounce ve hata durumu değişiklikleri masaüstü derlemesine
  dahil edildi; STM32 hedefinde derlenmedi veya fiziksel kartta denenmedi.
- Sensör modeli, analog ön uç, eşik kalibrasyonu ve kart pinleri doğrulanmadı.

## GitHub durumu

Kaynaklar bilgisayardaki proje klasöründe hazırlanmıştır. Bu çalışma alanında
yapılandırılmış bir GitHub uzak deposu yoktur; bu nedenle GitHub'a yükleme henüz
yapılmamıştır. Bağlı GitHub hesabında da erişilebilir depo görünmüyor.

## Önemli sınırlamalar

Bu çalışma görüşme demosudur; sertifikalı yangın alarmı değildir. ADC'nin 0..4095
aralığında değer vermesi tek başına sensör bağlantısının sağlam olduğunu kanıtlamaz.
Buzzer uygun sürücü devresiyle bağlanmalı, doğrudan MCU pininden sürülmemelidir.
