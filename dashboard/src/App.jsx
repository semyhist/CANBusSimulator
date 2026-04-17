import { useState, useEffect, useRef } from "react";
import Gauge    from "./Gauge";
import BarGauge from "./BarGauge";

const VARSAYILAN = {
  rpm: 0, hiz: 0, sicaklik: 20, yakit: 100,
  abs: false, fren: false,
  airbag_aktif: false,
  klima_hedef: 22, klima_kompressor: false,
  direksiyon_aci: 0,
  ic_sicaklik: 22, dis_sicaklik: 15,
  lastik: [32, 32, 32, 32],
};

export default function App() {
  const [veri,     setVeri]     = useState(null);
  const [toastlar, setToastlar] = useState([]);
  const aktifToastlar           = useRef({});  /* anahtar → id */

  function toast(mesaj, tip = "warn") {
    const id = Date.now() + Math.random();
    setToastlar(t => [...t, { id, mesaj, tip }]);
    setTimeout(() => setToastlar(t => t.filter(x => x.id !== id)), 3000);
    return id;
  }

  function anomaliToast(anahtar, mesaj) {
    if (aktifToastlar.current[anahtar]) {
      /* zaten ekranda, sayacı artır */
      setToastlar(t => t.map(x =>
        x.id === aktifToastlar.current[anahtar]
          ? { ...x, mesaj: mesaj + (x.sayac ? ` (${x.sayac + 1}x)` : " (2x)"), sayac: (x.sayac || 1) + 1 }
          : x
      ));
    } else {
      const id = toast(mesaj);
      aktifToastlar.current[anahtar] = id;
      setTimeout(() => { delete aktifToastlar.current[anahtar]; }, 3000);
    }
  }

  function anomaliKontrol(d) {
    if (d.airbag_aktif)                         anomaliToast("airbag",  "⚠ Airbag deployed");
    if (d.abs)                                  anomaliToast("abs",     "⚠ ABS active");
    if (d.sicaklik >= 100)                      anomaliToast("sicaklik","⚠ Engine overheating");
    if (d.yakit <= 15)                          anomaliToast("yakit",   "⚠ Low fuel");
    if (d.lastik && d.lastik.some(p => p < 28)) anomaliToast("lastik",  "⚠ Low tire pressure");
  }

  useEffect(() => {
    let bagliMiydi = false;

    const oku = () => {
      fetch("/can_dashboard.json?t=" + Date.now())
        .then(r => r.json())
        .then(d => {
          if (!bagliMiydi) {
            bagliMiydi = true;
            toast("Simulator connected", "ok");
          }
          anomaliKontrol(d);
          setVeri(d);
        })
        .catch(() => {
          if (bagliMiydi) {
            bagliMiydi = false;
            toast("Simulator disconnected", "err");
          }
          setVeri(null);
        });
    };

    oku();
    const iv = setInterval(oku, 100);
    return () => clearInterval(iv);
  }, []);

  return (
    <div style={stil.kapsayici}>
      <div style={stil.baslik}>CAN BUS SIMULATOR</div>

      {!veri ? (
        <div style={stil.bekle}>Waiting for simulator...</div>
      ) : (
        <>
          {/* Motor */}
          <Bolum baslik="ENGINE">
            <Gauge label="RPM"  value={veri.rpm}      min={0} max={8000} unit="rpm"  renk="#e74c3c" />
            <Gauge label="SPEED" value={veri.hiz}     min={0} max={240}  unit="km/h" renk="#3498db" />
            <BarGauge label="Temperature" value={veri.sicaklik} min={0} max={120} unit="°C" renk="#e67e22" uyariEsigi={100} />
            <BarGauge label="Fuel"        value={veri.yakit}    min={0} max={100} unit="%" renk="#2ecc71" uyariEsigi={15} />
          </Bolum>

          {/* ABS */}
          <Bolum baslik="ABS / BRAKES">
            <Isik aktif={veri.fren} renk="#e74c3c" label="BRAKE" />
            <Isik aktif={veri.abs}  renk="#f39c12" label="ABS"   />
          </Bolum>

          {/* Airbag */}
          <Bolum baslik="AIRBAG">
            <Isik aktif={veri.airbag_aktif} renk="#e74c3c" label="DEPLOYED" />
          </Bolum>

          {/* Klima */}
          <Bolum baslik="CLIMATE">
            <BarGauge label="Target Temp" value={veri.klima_hedef}  min={16} max={30} unit="°C" renk="#3498db" />
            <BarGauge label="Cabin Temp"  value={veri.ic_sicaklik}  min={-10} max={50} unit="°C" renk="#9b59b6" />
            <BarGauge label="Outside"     value={veri.dis_sicaklik} min={-20} max={50} unit="°C" renk="#7f8c8d" />
            <Isik aktif={veri.klima_kompressor} renk="#3498db" label="A/C" />
          </Bolum>

          {/* Direksiyon */}
          <Bolum baslik="STEERING">
            <Gauge label="ANGLE" value={veri.direksiyon_aci} min={-540} max={540} unit="°" renk="#1abc9c" />
          </Bolum>

          {/* Lastik */}
          <Bolum baslik="TIRE PRESSURE">
            {["FL","FR","RL","RR"].map((k, i) => (
              <BarGauge
                key={k}
                label={k}
                value={veri.lastik?.[i] ?? 32}
                min={20} max={40}
                unit="PSI"
                renk="#2ecc71"
                uyariEsigi={28}
                uyariAlti
              />
            ))}
          </Bolum>
        </>
      )}

      {/* Toast bildirimleri */}
      <div style={stil.toastKapsayici}>
        {toastlar.map(t => (
          <div key={t.id} style={{ ...stil.toast, ...stilToast(t.tip) }}>
            {t.mesaj}
          </div>
        ))}
      </div>
    </div>
  );
}

