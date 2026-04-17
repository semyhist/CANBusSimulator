/* Çubuk gösterge — yakıt ve sıcaklık için */
export default function BarGauge({ label, value, min, max, unit, renk, uyariEsigi }) {
  const yuzde  = Math.min(Math.max((value - min) / (max - min), 0), 1) * 100;
  const aktifRenk = uyariEsigi && value >= uyariEsigi ? "#e74c3c" : renk;

  return (
    <div style={{ width: "140px" }}>
      <div style={{ color: "#aaa", fontSize: "13px", marginBottom: "6px" }}>{label}</div>
      <div style={{
        background: "#333",
        borderRadius: "6px",
        height: "12px",
        overflow: "hidden",
      }}>
        <div style={{
          width: `${yuzde}%`,
          height: "100%",
          background: aktifRenk,
          borderRadius: "6px",
          transition: "width 0.2s ease",
        }} />
      </div>
      <div style={{ color: "white", fontSize: "16px", fontWeight: "bold", marginTop: "4px" }}>
        {value} <span style={{ color: "#aaa", fontSize: "12px" }}>{unit}</span>
      </div>
    </div>
  );
}
