/* Dairesel gösterge — RPM ve hız için */
export default function Gauge({ label, value, min, max, unit, renk }) {
  const aci_baslangic = 135;   /* sol alt */
  const aci_aralik    = 270;   /* toplam açı */

  const yuzde  = Math.min(Math.max((value - min) / (max - min), 0), 1);
  const aci    = aci_baslangic + yuzde * aci_aralik;

  /* SVG arc hesabı */
  const r      = 80;
  const cx     = 100;
  const cy     = 100;

  function noktaHesapla(derece) {
    const rad = (derece * Math.PI) / 180;
    return {
      x: cx + r * Math.cos(rad),
      y: cy + r * Math.sin(rad),
    };
  }

  function arcPath(baslangic, bitis) {
    const b = noktaHesapla(baslangic);
    const e = noktaHesapla(bitis);
    const buyukArc = bitis - baslangic > 180 ? 1 : 0;
    return `M ${b.x} ${b.y} A ${r} ${r} 0 ${buyukArc} 1 ${e.x} ${e.y}`;
  }

  const ibre = noktaHesapla(aci);

  return (
    <div style={{ textAlign: "center" }}>
      <svg width="200" height="200" viewBox="0 0 200 200">
        {/* arka plan arc */}
        <path
          d={arcPath(aci_baslangic, aci_baslangic + aci_aralik)}
          fill="none"
          stroke="#333"
          strokeWidth="12"
          strokeLinecap="round"
        />
        {/* dolu arc */}
        <path
          d={arcPath(aci_baslangic, aci)}
          fill="none"
          stroke={renk}
          strokeWidth="12"
          strokeLinecap="round"
        />
        {/* ibre */}
        <line
          x1={cx}
          y1={cy}
          x2={ibre.x}
          y2={ibre.y}
          stroke="white"
          strokeWidth="2"
          strokeLinecap="round"
        />
        {/* merkez nokta */}
        <circle cx={cx} cy={cy} r="5" fill={renk} />
        {/* değer */}
        <text x={cx} y={cy + 30} textAnchor="middle" fill="white" fontSize="22" fontWeight="bold">
          {value}
        </text>
        <text x={cx} y={cy + 48} textAnchor="middle" fill="#aaa" fontSize="11">
          {unit}
        </text>
      </svg>
      <div style={{ color: "#aaa", fontSize: "14px", marginTop: "-8px" }}>{label}</div>
    </div>
  );
}
