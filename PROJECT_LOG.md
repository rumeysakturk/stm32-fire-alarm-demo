# Proje çalışma günlüğü

Bu dosya yöneticiye yönelik kısa ilerleme kaydıdır. Yeni kararlar ve değişiklikler
tarihli olarak en alta eklenecek.

## 2026-11-27 — Başlangıç taslağı

- Devir raporundaki dosyalar çalışma klasöründe yoktu; kaynak iskeleti bu klasörde
  yeniden kuruldu.
- Başlangıç varsayımları korundu: STM32F401RE, HSI 16 MHz; PA0/PA1 ADC, TIM2 1 Hz,
  USART2 115200, PA5 LED, PB0 buzzer sürücü ve PC13 aktif-yüksek buton.
- Uygulama modülü, HAL başlangıç kodu, kesme dosyası, çevre birimi özeti ve README
  eklendi. Varsayılan simülasyon normal, yüksek duman ve geçersiz örnek üretir.
- İlk kaynak gözden geçirmesinde ADC taramasının iki rank'ını tek dönüşüm dizisinde
  okumak ve ADC/UART/TIM2 saat, pin ve NVIC MSP kurulumlarını eklemek gerekti; düzeltildi.
- Durum makinesi üç ardışık geçerli eşik aşımında alarmı kilitler. Geçersiz örnekte
  Fault görünür; kilitli alarm durumu sensörler düzelene kadar bellekte tutulur.
  Reset yalnızca geçerli ve tehlikesiz örnekten sonra kabul edilir.
- Gerçek hedef derlemesi yapılmadı; bu klasörde HAL/CMSIS/startup/linker ve `.ioc`
  yok. Bu nedenle proje henüz CubeIDE'de tek başına derlenebilir değil.

## 2026-11-27 — GitHub ve donanımsız gösterim

- Kullanıcının fiziksel kartı olmadığı, projenin iş görüşmesi öncesi deneme projesi
  olduğu netleşti. NUCLEO-F401RE yalnızca örnek hedef olarak dokümante edildi.
- GitHub deposunda durum makinesini kart olmadan gösterebilmek için CMake tabanlı
  masaüstü demo eklendi. Bu demo, firmware ile aynı `fire_alarm.c` dosyasını kullanır;
  terminalde normal, alarm, fault ve güvenli reset akışını gösterir.
- Bu adım gerçek STM32 firmware derlemesinin yerini tutmaz; hedefe özel CubeIDE
  projesi ve HAL paketleri hâlâ ayrıca gerekir.

## 2026-11-27 — Teslim hazırlığı

- Demoyu CMake olmadan çalıştırmak için `make demo` hedefi eklendi; CMake yolu da
  README'de alternatif olarak bırakıldı.
- Kaynak incelemesinde sensör Fault olduğunda ardışık yüksek örnek sayacının
  sıfırlanması ve alarm önceden kilitliyse çıkışların etkin kalması sağlandı.
- Buton debounce mantığı, basma seviyesinin en az 30 ms kararlı kalmasını bekleyecek
  şekilde düzenlendi.
- Yönetici için `TESLIM_RAPORU.md` oluşturuldu. Depoda GitHub remote'u olmadığından
  kodlar yerelde kaldı; hedef repo belirlenince upload yapılabilir.
- GNU Make denemesinde `build` adlı hedef ile çıktı klasörü adının çakıştığı fark
  edildi ve düzeltildi. GitHub bağlantısı kontrol edildi; listelenebilir depo yok.

### Yönetici kararına / donanım doğrulamasına açık noktalar

1. Kart gerçekten NUCLEO-F401RE mi? Farklıysa pin ve saat ayarları değişmeli.
2. Duman ve sıcaklık sensörlerinin modeli, analog çıkış aralığı ve eşik dönüşümü nedir?
3. PC13 butonunun basılma polaritesi ve pull-up/pull-down bağlantısı kartta doğrulanmalı.
4. Buzzer sürücüsü ve besleme devresi belirlenmeli; MCU pini yükü doğrudan sürmemeli.
5. Gerçek sensör arıza teşhisi ve güvenli durumda reset davranışı donanım gereksinimiyle teyit edilmeli.
