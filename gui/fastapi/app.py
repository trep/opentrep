"""
OpenTREP travel search - FastAPI backend
"""
import json, math, logging
from contextlib import asynccontextmanager
from fastapi import FastAPI, Query
from fastapi.staticfiles import StaticFiles
from fastapi.responses import HTMLResponse, FileResponse

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

POR_PATH    = "/usr/share/opentrep/data/por/optd_por_public_fixed3.csv"
XAPIAN_DIR  = "/var/www/webapps/opentrep/trep/traveldb"
SQLITE_PATH = "/var/www/webapps/opentrep/trep/sqlite_travel.db"
LOG_PATH    = "/var/log/webapps/search/pyopentrep.log"

_trep = None

@asynccontextmanager
async def lifespan(app: FastAPI):
    global _trep
    from pyopentrep.pyopentrep import OpenTrepSearcher
    _trep = OpenTrepSearcher()
    ok = _trep.init(POR_PATH, XAPIAN_DIR, "sqlite", SQLITE_PATH,
                    0, False, True, True, LOG_PATH)
    if not ok:
        raise RuntimeError("OpenTREP init failed")
    logger.info("OpenTREP initialised")
    yield
    _trep.finalize()
    logger.info("OpenTREP finalised")

app = FastAPI(title="Travel Search", lifespan=lifespan)

def _great_circle_km(lat1, lon1, lat2, lon2):
    r = math.pi / 180.0
    lat1, lon1, lat2, lon2 = lat1*r, lon1*r, lat2*r, lon2*r
    dlat = (lat2 - lat1) / 2.0
    dlon = (lon2 - lon1) / 2.0
    a = math.sin(dlat)**2 + math.cos(lat1)*math.cos(lat2)*math.sin(dlon)**2
    return 12742.0 * math.asin(math.sqrt(a))

def _enrich(locations: list) -> list:
    for loc in locations:
        loc["lat"] = float(loc.get("lat", 0))
        loc["lon"] = float(loc.get("lon", 0))
        loc["page_rank"] = float(loc.get("page_rank", 0))
    for i, loc in enumerate(locations):
        loc["distance_km"] = 0 if i == 0 else round(
            _great_circle_km(locations[i-1]["lat"], locations[i-1]["lon"],
                             loc["lat"], loc["lon"]))
    return locations

@app.get("/api/search")
def search(q: str = Query(..., min_length=1, max_length=200)):
    raw = _trep.search("J", q)
    data = json.loads(raw)
    locations = _enrich(data.get("locations", []))
    total_km = sum(loc["distance_km"] for loc in locations)
    return {"query": q, "locations": locations, "total_km": total_km}

@app.get("/api/random")
def random_airports(n: int = Query(1, ge=1, le=10)):
    raw = _trep.generate("S", n)
    codes = [part.split("/")[0] for part in raw.split(",") if "/" in part]
    return search(" ".join(codes))

app.mount("/static", StaticFiles(directory="/var/www/webapps/search/static"), name="static")

@app.get("/favicon.ico", include_in_schema=False)
def favicon():
    return FileResponse("/var/www/webapps/search/static/icons/favicon.ico")