function Bolum({ baslik, children }) {
  return (
    <div style={stil.bolum}>
      <div style={stil.bolumBaslik}>{baslik}</div>
      <div style={stil.bolumIcerik}>{children}</div>
    </div>
  );
}

function Isik({ aktif, renk, label }) {
  return (
    <div style={{ textAlign: "center" }}>
      <div style={{
        width: "10px", height: "10px", borderRadius: "50%",
        background: aktif ? renk : "#2a2a2a",
        margin: "0 auto 4px",
      }} />
      <div style={{ color: aktif ? "#aaa" : "#333", fontSize: "10px" }}>{label}</div>
    </div>
  );
}

const stil = {
  kapsayici: {
    minHeight: "100vh",
    background: "#0e0e0e",
    display: "flex",
    flexDirection: "column",
    alignItems: "center",
    padding: "32px 16px",
    fontFamily: "monospace",
    gap: "24px",
    color: "#ccc",
  },
  baslik: {
    fontSize: "11px",
    letterSpacing: "4px",
    color: "#555",
  },
  bekle: {
    marginTop: "80px",
    color: "#444",
    fontSize: "13px",
    letterSpacing: "2px",
  },
  bolum: {
    width: "100%",
    maxWidth: "720px",
    borderTop: "1px solid #1e1e1e",
    paddingTop: "16px",
  },
  bolumBaslik: {
    fontSize: "10px",
    letterSpacing: "3px",
    color: "#444",
    marginBottom: "12px",
  },
  bolumIcerik: {
    display: "flex",
    flexWrap: "wrap",
    gap: "24px",
    alignItems: "center",
  },
  toastKapsayici: {
    position: "fixed",
    bottom: "24px",
    right: "24px",
    display: "flex",
    flexDirection: "column",
    gap: "8px",
    zIndex: 999,
  },
  toast: {
    padding: "10px 16px",
    borderRadius: "4px",
    fontSize: "12px",
    letterSpacing: "1px",
    animation: "fadeIn 0.2s ease",
  },
};

function stilToast(tip) {
  if (tip === "ok")  return { background: "#1a1a1a", color: "#888",    border: "1px solid #2a2a2a" };
  if (tip === "err") return { background: "#1a1a1a", color: "#888",    border: "1px solid #2a2a2a" };
  return                     { background: "#1a1a1a", color: "#888",    border: "1px solid #2a2a2a" };
}
