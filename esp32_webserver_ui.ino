#include <WiFi.h>
#include <WebServer.h>

const char* ssid     = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

WebServer server(80);

// Track LED state
bool ledState = false;

// ─── HTML Page ────────────────────────────────────────────────────────────────
const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 Control Panel</title>
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; }

    body {
      font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', sans-serif;
      background: #0f172a;
      color: #e2e8f0;
      min-height: 100vh;
      display: flex;
      align-items: center;
      justify-content: center;
      padding: 1rem;
    }

    .card {
      background: #1e293b;
      border-radius: 16px;
      padding: 2rem;
      width: 100%;
      max-width: 420px;
      box-shadow: 0 25px 50px rgba(0,0,0,0.5);
      border: 1px solid #334155;
    }

    .card-header {
      text-align: center;
      margin-bottom: 2rem;
    }

    .card-header h1 {
      font-size: 1.5rem;
      font-weight: 700;
      color: #f8fafc;
    }

    .card-header p {
      font-size: 0.85rem;
      color: #64748b;
      margin-top: 0.25rem;
    }

    /* ── Status Badge ── */
    .status-badge {
      display: inline-flex;
      align-items: center;
      gap: 6px;
      padding: 4px 12px;
      border-radius: 999px;
      font-size: 0.8rem;
      font-weight: 600;
      margin-bottom: 1.5rem;
    }
    .status-badge.on  { background: #14532d; color: #86efac; }
    .status-badge.off { background: #1c1917; color: #78716c; }
    .status-dot {
      width: 8px; height: 8px;
      border-radius: 50%;
    }
    .status-badge.on  .status-dot { background: #4ade80; }
    .status-badge.off .status-dot { background: #57534e; }

    /* ── Buttons ── */
    .btn {
      display: inline-flex;
      align-items: center;
      justify-content: center;
      gap: 8px;
      padding: 0.7rem 1.4rem;
      border: none;
      border-radius: 10px;
      font-size: 0.95rem;
      font-weight: 600;
      cursor: pointer;
      transition: transform 0.1s, filter 0.15s, background 0.2s;
      text-decoration: none;
    }
    .btn:active { transform: scale(0.96); }

    /* Variants */
    .btn-success { background: #16a34a; color: #fff; }
    .btn-success:hover { filter: brightness(1.15); }

    .btn-danger  { background: #dc2626; color: #fff; }
    .btn-danger:hover  { filter: brightness(1.15); }

    .btn-primary { background: #2563eb; color: #fff; }
    .btn-primary:hover { filter: brightness(1.15); }

    .btn-warning { background: #d97706; color: #fff; }
    .btn-warning:hover { filter: brightness(1.15); }

    .btn-secondary { background: #475569; color: #e2e8f0; }
    .btn-secondary:hover { filter: brightness(1.2); }

    .btn-outline {
      background: transparent;
      color: #94a3b8;
      border: 1.5px solid #475569;
    }
    .btn-outline:hover { border-color: #94a3b8; color: #f1f5f9; }

    /* Sizes */
    .btn-lg { padding: 0.85rem 1.8rem; font-size: 1.05rem; border-radius: 12px; }
    .btn-sm { padding: 0.4rem 0.9rem; font-size: 0.8rem; border-radius: 8px; }

    /* Full-width */
    .btn-block { width: 100%; }

    /* ── Section ── */
    .section { margin-bottom: 1.5rem; }
    .section-title {
      font-size: 0.75rem;
      font-weight: 600;
      text-transform: uppercase;
      letter-spacing: 0.08em;
      color: #64748b;
      margin-bottom: 0.75rem;
    }

    /* ── Button Row ── */
    .btn-row {
      display: flex;
      gap: 8px;
      flex-wrap: wrap;
    }

    /* ── Divider ── */
    .divider {
      border: none;
      border-top: 1px solid #334155;
      margin: 1.5rem 0;
    }

    /* ── Info Row ── */
    .info-row {
      display: flex;
      justify-content: space-between;
      align-items: center;
      padding: 0.6rem 0;
      border-bottom: 1px solid #1e293b;
    }
    .info-row:last-child { border-bottom: none; }
    .info-label { font-size: 0.85rem; color: #64748b; }
    .info-value { font-size: 0.85rem; font-weight: 500; color: #e2e8f0; }

    /* ── Alert ── */
    .alert {
      padding: 0.75rem 1rem;
      border-radius: 10px;
      font-size: 0.85rem;
      margin-bottom: 1rem;
      display: none;
    }
    .alert-success { background: #14532d; color: #86efac; border-left: 3px solid #16a34a; }
    .alert-danger  { background: #450a0a; color: #fca5a5; border-left: 3px solid #dc2626; }
    .alert.show { display: block; }
  </style>
</head>
<body>

<div class="card">

  <div class="card-header">
    <h1>&#9889; ESP32 Panel</h1>
    <p>Control your device from anywhere</p>
  </div>

  <!-- Alert -->
  <div class="alert" id="alert"></div>

  <!-- LED Status + Toggle -->
  <div class="section" style="text-align:center;">
    <div class="status-badge %STATUS_CLASS%" id="badge">
      <span class="status-dot"></span>
      <span id="badge-text">LED is %STATUS_TEXT%</span>
    </div>
    <br>
    <button class="btn btn-lg btn-block %BTN_CLASS%" id="toggleBtn"
            onclick="toggleLED()">
      %BTN_LABEL%
    </button>
  </div>

  <hr class="divider">

  <!-- Button showcase -->
  <div class="section">
    <p class="section-title">Button Variants</p>
    <div class="btn-row">
      <button class="btn btn-primary"   onclick="flash('primary')">Primary</button>
      <button class="btn btn-success"   onclick="flash('success')">Success</button>
      <button class="btn btn-danger"    onclick="flash('danger')">Danger</button>
      <button class="btn btn-warning"   onclick="flash('warning')">Warning</button>
      <button class="btn btn-secondary" onclick="flash('secondary')">Secondary</button>
      <button class="btn btn-outline"   onclick="flash('outline')">Outline</button>
    </div>
  </div>

  <div class="section">
    <p class="section-title">Sizes</p>
    <div class="btn-row" style="align-items:center;">
      <button class="btn btn-primary btn-sm">Small</button>
      <button class="btn btn-primary">Default</button>
      <button class="btn btn-primary btn-lg">Large</button>
    </div>
  </div>

  <hr class="divider">

  <!-- Device Info -->
  <div class="section">
    <p class="section-title">Device Info</p>
    <div class="info-row">
      <span class="info-label">IP Address</span>
      <span class="info-value">%IP_ADDRESS%</span>
    </div>
    <div class="info-row">
      <span class="info-label">Uptime</span>
      <span class="info-value" id="uptime">--</span>
    </div>
    <div class="info-row">
      <span class="info-label">Free Heap</span>
      <span class="info-value">%FREE_HEAP% KB</span>
    </div>
  </div>

</div>

<script>
  // Live uptime counter
  let startMs = Date.now();
  setInterval(() => {
    let s = Math.floor((Date.now() - startMs) / 1000);
    let m = Math.floor(s / 60); s %= 60;
    let h = Math.floor(m / 60); m %= 60;
    document.getElementById('uptime').textContent =
      (h ? h + 'h ' : '') + (m ? m + 'm ' : '') + s + 's';
  }, 1000);

  // Toggle LED via fetch
  function toggleLED() {
    fetch('/toggle')
      .then(r => r.json())
      .then(data => {
        let on = data.state === 'on';
        // Update badge
        let badge = document.getElementById('badge');
        badge.className = 'status-badge ' + (on ? 'on' : 'off');
        document.getElementById('badge-text').textContent = 'LED is ' + (on ? 'ON' : 'OFF');
        // Update button
        let btn = document.getElementById('toggleBtn');
        btn.className = 'btn btn-lg btn-block ' + (on ? 'btn-danger' : 'btn-success');
        btn.textContent = on ? '&#9940; Turn OFF' : '&#9889; Turn ON';
        showAlert('LED turned ' + data.state.toUpperCase() + '!', on ? 'success' : 'danger');
      })
      .catch(() => showAlert('Connection error!', 'danger'));
  }

  function showAlert(msg, type) {
    let el = document.getElementById('alert');
    el.textContent = msg;
    el.className = 'alert alert-' + type + ' show';
    setTimeout(() => el.classList.remove('show'), 2500);
  }

  // Demo for variant buttons
  function flash(type) {
    showAlert('You clicked: ' + type, type === 'danger' ? 'danger' : 'success');
  }
</script>
</body>
</html>
)rawliteral";

// ─── Route Handlers ───────────────────────────────────────────────────────────
void handleRoot() {
  String html = INDEX_HTML;

  // Inject dynamic values
  html.replace("%STATUS_CLASS%", ledState ? "on" : "off");
  html.replace("%STATUS_TEXT%",  ledState ? "ON"  : "OFF");
  html.replace("%BTN_CLASS%",    ledState ? "btn-danger" : "btn-success");
  html.replace("%BTN_LABEL%",    ledState ? "&#9940; Turn OFF" : "&#9889; Turn ON");
  html.replace("%IP_ADDRESS%",   WiFi.localIP().toString());
  html.replace("%FREE_HEAP%",    String(ESP.getFreeHeap() / 1024));

  server.send(200, "text/html", html);
}

void handleToggle() {
  ledState = !ledState;
  digitalWrite(LED_BUILTIN, ledState ? HIGH : LOW);

  // Return JSON so the browser can update the UI without a page reload
  String json = "{\"state\":\"" + String(ledState ? "on" : "off") + "\"}";
  server.send(200, "application/json", json);
}

void handleNotFound() {
  server.send(404, "text/plain", "404: Not Found");
}

// ─── Setup & Loop ─────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());

  // Register routes
  server.on("/",       handleRoot);
  server.on("/toggle", handleToggle);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("Web server started.");
}

void loop() {
  server.handleClient();
}
