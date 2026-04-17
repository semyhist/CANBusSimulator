# CAN Bus Simulator

C dilinde yazılmış, gerçek CAN Bus protokolünü simüle eden bir proje.
Hafta hafta inşa edildi — temel frame yapısından güvenlik modülüne kadar.

---

## Mimari

```
┌─────────────────────────────────────────────────────────────┐
│                        Uygulama Katmanı                     │
│                                                             │
│   ┌─────────────┐  ┌─────────────┐  ┌──────────────────┐   │
│   │  Motor ECU  │  │   ABS ECU   │  │  Dashboard ECU   │   │
│   │             │  │             │  │                  │   │
│   │ RPM         │  │ Tekerlek hz │  │ Hepsini dinler   │   │
│   │ Sıcaklık    │  │ Fren durumu │  │ JSON'a yazar     │   │
│   │ Yakıt       │  │ ABS aktif   │  │                  │   │
│   └──────┬──────┘  └──────┬──────┘  └────────┬─────────┘   │
│          │                │                  │             │
└──────────┼────────────────┼──────────────────┼─────────────┘
           │                │                  │
┌──────────▼────────────────▼──────────────────▼─────────────┐
│                        Node Katmanı                         │
│                                                             │
│   ┌──────────────────────────────────────────────────────┐  │
│   │  CANNode: ID, filtreler, hata sayacı, durum          │  │
│   │  NORMAL → ERROR PASSIVE → BUS-OFF                    │  │
│   └──────────────────────────┬───────────────────────────┘  │
│                              │                              │
└──────────────────────────────┼──────────────────────────────┘
                               │
┌──────────────────────────────▼──────────────────────────────┐
│                        Bus Katmanı                          │
│                                                             │
│   ┌──────────────────────────────────────────────────────┐  │
│   │  CANBus: 32 slotluk ring buffer                      │  │
│   │  Arbitration: en küçük ID önce geçer                 │  │
│   │  Broadcast: mesaj herkese gider                      │  │
│   └──────────────────────────────────────────────────────┘  │
│                                                             │
└─────────────────────────────────────────────────────────────┘
           │                              │
┌──────────▼──────────┐       ┌───────────▼───────────────────┐
│   Frame Katmanı     │       │   Güvenlik Katmanı            │
│                     │       │                               │
│ CANFrame struct     │       │ CANFuzzer: 5 saldırı tipi     │
│ Encode / Decode     │       │ CANDetector: 4 kural          │
│ Bit Stuffing        │       │ anomali.log                   │
└─────────────────────┘       └───────────────────────────────┘
           │
┌──────────▼──────────────────────────────────────────────────┐
│                     Araçlar                                 │
│                                                             │
│  CANLogger  → can_traffic.log  (candump formatı)            │
│  CANParser  → log okuma, filtreleme, istatistik             │
│  CANJson    → can_dashboard.json (React için)               │
└─────────────────────────────────────────────────────────────┘
           │
┌──────────▼──────────────────────────────────────────────────┐
│                   React Dashboard                           │
│                                                             │
│  Her 100ms'de JSON okur → Gauge, BarGauge, durum ışıkları  │
└─────────────────────────────────────────────────────────────┘
```

---

## Dosya Yapısı

```
CANBusProject/
│
├── can_frame.h/c       → CAN frame struct, encode/decode, bit stuffing
├── can_bus.h/c         → Sanal bus, ring buffer, arbitration
├── can_node.h/c        → ECU node, hata sayacı, bus-off
├── can_error.h         → Hata tipleri ve durum sabitleri
├── can_ids.h           → Bilinen mesaj ID'leri ve değer aralıkları
│
├── ecu_motor.h/c       → Motor ECU (RPM, sıcaklık, yakıt)
├── ecu_abs.h/c         → ABS ECU (tekerlek hızı, fren)
├── ecu_dashboard.h/c   → Dashboard ECU (hepsini dinler)
│
├── can_logger.h/c      → Bus trafiğini dosyaya logla
├── can_parser.h/c      → Log dosyasını parse et, filtrele, istatistik
├── can_json.h/c        → Dashboard verisini JSON'a yaz
│
├── can_fuzzer.h/c      → Geçersiz frame üretici
├── can_detector.h/c    → Anomali tespit kural motoru
│
├── main.c              → Giriş noktası
│
└── dashboard/          → React uygulaması
    └── src/
        ├── App.jsx     → Ana component, JSON polling
        ├── Gauge.jsx   → Dairesel gösterge (RPM, hız)
        └── BarGauge.jsx→ Çubuk gösterge (yakıt, sıcaklık)
```

