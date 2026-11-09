# Masar (مسار) — Project brief

Read this file first in every session and follow its rules.

Owner: Abdullah Binsalman (GitHub `Abdullah-BS`), ElectroFusion.
Repo: `Abdullah-BS/Masar` (not renamed; keep the capital M). Live site (GitHub Pages, `main` / root): `https://abdullah-bs.github.io/Masar/` (the lowercase `/masar/` returns 404).
Site files: `index.html` (self-contained), `masar_story.mp4` (720p story video), `poster.jpg` (video poster).

## What Masar is

- A smart, interactive evacuation guidance system for factories and other changing workplaces (construction sites, warehouses, tunnels).
- Wireless, portable LED dot-matrix screens stand on the ground (weighted base, two legs, sun hood, high-brightness two-sided face readable in sunlight) and show live arrows. Each screen leads to the next screen, turn by turn, until the safe assembly point.
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
2. Masar is for changing workplaces: the simulator shows a construction site (default map) and a factory (second example). Warehouses and tunnels are other places it can be used.
3. Never show a headcount or "X/18 people out" anywhere.
4. Never invent statistics. Every market or price figure keeps a visible «تقدير» / "estimate" label. Official figures keep their source.
5. Do not claim regulations mandate Masar. Say it is a technical way to strengthen compliance with escape-route requirements.
6. Do not say "no competitors exist". Say the core technology exists for fixed buildings, and we found no product combining live guidance, portability between sites and detector-driven rerouting.
7. Keep the site a single self-contained `index.html` (inline CSS and JS) plus the video and poster. Only external loads: Google Fonts, and three.js r128 loaded lazily when the 3D view opens.
8. It must work at phone width (about 400px) with no horizontal scroll, in light and dark mode, and respect `prefers-reduced-motion`.

## Site structure (`index.html`)

Wrapper: `<div class="page" id="page" dir="rtl" lang="ar" data-lang="ar">`. Bilingual text uses paired elements with classes `ar` / `en`, and CSS hides the inactive language (`.page[data-lang="ar"] .en{display:none}`). Dynamic strings in JS use `T(arabic, english)`. The language choice is kept in `localStorage` key `masar-lang`. The toggle button is `#langBtn`.

Purpose: a demonstration of the idea, not the pitch deck. Keep text minimal; detailed figures live in the slides.

Design world: "the sign is the interface". Brand Paper page; wine-gradient hero and footer with rose stripes; the simulator runs full width on an always-dark "control room" stage with a 4-part console under the map. No sticky nav.

Sections in order (ids):

1. Hero: logo, `#langBtn`, «مسار», one-line lede, CTA to `#sim`, and the animated LED sign (`#heroArrow`, `#heroLabel`, `#heroFoot`)
2. `#sim` (class `stage`): head with `#viewSeg`, `.map` (canvas `#site`, `#view3d`, `#legend2d`), `.console` (readout, official point, click mode + actions, link/evacuation + `#log`)
3. `#prototype`: annotated SVG of the floor screen (sun hood, LED matrix, ESP32, battery in base, weighted base; x3-5) and six component cards with drawings and quantities (ESP32, battery, control panel, fire detector, route-close button, 3 assembly point signs). Real photos replace the drawings later.
4. Footer `#contact`: logo, contact details, copy button. No QR code (removed on request).

Removed on purpose (kept in the pitch deck): story video, problem, solution, how it works, comparison, market, regulations, municipal, roadmap, FAQ, demo script, QR code. `masar_story.mp4` and `poster.jpg` are no longer used by the page.

## Simulator (`#sim`)

