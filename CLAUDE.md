# Masar (مسار) — Project brief

Read this file first in every session and follow its rules.

Owner: Abdullah Binsalman (GitHub `Abdullah-BS`), ElectroFusion.
Repo: `Abdullah-BS/Masar` (not renamed; keep the capital M). Live site (GitHub Pages, `main` / root): `https://abdullah-bs.github.io/Masar/` (the lowercase `/masar/` returns 404).
Site files: `index.html` (self-contained), `masar_story.mp4` (720p story video), `poster.jpg` (video poster).

## What Masar is

- A smart, interactive evacuation guidance system for factories and other changing workplaces (construction sites, warehouses, tunnels).
- Wireless, portable LED dot-matrix screens hang from the ceiling and show live arrows. Each screen leads to the next screen, turn by turn, until the safe assembly point.
- The safety manager changes the assembly point from a control panel with one click, and every screen re-syncs automatically.
- Several fire detectors (heat and smoke) work together, and the overlap of their readings pinpoints the fire zone. Screens near the fire show a red X, and the route is recalculated around the fire.
- Fail-safe: if the connection drops, every screen falls back to its pre-approved default route. Each screen has a battery.
- Design principle: pre-approved routes with cause-and-effect logic, not free AI decisions during a fire. The safety manager always has a manual override.
- The control-room dashboard shows the fire location, closed routes, the active assembly point, screen status and the alternative route. It must NOT show or count how many people got out. The system does not track people.

## Who and why

- Team: ElectroFusion, a Saudi engineering startup (automation, IoT, electronics). The team section shows the company only, no individual names.
- Contact: email `electrofusion.ksa@gmail.com`, X `@EFusionKSA`, Instagram and TikTok `electrofusion.sa`.
- Event: Municipalthon by Massaq (`municipalthon.massaq.sa`). Best-fit track: Track 4, Emerging Technologies and Data Management (IoT). Second choice: Track 1, Smart Cities.
- Judging criteria: impact on municipal services 30%, technical feasibility and prototype 25%, innovation 20%, scalability, sustainability and cost 15%, presentation 10%.

## Brand (ElectroFusion visual identity)

- Colors: Wine (primary) `#A62746`, Deep wine `#750438`, Plum `#59022A`, Rose (accent) `#DD325C`, Paper `#E7E9E8`, Ink `#1E0812`.
- Hero and footer gradient: `linear-gradient(60deg,#DD325C 0%,#A62746 24%,#750438 55%,#59022A 100%)`, with thick rose diagonal stripes (rotated about -37°) as a motif.
- Safety colors are used only in the simulator and screens: LED green `#39E27A` / `#3BE37A`, fire orange `#FF6A1A`, red X `#FF3B4E`, amber (fail-safe) `#FFB020`.
- Fonts (Google Fonts): Readex Pro for headings, IBM Plex Sans Arabic for body text.
- Logo: the ElectroFusion wordmark (arrow-in-square icon plus "ElectroFusion"), white version on the wine backgrounds. Embedded in `index.html` as a base64 PNG data URI (search for `data:image/png;base64` inside `<img ... alt="ElectroFusion">`).

## Hard rules for any change

1. Arabic is the default language and must always be right-to-left. English is a toggle.
2. The main story setting is a production factory. Construction sites, warehouses and tunnels are listed as other places Masar can be used.
3. Never show a headcount or "X/18 people out" anywhere.
4. Never invent statistics. Every market or price figure keeps a visible «تقدير» / "estimate" label. Official figures keep their source.
5. Do not claim regulations mandate Masar. Say it is a technical way to strengthen compliance with escape-route requirements.
6. Do not say "no competitors exist". Say the core technology exists for fixed buildings, and we found no product combining live guidance, portability between sites and detector-driven rerouting.
7. Keep the site a single self-contained `index.html` (inline CSS and JS) plus the video and poster. Only external loads: Google Fonts, and three.js r128 loaded lazily when the 3D view opens.
8. It must work at phone width (about 400px) with no horizontal scroll, in light and dark mode, and respect `prefers-reduced-motion`.

## Site structure (`index.html`)

Wrapper: `<div class="page" id="page" dir="rtl" lang="ar" data-lang="ar">`. Bilingual text uses paired elements with classes `ar` / `en`, and CSS hides the inactive language (`.page[data-lang="ar"] .en{display:none}`). Dynamic strings in JS use `T(arabic, english)`. The language choice is kept in `localStorage` key `masar-lang`. The toggle button is `#langBtn`.

