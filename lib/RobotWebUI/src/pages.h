#pragma once
#include <Arduino.h>

static const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1.0">
<title>Robot Dashboard</title>
<style>
*{box-sizing:border-box;margin:0;padding:0}
:root{--bg:#0f172a;--card:#1e293b;--border:#334155;--text:#e2e8f0;--text-bright:#f8fafc;--text-muted:#64748b;--text-dim:#94a3b8;--accent:#2563eb;--success:#16a34a;--success-light:#4ade80;--danger:#dc2626;--success-bg:#14532d;--danger-bg:#450a0a;--radius-card:16px;--radius-badge:999px;--radius-alert:10px;--shadow-card:0 25px 50px rgba(0,0,0,0.5);--font-stack:-apple-system,BlinkMacSystemFont,'Segoe UI',Roboto,sans-serif;--transition-fast:0.1s;--transition-normal:0.2s}
body{font-family:var(--font-stack);background:var(--bg);color:var(--text);min-height:100vh;display:flex;align-items:center;justify-content:center;padding:1rem}
.card{background:var(--card);border-radius:var(--radius-card);padding:2rem;width:100%;max-width:420px;box-shadow:var(--shadow-card);border:1px solid var(--border)}
.card-header{text-align:center;margin-bottom:2rem}
.card-header h1{font-size:1.5rem;font-weight:700;color:var(--text-bright)}
.card-header p{font-size:0.85rem;color:var(--text-muted);margin-top:0.25rem}
.status-badge{display:inline-flex;align-items:center;gap:6px;padding:4px 12px;border-radius:var(--radius-badge);font-size:0.8rem;font-weight:700}
.connected{background:var(--success-bg);color:#86efac}
.disconnected{background:var(--danger-bg);color:#fca5a5}
.status-dot{width:8px;height:8px;border-radius:50%;transition:background var(--transition-normal)}
.connected .status-dot{background:var(--success-light)}
.disconnected .status-dot{background:var(--danger)}
.section{margin-bottom:1.5rem}
.section-title{font-size:0.75rem;font-weight:400;text-transform:uppercase;letter-spacing:0.08em;color:var(--text-muted);margin-bottom:0.75rem}
.info-row{display:flex;justify-content:space-between;align-items:center;padding:0.6rem 0;border-bottom:1px solid var(--card)}
.info-row:last-child{border-bottom:none}
.info-label{font-size:0.85rem;color:var(--text-muted)}
.info-value{font-size:0.85rem;font-weight:500;color:var(--text)}
.divider{border:none;border-top:1px solid var(--border);margin:1.5rem 0}
.alert{padding:0.75rem 1rem;border-radius:var(--radius-alert);font-size:0.85rem;margin-bottom:1rem;display:none}
.alert-success{background:var(--success-bg);color:#86efac;border-left:3px solid var(--success)}
.alert-danger{background:var(--danger-bg);color:#fca5a5;border-left:3px solid var(--danger)}
.alert.show{display:block}
</style>
</head>
<body>
<div class="card">
<div class="card-header">
<h1>Robot Dashboard</h1>
<p>Live system status</p>
</div>
<div class="alert" id="alert"></div>
<div class="section">
<p class="section-title">CONNECTION</p>
<div class="status-badge disconnected" id="ws-badge">
<span class="status-dot" id="ws-dot"></span>
<span id="ws-status">Disconnected</span>
</div>
</div>
<hr class="divider">
<div class="section">
<p class="section-title">SYSTEM</p>
<div class="info-row">
<span class="info-label">IP Address</span>
<span class="info-value" id="val-ip">--</span>
</div>
<div class="info-row">
<span class="info-label">Uptime</span>
<span class="info-value" id="val-up">--</span>
</div>
<div class="info-row">
<span class="info-label">Free Heap</span>
<span class="info-value" id="val-heap">--</span>
</div>
<div class="info-row">
<span class="info-label">WiFi Signal</span>
<span class="info-value" id="val-rssi">--</span>
</div>
</div>
</div>
<script>
var ws,rDelay=1000;
function connectWS(){
ws=new WebSocket('ws://'+location.host+'/ws');
ws.onopen=function(){
rDelay=1000;
updateConnection(true);
showAlert('Connection restored','success');
};
ws.onclose=function(){
updateConnection(false);
showAlert('Connection lost -- reconnecting...','danger');
setTimeout(connectWS,rDelay);
rDelay=Math.min(rDelay*2,10000);
};
ws.onmessage=function(e){
var msg=JSON.parse(e.data);
if(msg.t==='sys')updateSystemInfo(msg.d);
};
}
function updateConnection(on){
var badge=document.getElementById('ws-badge');
var status=document.getElementById('ws-status');
if(on){
badge.className='status-badge connected';
status.textContent='Connected';
}else{
badge.className='status-badge disconnected';
status.textContent='Disconnected';
}
}
function updateSystemInfo(d){
if(d.ip!==undefined)document.getElementById('val-ip').textContent=d.ip;
if(d.up!==undefined)document.getElementById('val-up').textContent=formatUptime(d.up);
if(d.heap!==undefined)document.getElementById('val-heap').textContent=d.heap+' KB';
if(d.rssi!==undefined)document.getElementById('val-rssi').textContent=d.rssi+' dBm';
}
function formatUptime(s){
var h=Math.floor(s/3600);
var m=Math.floor((s%3600)/60);
s=s%60;
return(h?h+'h ':'')+(m?m+'m ':'')+s+'s';
}
function showAlert(msg,type){
var el=document.getElementById('alert');
el.textContent=msg;
el.className='alert alert-'+type+' show';
setTimeout(function(){el.classList.remove('show');},2500);
}
connectWS();
</script>
</body>
</html>
)rawliteral";