- Data-driven: `MAPS.site` (default) and `MAPS.factory`, switched by `#mapSeg`; view switched by `#viewSeg` (2D canvas `#site` / 3D `#view3d`). `setMap(id)` resets state and rebuilds both views.
- Each map defines: `VW` view box, `BLD` boundary (site fence or factory walls), `gates` (walkable rects outside, leading to points), `walls` polylines, `blocks` (site kinds `bldg`, `crane`, `stock`, `pit`, `mix`, `cabin`; factory `m`, `l`, `r`), `hazard` (preset fire spot + button text), `closeAt` (preset closure), `lanes`, three points `pts` A/B/C with `side` labels, `official` default, screens `scr` as `[x, y, FACE, dx, dy]` (x, y = logical point in the middle of the walkway used for the arrow; dx, dy = where the sign is drawn, at the side of the walkway so it never blocks the path), sensor grid `sensX` x `sensY` (filtered to walkable), `work` (12 workers), 3D `cams` (overview only), 2D palette `pal`.
- Site map: 3400 x 2100, fence 250-3150 x 250-1850, gates west (A), north (B), south (C); 8 floor screens; fire preset at the welding area; closure preset in the walkway between the building and the materials yard.
- Factory map: 3000 x 1900 view, doors west (A), east (B) and south (C); 6 floor screens; fire preset at the press.
- Route network: `routeEdges` traces every non-X screen to the active point (a tree); 2D draws it with dots flowing toward the exit, 3D with pulses. It changes with every fire, closure, point change or link loss.
- Routing: 50-unit grid, BFS from each point (`bfs`), `OPEN` grid precomputed per map, arrows by steepest descent (`dirAt`), `trace` for routes and walkers.
- Fire safety (hard rule: nobody is ever routed past a fire): the first detector reading closes a no-go zone of `FIRE_BLOCK` = 300 around the fire at once (the 1.4 s confirmation only pinpoints it visually); cells within `FIRE_SOFT` = 480 cost 60x, so routes use them only when there is no other way; screens within `SCREEN_X` = 330 show X; a point is unsafe if a fire is within `POINT_UNSAFE` = 560. Sensors light with delay distance/900 s (red < 380, amber < 650).
- Walkers (`planWalkers`, `walkerPath`, `escapeTo`): always respect fire and barriers, even with the connection cut; re-plan live from where they stand whenever the situation changes; anyone inside the zone first walks out with every step farther from the fire; with no reachable point they still leave the zone and wait. Stress test (44 scenarios, both maps): zero approaches, closest 302 for anyone who started outside the zone. If the official point is unsafe or unreachable, the system switches to the safe point reachable by the most screens and says why.
- Closures (`makeClosure`): a line of red/white water-filled barriers with cones at both ends, spanning the full width of the walkway, so routes and walkers cannot pass. Clicking it again removes it.
- Controls: `#points` (`#ptA`, `#ptB`, `#ptC`), `#modes`, `#fireBtn`, `#closeMid`, `#clearBtn`, `#linkBtn` (default routes, amber), `#evacBtn` (12 workers at 260 units/s), `#log`.

## 3D view

- three.js r128 + OrbitControls lazy-loaded from `cdnjs.cloudflare.com/ajax/libs/three.js/r128/three.min.js` and `cdn.jsdelivr.net/npm/three@0.128.0/examples/js/controls/OrbitControls.js`. 1 3D unit = 100 plan units.
- `buildWorld()` rebuilds the map scene. Site is outdoor (sky, sun, fence, building under construction with scaffolding, rotating tower crane, materials, fenced excavation, mixer, site cabins, gas cylinders at the welding spot); factory is indoor.
- Floor screens: hazard-striped weighted base, two legs, two-faced panel with a sun hood; arrows drawn relative to the walker (`relArrow`, `ROTY` per FACE).
- Only one camera button: `#cam3Over` (reset view). Worker's-eye and at-the-turn views were removed on request.
- Local testing: the sandbox blocks the CDNs; get three@0.128.0 with `npm pack` and route the two URLs to the local files in Playwright (`--use-gl=swiftshader`).

## Content and numbers (keep sources; estimates labelled)

- Problem figures (from the presentation): 3.3 million construction workers (GASTAT), about 4.4 million by a wider definition (Contractors Authority); 78,000 building permits in 2025 (Ministry of Municipalities and Housing); 34% of registered work injuries in 2022 were in construction (9,607 of 28,227; GOSI); 89% of construction labour are migrants (Buildings 2024 study). No published Saudi data separates construction-site fires or evacuation incidents.
- Market, all estimates: method = sites × screens × rental + platform fee. 21,000 active non-residential sites (78K permits × 18% × 1.5 years). SAR 54,000 per site per year (12 screens × SAR 250 × 12 months + platform SAR 1,500 × 12). TAM SAR 1.13B, SAM SAR 363M, SOM SAR 10.9M (about 200 sites, 3% by year 3). SAM range SAR 161M–847M.
- Pricing, all estimates: screen rental SAR 250–400 per month (3-month minimum); platform SAR 1,000–2,500 per site per month; setup SAR 5,000–15,000; screen sale SAR 3,000–5,000; pilot SAR 15–30K for 8–12 weeks, or free for data for the first 3–5 sites. About 0.07% of an average project's value.
- Regulations: Labor Law (prevent fire, keep escape exits usable); OSH regulation for sites with 50+ workers; SBC 801 has a means-of-egress chapter (do not quote clause numbers); Ministry of Municipalities draft construction-site guide (April 2026) has no dynamic-guidance clause, which is the opportunity; precedent in worker-housing rules; HRSD precaution guide (November 2025).
- Competitors: Eaton, Zumtobel SMARTSIGN, Advanced, Evaclite (fixed adaptive signage); Triax Spot-r EvacTag (on-site alert, not directional); WakeCap (Saudi, worker tracking; a likely partner).
- Roadmap: pilot of 2–3 sites in Riyadh or Jeddah (0–6 months); developer and ministry adoption (6–18 months); then Riyadh → Jeddah/Makkah → Eastern Province → GCC.
- Prototype demo kit: 3–5 wireless LED screens, 1 control panel, 3 assembly points (A, B, C), 1 fire sensor, 1 manual route-close button. Demo script: normal state → change the point to B → fire blocks route 1 → manual closure → connection drops and screens fall back safely.

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