Sections in order (ids):

1. Hero with an animated LED sign
2. Sticky nav
3. `#story` — video `masar_story.mp4` with `poster.jpg`
4. `#problem`
5. `#solution`
6. `#sim` — simulator
7. `#how` — 4 steps plus 4 component cards
8. `#compare` — today vs Masar table, plus competitors table rendered from the JS array `comp`
9. `#market` — TAM/SAM/SOM funnel plus pricing table
10. `#regs`
11. `#municipal` — Balady permit → Masar site → municipal dashboard flow, 574K permits stat, where-it-applies chips
12. `#prototype` — demo kit plus a photo placeholder saying «قريباً»
13. `#plan` — roadmap
14. `#faq` — rendered from the JS array `faqs`
15. Footer `#contact` — logo, contact details, copy button, QR (`<div class="qr">` with an inline `<svg>`; it should encode `https://abdullah-bs.github.io/Masar/`)

### QR code regeneration

```python
import qrcode, qrcode.image.svg
img = qrcode.make("https://abdullah-bs.github.io/Masar/",
                  image_factory=qrcode.image.svg.SvgPathImage, box_size=10, border=2)
```

Remove the `width`/`height` attributes from the generated `<svg>`, add `role="img" aria-label="QR code"`, and replace the `<svg>` inside `<div class="qr">`. Touch nothing else.

## Simulator (`#sim`)

- World units are pixels of a 3000 × 1800 plan. View: `VW=[0,150,3000,1750]`. Building: `BLD=[300,200,2700,1600]`.
- Obstacle blocks (`BLOCKS`): machines row along the north wall `[380,262,2620,380]`; production line `[380,640,1500,1170]`; storage racks `[1800,640,2540,1170]`; machines `[380,1430,1500,1540]`; racks `[1800,1430,2540,1540]`. The press (fire source) is at `[610,262,800,390]` with fire point `PRESS=[700,470]`.
- Aisles: north aisle (y ≈ 380–640), middle vertical aisle (x 1500–1800), south aisle (y 1170–1430), east aisle (x 2540–2700).
- Doors: west wall at y 430–590 leads to assembly point A at `(140,510)`. East wall at y 1220–1380 leads to point B at `(2880,1300)`.
- 8 screens `SCR`: `[520,510],[900,510],[1250,510],[1650,510],[1650,900],[1650,1300],[2150,1300],[2620,900]`.
- 24 sensors `SENS`: x from 450 to 2550 in steps of 300, at y 470, 900 and 1330.
- Routing: 50-unit grid (60 × 36) with BFS from the active point (`bfs`). A screen's arrow is the steepest-descent neighbour (`dirAt`). Routes are traced with `trace`.
- Fire logic: a fire is "confirmed" 1.4 s after it starts and then blocks cells within 175 units. Sensors light up with a delay of distance/900 s (red under 380 units, amber under 650), with expanding rings. Screens within 260 units of a confirmed fire show X. A point is unsafe if a fire is within 520 units. If the official point is reached by fewer screens than the other point, the system auto-switches and explains why.
- Controls: official point A/B (`#points`), click mode fire / close aisle (`#modes`), fire at the press (`#fireBtn`), close the middle aisle (`#closeMid`, closure at 1650,760), reset (`#clearBtn`), cut connection (`#linkBtn`, screens use the static default route and turn amber), start evacuation (`#evacBtn`, 12 workers in `WORK` walk the BFS path at 260 units/s), event log (`#log`).
- Shared helpers used by both views: `compute(t)` returns state (active point, reason, dirs, routeD, alt), `walkerPos(t)`, and `clickAt(x,y)`.

## 3D view