INDEX_HTML = r"""<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Travel Search</title>
  <link rel="icon" href="/favicon.ico">
  <!-- Google tag (gtag.js) -->
  <script async src="https://www.googletagmanager.com/gtag/js?id=G-XJE7QT0JR3"></script>
  <script>
    window.dataLayer = window.dataLayer || [];
    function gtag(){dataLayer.push(arguments);}
    gtag('js', new Date());
    gtag('config', 'G-XJE7QT0JR3');
  </script>
  <!-- Leaflet -->
  <link rel="stylesheet" href="https://unpkg.com/leaflet@1.9.4/dist/leaflet.css">
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; }
    body { font-family: Arial, sans-serif; background: #f5f5f5; color: #333;
           display: flex; flex-direction: column; height: 100vh; }

    /* ── header ── */
    header { background: #fff; border-bottom: 1px solid #ddd;
             padding: 10px 20px; display: flex; align-items: center; gap: 14px;
             flex-shrink: 0; }
    header img { height: 52px; }
    .search-area { display: flex; flex-direction: column; gap: 6px; flex: 1; }
    .search-bar  { display: flex; gap: 6px; flex-wrap: wrap; }
    .search-bar input { flex: 1; min-width: 200px; padding: 7px 11px;
                        font-size: 1rem; border: 1px solid #ccc; border-radius: 4px; }
    .btn { padding: 7px 14px; font-size: 0.88rem; cursor: pointer; border: none;
           border-radius: 4px; background: #4285f4; color: #fff; white-space: nowrap; }
    .btn:hover { background: #3367d6; }
    .btn.sec { background: #5f6368; }
    .btn.sec:hover { background: #3c4043; }

    /* ── summary strip ── */
    #summary { flex-shrink: 0; background: #fff; border-bottom: 1px solid #ddd;
               padding: 6px 20px; font-size: 0.88rem; min-height: 30px;
               display: flex; align-items: center; gap: 6px; flex-wrap: wrap; }
    #summary .chip { background: #e8f0fe; color: #1a73e8; border-radius: 12px;
                     padding: 2px 9px; font-weight: bold; }
    #summary .arrow { color: #aaa; }
    #summary .km  { color: #555; }
    #summary .total { font-weight: bold; margin-left: 8px; }
    #status { color: #888; font-style: italic; }
    #err    { color: #c00; }

    /* ── map fills the rest ── */
    #map { flex: 1; }

    /* ── Leaflet popup ── */
    .pop-title { font-size: 1.05rem; font-weight: bold; margin-bottom: 4px; }
    .pop-iata  { font-size: 1.4rem; font-weight: bold; color: #1a73e8; margin-right: 6px; }
    .pop-row   { margin: 2px 0; font-size: 0.82rem; }
    .pop-row a { color: #1a73e8; }
  </style>
</head>
<body>

<header>
  <img src="/static/icons/airports.gif" alt="Travel Search">
  <div class="search-area">
    <div class="search-bar">
      <input id="q" type="text" placeholder="e.g. Nice, CDG, Tokyo London" autofocus>
      <button class="btn"     onclick="doSearch()">Search</button>
      <button class="btn sec" onclick="doRandom(1)">Random Airport</button>
      <button class="btn sec" onclick="doRandom(3)">Random Itinerary</button>
    </div>
    <div id="summary"><span id="status">Enter a query or click a button.</span></div>
  </div>
</header>

<div id="map"></div>

<!-- Leaflet + geodesic lines -->
<script src="https://unpkg.com/leaflet@1.9.4/dist/leaflet.js"></script>
<script src="https://cdn.jsdelivr.net/npm/leaflet.geodesic@2.7.1/dist/leaflet.geodesic.umd.min.js"></script>
<script>
// ── Map init ────────────────────────────────────────────────────────────────
var _layers = [];   // must be declared before L.map() fires any events

const map = L.map("map").setView([20, 0], 2);
L.tileLayer("https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png", {
  maxZoom: 19,
  attribution: '&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors'
}).addTo(map);

function clearLayers() {
  _layers.forEach(l => map.removeLayer(l));
  _layers = [];
}

// ── Search ──────────────────────────────────────────────────────────────────
document.getElementById("q").addEventListener("keydown", e => {
  if (e.key === "Enter") doSearch();
});

async function doSearch() {
  const q = document.getElementById("q").value.trim();
  if (!q) return;
  setStatus("Searching…");
  try {
    const r = await fetch("/api/search?q=" + encodeURIComponent(q));
    if (!r.ok) throw new Error((await r.json()).detail || r.statusText);
    render(await r.json());
  } catch(e) { setErr(e.message); }
}

async function doRandom(n) {
  setStatus("Generating…");
  try {
    const r = await fetch("/api/random?n=" + n);
    if (!r.ok) throw new Error((await r.json()).detail || r.statusText);
    const data = await r.json();
    document.getElementById("q").value = data.locations.map(l => l.iata_code).join(" ");
    render(data);
  } catch(e) { setErr(e.message); }
}

function setStatus(msg) {
  const s = document.getElementById("summary");
  s.innerHTML = '<span id="status">' + msg + '</span>';
}
function setErr(msg) { setStatus('<span id="err">Error: ' + msg + '</span>'); }

// ── Render ───────────────────────────────────────────────────────────────────
function cityName(loc) {
  try { return loc.cities.city_details.name_utf; } catch(e) { return ""; }
}

function popupHtml(loc) {
  const city = cityName(loc);

  // Location line: city, adm2 (if not Z), adm1 (if not Z), country — mirrors CT186
  const skip = v => !v || v === "Z" || v === "NA";
  let locParts = [city];
  if (!skip(loc.adm2_name_utf)) locParts.push(loc.adm2_name_utf);
  if (!skip(loc.adm1_name_utf)) locParts.push(loc.adm1_name_utf);
  locParts.push(loc.country_name || "");

  // ICAO: suppress placeholder "ZZZZ"
  const icao = (loc.icao_code && loc.icao_code !== "ZZZZ") ? loc.icao_code : "";

  // Geonames link
  const geoLink = (loc.geonames_id && loc.geonames_id !== "0")
    ? `<a href="https://www.geonames.org/${loc.geonames_id}" target="_blank">${loc.geonames_id}</a>` : "";

  // Wikipedia link(s)
  const wikiLink = loc.wiki_link
    ? `<a href="${loc.wiki_link}" target="_blank">${loc.wiki_link}</a>.` : "";

  // Original / corrected keywords
  // Workaround: original_keywords is "NA" when Xapian is bypassed for pure
  // IATA-code queries (OpenTREP bug); fall back to corrected_keywords.
  const corrKw = (loc.corrected_keywords && loc.corrected_keywords !== "NA") ? loc.corrected_keywords : "";
  const rawOrig = (loc.original_keywords  && loc.original_keywords  !== "NA") ? loc.original_keywords  : "";
  const origKw  = rawOrig || corrKw;

  // Matching percentage
  const matchPct = parseFloat(loc.matching_percentage || 0).toFixed(1);

  // GMT offset with sign
  const gmt = loc.offset_gmt, dst = loc.offset_dst, raw = loc.offset_raw;

  return `
  <div class="pop-title"><span class="pop-iata">${loc.iata_code}</span> &mdash; ${loc.name_common}</div>
  <div class="pop-row">Travel-related name: <b>${loc.name_common}</b></div>
  <div class="pop-row">Location (city, state, country): <b>${locParts.join(", ")}</b></div>
  <div class="pop-row">PageRank: <b>${loc.page_rank.toFixed(8)}%</b></div>
  <div class="pop-row">IATA code: <b>${loc.iata_code}</b></div>
  <div class="pop-row">ICAO code: ${icao ? "<b>" + icao + "</b>" : ""}</div>
  <div class="pop-row">Geonames ID: ${geoLink}</div>
  ${wikiLink ? '<div class="pop-row">Wikipedia: ' + wikiLink + '</div>' : ""}
  <div class="pop-row">Time zone and offset (GMT, DST, raw): <b>${loc.time_zone}</b> (${gmt}, ${dst}, ${raw})</div>
  <div class="pop-row">Original keywords: ${origKw}</div>
  ${corrKw && corrKw !== origKw ? '<div class="pop-row">Corrected keywords: <b>' + corrKw + '</b></div>' : ""}
  <div class="pop-row">Matching: <b>${matchPct}%</b></div>`;
}

function render(data) {
  clearLayers();
  const locs = data.locations;

  if (!locs || locs.length === 0) {
    setStatus("No results found.");
    return;
  }

  // Summary strip
  let parts = [];
  locs.forEach((l, i) => {
    if (i > 0 && l.distance_km > 0)
      parts.push(`<span class="km arrow">→</span> <span class="km">${l.distance_km.toLocaleString()} km</span>`);
    parts.push(`<span class="chip">${l.iata_code}</span> ${l.name_common}`);
  });
  if (data.total_km > 0)
    parts.push(`<span class="total">| Total: ${data.total_km.toLocaleString()} km</span>`);
  document.getElementById("summary").innerHTML = parts.join(" ");

  // Markers — each added directly to map so bindPopup works
  const latLngs = [];
  locs.forEach(loc => {
    const ll = [loc.lat, loc.lon];
    latLngs.push(ll);
    const marker = L.marker(ll)
      .bindPopup(popupHtml(loc), { maxWidth: 360 })
      .addTo(map);
    _layers.push(marker);
  });

  // Geodesic line (great-circle arc)
  if (latLngs.length >= 2) {
    const line = L.geodesic([latLngs], {
      weight: 2.5, opacity: 0.9, color: "#e53935", steps: 100
    }).addTo(map);
    _layers.push(line);
  }

  // Fit bounds
  if (locs.length === 1) {
    map.setView([locs[0].lat, locs[0].lon], 8);
  } else {
    const group = L.featureGroup(_layers.filter(l => l instanceof L.Marker));
    map.fitBounds(group.getBounds().pad(0.15));
  }
}
</script>
</body>
</html>
"""

@app.get("/", response_class=HTMLResponse)
def index():
    return INDEX_HTML
