# Proje çalışma günlüğü

Bu dosya proje ilerlemesini ve teslim durumunu yönetici için özetler. Son güncelleme: 27 Kasım 2026.

## Proje kapsamı ve varsayımlar

- Proje, iş görüşmesi öncesi teknik değerlendirme için hazırlanmış bir STM32 HAL/C yangın alarm demosudur. Kullanıcının fiziksel STM32 kartı yoktur.
- Firmware taslağında örnek hedef olarak NUCLEO-F401RE / STM32F401RE, HSI 16 MHz, PA0/PA1 ADC, TIM2 1 Hz, USART2 115200, PA5 LED, PB0 buzzer sürücü çıkışı ve PC13 buton varsayılmıştır. Kart/sensör doğrulaması yapılmadı.
- ADC eşikleri örnek ham 12 bit sayımlarıdır; gerçek sensör kalibrasyonu değildir.

## Tamamlanan işler

- HAL tabanlı firmware kaynak taslağı oluşturuldu: ADC, GPIO, TIM2, USART2, durum makinesi ve kesme işleyicileri.
- Alarm, üç ardışık geçerli yüksek ölçümden sonra kilitlenir. Geçersiz örnek Fault üretir ve ardışık aşım sayacını sıfırlar. Alarm önceden kilitliyse sensör Fault durumuna geçse bile uyarı çıkışları etkin kalır.
- Reset butonu 30 ms debounce edilir; alarm yalnızca ölçümler geçerli ve tehlike koşulu sona ermişse temizlenir.
- Kart olmadan çalıştırılabilmesi için firmware ile aynı durum makinesini kullanan masaüstü HAL simülasyonu eklendi. `make demo` komutu GCC ve GNU Make ile derleyip Normal, Alarm, Fault ve güvenli reset akışını gösterir.
- README, çevre birimi varsayımları ve sınırlamalar belgesi hazırlandı.

## Doğrulama ve teslim

- Masaüstü demo bu ortamda GCC ile başarıyla derlendi ve senaryo çıktısı gözlemlendi. CMake bu ortamda mevcut olmadığından doğrulama `make demo` ile yapıldı.
- GitHub deposu oluşturuldu ve proje kaynakları/belgeleri `main` dalına yüklendi: https://github.com/rumeysakturk/stm32-fire-alarm-demo
- Depo herkese açıktır. README, `Core`, `host_demo` ve proje belgeleri Firefox üzerinden GitHub'da kontrol edildi.
- Bu yükleme gerçek STM32 hedef derlemesi anlamına gelmez. `.ioc`, HAL/CMSIS paketleri, startup kodu ve linker betiği depoda yoktur; STM32CubeIDE derlemesi ve kart testi yapılmadı.

## Açık teknik noktalar

1. Gerçek kart ve sensör modelleri belli olduğunda pin eşlemesi, analog ön uç ve eşikler doğrulanmalı.
2. Sensör kopukluğu yalnızca ADC'nin 0–4095 aralığına bakılarak her devrede belirlenemez; devreye uygun hata teşhisi tanımlanmalı.
3. Buton polaritesi ve pull-up/pull-down bağlantısı kart üzerinde teyit edilmeli.
4. Buzzer uygun sürücü devresiyle bağlanmalı; MCU pini buzzer yükünü doğrudan sürmemeli.
5. Bu demo sertifikalı bir yangın güvenlik ürünü değildir.