- Toggle `#viewSeg` (2D / 3D). The 3D container is `#view3d`. three.js r128 and OrbitControls are lazy-loaded from `cdnjs.cloudflare.com/ajax/libs/three.js/r128/three.min.js` and `cdn.jsdelivr.net/npm/three@0.128.0/examples/js/controls/OrbitControls.js`.
- Scale: 1 3D unit = 100 plan units (x → x, y → z). Shows the floor, aisle markings, machines, the production line with robot arms, racks, see-through walls, ceiling sensors, assembly points with labels, fire (flames, smoke, point light), sensor rings, the confirmed-zone circle, striped barriers for closures, a glowing route and walking workers.
- Each screen has two faces. `FACE=['E','E','E','E','S','S','E','S']` is the walking direction of people who see the front face. The arrow is drawn relative to the walker (up = straight ahead, right = turn right) by `relArrow`.
- Camera presets: `#cam3Over` overview, `#cam3Eye` worker's eye in the north aisle, `#cam3Turn` at the first turn. Clicking the floor calls `clickAt` (fire or close aisle).
- Proof-of-version marker: the new page contains the text `عرض ثلاثي الأبعاد`.

## Content and numbers (keep sources; estimates labelled)

- Problem figures (from the presentation): 3.3 million construction workers (GASTAT), about 4.4 million by a wider definition (Contractors Authority); 78,000 building permits in 2025 (Ministry of Municipalities and Housing); 34% of registered work injuries in 2022 were in construction (9,607 of 28,227; GOSI); 89% of construction labour are migrants (Buildings 2024 study). No published Saudi data separates construction-site fires or evacuation incidents.
- Market, all estimates: method = sites × screens × rental + platform fee. 21,000 active non-residential sites (78K permits × 18% × 1.5 years). SAR 54,000 per site per year (12 screens × SAR 250 × 12 months + platform SAR 1,500 × 12). TAM SAR 1.13B, SAM SAR 363M, SOM SAR 10.9M (about 200 sites, 3% by year 3). SAM range SAR 161M–847M.
- Pricing, all estimates: screen rental SAR 250–400 per month (3-month minimum); platform SAR 1,000–2,500 per site per month; setup SAR 5,000–15,000; screen sale SAR 3,000–5,000; pilot SAR 15–30K for 8–12 weeks, or free for data for the first 3–5 sites. About 0.07% of an average project's value.
- Regulations: Labor Law (prevent fire, keep escape exits usable); OSH regulation for sites with 50+ workers; SBC 801 has a means-of-egress chapter (do not quote clause numbers); Ministry of Municipalities draft construction-site guide (April 2026) has no dynamic-guidance clause, which is the opportunity; precedent in worker-housing rules; HRSD precaution guide (November 2025).
- Competitors: Eaton, Zumtobel SMARTSIGN, Advanced, Evaclite (fixed adaptive signage); Triax Spot-r EvacTag (on-site alert, not directional); WakeCap (Saudi, worker tracking; a likely partner).
- Roadmap: pilot of 2–3 sites in Riyadh or Jeddah (0–6 months); developer and ministry adoption (6–18 months); then Riyadh → Jeddah/Makkah → Eastern Province → GCC.
- Prototype demo kit: 3–5 wireless LED screens, 1 control panel, 2 assembly points (A, B), 1 fire sensor, 1 manual route-close button. Demo script: normal state → change the point to B → fire blocks route 1 → manual closure → connection drops and screens fall back safely.

## Related assets (outside the repo)

- Story video: 92 s, 1920×1080 original, 720p copy on the site. Scenes: title → normal factory day (point A) → control room moves the point to B → screens flip → fire at the press → sensor fusion map → confusion and alarm → workers follow the screens through turns (right, then left) → control room dashboard → everyone at point B → end card. Narration is in Saudi dialect in Abdullah's own cloned ElevenLabs voice. Captions are Arabic, right-to-left.
- A 16-slide Arabic pitch deck («مسار - عرض بلديثون») and an earlier 10-slide concept deck, both in Claude Slides and Canva.
- Earlier artifact copy of the site: `https://claude.ai/artifact/V8pMFMJ2JygA7u4FJQi9MC`.

## Ideas already discussed for later

- Replace the prototype placeholder with real photos and video once the ESP32 + LED matrix model is built.
- Background music under the video (calm → suspense → hopeful), kept about 20 dB under the voice.
- Optional: a judges-only "pitch mode" that walks through the demo script automatically.

## How to work with Abdullah

- Before big changes, give the plan in a few lines and ask. After changes, run a quick local check (open the page in a headless browser, click the simulator controls, confirm no console errors and no horizontal scroll at 400px). Then commit with a clear message and push.
- Ask before anything destructive (repo rename, deleting files, force-push).
- Keep replies short. Arabic UI text must be natural and right-to-left; keep the existing Saudi-friendly wording style.