---

## Mesaj ID Tablosu

| ID     | Kaynak    | İçerik              | DLC | Değer Aralığı     |
|--------|-----------|---------------------|-----|-------------------|
| 0x100  | Motor ECU | RPM                 | 2   | 0 – 8000          |
| 0x101  | Motor ECU | Sıcaklık (°C)       | 1   | 0 – 120           |
| 0x102  | Motor ECU | Yakıt (%)           | 1   | 0 – 100           |
| 0x200  | ABS ECU   | Tekerlek hızları    | 4   | 0 – 250 km/h      |
| 0x201  | ABS ECU   | Fren / ABS durumu   | 2   | 0 veya 1          |

---

## Haftalık İlerleme

| Hafta | Konu                        | Dosyalar                              |
|-------|-----------------------------|---------------------------------------|
| 1     | CAN Frame Temeli            | can_frame.h/c                         |
| 2     | Bus ve Node Sistemi         | can_bus.h/c, can_node.h/c             |
| 3     | ECU Simülasyonları          | ecu_motor, ecu_abs, ecu_dashboard     |
| 4     | Arbitration ve Hata Yönetimi| can_error.h, bit stuffing             |
| 5     | Loglama ve Parser           | can_logger, can_parser                |
| 6     | React Dashboard             | dashboard/                            |
| 7     | Fuzzing ve Güvenlik         | can_fuzzer, can_detector              |
| 8     | Dokümantasyon               | README.md                             |

---

## Derleme ve Çalıştırma

### Gereksinimler
- GCC (MinGW Windows'ta)
- Node.js 18+ (React dashboard için)

### C Simülatörü

```bash
# Fuzzing testi
gcc -o can_test main.c can_frame.c can_bus.c can_node.c \
    can_logger.c can_parser.c can_fuzzer.c can_detector.c \
    ecu_motor.c ecu_abs.c ecu_dashboard.c
./can_test

# Canlı dashboard simülatörü
gcc -o can_sim main.c can_frame.c can_bus.c can_node.c \
    can_json.c ecu_motor.c ecu_abs.c ecu_dashboard.c
./can_sim
```

### Log Araçları (candump benzeri)

```bash
./can_test --oku                  # tüm log'u göster
./can_test --filtre 100           # sadece 0x100 ID'li mesajlar
./can_test --istatistik           # ID bazlı istatistik
```

### React Dashboard

```bash
cd dashboard
npm install
npm run dev
# Tarayıcıda: http://localhost:5173
```

> React dashboard çalışırken `can_sim` de çalışıyor olmalı.

---

## Uygulanan CAN Protokol Özellikleri

- **Arbitration**: Aynı anda birden fazla node mesaj göndermek isterse, ID'si küçük olan kazanır. Gerçek CAN'da dominant bit (0) recessive bit'i (1) ezer.
- **Broadcast**: Mesaj tüm node'lara gider, her node kendi filtresine göre okur.
- **Bit Stuffing**: 5 aynı bit üst üste gelirse araya zıt bit eklenir, alıcı bunu çıkarır.
- **Hata Yönetimi**: Hata sayacı 128'i geçince Error Passive, 256'yı geçince Bus-Off. Bus-Off'ta node susturulur.
- **Big-Endian ID**: ID 4 byte'a büyük byte önce yazılır, `memcmp` ile sıralama korunur.

---

## Güvenlik Modülü

Fuzzer 5 farklı saldırı tipi üretir:

| Tip               | Açıklama                                      |
|-------------------|-----------------------------------------------|
| Geçersiz DLC      | DLC > 8, frame boyutu protokol dışı           |
| Bilinmeyen ID     | Tanımsız mesaj ID'si                          |
| Değer taşma       | Bilinen ID ama değer aralık dışı (RPM=60000)  |
| Flood             | Aynı ID'den kısa sürede çok fazla mesaj       |
| Rastgele          | Her alanı tamamen rastgele                    |

Detector 4 kural ile anomali yakalar ve `anomali.log`'a yazar.
