# CAN Bus Simulator

> A CAN Bus protocol simulator written in C with a React dashboard.
> Simulates 8 ECUs, arbitration, error handling, fuzzing, and anomaly detection.

---

> C dilinde yazılmış, gerçek CAN Bus protokolünü simüle eden bir proje.
> 8 ECU, arbitration, hata yönetimi, fuzzing ve anomali tespiti içerir.

---

## Architecture / Mimari

```
┌─────────────────────────────────────────────────────────────┐
│                     Application Layer / Uygulama Katmanı    │
│                                                             │
│  Motor  ABS  Airbag  Klima  Direksiyon  Kabin  Lastik  Dash │
└──────────────────────────┬──────────────────────────────────┘
                           │
┌──────────────────────────▼──────────────────────────────────┐
│                Node Layer / Node Katmanı                    │
│   CANNode: ID, filters, error counter, NORMAL→BUS-OFF       │
└──────────────────────────┬──────────────────────────────────┘
                           │
┌──────────────────────────▼──────────────────────────────────┐
│                 Bus Layer / Bus Katmanı                     │
│   32-slot ring buffer · Arbitration · Broadcast             │
└──────────────────────────┬──────────────────────────────────┘
           ┌───────────────┴───────────────┐
┌──────────▼──────────┐       ┌────────────▼──────────────────┐
│   Frame Layer       │       │   Security Layer              │
│   CANFrame struct   │       │   CANFuzzer: 5 attack types   │
│   Encode / Decode   │       │   CANDetector: 4 rules        │
│   Bit Stuffing      │       │   logs/anomali.log            │
└─────────────────────┘       └───────────────────────────────┘
           │
┌──────────▼──────────────────────────────────────────────────┐
│                      Tools / Araçlar                        │
│  CANLogger → logs/can_traffic.log  (candump format)         │
│  CANParser → read, filter, statistics                       │
│  CANJson   → dashboard/public/can_dashboard.json            │
└──────────┬──────────────────────────────────────────────────┘
           │
┌──────────▼──────────────────────────────────────────────────┐
│                   React Dashboard                           │
│   Polls JSON every 100ms · Gauges · Toast alerts            │
└─────────────────────────────────────────────────────────────┘
```

---

## File Structure / Dosya Yapısı

```
CANBusProject/
├── src/
│   ├── can_frame.h/c       → CAN frame struct, encode/decode, bit stuffing
│   ├── can_bus.h/c         → Virtual bus, ring buffer, arbitration
│   ├── can_node.h/c        → ECU node, error counter, bus-off
│   ├── can_error.h         → Error types and state constants
│   ├── can_ids.h           → Message IDs and value ranges
│   ├── ecu_motor.h/c       → Engine ECU (RPM, temperature, fuel)
│   ├── ecu_abs.h/c         → ABS ECU (wheel speed, brakes)
│   ├── ecu_dashboard.h/c   → Dashboard ECU (listens to all)
│   ├── ecu_airbag.h/c      → Airbag ECU (crash detection)
│   ├── ecu_klima.h/c       → Climate ECU (A/C control)
│   ├── ecu_direksiyon.h/c  → Steering ECU (angle, torque)
│   ├── ecu_kabin.h/c       → Cabin temp ECU (interior/exterior)
│   ├── ecu_lastik.h/c      → Tire pressure ECU (4 wheels, PSI)
│   ├── can_logger.h/c      → Log bus traffic to file
│   ├── can_parser.h/c      → Parse, filter, statistics
│   ├── can_json.h/c        → Write dashboard data to JSON
│   ├── can_fuzzer.h/c      → Invalid frame generator
│   ├── can_detector.h/c    → Anomaly detection rule engine
│   └── main.c              → Entry point
├── dashboard/              → React application
│   └── src/
│       ├── App.jsx         → Main component, JSON polling, toasts
│       ├── Gauge.jsx       → Circular gauge (RPM, speed, steering)
│       └── BarGauge.jsx    → Bar gauge (fuel, temperature, pressure)
├── logs/                   → Runtime log files (gitignored)
├── bin/                    → Compiled binaries (gitignored)
├── Makefile
├── LICENSE
└── README.md
```

---

## Message ID Table / Mesaj ID Tablosu

| ID     | Source / Kaynak | Content / İçerik     | DLC | Range / Aralık  |
|--------|-----------------|----------------------|-----|-----------------|
| 0x100  | Engine ECU      | RPM                  | 2   | 0 – 8000        |
| 0x101  | Engine ECU      | Temperature (°C)     | 1   | 0 – 120         |
| 0x102  | Engine ECU      | Fuel (%)             | 1   | 0 – 100         |
| 0x200  | ABS ECU         | Wheel speeds         | 4   | 0 – 250 km/h    |
| 0x201  | ABS ECU         | Brake / ABS status   | 2   | 0 or 1          |
| 0x300  | Airbag ECU      | Crash / deployed     | 1   | 0 or 1          |
| 0x301  | Climate ECU     | Target temp, A/C     | 2   | 16–30 °C        |
| 0x302  | Steering ECU    | Angle, torque        | 4   | ±540°, 0–100 Nm |
| 0x303  | Cabin ECU       | Interior/exterior °C | 2   | -20 – 50 °C     |
| 0x304  | Tire ECU        | 4-wheel pressure     | 4   | 20 – 40 PSI     |

---

## Build & Run / Derleme ve Çalıştırma

### Requirements / Gereksinimler
- GCC (MinGW on Windows)
- Node.js 18+

### C Simulator

```bash
# Build both targets
make

# Build + run simulator and dashboard
make run

# Clean binaries and logs
make clean
```

### Log Tools / Log Araçları

```bash
bin/can_test --oku              # show all logs
bin/can_test --filtre 100       # filter by ID 0x100
bin/can_test --istatistik       # per-ID statistics
```

### React Dashboard

```bash
cd dashboard
npm install
npm run dev
# Open: http://localhost:5173
```

> The React dashboard must run alongside `bin/can_sim`.

---

## CAN Protocol Features / Uygulanan CAN Protokol Özellikleri

- **Arbitration**: Lowest ID wins. Mirrors real CAN dominant-bit behavior.
- **Broadcast**: Every message reaches all nodes; each node filters by ID.
- **Bit Stuffing**: After 5 identical bits, an opposite bit is inserted and stripped on receive.
- **Error Management**: TEC/REC counters → Error Passive at 128, Bus-Off at 256.
- **Big-Endian ID**: 4-byte ID, MSB first, enabling `memcmp`-based ordering.

---

## Security Module / Güvenlik Modülü

| Attack Type / Saldırı Tipi | Description / Açıklama                        |
|----------------------------|-----------------------------------------------|
| Invalid DLC                | DLC > 8, outside protocol spec                |
| Unknown ID                 | Undefined message ID                          |
| Value overflow             | Known ID but value out of range (RPM=60000)   |
| Flood                      | Too many messages from same ID in short time  |
| Random                     | All fields completely randomized              |

Detector applies 4 rules and writes anomalies to `logs/anomali.log`.
Dashboard shows toast notifications for detected anomalies.

---

## License

MIT — see [LICENSE](LICENSE).
