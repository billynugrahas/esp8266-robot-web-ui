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
:root{--bg:#0f172a;--card:#1e293b;--border:#334155;--text:#e2e8f0;--text-bright:#f8fafc;--text-muted:#64748b;--text-dim:#94a3b8;--accent:#2563eb;--success:#16a34a;--success-light:#4ade80;--danger:#dc2626;--success-bg:#14532d;--danger-bg:#450a0a;--radius-card:16px;--radius-badge:999px;--radius-alert:10px;--shadow-card:0 25px 50px rgba(0,0,0,0.5);--font-stack:-apple-system,BlinkMacSystemFont,'Segoe UI',Roboto,sans-serif;--transition-fast:0.1s;--transition-normal:0.2s;--warning:#d97706;--warning-bg:#451a03;--btn-bg:#475569;--radius-btn:10px;--transition-alert:0.25s}
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
.alert-warning{background:#451a03;color:#fbbf24;border-left:3px solid #d97706}
.alert.show{display:block}
.estop-btn{width:100%;height:48px;background:var(--danger);color:#fff;border:none;border-radius:var(--radius-btn);font-size:0.85rem;font-weight:700;cursor:pointer;transition:transform var(--transition-fast);margin-bottom:1rem}
.estop-btn:active{transform:scale(0.96);background:#b91c1c}
.dpad{display:grid;grid-template-areas:". up ." "left center right" ". down .";grid-template-columns:repeat(3,48px);grid-template-rows:repeat(3,48px);gap:4px;justify-content:center;user-select:none;-webkit-user-select:none;-webkit-tap-highlight-color:transparent;touch-action:manipulation;margin:0 auto}
.dpad-up{grid-area:up}.dpad-left{grid-area:left}.dpad-center{grid-area:center}.dpad-right{grid-area:right}.dpad-down{grid-area:down}
.dpad button{background:var(--btn-bg);color:var(--text);border:none;border-radius:var(--radius-btn);font-size:1.2rem;cursor:pointer;transition:transform var(--transition-fast),background var(--transition-normal);display:flex;align-items:center;justify-content:center}
.dpad button:active{transform:scale(0.96);background:var(--accent)}
.speed-control{margin-top:1rem}
.speed-label{font-size:0.85rem;color:var(--text-muted);display:block;margin-bottom:4px}
.speed-slider{width:100%;height:44px;-webkit-appearance:none;appearance:none;background:var(--border);border-radius:8px;outline:none;cursor:pointer}
.speed-slider::-webkit-slider-thumb{-webkit-appearance:none;width:24px;height:24px;border-radius:50%;background:var(--accent);cursor:pointer}
.speed-slider::-moz-range-thumb{width:24px;height:24px;border-radius:50%;background:var(--accent);border:none;cursor:pointer}
.sensor-distance{text-align:center;padding:1rem;border-radius:var(--radius-btn);background:var(--success-bg);transition:background var(--transition-normal)}
.dist-value{font-size:2.5rem;font-weight:700;color:var(--text-bright)}
.dist-unit{font-size:0.9rem;color:var(--text-muted);margin-left:4px}
.ir-badge{display:inline-block;padding:6px 16px;border-radius:var(--radius-badge);font-size:0.85rem;font-weight:600;transition:background var(--transition-normal),color var(--transition-normal)}
.ir-badge.clear{background:var(--success-bg);color:#86efac}
.ir-badge.detected{background:var(--danger-bg);color:#fca5a5}
.switch-badge{display:inline-flex;align-items:center;gap:8px;margin:4px 0;padding:4px 0}
.switch-badge .sw-name{font-size:0.85rem;color:var(--text-muted);min-width:70px}
.sw-state{display:inline-block;padding:2px 10px;border-radius:var(--radius-badge);font-size:0.75rem;font-weight:700;transition:background var(--transition-normal),color var(--transition-normal)}
.sw-state.on{background:var(--success);color:#fff}
.sw-state.off{background:var(--border);color:var(--text-muted)}
.wifi-row{display:flex;justify-content:space-between;align-items:center;padding:0.6rem 0.5rem;border-bottom:1px solid var(--border);cursor:pointer;transition:background var(--transition-fast)}
.wifi-row:hover{background:var(--border)}
.wifi-row:last-child{border-bottom:none}
.wifi-ssid{font-size:0.85rem;color:var(--text);display:flex;align-items:center;gap:6px}
.wifi-bars{font-size:0.7rem;letter-spacing:1px;color:var(--success-light)}
.scan-btn{width:100%;height:44px;background:var(--btn-bg);color:var(--text);border:none;border-radius:var(--radius-btn);font-size:0.85rem;font-weight:700;cursor:pointer;transition:transform var(--transition-fast),background var(--transition-normal);margin-bottom:0.5rem}
.scan-btn:active{transform:scale(0.96)}
.scan-btn:disabled{opacity:0.5;cursor:not-allowed}
.wifi-input{flex:1;height:44px;padding:0 12px;background:var(--card);border:1px solid var(--border);color:var(--text);border-radius:6px;font-size:0.85rem;font-family:var(--font-stack)}
.wifi-input:focus{outline:none;border-color:var(--accent)}
.wifi-toggle{cursor:pointer;font-size:20px;color:var(--text-muted);user-select:none;-webkit-user-select:none;padding:0 8px}
.connect-btn{width:100%;height:44px;background:var(--accent);color:#fff;border:none;border-radius:var(--radius-btn);font-size:0.85rem;font-weight:700;cursor:pointer;transition:transform var(--transition-fast);margin-top:0.5rem}
.connect-btn:active{transform:scale(0.96)}
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
<p class="section-title">CONTROLS</p>
<button class="estop-btn" id="estop-btn">EMERGENCY STOP</button>
<div class="dpad">
<button class="dpad-up" data-dir="forward">&#9650;</button>
<button class="dpad-left" data-dir="left">&#9664;</button>
<button class="dpad-center" data-dir="stop">&#9632;</button>
<button class="dpad-right" data-dir="right">&#9654;</button>
<button class="dpad-down" data-dir="back">&#9660;</button>
</div>
<div class="speed-control">
<label class="speed-label">Speed: <span id="speed-val">75</span>%</label>
<input type="range" id="speed-slider" min="0" max="100" value="75" class="speed-slider">
</div>
</div>
<hr class="divider">
<div class="section">
<p class="section-title">DISTANCE</p>
<div class="sensor-distance" id="dist-display">
<span class="dist-value" id="dist-val">--</span>
<span class="dist-unit">cm</span>
</div>
</div>
<div class="section">
<p class="section-title">IR PROXIMITY</p>
<div class="ir-badge clear" id="ir-display">Clear</div>
</div>
<div class="section">
<p class="section-title">ODOMETRY</p>
<div class="info-row"><span class="info-label">X Position</span><span class="info-value" id="odo-x">--</span></div>
<div class="info-row"><span class="info-label">Y Position</span><span class="info-value" id="odo-y">--</span></div>
<div class="info-row"><span class="info-label">Heading</span><span class="info-value" id="odo-hdg">--</span></div>
</div>
<div class="section">
<p class="section-title">LIMIT SWITCHES</p>
<div id="switch-container"></div>
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
<hr class="divider">
<div class="section">
<p class="section-title">WIFI NETWORKS</p>
<button class="scan-btn" id="scanBtn">Scan Networks</button>
<div id="scanResults" style="max-height:180px;overflow-y:auto"></div>
<div id="wifiConnect" style="display:none">
<div style="display:flex;align-items:center;gap:8px;margin-top:0.5rem">
<input id="wifiPw" type="password" class="wifi-input" placeholder="Enter password">
<span class="wifi-toggle" id="pwToggle">&#128065;</span>
</div>
<input id="wifiSsid" type="hidden">
<button class="connect-btn" id="connectBtn">Connect</button>
</div>
</div>
</div>
<script>
var ws,rDelay=1000,curSpeed=75;
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
if(msg.t==='dist')updateDist(msg.d);
if(msg.t==='ir')updateIR(msg.d);
if(msg.t==='odo')updateOdo(msg.d);
if(msg.t==='bool')updateSwitch(msg.d.idx,msg.d.st);
if(msg.t==='ack'&&msg.d&&msg.d.msg==='motor_timeout')showAlert('Motor timeout -- auto-stopped','warning');
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
function sendMotor(dir){
if(ws&&ws.readyState===1)ws.send(JSON.stringify({t:"motor",d:{dir:dir,spd:curSpeed}}));
}
function updateDist(d){
if(d.cm===undefined)return;
var val=document.getElementById('dist-val');
var el=document.getElementById('dist-display');
val.textContent=d.cm.toFixed(1);
var bg=d.cm>30?'var(--success-bg)':d.cm>15?'#451a03':'var(--danger-bg)';
el.style.background=bg;
if(d.cm<15)showAlert('Obstacle detected','danger');
}
function updateIR(d){
var el=document.getElementById('ir-display');
el.textContent=d.det?'Detected':'Clear';
el.className='ir-badge '+(d.det?'detected':'clear');
}
function updateOdo(d){
if(d.x!==undefined)document.getElementById('odo-x').textContent=d.x.toFixed(2);
if(d.y!==undefined)document.getElementById('odo-y').textContent=d.y.toFixed(2);
if(d.hdg!==undefined)document.getElementById('odo-hdg').textContent=d.hdg.toFixed(1)+'\u00B0';
}
var switches={};
function updateSwitch(idx,st){
if(!switches[idx]){
var row=document.createElement('div');
row.className='switch-badge';
var name=document.createElement('span');
name.className='sw-name';
name.textContent='Switch '+(idx+1);
var state=document.createElement('span');
state.className='sw-state off';
state.id='sw-'+idx;
state.textContent='OFF';
row.appendChild(name);
row.appendChild(state);
document.getElementById('switch-container').appendChild(row);
switches[idx]=true;
}
var el=document.getElementById('sw-'+idx);
el.textContent=st?'ON':'OFF';
el.className='sw-state '+(st?'on':'off');
}
var slider=document.getElementById('speed-slider');
var speedVal=document.getElementById('speed-val');
slider.oninput=function(){curSpeed=parseInt(this.value);speedVal.textContent=curSpeed};
var dpadBtns=document.querySelectorAll('.dpad button');
for(var i=0;i<dpadBtns.length;i++){
dpadBtns[i].addEventListener('pointerdown',function(e){
e.preventDefault();
sendMotor(this.getAttribute('data-dir'));
});
}
document.getElementById('estop-btn').addEventListener('pointerdown',function(e){
e.preventDefault();
sendMotor('stop');
});
connectWS();
</script>
</body>
</html>
)rawliteral";
