import { useState, useEffect } from "react";
import Gauge    from "./Gauge";
import BarGauge from "./BarGauge";

const VARSAYILAN = { rpm: 0, hiz: 0, sicaklik: 20, yakit: 100, abs: false, fren: false };

export default function App() {
  const [veri, setVeri] = useState(VARSAYILAN);

  useEffect(() => {
    const oku = () => {
      fetch("/can_dashboard.json?t=" + Date.now())  /* cache'i atla */
        .then(r => r.json())
        .then(setVeri)
        .catch(() => {});  /* simülatör henüz başlamamışsa sessiz kal */
    };

    oku();
    const interval = setInterval(oku, 100);  /* 100ms'de bir güncelle */
    return () => clearInterval(interval);
  }, []);

  return (
    <div style={{
      minHeight: "100vh",
      background: "#111",
      display: "flex",
      flexDirection: "column",
      alignItems: "center",
      justifyContent: "center",
      fontFamily: "monospace",
      gap: "40px",
    }}>

      {/* başlık */}
      <div style={{ color: "#aaa", fontSize: "13px", letterSpacing: "3px" }}>
        CAN BUS DASHBOARD
      </div>

      {/* ana göstergeler */}
      <div style={{ display: "flex", gap: "40px", alignItems: "center" }}>
        <Gauge label="RPM"  value={veri.rpm} min={0} max={8000} unit="rpm"  renk="#e74c3c" />
        <Gauge label="HIZ"  value={veri.hiz} min={0} max={240}  unit="km/h" renk="#3498db" />
      </div>

      {/* alt göstergeler */}
      <div style={{ display: "flex", gap: "40px", alignItems: "flex-start" }}>
        <BarGauge
          label="Sıcaklık"
          value={veri.sicaklik}
          min={0} max={120}
          unit="°C"
          renk="#e67e22"
          uyariEsigi={100}
        />
        <BarGauge
          label="Yakıt"
          value={veri.yakit}
          min={0} max={100}
          unit="%"
          renk="#2ecc71"
          uyariEsigi={15}
        />
      </div>

      {/* durum ışıkları */}
      <div style={{ display: "flex", gap: "24px" }}>
        <Isik aktif={veri.fren} renk="#e74c3c" label="FREN" />
        <Isik aktif={veri.abs}  renk="#f39c12" label="ABS"  />
      </div>

    </div>
  );
}

function Isik({ aktif, renk, label }) {
  return (
    <div style={{ textAlign: "center" }}>
      <div style={{
        width: "16px",
        height: "16px",
        borderRadius: "50%",
        background: aktif ? renk : "#333",
        margin: "0 auto 4px",
        boxShadow: aktif ? `0 0 10px ${renk}` : "none",
        transition: "all 0.1s",
      }} />
      <div style={{ color: aktif ? renk : "#555", fontSize: "11px" }}>{label}</div>
    </div>
  );
}
