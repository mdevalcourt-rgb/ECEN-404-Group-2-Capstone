#include "WebContent.h"

const char INDEX_HTML[] PROGMEM = R"HTML(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>MADAM3</title>
  <style>
    :root {
      --bg:#0a0a0a; --bg2:#111111; --bgi:#060606;
      --a:#e8e8e8; --ad:#888888; --ah:#ffffff;
      --af:rgba(255,255,255,0.07); --ab:rgba(255,255,255,0.20);
      --font:"Courier New",Courier,"Lucida Console",Monaco,monospace;
    }
    *,*::before,*::after{box-sizing:border-box;}
    html{background:var(--bg);color:var(--a);font-family:var(--font);font-size:15px;line-height:1.5;}
    body{margin:0;padding:1.5rem;min-height:100vh;}
    h1,h2,h3,h4{font-family:var(--font);font-weight:normal;text-transform:uppercase;letter-spacing:.1em;}
    p{margin:0;}
    .tw{max-width:1080px;margin:0 auto;}
    /* ── Sections ── */
    .ts{border:1px solid var(--ab);margin-top:1.25rem;background:var(--bg2);}
    .ts-head{display:flex;align-items:center;justify-content:space-between;padding:.2rem .75rem;background:var(--ad);border-bottom:1px solid var(--ab);font-size:.7rem;letter-spacing:.18em;text-transform:uppercase;color:var(--bg);font-weight:bold;}
    .ts-body{padding:.75rem 1rem;}
    .ts-body.p0{padding:.5rem;}
    /* ── Buttons ── */
    button{appearance:none;font-family:var(--font);font-size:.8rem;background:transparent;color:var(--a);border:1px solid var(--ad);padding:.28rem .7rem;cursor:pointer;text-transform:uppercase;letter-spacing:.05em;transition:background .1s,color .1s,border-color .1s;border-radius:0;}
    button:hover{background:rgba(0,224,96,0.12);border-color:#00e060;color:#00e060;}
    button.active{background:#00e060;color:#0a0a0a;border-color:#00e060;}
    button:disabled{opacity:.35;cursor:not-allowed;}
    /* ── Inputs ── */
    input[type="number"],input[type="text"],select{font-family:var(--font);font-size:.8rem;background:var(--bgi);color:var(--a);border:1px solid var(--ad);padding:.22rem .4rem;border-radius:0;outline:none;}
    input:focus,select:focus{border-color:var(--a);}
    /* ── Layouts ── */
    .btn-row{display:flex;flex-wrap:wrap;gap:.3rem;}
    .input-row{display:flex;align-items:center;gap:.45rem;font-size:.8rem;color:#ffd700;margin-top:.4rem;}
    .ctrl-grp{margin-top:.65rem;padding:.45rem .65rem;border:1px solid var(--ab);}
    .ctrl-lbl{font-size:.65rem;letter-spacing:.18em;text-transform:uppercase;color:#00e060;margin-bottom:.3rem;}
    .two-col{display:grid;grid-template-columns:2fr 1fr;gap:.65rem;}
    @media(max-width:860px){.two-col{grid-template-columns:1fr;}}
    /* ── Canvas + scanlines ── */
    .canvas-wrap{position:relative;}
    .canvas-wrap::after{content:'';position:absolute;inset:0;pointer-events:none;background:repeating-linear-gradient(to bottom,transparent 0px,transparent 3px,rgba(0,0,0,.10) 3px,rgba(0,0,0,.10) 4px);}
    canvas{display:block;width:100%;height:clamp(200px,38vw,400px);background:#080808;}
    strong,b{color:#00e060;}
    /* ── Status bar ── */
    .status-bar{display:flex;flex-wrap:wrap;gap:.25rem 1.1rem;font-size:.72rem;color:var(--ad);border-top:1px solid var(--ab);padding-top:.35rem;margin-top:.35rem;}
    .status-bar strong{color:#00e060;}
    /* ── Legend ── */
    .legend{display:flex;flex-wrap:wrap;gap:.35rem;margin:.4rem 0;font-size:.78rem;}
    .legend-item{display:flex;align-items:center;gap:.3rem;border:1px solid var(--ab);padding:.1rem .4rem;}
    .legend-color{width:.55rem;height:.55rem;flex-shrink:0;}
    /* ── Cursor measurements ── */
    .meas-grid{display:grid;gap:.45rem;grid-template-columns:repeat(auto-fit,minmax(160px,1fr));margin-top:.2rem;}
    .meas-card{border:1px solid var(--ab);padding:.45rem .6rem;background:var(--bg);}
    .meas-card h3{font-size:.65rem;letter-spacing:.14em;margin-bottom:.22rem;padding-bottom:.14rem;border-bottom:1px solid var(--ab);}
    .meas-card p{font-size:.78rem;margin:.08rem 0;}
    .delta-summary{font-size:.85rem;color:var(--ah);margin-top:.45rem;}
    #cursor-status{font-size:.75rem;color:var(--ad);margin-top:.3rem;}
    /* ── Signal measurements ── */
    .sig-meas-channels{display:grid;grid-template-columns:repeat(auto-fit,minmax(200px,1fr));gap:.45rem;}
    .sig-meas-card{border:1px solid var(--ab);padding:.45rem .6rem;background:var(--bg);}
    .sig-meas-card h3{font-size:.65rem;letter-spacing:.14em;text-transform:uppercase;margin-bottom:.25rem;padding-bottom:.12rem;border-bottom:1px solid var(--ab);}
    .sig-meas-row{display:flex;justify-content:space-between;font-size:.78rem;padding:.07rem 0;}
    .sig-meas-label{color:var(--ad);}
    .sig-meas-value{color:var(--a);}
    /* ── Meter cards ── */
    .meter-grid{display:grid;gap:.45rem;grid-template-columns:repeat(auto-fit,minmax(220px,1fr));}
    .meter-card{border:1px solid var(--ab);padding:.6rem .7rem;background:var(--bg);}
    .meter-label{font-size:.65rem;letter-spacing:.14em;text-transform:uppercase;color:#00e060;font-weight:bold;}
    .meter-value{font-size:1.85rem;color:var(--ah);line-height:1.2;margin:.12rem 0;}
    .meter-meta{font-size:.72rem;color:var(--ad);}
    /* ── Instrument panels ── */
    .instr-panel{border:1px solid var(--ab);padding:.65rem .75rem;background:var(--bg);}
    .instr-ph{font-size:.68rem;letter-spacing:.14em;text-transform:uppercase;color:var(--ad);margin-bottom:.4rem;padding-bottom:.25rem;border-bottom:1px solid var(--ab);display:flex;justify-content:space-between;}
    .instr-ph>span:first-child{color:#00e060;font-weight:bold;}
    .chip{border:1px solid var(--ad);padding:.08rem .35rem;font-size:.62rem;letter-spacing:.07em;color:var(--ad);}
    .instr-desc{font-size:.75rem;color:var(--ad);margin-bottom:.45rem;}
    /* ── Supply ── */
    .supply-card{border:1px solid var(--ab);padding:.45rem .6rem;background:var(--bg2);margin-top:.45rem;}
    .supply-heading{font-size:.65rem;text-transform:uppercase;letter-spacing:.14em;color:#ffd700;margin-bottom:.3rem;}
    .supply-input-row{display:flex;align-items:center;gap:.35rem;}
    .supply-input-row input{width:82px;}
    .supply-unit{color:#ffd700;font-size:.8rem;}
    .supply-status{font-size:.72rem;color:var(--ad);margin-top:.22rem;}
    /* ── Waveform ── */
    .wave-grid{display:grid;gap:.4rem;margin-top:.45rem;grid-template-columns:repeat(auto-fit,minmax(130px,1fr));}
    .wave-field{display:flex;flex-direction:column;gap:.15rem;}
    .wave-field label{font-size:.65rem;color:#ffd700;text-transform:uppercase;letter-spacing:.07em;}
    .wave-field input{width:100%;}
    .gain-row{display:flex;flex-wrap:wrap;gap:.3rem;margin-top:.45rem;align-items:center;}
    .gain-row>label{font-size:.65rem;color:#ffd700;text-transform:uppercase;letter-spacing:.07em;width:100%;}
    .gain-btn{font-family:var(--font);font-size:.75rem;background:transparent;color:var(--a);border:1px solid var(--ad);padding:.22rem .5rem;cursor:pointer;border-radius:0;text-transform:uppercase;}
    .gain-btn:hover{background:rgba(0,224,96,0.12);border-color:#00e060;color:#00e060;}
    .gain-btn.active{background:#00e060;color:#0a0a0a;border-color:#00e060;}
    .gain-pot-display{font-size:.7rem;color:var(--ad);margin-top:.18rem;}
    .waveform-status,.bode-status{font-size:.72rem;color:var(--ad);margin-top:.28rem;min-height:1em;}
    /* ── Connection pill ── */
    .conn-pill{position:fixed;top:.65rem;left:.65rem;z-index:900;display:flex;align-items:center;gap:.32rem;padding:.18rem .52rem;border:1px solid var(--ab);background:var(--bg);font-size:.66rem;font-family:var(--font);color:var(--ad);text-transform:uppercase;letter-spacing:.04em;pointer-events:none;user-select:none;}
    .conn-dot{width:.42rem;height:.42rem;flex-shrink:0;background:var(--ad);}
    .conn-pill.conn-live{color:#00ff44;border-color:rgba(0,255,68,.3);}
    .conn-pill.conn-live .conn-dot{background:#00ff44;animation:cpulse 2s ease-out infinite;}
    .conn-pill.conn-slow{color:var(--ah);border-color:var(--ab);}
    .conn-pill.conn-slow .conn-dot{background:var(--ah);}
    .conn-pill.conn-dead{color:#ff4444;border-color:rgba(255,68,68,.3);}
    .conn-pill.conn-dead .conn-dot{background:#ff4444;}
    @keyframes cpulse{0%{box-shadow:0 0 0 0 rgba(0,255,68,.6)}70%{box-shadow:0 0 0 5px rgba(0,255,68,0)}100%{box-shadow:0 0 0 0 rgba(0,255,68,0)}}
    body.is-dev .conn-pill{top:calc(.65rem + 1.75rem);}
    /* ── Help button ── */
    .help-btn{position:fixed;bottom:1.2rem;right:1.2rem;z-index:1000;width:2.3rem;height:2.3rem;padding:0;background:#ffd700;border:1px solid #ffd700;color:#0a0a0a;font-family:var(--font);font-size:1rem;font-weight:bold;cursor:pointer;display:flex;align-items:center;justify-content:center;border-radius:0;}
    .help-btn:hover{background:#ffe84d;border-color:#ffe84d;color:#0a0a0a;}
    /* ── Help drawer ── */
    .help-overlay{display:none;position:fixed;inset:0;background:rgba(0,0,0,.6);z-index:1001;}
    .help-overlay.open{display:block;}
    .help-drawer{position:fixed;top:0;right:0;bottom:0;width:min(420px,100vw);background:var(--bg);border-left:1px solid var(--ab);z-index:1002;display:flex;flex-direction:column;transform:translateX(100%);transition:transform .2s ease;}
    .help-drawer.open{transform:translateX(0);}
    .help-drawer-header{display:flex;align-items:center;justify-content:space-between;padding:.6rem .85rem;border-bottom:1px solid var(--ab);flex-shrink:0;}
    .help-drawer-header h2{color:var(--ah);font-size:.8rem;letter-spacing:.2em;}
    .help-close-btn{background:none;border:1px solid var(--ad);color:var(--a);font-family:var(--font);font-size:.9rem;cursor:pointer;padding:.1rem .38rem;border-radius:0;}
    .help-close-btn:hover{background:var(--a);color:var(--bg);border-color:var(--a);}
    .help-tabs{display:flex;overflow-x:auto;border-bottom:1px solid var(--ab);flex-shrink:0;scrollbar-width:none;}
    .help-tabs::-webkit-scrollbar{display:none;}
    .help-tab{flex-shrink:0;border:none;border-right:1px solid var(--ab);background:none;color:var(--ad);font-family:var(--font);font-size:.66rem;letter-spacing:.06em;text-transform:uppercase;padding:.42rem .6rem;border-bottom:2px solid transparent;cursor:pointer;border-radius:0;}
    .help-tab:hover{color:var(--a);background:var(--af);border-color:transparent;border-bottom-color:transparent;}
    .help-tab.active{color:var(--ah);border-bottom-color:var(--ah);}
    .help-content{overflow-y:auto;padding:.85rem 1rem;flex:1;font-size:.8rem;line-height:1.65;color:var(--a);}
    .help-content h2{color:var(--ah);font-size:.88rem;margin:0 0 .38rem;border-bottom:1px solid var(--ab);padding-bottom:.18rem;}
    .help-content h3{color:var(--ad);font-size:.65rem;letter-spacing:.18em;text-transform:uppercase;margin:.9rem 0 .22rem;}
    .help-content p{margin:.22rem 0;}
    .help-content ul,.help-content ol{margin:.22rem 0;padding-left:1.1rem;}
    .help-content li{margin:.1rem 0;}
    .help-content strong{color:#00e060;}
    .help-content table{width:100%;border-collapse:collapse;font-size:.76rem;margin:.4rem 0;}
    .help-content th{color:var(--ad);text-align:left;padding:.2rem .4rem;border-bottom:1px solid var(--ab);font-size:.62rem;letter-spacing:.1em;text-transform:uppercase;}
    .help-content td{padding:.2rem .4rem;border-bottom:1px solid rgba(255,255,255,.07);vertical-align:top;}
    .help-content blockquote{margin:.45rem 0;padding:.32rem .65rem;border-left:2px solid var(--a);background:var(--af);color:var(--ah);font-size:.78rem;}
    .help-content pre{background:var(--bgi);border:1px solid var(--ab);padding:.38rem .62rem;overflow-x:auto;font-size:.75rem;color:var(--ad);margin:.4rem 0;}
    .help-content code{font-family:var(--font);}
    /* ── Dev bar + panel ── */
    .dev-bar{display:none;position:fixed;top:0;left:0;right:0;z-index:1100;height:1.75rem;background:#1a1a1a;color:var(--ah);font-family:var(--font);font-size:.66rem;font-weight:bold;letter-spacing:.1em;text-transform:uppercase;align-items:center;justify-content:center;gap:.5rem;cursor:pointer;user-select:none;border-bottom:1px solid var(--a);}
    .dev-bar.visible{display:flex;}
    .dev-bar-chevron{font-size:.6rem;transition:transform .2s;}
    .dev-bar.panel-open .dev-bar-chevron{transform:rotate(180deg);}
    .dev-panel{position:fixed;top:1.75rem;left:0;bottom:0;z-index:1099;width:270px;background:var(--bg);border-right:1px solid var(--ab);display:flex;flex-direction:column;transform:translateX(-100%);transition:transform .2s ease;overflow:hidden;}
    .dev-panel.open{transform:translateX(0);}
    .dev-panel-header{padding:.42rem .7rem;border-bottom:1px solid var(--ab);font-size:.62rem;font-weight:bold;letter-spacing:.12em;color:var(--ah);text-transform:uppercase;}
    .dev-panel-body{flex:1;overflow-y:auto;padding:.45rem .7rem 1rem;}
    .dev-section-title{font-size:.57rem;font-weight:bold;letter-spacing:.12em;color:var(--ad);text-transform:uppercase;margin:.65rem 0 .25rem;padding-bottom:.15rem;border-bottom:1px solid var(--ab);}
    .dev-row{display:grid;grid-template-columns:1fr 1fr;gap:.22rem .45rem;margin-bottom:.22rem;}
    .dev-row.full{grid-template-columns:1fr;}
    .dev-field{display:flex;flex-direction:column;gap:.08rem;}
    .dev-field label{font-size:.57rem;color:var(--ad);letter-spacing:.04em;white-space:nowrap;}
    .dev-field input,.dev-field select{background:var(--bgi);border:1px solid var(--ad);color:var(--a);font-family:var(--font);font-size:.66rem;padding:.17rem .28rem;width:100%;}
    .dev-field input:focus,.dev-field select:focus{outline:none;border-color:var(--a);}
    .dev-bar,.dev-panel{--a:#ffd700;--ad:#a07800;--ah:#ffe84d;--af:rgba(255,215,0,0.10);--ab:rgba(255,215,0,0.28);}
    @media(max-width:480px){body{padding:.5rem;}}
  </style>
</head>
<body>
  <div class="dev-bar" id="dev-bar">
    &#9964; SELF-HOST DEV MODE
    <span class="dev-bar-chevron" id="dev-bar-chevron">&#9660;</span>
  </div>
  <aside class="dev-panel" id="dev-panel">
    <div class="dev-panel-header">&gt; FAKE DATA CONTROLS</div>
    <div class="dev-panel-body">

      <div class="dev-section-title">Oscilloscope &mdash; CH A</div>
      <div class="dev-row">
        <div class="dev-field">
          <label>Shape</label>
          <select id="dev-chA-shape">
            <option value="sine">Sine</option>
            <option value="square">Square</option>
            <option value="triangle">Triangle</option>
          </select>
        </div>
        <div class="dev-field">
          <label>Frequency (Hz)</label>
          <input id="dev-chA-freq" type="number" min="1" max="30000" step="10" value="1000" />
        </div>
      </div>
      <div class="dev-row">
        <div class="dev-field">
          <label>Amplitude (V pk)</label>
          <input id="dev-chA-amp" type="number" min="0" max="2" step="0.05" value="1.5" />
        </div>
        <div class="dev-field">
          <label>Phase (&deg;)</label>
          <input id="dev-chA-phase" type="number" min="0" max="360" step="1" value="0" />
        </div>
      </div>

      <div class="dev-section-title">Oscilloscope &mdash; CH B</div>
      <div class="dev-row">
        <div class="dev-field">
          <label>Shape</label>
          <select id="dev-chB-shape">
            <option value="sine">Sine</option>
            <option value="square">Square</option>
            <option value="triangle">Triangle</option>
          </select>
        </div>
        <div class="dev-field">
          <label>Frequency (Hz)</label>
          <input id="dev-chB-freq" type="number" min="1" max="30000" step="10" value="1000" />
        </div>
      </div>
      <div class="dev-row">
        <div class="dev-field">
          <label>Amplitude (V pk)</label>
          <input id="dev-chB-amp" type="number" min="0" max="2" step="0.05" value="1.0" />
        </div>
        <div class="dev-field">
          <label>Phase (&deg;)</label>
          <input id="dev-chB-phase" type="number" min="0" max="360" step="1" value="90" />
        </div>
      </div>

      <div class="dev-section-title">Oscilloscope &mdash; Shared</div>
      <div class="dev-row full">
        <div class="dev-field">
          <label>Noise (V rms)</label>
          <input id="dev-noise" type="number" min="0" max="0.5" step="0.005" value="0.02" />
        </div>
      </div>

      <div class="dev-section-title">DMM</div>
      <div class="dev-row">
        <div class="dev-field">
          <label>DC Voltage (V)</label>
          <input id="dev-dmm-voltage" type="number" min="-2" max="2" step="0.01" value="3.3" />
        </div>
        <div class="dev-field">
          <label>Current (A)</label>
          <input id="dev-dmm-current" type="number" min="0" max="5" step="0.001" value="0.15" />
        </div>
      </div>
      <div class="dev-row full">
        <div class="dev-field">
          <label>Resistance (&Omega;)</label>
          <input id="dev-dmm-resistance" type="number" min="0" max="1000000" step="1" value="470" />
        </div>
      </div>

      <div class="dev-section-title">PSU</div>
      <div class="dev-row full">
        <div class="dev-field">
          <label>Supply Voltage (V)</label>
          <input id="dev-psu-voltage" type="number" min="0.64" max="5.25" step="0.01" value="3.3" />
        </div>
      </div>

      <div class="dev-section-title">Bode Plot</div>
      <div class="dev-row">
        <div class="dev-field">
          <label>Filter Type</label>
          <select id="dev-bode-type">
            <option value="lp1">LP 1st order</option>
            <option value="lp2">LP 2nd order</option>
            <option value="hp1">HP 1st order</option>
            <option value="bp2">Band-pass 2nd</option>
          </select>
        </div>
        <div class="dev-field">
          <label>Cutoff (Hz)</label>
          <input id="dev-bode-fc" type="number" min="10" max="5000" step="10" value="500" />
        </div>
      </div>
      <div class="dev-row">
        <div class="dev-field">
          <label>DC Gain (dB)</label>
          <input id="dev-bode-gain" type="number" min="-40" max="20" step="1" value="0" />
        </div>
        <div class="dev-field">
          <label>Q Factor</label>
          <input id="dev-bode-q" type="number" min="0.1" max="10" step="0.1" value="0.707" />
        </div>
      </div>
      <div class="dev-row full">
        <div class="dev-field">
          <label>Noise (dB rms)</label>
          <input id="dev-bode-noise" type="number" min="0" max="5" step="0.1" value="0.3" />
        </div>
      </div>
      <div class="dev-row full">
        <div class="dev-field">
          <button id="dev-bode-inject" style="width:100%;margin-top:.15rem">&#9654; Inject Bode Data</button>
        </div>
      </div>

    </div>
  </aside>

  <div class="tw">
    <div style="padding-top:2.1rem">
      <p style="font-size:.66rem;letter-spacing:.28em;color:var(--ad);text-transform:uppercase">&gt; MADAM3 &mdash; WEB INSTRUMENT v1.1</p>
      <h1 style="font-size:1.35rem;border-bottom:1px solid var(--ab);padding-bottom:.35rem;margin:.08rem 0">ADC STREAM</h1>
      <p style="font-size:.75rem;color:var(--ad);margin-top:.18rem">AD7356 CH A / CH B &mdash; calibrated &plusmn;2&thinsp;V (piecewise linear, cal_20260327)</p>
    </div>

    <div class="ts">
      <div class="ts-head"><span>&gt; OSCILLOSCOPE</span><span>AD7356 DUAL 12-BIT</span></div>
      <div class="ts-body p0">
        <div class="canvas-wrap">
          <canvas id="graph" width="960" height="400"></canvas>
          <canvas id="bode-canvas" width="960" height="400" style="display:none"></canvas>
        </div>
      </div>
    </div>

    <div class="ts">
      <div class="ts-head"><span>&gt; SCOPE CONTROLS</span></div>
      <div class="ts-body">
        <div class="ctrl-grp">
          <p class="ctrl-lbl">Window &mdash; X axis</p>
          <div class="btn-row">
            <button class="window-btn active" id="mode-fast">Fast (60 kHz)</button>
            <button class="window-btn" id="mode-slow">Slow (~170 Hz)</button>
          </div>
          <div class="btn-row" id="fast-window-btns" style="margin-top:.28rem">
            <button class="window-btn" data-window-ms="1">1 ms</button>
            <button class="window-btn" data-window-ms="2">2 ms</button>
            <button class="window-btn active" data-window-ms="4">4 ms</button>
            <button class="window-btn" data-window-ms="8">8 ms</button>
          </div>
          <div class="btn-row" id="slow-window-btns" style="display:none;margin-top:.28rem">
            <button class="window-btn" data-window-ms="500">500 ms</button>
            <button class="window-btn" data-window-ms="1000">1 s</button>
            <button class="window-btn" data-window-ms="2000">2 s</button>
            <button class="window-btn" data-window-ms="3000">3 s</button>
          </div>
          <label class="input-row">
            <span>Custom (ms)</span>
            <input type="number" id="window-custom" min="0.1" max="8.5" step="0.1" value="4.00" style="width:82px" />
            <button id="window-custom-set">Set</button>
          </label>
        </div>
        <div class="ctrl-grp">
          <p class="ctrl-lbl">V Range &mdash; Y axis</p>
          <div class="btn-row">
            <button class="range-btn" data-range="0.5">0.5 V</button>
            <button class="range-btn" data-range="1">1 V</button>
            <button class="range-btn" data-range="2">2 V</button>
            <button class="range-btn active" data-range="4">4 V</button>
          </div>
          <label class="input-row">
            <span>Custom (V)</span>
            <input type="number" id="range-input" step="0.1" min="0.1" value="4" style="width:75px" />
          </label>
        </div>
        <div class="ctrl-grp">
          <p class="ctrl-lbl">Center &mdash; Y position</p>
          <div class="btn-row">
            <button class="center-btn" data-center="-1">-1 V</button>
            <button class="center-btn" data-center="-0.5">-0.5 V</button>
            <button class="center-btn active" data-center="0">0 V</button>
            <button class="center-btn" data-center="0.5">+0.5 V</button>
            <button class="center-btn" data-center="1">+1 V</button>
          </div>
          <label class="input-row">
            <span>Manual (V)</span>
            <input type="number" id="center-input" step="0.1" value="0" style="width:75px" />
          </label>
        </div>
        <div class="ctrl-grp">
          <p class="ctrl-lbl">Acquisition</p>
          <div class="btn-row">
            <button id="toggle-stream">Pause</button>
            <button class="cursor-btn" data-cursor="A">Set Cursor A</button>
            <button class="cursor-btn" data-cursor="B">Set Cursor B</button>
            <button id="clear-cursors">Clear Cursors</button>
          </div>
        </div>
        <div class="ctrl-grp">
          <p class="ctrl-lbl">Trigger</p>
          <div class="btn-row">
            <button id="trigger-toggle">Trigger: Off</button>
            <button class="trig-edge-btn active" data-edge="rising">Rising &#x2191;</button>
            <button class="trig-edge-btn" data-edge="falling">Falling &#x2193;</button>
            <button class="trig-ch-btn active" data-ch="0">CH A</button>
            <button class="trig-ch-btn" data-ch="1">CH B</button>
          </div>
          <label class="input-row">
            <span>Level (V)</span>
            <input type="number" id="trigger-level" step="0.05" value="0.00" style="width:75px" />
          </label>
        </div>
        <div class="ctrl-grp">
          <p class="ctrl-lbl">Bode Plot</p>
          <div class="btn-row">
            <button id="bode-start-btn">Run Bode Sweep</button>
            <button id="bode-back-btn" style="display:none">&#x2190; Back to Scope</button>
          </div>
          <label class="input-row">
            <span>Dwell (ms)</span>
            <input id="bode-dwell" type="number" min="10" max="2000" step="5" value="50" style="width:75px" />
          </label>
          <p class="bode-status" id="bode-status"></p>
        </div>
      </div>
    </div>

    <div class="status-bar">
      <span>Updated: <strong id="last-update">--</strong></span>
      <span>Window: <strong id="window-label">--</strong></span>
      <span>Range: <strong id="range-display">4.00 V</strong></span>
      <span>Center: <strong id="center-display">0.00 V</strong></span>
      <span>Trigger: <strong id="trigger-status">Off</strong></span>
      <span>Stream: <strong id="stream-status">Running</strong></span>
    </div>

    <div id="legend" class="legend"></div>

    <div class="ts">
      <div class="ts-head"><span>&gt; MEASUREMENTS</span></div>
      <div class="ts-body" style="display:grid;grid-template-columns:1fr 1fr;gap:.65rem;">
        <div>
          <p style="font-size:.62rem;letter-spacing:.14em;text-transform:uppercase;color:#00e060;margin-bottom:.3rem;">Cursors</p>
          <p id="cursor-status">Select Cursor A or B, then click the plot to place it.</p>
          <div id="cursor-values" class="meas-grid"></div>
          <div id="delta-summary" class="delta-summary">&#916;t: -- s</div>
        </div>
        <div>
          <p style="font-size:.62rem;letter-spacing:.14em;text-transform:uppercase;color:#00e060;margin-bottom:.3rem;">Signal</p>
          <div class="sig-meas-channels" id="sig-meas-channels"></div>
        </div>
      </div>
    </div>

    <div class="ts">
      <div class="ts-head"><span>&gt; ADS1115 SENSORS</span><span>I&#xB2;C 0x48</span></div>
      <div class="ts-body">
        <div class="meter-grid">
          <div class="meter-card">
            <p class="meter-label">Resistance</p>
            <p class="meter-value" id="ads-ohms">--</p>
            <p class="meter-meta" id="ads-raw0">AIN0: --</p>
            <button class="ads-measure-btn" data-ch="0" style="margin-top:.38rem">Measure</button>
          </div>
          <div class="meter-card">
            <p class="meter-label">Current</p>
            <p class="meter-value" id="ads-amps">--</p>
            <p class="meter-meta" id="ads-raw1">AIN1: --</p>
            <button class="ads-measure-btn" data-ch="1" style="margin-top:.38rem">Measure</button>
          </div>
          <div class="meter-card">
            <p class="meter-label">DC Voltage</p>
            <p class="meter-value" id="ads-dc">--</p>
            <p class="meter-meta" id="ads-raw2">AIN2&#x2012;3: --</p>
            <button class="ads-measure-btn" data-ch="2" style="margin-top:.38rem">Measure</button>
          </div>
        </div>
      </div>
    </div>

    <div class="ts">
      <div class="ts-head"><span>&gt; INSTRUMENT PANEL</span><span>AD9833 + MCP41010</span></div>
      <div class="ts-body">
        <p style="font-size:.72rem;color:var(--ad);margin-bottom:.65rem">SPI power supply and waveform generator controls.</p>
        <div class="two-col">
          <div class="instr-panel">
            <div class="instr-ph"><span>Power Supply</span><span class="chip">MIC24045</span></div>
            <p class="instr-desc">Set output over I&sup2;C. Valid: 0.640&ndash;3.420&thinsp;V and 4.750&ndash;5.250&thinsp;V.</p>
            <div class="supply-card">
              <div class="supply-heading">Supply A</div>
              <div class="supply-input-row">
                <input id="supply-a-input" type="number" step="0.010" min="0.640" max="5.250" value="1.000" />
                <span class="supply-unit">V</span>
                <button id="supply-a-set">Set</button>
              </div>
              <p class="supply-status" id="supply-a-status">Waiting for input&hellip;</p>
            </div>
          </div>
          <div class="instr-panel">
            <div class="instr-ph"><span>Waveform Generator</span><span class="chip">SPI</span></div>
            <p class="instr-desc">Select shape to push to the generator.</p>
            <div class="btn-row">
              <button class="waveform-btn active" data-wave="square">Square</button>
              <button class="waveform-btn" data-wave="sine">Sine</button>
              <button class="waveform-btn" data-wave="triangle">Triangle</button>
            </div>
            <div class="wave-grid">
              <div class="wave-field">
                <label for="waveform-frequency">Frequency (Hz)</label>
                <input id="waveform-frequency" type="number" min="0.1" step="0.1" value="1000" />
              </div>
              <div class="wave-field">
                <label for="waveform-amplitude">Amplitude (Vpp)</label>
                <input id="waveform-amplitude" type="number" min="0" max="5" step="0.1" value="1.0" />
              </div>
              <div class="wave-field">
                <label for="waveform-offset">Offset (V)</label>
                <input id="waveform-offset" type="number" min="-5" max="5" step="0.1" value="0.0" />
              </div>
              <div class="wave-field">
                <label for="waveform-phase">Phase (&deg;)</label>
                <input id="waveform-phase" type="number" min="0" max="360" step="1" value="0" />
              </div>
            </div>
            <div class="gain-row">
              <label>Gain Output</label>
              <button class="gain-btn" data-gain-step="65">0.5 V</button>
              <button class="gain-btn" data-gain-step="165">1.0 V</button>
              <button class="gain-btn" data-gain-step="255">1.7 V</button>
              <p class="gain-pot-display" id="gain-pot-display"></p>
            </div>
            <p class="waveform-status" id="waveform-status">Waiting for selection&hellip;</p>
          </div>
        </div>
      </div>
    </div>

  </div>

  <div class="conn-pill" id="conn-pill">
    <span class="conn-dot" id="conn-dot"></span>
    <span id="conn-label">Connecting&hellip;</span>
  </div>
  <button class="help-btn" id="help-open-btn" title="Help">?</button>
  <div class="help-overlay" id="help-overlay"></div>
  <aside class="help-drawer" id="help-drawer">
    <div class="help-drawer-header">
      <h2>&gt; HELP</h2>
      <button class="help-close-btn" id="help-close-btn">&times;</button>
    </div>
    <div class="help-tabs" id="help-tabs">
      <button class="help-tab active" data-help="0">Home</button>
      <button class="help-tab" data-help="1">Scope</button>
      <button class="help-tab" data-help="2">DC Voltage</button>
      <button class="help-tab" data-help="3">Current</button>
      <button class="help-tab" data-help="4">Resistance</button>
      <button class="help-tab" data-help="5">Wavegen</button>
      <button class="help-tab" data-help="6">PSU</button>
    </div>
    <div class="help-content" id="help-content"></div>
  </aside>

  <script>
    const colors = ["#ff006e", "#00b4d8"];
    const labels = ["CH A", "CH B"];
    const pollIntervalMs  = 50;       // 20 fps display refresh
    const kSampleRateHz   = 60000;    // nominal fast-mode hardware sample rate
    let samplingMode      = "fast";   // "fast" | "slow"
    let effectivePeriodUs = 17;       // inter-sample period sent to /adc/burst
    let maxWindowMs = 4.0;            // capture window in milliseconds
    let maxPoints   = Math.round(maxWindowMs * kSampleRateHz / 1000); // samples
    const series = labels.map(() => []);
    let timestamps = [];
    let displayRange  = 4.0;   // total voltage span shown on Y axis (V)
    let displayCenter = 0.0;   // voltage at vertical centre of plot (V)
    let lastSample = [];
    let isPaused = false;
    let cursorMode = null;
    let triggerEnabled = false;
    let triggerChannel = 0;       // 0 = CH A, 1 = CH B
    let triggerEdge    = "rising"; // "rising" | "falling"
    let triggerLevel   = 0.0;      // voltage threshold (V)
    const cursorPositions = { A: null, B: null }; // { ratio: number } in [0,1]
    let cursorPreview = null; // { key: 'A'|'B', ratio }
    const cursorColors = { A: "#facc15", B: "#38bdf8" };
    let pollHandle = null;
    let dpr = 1;
    let bodeDpr    = 1;
    let bodeMode   = false;
    let bodePoints = [];   // [{i, f, gain, phase}]
    let bodeEvtSrc = null; // active EventSource
    let lastSuccessMs = Date.now();  // updated on every successful ESP32 fetch
    const geometry = { left: 60, right: 20, top: 20, bottom: 70, plotWidth: 0, plotHeight: 0, stepX: 0 };

    const canvas = document.getElementById("graph");
    const ctx = canvas.getContext("2d");
    const legendEl = document.getElementById("legend");
    const lastUpdateEl = document.getElementById("last-update");
    const windowLabelEl = document.getElementById("window-label");
    const rangeDisplayEl = document.getElementById("range-display");
    const centerDisplayEl = document.getElementById("center-display");
    const streamStatusEl = document.getElementById("stream-status");
    const windowButtons = document.querySelectorAll(".window-btn");
    const rangeButtons = document.querySelectorAll(".range-btn");
    const rangeInput = document.getElementById("range-input");
    const centerButtons = document.querySelectorAll(".center-btn");
    const centerInput = document.getElementById("center-input");
    const windowCustomInput = document.getElementById("window-custom");
    const windowCustomSetBtn = document.getElementById("window-custom-set");
    const streamBtn = document.getElementById("toggle-stream");
    const cursorButtons = document.querySelectorAll(".cursor-btn");
    const clearCursorsBtn = document.getElementById("clear-cursors");
    const cursorValuesEl = document.getElementById("cursor-values");
    const cursorStatusEl = document.getElementById("cursor-status");
    const deltaSummaryEl = document.getElementById("delta-summary");
    const sigMeasChannelsEl  = document.getElementById("sig-meas-channels");
    const triggerToggleBtn   = document.getElementById("trigger-toggle");
    const triggerLevelInput  = document.getElementById("trigger-level");
    const triggerEdgeButtons = document.querySelectorAll(".trig-edge-btn");
    const triggerChButtons   = document.querySelectorAll(".trig-ch-btn");
    const triggerStatusEl    = document.getElementById("trigger-status");
    const supplyChannels = [
      {
        input:  document.getElementById("supply-a-input"),
        setBtn: document.getElementById("supply-a-set"),
        status: document.getElementById("supply-a-status")
      }
    ];
    const waveformButtons = document.querySelectorAll(".waveform-btn");
    const waveformStatusEl = document.getElementById("waveform-status");
    const waveformFrequencyEl = document.getElementById("waveform-frequency");
    const waveformAmplitudeEl = document.getElementById("waveform-amplitude");
    const waveformOffsetEl = document.getElementById("waveform-offset");
    const waveformPhaseEl  = document.getElementById("waveform-phase");
    let activeWaveform = "square";

    function clampRatio(value) {
      if (typeof value !== "number" || Number.isNaN(value)) {
        return null;
      }
      return Math.min(1, Math.max(0, value));
    }

    function sampleCount() {
      return series[0]?.length || 0;
    }

    function pointerRatioFromEvent(event) {
      if (!geometry || geometry.plotWidth <= 0) {
        return null;
      }
      const rect = canvas.getBoundingClientRect();
      // Support both mouse and touch events.
      const src = event.touches        ? event.touches[0]
                : event.changedTouches ? event.changedTouches[0]
                : event;
      // rect and clientX/Y are in CSS pixels; geometry is also in CSS pixels
      // (drawGraph scales the context by dpr so all coordinates are CSS-space).
      const x = src.clientX - rect.left;
      const ratio = (x - geometry.left) / geometry.plotWidth;
      if (ratio < 0 || ratio > 1) {
        return null;
      }
      return clampRatio(ratio);
    }

    function dataPositionFromRatio(ratio, length) {
      if (length === 0 || ratio === null || typeof ratio !== "number") {
        return null;
      }
      if (length === 1) {
        return { lower: 0, upper: 0, fraction: 0 };
      }
      const position = ratio * (length - 1);
      const lower = Math.floor(position);
      const upper = Math.min(length - 1, Math.ceil(position));
      const fraction = position - lower;
      return { lower, upper, fraction };
    }

    function interpolateSeries(seriesValues, ratio) {
      const count = seriesValues.length;
      const pos = dataPositionFromRatio(ratio, count);
      if (!pos) {
        return null;
      }
      if (pos.upper === pos.lower) {
        return seriesValues[pos.lower];
      }
      const lowerValue = seriesValues[pos.lower];
      const upperValue = seriesValues[pos.upper];
      if (typeof lowerValue !== "number" || typeof upperValue !== "number") {
        return null;
      }
      return lowerValue + pos.fraction * (upperValue - lowerValue);
    }

    function valueAtRatio(channelIdx, ratio) {
      if (ratio === null) {
        return null;
      }
      const baseValue = interpolateSeries(series[channelIdx], ratio);
      if (typeof baseValue !== "number") {
        return null;
      }
      return baseValue;
    }

    function timestampAtRatio(ratio) {
      const pos = dataPositionFromRatio(ratio, timestamps.length);
      if (!pos) {
        return null;
      }
      if (pos.upper === pos.lower) {
        return timestamps[pos.lower];
      }
      const lowerTime = timestamps[pos.lower];
      const upperTime = timestamps[pos.upper];
      if (typeof lowerTime !== "number" || typeof upperTime !== "number") {
        return null;
      }
      return lowerTime + pos.fraction * (upperTime - lowerTime);
    }

    function formatCursorTime(ms) {
      if (typeof ms !== "number") {
        return "--";
      }
      const date = new Date(ms);
      const time = date.toLocaleTimeString([], { hour12: false });
      const msPart = String(date.getMilliseconds()).padStart(3, "0");
      return `${time}.${msPart}`;
    }

    // ── Connection status indicator ───────────────────────────────────────────
    const connPill  = document.getElementById("conn-pill");
    const connLabel = document.getElementById("conn-label");

    function markConnected() {
      lastSuccessMs = Date.now();
    }

    function updateConnectionStatus() {
      const gapMs = Date.now() - lastSuccessMs;
      connPill.classList.remove("conn-live", "conn-slow", "conn-dead");
      if (gapMs < 3000) {
        connPill.classList.add("conn-live");
        connLabel.textContent = "Live";
      } else if (gapMs < 6000) {
        connPill.classList.add("conn-slow");
        connLabel.textContent = "Slow";
      } else {
        connPill.classList.add("conn-dead");
        connLabel.textContent = "No signal";
      }
    }

    // Heartbeat: poll /control-state every 2 s regardless of scope/DMM activity.
    setInterval(async () => {
      try {
        const r = await fetch("/control-state");
        if (r.ok) markConnected();
      } catch (_) { /* gap widens — indicator will update on next checker tick */ }
    }, 2000);

    // Checker: evaluate gap and update pill every second.
    setInterval(updateConnectionStatus, 1000);

    function schedulePoll() {
      if (pollHandle) {
        clearTimeout(pollHandle);
      }
      pollHandle = setTimeout(poll, pollIntervalMs);
    }

    function enforceLengthLimit() {
      while (timestamps.length > maxPoints) {
        timestamps.shift();
        series.forEach(channel => channel.shift());
      }
    }

    // Returns the index of the first sample where the signal crosses `level`
    // in the specified direction. Returns -1 if no crossing is found.
    function findTriggerIndex(data, level, edge) {
      for (let i = 1; i < data.length; i++) {
        if (edge === "rising"  && data[i - 1] < level && data[i] >= level) return i - 1;
        if (edge === "falling" && data[i - 1] > level && data[i] <= level) return i - 1;
      }
      return -1;
    }

    // Replace all series data with a fresh burst snapshot.
    // When trigger is active, the data is sliced so the trigger crossing appears
    // at the left edge of the display. If no crossing is found the frame is held.
    // timestamps are real-time offsets so cursor Δt measurements stay accurate.
    function loadBurst(chA, chB) {

      let startIdx = 0;
      if (triggerEnabled) {
        const trigData = triggerChannel === 0 ? chA : chB;
        const idx = findTriggerIndex(trigData, triggerLevel, triggerEdge);
        if (idx < 0) return; // no edge found — hold current frame
        startIdx = idx;
      }

      const sliceA = chA.slice(startIdx, startIdx + maxPoints);
      const sliceB = chB.slice(startIdx, startIdx + maxPoints);
      const n = sliceA.length;
      const intervalMs = effectivePeriodUs / 1000;
      const burstStartMs = Date.now() - (n - 1) * intervalMs;
      timestamps = sliceA.map((_, i) => burstStartMs + i * intervalMs);
      series[0] = sliceA;
      series[1] = sliceB;
      lastSample = [sliceA[sliceA.length - 1], sliceB[sliceB.length - 1]];
      enforceLengthLimit();
      ensureCursorValidity();
    }

    function computeChannelStats(data) {
      if (!data || data.length < 2) return null;
      const n = data.length;
      let max = -Infinity, min = Infinity, sumSq = 0;
      for (let i = 0; i < n; i++) {
        if (data[i] > max) max = data[i];
        if (data[i] < min) min = data[i];
        sumSq += data[i] * data[i];
      }
      const rms = Math.sqrt(sumSq / n);
      const vpp = max - min;
      // Count positive-going crossings of the signal midpoint to estimate frequency.
      const mid = (max + min) / 2;
      let crossings = 0;
      for (let i = 1; i < n; i++) {
        if (data[i - 1] < mid && data[i] >= mid) crossings++;
      }
      const durationSec = (n - 1) * effectivePeriodUs / 1e6;
      const freq = crossings > 0 && durationSec > 0 ? crossings / durationSec : null;
      return { max, min, vpp, rms, freq };
    }

    function renderSignalMeasurements() {
      if (!sigMeasChannelsEl) return;
      const fmtV = v => typeof v === "number" ? v.toFixed(3) + " V" : "--";
      const fmtHz = v => {
        if (v === null || v === undefined) return "--";
        if (v >= 1000) return (v / 1000).toFixed(2) + " kHz";
        return v.toFixed(1) + " Hz";
      };
      sigMeasChannelsEl.innerHTML = labels.map((label, idx) => {
        const stats = computeChannelStats(series[idx]);
        const color = colors[idx];
        const rows = [
          ["Vpp",  stats ? fmtV(stats.vpp)   : "--"],
          ["RMS",  stats ? fmtV(stats.rms)   : "--"],
          ["Max",  stats ? fmtV(stats.max)   : "--"],
          ["Min",  stats ? fmtV(stats.min)   : "--"],
          ["Freq", stats ? fmtHz(stats.freq) : "--"],
        ];
        return `<div class="sig-meas-card">
          <h3 style="color:${color}">${label}</h3>
          ${rows.map(([l, v]) =>
            `<div class="sig-meas-row"><span class="sig-meas-label">${l}</span><span class="sig-meas-value">${v}</span></div>`
          ).join("")}
        </div>`;
      }).join("");
    }

    function updateLegend() {
      const values = lastSample.length ? lastSample : labels.map(() => null);
      legendEl.innerHTML = values
        .map(
          (value, index) => `
            <span class="legend-item">
              <span class="legend-color" style="background:${colors[index]}"></span>
              ${labels[index]}: ${
                typeof value === "number" ? value.toFixed(3) + " V" : "--"
              }
            </span>`
        )
        .join("");
      if (timestamps.length) {
        lastUpdateEl.textContent = new Date(timestamps[timestamps.length - 1]).toLocaleTimeString();
      } else {
        lastUpdateEl.textContent = "--";
      }
    }

    function drawGraph() {
      // All drawing is done in CSS pixel space; ctx is pre-scaled by dpr.
      const cssW = canvas.width / dpr;
      const cssH = canvas.height / dpr;
      const isNarrow = cssW < 420;

      // Update adaptive margins so pointerRatioFromEvent stays in sync.
      geometry.left   = isNarrow ? 44 : 58;
      geometry.right  = 12;
      geometry.top    = 16;
      geometry.bottom = isNarrow ? 40 : 56;

      const left   = geometry.left;
      const right  = geometry.right;
      const top    = geometry.top;
      const bottom = geometry.bottom;
      geometry.plotWidth  = cssW - left - right;
      geometry.plotHeight = cssH - top - bottom;

      ctx.save();
      ctx.scale(dpr, dpr);
      ctx.clearRect(0, 0, cssW, cssH);

      const minVoltage    = displayCenter - displayRange / 2;
      const maxVoltage    = displayCenter + displayRange / 2;
      const verticalRange = displayRange;
      const axisFont      = (isNarrow ? "11px" : "12px") + " 'Courier New',Courier,monospace";

      ctx.strokeStyle = "rgba(232,232,232,0.13)";
      ctx.fillStyle = "#e8e8e8";
      ctx.font = axisFont;
      ctx.textAlign = "right";
      ctx.textBaseline = "middle";
      const divisions = 4;
      for (let i = 0; i <= divisions; i++) {
        const ratio = i / divisions;
        const y = top + ratio * geometry.plotHeight;
        ctx.beginPath();
        ctx.moveTo(left, y);
        ctx.lineTo(left + geometry.plotWidth, y);
        ctx.stroke();
        const valueLabel = (maxVoltage - ratio * verticalRange).toFixed(1);
        ctx.fillText(valueLabel + " V", left - 4, y);
      }

      // X-axis time labels.
      {
        const windowMs     = (maxPoints - 1) * effectivePeriodUs / 1000;
        const tickInterval = calcNiceInterval(windowMs, isNarrow ? 3 : 5);
        const tickY        = top + geometry.plotHeight;
        ctx.strokeStyle    = "rgba(232,232,232,0.22)";
        ctx.fillStyle      = "#e8e8e8";
        ctx.font           = axisFont;
        ctx.textAlign      = "center";
        ctx.textBaseline   = "top";
        ctx.lineWidth      = 1;
        // Guard against infinite loop when windowMs ≈ 0.
        const maxTicks = 20;
        let tMs = 0;
        for (let tick = 0; tick <= maxTicks && tMs <= windowMs * 1.001; tick++) {
          const x   = left + (windowMs > 0 ? tMs / windowMs : 0) * geometry.plotWidth;
          ctx.beginPath();
          ctx.moveTo(x, tickY);
          ctx.lineTo(x, tickY + 4);
          ctx.stroke();
          const lbl = tMs === 0    ? "0"
                    : tMs < 1    ? (tMs * 1000).toFixed(0) + "\u00b5s"
                    : tMs < 1000 ? tMs.toFixed(tMs < 10 ? 2 : 1) + " ms"
                    :              (tMs / 1000).toFixed(tMs < 10000 ? 2 : 1) + " s";
          ctx.fillText(lbl, x, tickY + 6);
          tMs = +(tMs + tickInterval).toFixed(12);
        }
      }

      const pointCount = sampleCount();
      geometry.stepX = pointCount > 1 ? geometry.plotWidth / (pointCount - 1) : geometry.plotWidth;

      if (pointCount >= 2) {
        series.forEach((data, idx) => {
          if (data.length < 2) return;
          ctx.beginPath();
          ctx.lineWidth = 1.5;
          ctx.strokeStyle = colors[idx];
          data.forEach((value, pointIdx) => {
            const normalized = (value - minVoltage) / verticalRange;
            const clamped    = Math.min(1, Math.max(0, normalized));
            const x = left + pointIdx * geometry.stepX;
            const y = top + (1 - clamped) * geometry.plotHeight;
            if (pointIdx === 0) ctx.moveTo(x, y); else ctx.lineTo(x, y);
          });
          ctx.stroke();
        });
      } else if (pointCount === 1) {
        // Render single-point indicators when only one sample is present.
        series.forEach((data, idx) => {
          if (!data.length) return;
          const normalized = (data[0] - minVoltage) / verticalRange;
          const clamped    = Math.min(1, Math.max(0, normalized));
          const x = left;
          const y = top + (1 - clamped) * geometry.plotHeight;
          ctx.fillStyle = colors[idx];
          ctx.beginPath();
          ctx.arc(x, y, 3, 0, Math.PI * 2);
          ctx.fill();
        });
      }

      drawCursorOverlay("A", cursorPositions.A?.ratio, false, isNarrow);
      drawCursorOverlay("B", cursorPositions.B?.ratio, false, isNarrow);
      if (cursorPreview && cursorMode === cursorPreview.key) {
        drawCursorOverlay(cursorPreview.key, cursorPreview.ratio, true, isNarrow);
      }

      // Trigger level line — amber dashed, drawn over signals and cursors.
      if (triggerEnabled) {
        const tMin  = displayCenter - displayRange / 2;
        const tNorm = (triggerLevel - tMin) / displayRange;
        const tY    = top + (1 - Math.min(1, Math.max(0, tNorm))) * geometry.plotHeight;
        ctx.save();
        ctx.strokeStyle = "#fbbf24";
        ctx.lineWidth   = 1;
        ctx.setLineDash([5, 4]);
        ctx.globalAlpha = 0.9;
        ctx.beginPath();
        ctx.moveTo(left, tY);
        ctx.lineTo(left + geometry.plotWidth, tY);
        ctx.stroke();
        ctx.setLineDash([]);
        ctx.globalAlpha = 1;
        const arrow = triggerEdge === "rising" ? "\u2191" : "\u2193";
        ctx.fillStyle = "#fbbf24";
        ctx.font = (isNarrow ? "9px" : "11px") + " 'Courier New',Courier,monospace";
        ctx.textAlign = "right";
        ctx.textBaseline = "bottom";
        ctx.fillText(arrow + " " + triggerLevel.toFixed(2) + " V", left + geometry.plotWidth - 4, tY - 2);
        ctx.restore();
      }

      ctx.restore();
    }

    function drawCursorOverlay(key, ratio, isPreview, isNarrow) {
      const safeRatio = clampRatio(ratio);
      if (safeRatio === null || geometry.plotWidth <= 0) {
        return;
      }
      const color   = cursorColors[key] || "#fff";
      const x       = geometry.left + safeRatio * geometry.plotWidth;
      const yBottom = geometry.top + geometry.plotHeight;
      if (isPreview) {
        ctx.setLineDash([6, 4]);
        ctx.globalAlpha = 0.5;
      } else {
        ctx.setLineDash([8, 6]);
        ctx.globalAlpha = 1;
      }
      ctx.strokeStyle = color;
      ctx.lineWidth = 1.5;
      ctx.beginPath();
      ctx.moveTo(x, geometry.top);
      ctx.lineTo(x, yBottom);
      ctx.stroke();
      ctx.setLineDash([]);
      ctx.fillStyle = color;
      ctx.globalAlpha = isPreview ? 0.7 : 1;
      ctx.font = (isNarrow ? "10px" : "11px") + " 'Courier New',Courier,monospace";
      const label = isPreview ? `${key}?` : key;
      ctx.fillText(label, x + 5, geometry.top + 8);
    }

    function resizeCanvas() {
      dpr = window.devicePixelRatio || 1;
      canvas.width  = Math.round(canvas.offsetWidth  * dpr);
      canvas.height = Math.round(canvas.offsetHeight * dpr);
      drawGraph();
      const bc = document.getElementById("bode-canvas");
      if (bc && bc.offsetWidth > 0) {
        bodeDpr = window.devicePixelRatio || 1;
        bc.width  = Math.round(bc.offsetWidth  * bodeDpr);
        bc.height = Math.round(bc.offsetHeight * bodeDpr);
        if (bodeMode) drawBodePlot();
      }
    }

    function ensureCursorValidity() {
      ["A", "B"].forEach(key => {
        const entry = cursorPositions[key];
        if (!entry || typeof entry.ratio !== "number") {
          cursorPositions[key] = null;
          return;
        }
        const clamped = clampRatio(entry.ratio);
        if (clamped === null) {
          cursorPositions[key] = null;
        } else {
          entry.ratio = clamped;
        }
      });
    }

    function formatValue(value) {
      return typeof value === "number" ? value.toFixed(2) + " V" : "--";
    }

    function getValueForCursor(channelIdx, key) {
      const ratio = cursorPositions[key]?.ratio ?? null;
      return valueAtRatio(channelIdx, ratio);
    }

    function renderMeasurements() {
      const ratioA = cursorPositions.A?.ratio ?? null;
      const ratioB = cursorPositions.B?.ratio ?? null;

      if (
        cursorMode &&
        cursorPreview &&
        cursorPreview.key === cursorMode &&
        typeof cursorPreview.ratio === "number"
      ) {
        const previewTime = timestampAtRatio(cursorPreview.ratio);
        cursorStatusEl.textContent = `Cursor ${cursorMode} preview @ ${formatCursorTime(
          previewTime
        )}. Click to confirm.`;
      } else if (cursorMode) {
        cursorStatusEl.textContent = `Move the pointer over the plot, then click to place Cursor ${cursorMode}.`;
      } else if (!cursorPositions.A && !cursorPositions.B) {
        cursorStatusEl.textContent = "Select Cursor A or B, then click the plot to place it.";
      } else {
        const parts = [];
        ["A", "B"].forEach(key => {
          const ratio = cursorPositions[key]?.ratio;
          if (ratio !== null && ratio !== undefined) {
            const ts = timestampAtRatio(ratio);
            parts.push(`Cursor ${key} @ ${formatCursorTime(ts)}`);
          }
        });
        cursorStatusEl.textContent = parts.join(" • ");
      }

      cursorValuesEl.innerHTML = labels
        .map((label, idx) => {
          const valueA = getValueForCursor(idx, "A");
          const valueB = getValueForCursor(idx, "B");
          const delta = valueA !== null && valueB !== null ? valueB - valueA : null;
          return `
            <div class="meas-card">
              <h3 style="color:${colors[idx]}">${label}</h3>
              <p>A: ${formatValue(valueA)}</p>
              <p>B: ${formatValue(valueB)}</p>
              <p style="color:#ffd700">&#916;V: ${formatValue(delta)}</p>
            </div>
          `;
        })
        .join("");

      if (ratioA !== null && ratioA !== undefined && ratioB !== null && ratioB !== undefined) {
        const timeA = timestampAtRatio(ratioA);
        const timeB = timestampAtRatio(ratioB);
        if (typeof timeA === "number" && typeof timeB === "number") {
          const deltaMs = Math.abs(timeB - timeA);
          const deltaSeconds = deltaMs / 1000;
          const frequency = deltaSeconds > 0 ? 1 / deltaSeconds : null;
          deltaSummaryEl.innerHTML = `<strong>Δt:</strong> ${deltaSeconds.toFixed(3)} s ${
            frequency ? `(<span>${frequency.toFixed(2)} Hz</span>)` : ""
          }`;
        } else {
          deltaSummaryEl.textContent = "Δt: -- s";
        }
      } else {
        deltaSummaryEl.textContent = "Δt: -- s";
      }
    }

    // Returns a "nice" round tick interval for a given data range and max tick count.
    function calcNiceInterval(range, maxDivisions) {
      if (range <= 0 || maxDivisions <= 0) return 1;
      const raw  = range / maxDivisions;
      const mag  = Math.pow(10, Math.floor(Math.log10(raw)));
      const norm = raw / mag;
      const nice = norm < 1.5 ? 1 : norm < 3.5 ? 2 : norm < 7.5 ? 5 : 10;
      return nice * mag;
    }

    function updateWindowLabel() {
      const rateHz   = Math.round(1e6 / effectivePeriodUs);
      const actualMs = (maxPoints - 1) * effectivePeriodUs / 1000;
      const rateStr  = rateHz >= 1000 ? `${(rateHz / 1000).toFixed(0)} kHz` : `${rateHz} Hz`;
      const timeStr  = actualMs >= 1000 ? `${(actualMs / 1000).toFixed(2)} s` : `${actualMs.toFixed(2)} ms`;
      windowLabelEl.textContent = `${timeStr} @ ${rateStr} (${maxPoints} samples)`;
    }

    function setSampleWindowMs(ms) {
      let clamped;
      if (samplingMode === "fast") {
        clamped           = Math.max(0.1, Math.min(8.5, ms));
        effectivePeriodUs = 17;
        maxPoints         = Math.max(2, Math.min(512, Math.round(clamped * kSampleRateHz / 1000)));
        if (windowCustomInput) windowCustomInput.value = clamped.toFixed(2);
      } else {
        clamped           = Math.max(10, Math.min(3000, ms));
        effectivePeriodUs = Math.max(17, Math.round(clamped * 1000 / 512));
        maxPoints         = 512;
        if (windowCustomInput) windowCustomInput.value = clamped.toFixed(0);
      }
      maxWindowMs = clamped;
      windowButtons.forEach(btn => {
        if (!btn.dataset.windowMs) return; // skip mode-toggle buttons
        btn.classList.toggle("active", Math.abs(Number(btn.dataset.windowMs) - clamped) < 0.05);
      });
      updateWindowLabel();
      enforceLengthLimit();
      ensureCursorValidity();
      drawGraph();
      renderMeasurements();
    }

    function setSamplingMode(mode) {
      samplingMode = mode;
      const fastRow = document.getElementById("fast-window-btns");
      const slowRow = document.getElementById("slow-window-btns");
      document.getElementById("mode-fast").classList.toggle("active", mode === "fast");
      document.getElementById("mode-slow").classList.toggle("active", mode === "slow");
      if (mode === "fast") {
        if (fastRow) fastRow.style.display = "";
        if (slowRow) slowRow.style.display = "none";
        if (windowCustomInput) {
          windowCustomInput.min  = "0.1";
          windowCustomInput.max  = "8.5";
          windowCustomInput.step = "0.1";
        }
        setSampleWindowMs(4.0);
      } else {
        if (fastRow) fastRow.style.display = "none";
        if (slowRow) slowRow.style.display = "";
        if (windowCustomInput) {
          windowCustomInput.min  = "10";
          windowCustomInput.max  = "3000";
          windowCustomInput.step = "10";
        }
        setSampleWindowMs(1000);
      }
    }

    function setDisplayRange(value) {
      const v = Math.max(0.1, value);
      displayRange = v;
      if (rangeInput) rangeInput.value = v.toFixed(2);
      rangeButtons.forEach(btn => btn.classList.toggle("active", Number(btn.dataset.range) === v));
      if (rangeDisplayEl) rangeDisplayEl.textContent = v.toFixed(2) + " V";
      updateLegend();
      drawGraph();
      renderMeasurements();
    }

    function setDisplayCenter(value) {
      displayCenter = value;
      if (centerInput) centerInput.value = value.toFixed(2);
      centerButtons.forEach(btn => btn.classList.toggle("active", Number(btn.dataset.center) === value));
      if (centerDisplayEl) centerDisplayEl.textContent = value.toFixed(2) + " V";
      updateLegend();
      drawGraph();
      renderMeasurements();
    }

    function updateStreamState() {
      streamBtn.textContent = isPaused ? "Resume" : "Pause";
      streamStatusEl.textContent = isPaused ? "Paused" : "Running";
    }

    function updateTriggerUI() {
      triggerToggleBtn.textContent = triggerEnabled ? "Trigger: On" : "Trigger: Off";
      triggerToggleBtn.classList.toggle("active", triggerEnabled);
      if (triggerStatusEl) {
        if (triggerEnabled) {
          const arrow = triggerEdge === "rising" ? "\u2191" : "\u2193";
          triggerStatusEl.textContent = `${labels[triggerChannel]} ${arrow} ${triggerLevel.toFixed(2)} V`;
        } else {
          triggerStatusEl.textContent = "Off";
        }
      }
      drawGraph();
    }

    function setCursorMode(mode) {
      cursorMode = mode;
      cursorPreview = null;
      // Disable browser scroll/pan on canvas while a cursor is being placed
      // so that touch drags register as cursor moves rather than page scrolls.
      canvas.style.touchAction = mode ? "none" : "";
      cursorButtons.forEach(btn => btn.classList.toggle("active", btn.dataset.cursor === mode));
      renderMeasurements();
      drawGraph();
    }

    function supplyVoltageInGap(v) {
      return v > 3.420 && v < 4.750;
    }

    async function sendSupplyVoltage(channel, volts) {
      if (channel.status) channel.status.textContent = "Setting\u2026";
      try {
        const response = await fetch(`/power?voltage=${volts.toFixed(3)}`, { method: "POST" });
        const payload = await response.json().catch(() => ({}));
        if (!response.ok) {
          if (channel.status) channel.status.textContent = payload.error || "Error from device.";
          return;
        }
        if (channel.input && typeof payload.voltage === "number") {
          channel.input.value = payload.voltage.toFixed(3);
        }
        const v = typeof payload.voltage === "number" ? payload.voltage.toFixed(3) : "?";
        let msg = `Set to ${v} V`;
        if (payload.powerGood === false) msg += " \u26A0 Not power good";
        if (payload.enabled === false)   msg += " \u26A0 EN pin low";
        if (payload.overcurrent)         msg += " \u26A0 Overcurrent";
        if (payload.thermalWarning)      msg += " \u26A0 Thermal warning";
        if (payload.thermalShutdown)     msg += " \u26A0 Thermal shutdown";
        if (channel.status) channel.status.textContent = msg;
      } catch (err) {
        console.error("Power update failed", err);
        if (channel.status) channel.status.textContent = "Update failed; retry.";
      }
    }

    function bindSupplyChannel(channel) {
      if (!channel.input || !channel.setBtn) return;
      const apply = () => {
        const v = parseFloat(channel.input.value);
        if (Number.isNaN(v)) {
          if (channel.status) channel.status.textContent = "Enter a valid voltage.";
          return;
        }
        if (v < 0.640 || v > 5.250) {
          if (channel.status) channel.status.textContent = "Out of range. Valid: 0.640\u20133.420 V or 4.750\u20135.250 V.";
          return;
        }
        if (supplyVoltageInGap(v)) {
          if (channel.status) channel.status.textContent = "Invalid gap \u2014 no codes between 3.420 and 4.750 V.";
          return;
        }
        sendSupplyVoltage(channel, v);
      };
      channel.setBtn.addEventListener("click", apply);
      channel.input.addEventListener("keydown", e => { if (e.key === "Enter") apply(); });
    }

    function updateWaveformButtons(target) {
      waveformButtons.forEach(btn => btn.classList.toggle("active", btn.dataset.wave === target));
    }

    function clampWaveformFrequency(value) {
      if (typeof value !== "number" || Number.isNaN(value)) return 1000;
      return Math.min(200000, Math.max(0.1, value));
    }

    function clampWaveformAmplitude(value) {
      if (typeof value !== "number" || Number.isNaN(value)) return 1;
      return Math.min(5, Math.max(0, value));
    }

    function clampWaveformOffset(value) {
      if (typeof value !== "number" || Number.isNaN(value)) return 0;
      return Math.min(5, Math.max(-5, value));
    }

    function clampWaveformPhase(value) {
      if (typeof value !== "number" || Number.isNaN(value)) return 0;
      return Math.min(360, Math.max(0, value));
    }

    function getWaveformPayload() {
      const frequency = clampWaveformFrequency(parseFloat(waveformFrequencyEl?.value));
      const amplitude = clampWaveformAmplitude(parseFloat(waveformAmplitudeEl?.value));
      const offset = clampWaveformOffset(parseFloat(waveformOffsetEl?.value));
      const phase = clampWaveformPhase(parseFloat(waveformPhaseEl?.value));
      if (waveformFrequencyEl) waveformFrequencyEl.value = frequency;
      if (waveformAmplitudeEl) waveformAmplitudeEl.value = amplitude;
      if (waveformOffsetEl) waveformOffsetEl.value = offset;
      if (waveformPhaseEl) waveformPhaseEl.value = phase;
      return { frequency, amplitude, offset, phase };
    }

    async function setWaveform(type) {
      activeWaveform = type;
      updateWaveformButtons(type);
      if (waveformStatusEl) {
        waveformStatusEl.textContent = "Sending waveform command...";
      }
      try {
        const payload = getWaveformPayload();
        const params = new URLSearchParams({
          type,
          frequency: payload.frequency.toString(),
          amplitude: payload.amplitude.toString(),
          offset: payload.offset.toString(),
          phase: payload.phase.toString()
        });
        const res = await fetch(`/waveform?${params.toString()}`, { method: "POST" });
        const data = await res.json().catch(() => ({}));
        const confirmedCfg = data.waveform || {};
        const confirmedMode = confirmedCfg.mode || type;
        updateWaveformButtons(confirmedMode);
        if (waveformStatusEl) {
          waveformStatusEl.textContent = `Waveform set to ${confirmedMode}`;
        }
        if (typeof confirmedCfg.frequency === "number" && waveformFrequencyEl) {
          waveformFrequencyEl.value = clampWaveformFrequency(confirmedCfg.frequency);
        }
        if (typeof confirmedCfg.amplitude === "number" && waveformAmplitudeEl) {
          waveformAmplitudeEl.value = clampWaveformAmplitude(confirmedCfg.amplitude);
          document.querySelectorAll(".gain-btn[data-gain-step]").forEach(b => b.classList.remove("active"));
          updatePotDisplay(voltsToPot(confirmedCfg.amplitude));
        }
        if (typeof confirmedCfg.offset === "number" && waveformOffsetEl) {
          waveformOffsetEl.value = clampWaveformOffset(confirmedCfg.offset);
        }
        if (typeof confirmedCfg.phase === "number" && waveformPhaseEl) {
          waveformPhaseEl.value = clampWaveformPhase(confirmedCfg.phase);
        }
      } catch (error) {
        console.error("Waveform request failed", error);
        if (waveformStatusEl) {
          waveformStatusEl.textContent = "Waveform update failed.";
        }
      }
    }

    function applyControlState(state) {
      if (state && Array.isArray(state.supplies)) {
        const v = parseFloat(state.supplies[0]);
        supplyChannels.forEach(channel => {
          if (channel.input && !Number.isNaN(v) && v > 0) {
            channel.input.value = v.toFixed(3);
            if (channel.status) channel.status.textContent = `Last set: ${v.toFixed(3)} V`;
          }
        });
      }
      if (state && state.waveform) {
        activeWaveform = state.waveform.mode || activeWaveform;
        updateWaveformButtons(activeWaveform);
        if (waveformStatusEl) {
          waveformStatusEl.textContent = `Waveform set to ${activeWaveform}`;
        }
        if (waveformFrequencyEl && typeof state.waveform.frequency === "number") {
          waveformFrequencyEl.value = clampWaveformFrequency(state.waveform.frequency);
        }
        if (waveformAmplitudeEl && typeof state.waveform.amplitude === "number") {
          waveformAmplitudeEl.value = clampWaveformAmplitude(state.waveform.amplitude);
        }
        if (waveformOffsetEl && typeof state.waveform.offset === "number") {
          waveformOffsetEl.value = clampWaveformOffset(state.waveform.offset);
        }
        if (waveformPhaseEl && typeof state.waveform.phase === "number") {
          waveformPhaseEl.value = clampWaveformPhase(state.waveform.phase);
        }
      }
    }

    async function fetchControlState() {
      try {
        const response = await fetch("/control-state");
        if (!response.ok) {
          throw new Error("Bad status");
        }
        markConnected();
        const state = await response.json();
        applyControlState(state);
      } catch (error) {
        console.warn("Unable to load control state", error);
      }
    }

    // ── Bode Plot ─────────────────────────────────────────────────────────────

    function drawBodePlot() {
      const bc = document.getElementById("bode-canvas");
      if (!bc) return;
      const bctx = bc.getContext("2d");
      const w = bc.width / bodeDpr;
      const h = bc.height / bodeDpr;

      bctx.save();
      bctx.scale(bodeDpr, bodeDpr);
      bctx.clearRect(0, 0, w, h);

      const L = 58, R = 12, T = 18, B = 34, GAP = 10;
      const plotW  = w - L - R;
      const totalH = h - T - B;
      const magH   = Math.floor((totalH - GAP) * 0.58);
      const phaseH = totalH - GAP - magH;
      const magT   = T;
      const magBot = magT + magH;
      const phaseT = magBot + GAP;

      const F_MIN = 10, F_MAX = 6000;
      const logFMin = Math.log10(F_MIN), logFMax = Math.log10(F_MAX);
      function xF(f) {
        return L + (Math.log10(Math.max(f, F_MIN)) - logFMin) /
               (logFMax - logFMin) * plotW;
      }

      // Auto-scale dB axis from collected data.
      let dbLo = -40, dbHi = 10;
      if (bodePoints.length > 0) {
        const gains = bodePoints.map(p => p.gain).filter(isFinite);
        if (gains.length) {
          dbLo = Math.floor((Math.min(...gains) - 5) / 10) * 10;
          dbHi = Math.ceil( (Math.max(...gains) + 5) / 10) * 10;
          if (dbHi - dbLo < 20) dbHi = dbLo + 20;
        }
      }
      function yDb(db) {
        return magT + magH * (1 - (db - dbLo) / (dbHi - dbLo));
      }
      function yPh(deg) {
        return phaseT + phaseH * (1 - (deg + 180) / 360);
      }

      const GRID  = "rgba(232,232,232,0.12)";
      const GRID0 = "rgba(232,232,232,0.35)";
      const LABEL = "#e8e8e8";
      const C_MAG = "#e8e8e8";
      const C_PH  = "#00ccff";
      const FONT  = `${Math.max(10, Math.min(12, Math.floor(w / 85)))}px 'Courier New',Courier,monospace`;

      // Frequency grid lines + x-axis labels.
      const fTicks = [10, 20, 50, 100, 200, 500, 1000, 2000, 5000];
      bctx.lineWidth = 1;
      bctx.font = FONT;
      fTicks.forEach(f => {
        const x = xF(f);
        bctx.strokeStyle = GRID;
        bctx.beginPath(); bctx.moveTo(x, magT); bctx.lineTo(x, phaseT + phaseH); bctx.stroke();
        bctx.fillStyle = LABEL;
        bctx.textAlign = "center";
        bctx.textBaseline = "top";
        bctx.fillText(f >= 1000 ? (f / 1000) + "k" : String(f), x, phaseT + phaseH + 3);
      });

      // dB grid + labels.
      const dbStep = (dbHi - dbLo) <= 40 ? 10 : 20;
      bctx.textAlign = "right";
      bctx.textBaseline = "middle";
      for (let db = dbLo; db <= dbHi; db += dbStep) {
        const y = yDb(db);
        if (y < magT - 1 || y > magBot + 1) continue;
        bctx.strokeStyle = db === 0 ? GRID0 : GRID;
        bctx.lineWidth   = db === 0 ? 1.5 : 1;
        bctx.beginPath(); bctx.moveTo(L, y); bctx.lineTo(L + plotW, y); bctx.stroke();
        bctx.fillStyle = LABEL;
        bctx.fillText(db + " dB", L - 4, y);
      }

      // Phase grid + labels.
      const phTicks = [-180, -90, 0, 90, 180];
      phTicks.forEach(deg => {
        const y = yPh(deg);
        bctx.strokeStyle = deg === 0 ? GRID0 : GRID;
        bctx.lineWidth   = deg === 0 ? 1.5 : 1;
        bctx.beginPath(); bctx.moveTo(L, y); bctx.lineTo(L + plotW, y); bctx.stroke();
        bctx.fillStyle = LABEL;
        bctx.textAlign = "right";
        bctx.textBaseline = "middle";
        bctx.fillText(deg + "\u00b0", L - 4, y);
      });

      // Panel outlines.
      bctx.strokeStyle = "rgba(232,232,232,0.35)";
      bctx.lineWidth = 1;
      bctx.strokeRect(L, magT, plotW, magH);
      bctx.strokeRect(L, phaseT, plotW, phaseH);

      // Panel labels.
      bctx.font = FONT;
      bctx.textAlign = "left";
      bctx.textBaseline = "top";
      bctx.fillStyle = C_MAG;
      bctx.fillText("Gain (dB)", L + 4, magT + 3);
      bctx.fillStyle = C_PH;
      bctx.fillText("Phase (\u00b0)", L + 4, phaseT + 3);

      // X axis title.
      bctx.fillStyle = LABEL;
      bctx.textAlign = "center";
      bctx.textBaseline = "bottom";
      bctx.fillText("Frequency (Hz)", L + plotW / 2, h - 2);

      if (bodePoints.length < 1) { bctx.restore(); return; }

      // Clip data lines to the plot panels.
      bctx.save();
      bctx.beginPath();
      bctx.rect(L, magT, plotW, phaseT + phaseH - magT);
      bctx.clip();

      // Magnitude line + dots.
      bctx.strokeStyle = C_MAG;
      bctx.lineWidth = 2;
      bctx.lineJoin = "round";
      bctx.beginPath();
      bodePoints.forEach((p, i) => {
        const x = xF(p.f), y = yDb(p.gain);
        i === 0 ? bctx.moveTo(x, y) : bctx.lineTo(x, y);
      });
      bctx.stroke();
      bodePoints.forEach(p => {
        bctx.fillStyle = C_MAG;
        bctx.beginPath();
        bctx.arc(xF(p.f), yDb(p.gain), 2.5, 0, 2 * Math.PI);
        bctx.fill();
      });

      // Phase line + dots.
      bctx.strokeStyle = C_PH;
      bctx.lineWidth = 2;
      bctx.beginPath();
      bodePoints.forEach((p, i) => {
        const x = xF(p.f), y = yPh(p.phase);
        i === 0 ? bctx.moveTo(x, y) : bctx.lineTo(x, y);
      });
      bctx.stroke();
      bodePoints.forEach(p => {
        bctx.fillStyle = C_PH;
        bctx.beginPath();
        bctx.arc(xF(p.f), yPh(p.phase), 2.5, 0, 2 * Math.PI);
        bctx.fill();
      });

      bctx.restore(); // remove clip
      bctx.restore(); // remove scale
    }

    function enterBodeMode() {
      bodeMode = true;
      isPaused = true;
      if (pollHandle) { clearTimeout(pollHandle); pollHandle = null; }
      canvas.style.display = "none";
      const bc = document.getElementById("bode-canvas");
      if (bc) {
        bc.style.display = "";
        bodeDpr = window.devicePixelRatio || 1;
        bc.width  = Math.round(bc.offsetWidth  * bodeDpr);
        bc.height = Math.round(bc.offsetHeight * bodeDpr);
        drawBodePlot();
      }
      const startBtn = document.getElementById("bode-start-btn");
      const backBtn  = document.getElementById("bode-back-btn");
      if (startBtn) startBtn.style.display = "none";
      if (backBtn)  backBtn.style.display  = "";
    }

    function exitBodeMode() {
      bodeMode = false;
      if (bodeEvtSrc) { bodeEvtSrc.close(); bodeEvtSrc = null; }
      const bc = document.getElementById("bode-canvas");
      if (bc) bc.style.display = "none";
      canvas.style.display = "";
      const startBtn = document.getElementById("bode-start-btn");
      const backBtn  = document.getElementById("bode-back-btn");
      if (startBtn) startBtn.style.display = "";
      if (backBtn)  backBtn.style.display  = "none";
      const statusEl = document.getElementById("bode-status");
      if (statusEl) statusEl.textContent = "";
      isPaused = false;
      schedulePoll();
    }

    function startBodeSweep() {
      bodePoints = [];
      enterBodeMode();
      const dwell = Math.max(10, Math.min(2000,
        parseInt(document.getElementById("bode-dwell")?.value ?? "50", 10) || 50));
      const statusEl = document.getElementById("bode-status");
      if (statusEl) statusEl.textContent = "Sweep running\u2026";

      bodeEvtSrc = new EventSource(`/bode/start?dwell_ms=${dwell}`);

      bodeEvtSrc.onmessage = e => {
        try {
          const d = JSON.parse(e.data);
          if (d.done) {
            if (statusEl) statusEl.textContent =
              `Sweep complete \u2014 ${bodePoints.length} points.`;
            bodeEvtSrc.close(); bodeEvtSrc = null;
            return;
          }
          if (typeof d.f === "number" && typeof d.gain === "number") {
            bodePoints.push(d);
            if (statusEl) {
              const fStr = d.f >= 1000
                ? (d.f / 1000).toFixed(2) + " kHz"
                : d.f.toFixed(1) + " Hz";
              statusEl.textContent =
                `Measuring\u2026 ${fStr} (${bodePoints.length}/50)`;
            }
            drawBodePlot();
          }
        } catch (err) { console.error("Bode SSE parse error", err); }
      };

      bodeEvtSrc.onerror = () => {
        if (!bodeEvtSrc) return; // already closed by done handler
        if (statusEl) statusEl.textContent = "Connection error during sweep.";
        bodeEvtSrc.close(); bodeEvtSrc = null;
      };
    }

    // ── End Bode ──────────────────────────────────────────────────────────────

    function initBenchControls() {
      supplyChannels.forEach(bindSupplyChannel);
      waveformButtons.forEach(btn =>
        btn.addEventListener("click", () => setWaveform(btn.dataset.wave))
      );
      [waveformFrequencyEl, waveformAmplitudeEl, waveformOffsetEl].forEach(input => {
        if (!input) return;
        input.addEventListener("change", () => setWaveform(activeWaveform));
      });
      const gainPotDisplayEl = document.getElementById("gain-pot-display");

      // Calibrated linear fit matching firmware: pot = 156.422 * V - 5.183
      function voltsToPot(v) {
        return Math.max(0, Math.min(255, Math.round(156.422 * v - 5.183)));
      }

      function updatePotDisplay(potValue) {
        if (gainPotDisplayEl) gainPotDisplayEl.textContent = `Digipot: ${potValue}`;
      }

      async function sendDigipot(rawValue) {
        const clamped = Math.max(0, Math.min(255, Math.round(rawValue)));
        updatePotDisplay(clamped);
        try {
          await fetch(`/digipot?value=${clamped}`, { method: "POST" });
        } catch (err) {
          console.error("Digipot request failed", err);
        }
      }

      document.querySelectorAll(".gain-btn[data-gain-step]").forEach(btn => {
        btn.addEventListener("click", () => {
          document.querySelectorAll(".gain-btn[data-gain-step]").forEach(b => b.classList.remove("active"));
          btn.classList.add("active");
          sendDigipot(parseInt(btn.dataset.gainStep, 10));
        });
      });
      const bodeStartBtn = document.getElementById("bode-start-btn");
      const bodeBackBtn  = document.getElementById("bode-back-btn");
      if (bodeStartBtn) bodeStartBtn.addEventListener("click", startBodeSweep);
      if (bodeBackBtn)  bodeBackBtn.addEventListener("click", exitBodeMode);
      fetchControlState();
    }

    canvas.addEventListener("mousemove", event => {
      if (!cursorMode) {
        if (cursorPreview) {
          cursorPreview = null;
          drawGraph();
          renderMeasurements();
        }
        return;
      }
      const ratio = pointerRatioFromEvent(event);
      if (ratio === null) {
        if (cursorPreview) {
          cursorPreview = null;
          drawGraph();
          renderMeasurements();
        }
        return;
      }
      cursorPreview = { key: cursorMode, ratio };
      drawGraph();
      renderMeasurements();
    });

    canvas.addEventListener("mouseleave", () => {
      if (cursorPreview) {
        cursorPreview = null;
        drawGraph();
        renderMeasurements();
      }
    });

    canvas.addEventListener("click", event => {
      if (!cursorMode) {
        return;
      }
      const ratio = pointerRatioFromEvent(event);
      if (ratio === null) {
        return;
      }
      cursorPositions[cursorMode] = { ratio };
      cursorPreview = null;
      setCursorMode(null);
    });

    // Touch support for cursor placement on mobile.
    canvas.addEventListener("touchstart", event => {
      if (!cursorMode) return;
      event.preventDefault();
      const ratio = pointerRatioFromEvent(event);
      if (ratio === null) return;
      cursorPreview = { key: cursorMode, ratio };
      drawGraph();
      renderMeasurements();
    }, { passive: false });

    canvas.addEventListener("touchmove", event => {
      if (!cursorMode) return;
      event.preventDefault();
      const ratio = pointerRatioFromEvent(event);
      if (ratio === null) {
        if (cursorPreview) { cursorPreview = null; drawGraph(); renderMeasurements(); }
        return;
      }
      cursorPreview = { key: cursorMode, ratio };
      drawGraph();
      renderMeasurements();
    }, { passive: false });

    canvas.addEventListener("touchend", event => {
      if (!cursorMode) return;
      event.preventDefault();
      // event.touches is empty on touchend so pointerRatioFromEvent returns null.
      // Use the last preview position set by touchstart/touchmove instead.
      const ratio = cursorPreview?.ratio ?? null;
      if (ratio === null) return;
      cursorPositions[cursorMode] = { ratio };
      cursorPreview = null;
      setCursorMode(null);
    }, { passive: false });

    const modeFastBtn = document.getElementById("mode-fast");
    const modeSlowBtn = document.getElementById("mode-slow");
    if (modeFastBtn) modeFastBtn.addEventListener("click", () => setSamplingMode("fast"));
    if (modeSlowBtn) modeSlowBtn.addEventListener("click", () => setSamplingMode("slow"));

    windowButtons.forEach(btn => {
      if (!btn.dataset.windowMs) return; // skip mode-toggle buttons (no data-window-ms)
      btn.addEventListener("click", () => setSampleWindowMs(Number(btn.dataset.windowMs)));
    });

    if (windowCustomSetBtn) {
      windowCustomSetBtn.addEventListener("click", () => {
        const ms = parseFloat(windowCustomInput?.value);
        if (!Number.isNaN(ms)) setSampleWindowMs(ms);
      });
    }
    if (windowCustomInput) {
      windowCustomInput.addEventListener("keydown", e => {
        if (e.key === "Enter") {
          const ms = parseFloat(windowCustomInput.value);
          if (!Number.isNaN(ms)) setSampleWindowMs(ms);
        }
      });
    }

    rangeButtons.forEach(btn =>
      btn.addEventListener("click", () => setDisplayRange(Number(btn.dataset.range)))
    );
    if (rangeInput) {
      rangeInput.addEventListener("input", () => {
        const v = parseFloat(rangeInput.value);
        if (!Number.isNaN(v) && v > 0) setDisplayRange(v);
      });
    }

    centerButtons.forEach(btn =>
      btn.addEventListener("click", () => setDisplayCenter(Number(btn.dataset.center)))
    );
    if (centerInput) {
      centerInput.addEventListener("input", () => {
        const v = parseFloat(centerInput.value);
        if (!Number.isNaN(v)) setDisplayCenter(v);
      });
    }

    streamBtn.addEventListener("click", () => {
      isPaused = !isPaused;
      updateStreamState();
      if (!isPaused) {
        schedulePoll();
      }
    });

    cursorButtons.forEach(btn =>
      btn.addEventListener("click", () => {
        const target = btn.dataset.cursor;
        setCursorMode(cursorMode === target ? null : target);
      })
    );

    clearCursorsBtn.addEventListener("click", () => {
      cursorPositions.A = null;
      cursorPositions.B = null;
      cursorPreview = null;
      setCursorMode(null);
    });

    triggerToggleBtn.addEventListener("click", () => {
      triggerEnabled = !triggerEnabled;
      updateTriggerUI();
    });

    triggerEdgeButtons.forEach(btn =>
      btn.addEventListener("click", () => {
        triggerEdge = btn.dataset.edge;
        triggerEdgeButtons.forEach(b => b.classList.toggle("active", b.dataset.edge === triggerEdge));
        updateTriggerUI();
      })
    );

    triggerChButtons.forEach(btn =>
      btn.addEventListener("click", () => {
        triggerChannel = Number(btn.dataset.ch);
        triggerChButtons.forEach(b => b.classList.toggle("active", Number(b.dataset.ch) === triggerChannel));
        updateTriggerUI();
      })
    );

    if (triggerLevelInput) {
      triggerLevelInput.addEventListener("input", () => {
        const v = parseFloat(triggerLevelInput.value);
        if (!Number.isNaN(v)) {
          triggerLevel = v;
          updateTriggerUI();
        }
      });
    }

    async function poll() {
      if (isPaused) {
        schedulePoll();
        return;
      }
      try {
        // When trigger is active, request up to 2× maxPoints so there is enough
        // pre-trigger data to reliably find the crossing edge.
        const burstN = triggerEnabled ? Math.min(512, maxPoints * 2) : maxPoints;
        const response = await fetch(`/adc/burst?n=${burstN}&period_us=${effectivePeriodUs}`);
        const payload = await response.json();
        if (Array.isArray(payload.chA) && Array.isArray(payload.chB)) {
          markConnected();
          loadBurst(payload.chA, payload.chB);
          updateWindowLabel();
          updateLegend();
          renderMeasurements();
          renderSignalMeasurements();
          drawGraph();
        }
      } catch (error) {
        console.error("ADC fetch failed", error);
      } finally {
        schedulePoll();
      }
    }

    // ADS1115 manual measurement — one channel at a time, on button press.
    // ch=0: AIN0 Resistance, ch=1: AIN1 Current, ch=2: AIN2-3 Diff DC Voltage
    const adsValueEls = [
      document.getElementById("ads-ohms"),
      document.getElementById("ads-amps"),
      document.getElementById("ads-dc"),
    ];
    const adsRawEls = [
      document.getElementById("ads-raw0"),
      document.getElementById("ads-raw1"),
      document.getElementById("ads-raw2"),
    ];
    const adsUnits  = ["\u03A9", "A", "V"];
    const adsLabels = ["AIN0", "AIN1", "AIN2\u20123"];

    document.querySelectorAll(".ads-measure-btn").forEach(btn => {
      btn.addEventListener("click", async () => {
        const ch = parseInt(btn.dataset.ch, 10);
        const valueEl = adsValueEls[ch];
        const rawEl   = adsRawEls[ch];
        btn.disabled = true;
        btn.textContent = "Measuring\u2026";
        if (valueEl) valueEl.textContent = "\u2026";
        try {
          const response = await fetch(`/ads/measure?ch=${ch}`);
          if (!response.ok) throw new Error("Bad status");
          markConnected();
          const data = await response.json();
          if (valueEl) {
            if (ch === 0) {
              if (data.isOC) {
                valueEl.textContent = "O.C.";
              } else {
                valueEl.textContent = data.value >= 1000
                  ? (data.value / 1000).toFixed(2) + " k\u03A9"
                  : data.value.toFixed(1) + " \u03A9";
              }
            } else {
              valueEl.textContent = data.value.toFixed(3) + " " + adsUnits[ch];
            }
          }
          if (rawEl) rawEl.textContent = adsLabels[ch] + ": " + data.rawAvg.toFixed(1);
        } catch (e) {
          console.warn("ADS measure failed", e);
          if (valueEl) valueEl.textContent = "Error";
        }
        btn.disabled = false;
        btn.textContent = "Measure";
      });
    });

    function init() {
      if (rangeDisplayEl)  rangeDisplayEl.textContent  = displayRange.toFixed(2) + " V";
      if (centerDisplayEl) centerDisplayEl.textContent = displayCenter.toFixed(2) + " V";
      resizeCanvas();   // sets dpr, sizes canvas buffer, draws first frame
      updateLegend();
      updateWindowLabel();
      updateStreamState();
      updateTriggerUI();
      renderMeasurements();
      renderSignalMeasurements();
      initBenchControls();
      poll();
    }

    window.addEventListener("resize", resizeCanvas);
    init();

    // ── Dev mode (localhost only) ─────────────────────────────────────────────
    (function () {
      if (window.location.hostname !== "localhost") return;

      // Show bar, push conn-pill down.
      const devBar   = document.getElementById("dev-bar");
      const devPanel = document.getElementById("dev-panel");
      document.body.classList.add("is-dev");
      devBar.classList.add("visible");

      // Toggle panel open/close when the bar is clicked.
      devBar.addEventListener("click", () => {
        const open = devPanel.classList.toggle("open");
        devBar.classList.toggle("panel-open", open);
      });

      // Load current config from the mock server on startup.
      fetch("/dev/config").then(r => r.json()).then(cfg => {
        const sa = cfg.scope?.chA || {};
        const sb = cfg.scope?.chB || {};
        const dm = cfg.dmm || {};
        const ps = cfg.psu || {};
        if (sa.shape)     document.getElementById("dev-chA-shape").value = sa.shape;
        if (sa.frequency) document.getElementById("dev-chA-freq").value  = sa.frequency;
        if (sa.amplitude) document.getElementById("dev-chA-amp").value   = sa.amplitude;
        if (sa.phase !== undefined) document.getElementById("dev-chA-phase").value = sa.phase;
        if (sb.shape)     document.getElementById("dev-chB-shape").value = sb.shape;
        if (sb.frequency) document.getElementById("dev-chB-freq").value  = sb.frequency;
        if (sb.amplitude) document.getElementById("dev-chB-amp").value   = sb.amplitude;
        if (sb.phase !== undefined) document.getElementById("dev-chB-phase").value = sb.phase;
        if (cfg.scope?.noise !== undefined) document.getElementById("dev-noise").value = cfg.scope.noise;
        if (dm.voltage    !== undefined) document.getElementById("dev-dmm-voltage").value    = dm.voltage;
        if (dm.current    !== undefined) document.getElementById("dev-dmm-current").value    = dm.current;
        if (dm.resistance !== undefined) document.getElementById("dev-dmm-resistance").value = dm.resistance;
        if (ps.voltage    !== undefined) document.getElementById("dev-psu-voltage").value    = ps.voltage;
      }).catch(() => {});

      // Post a partial config update when any input changes.
      function postDevConfig(patch) {
        fetch("/dev/config", {
          method: "POST",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify(patch),
        }).catch(() => {});
      }

      // Wire each input to the corresponding config key.
      const bindings = [
        ["dev-chA-shape", patch => ({ scope: { chA: { shape:     patch } } })],
        ["dev-chA-freq",  patch => ({ scope: { chA: { frequency: parseFloat(patch) } } })],
        ["dev-chA-amp",   patch => ({ scope: { chA: { amplitude: parseFloat(patch) } } })],
        ["dev-chA-phase", patch => ({ scope: { chA: { phase:     parseFloat(patch) } } })],
        ["dev-chB-shape", patch => ({ scope: { chB: { shape:     patch } } })],
        ["dev-chB-freq",  patch => ({ scope: { chB: { frequency: parseFloat(patch) } } })],
        ["dev-chB-amp",   patch => ({ scope: { chB: { amplitude: parseFloat(patch) } } })],
        ["dev-chB-phase", patch => ({ scope: { chB: { phase:     parseFloat(patch) } } })],
        ["dev-noise",     patch => ({ scope: { noise: parseFloat(patch) } })],
        ["dev-dmm-voltage",    patch => ({ dmm: { voltage:    parseFloat(patch) } })],
        ["dev-dmm-current",    patch => ({ dmm: { current:    parseFloat(patch) } })],
        ["dev-dmm-resistance", patch => ({ dmm: { resistance: parseFloat(patch) } })],
        ["dev-psu-voltage",    patch => ({ psu: { voltage:    parseFloat(patch) } })],
      ];
      bindings.forEach(([id, toPatch]) => {
        const el = document.getElementById(id);
        if (!el) return;
        el.addEventListener("change", () => postDevConfig(toPatch(el.value)));
      });

      // ── Bode plot injector ────────────────────────────────────────────────────
      document.getElementById("dev-bode-inject").addEventListener("click", () => {
        const type   = document.getElementById("dev-bode-type").value;
        const fc     = parseFloat(document.getElementById("dev-bode-fc").value)    || 500;
        const dcGain = parseFloat(document.getElementById("dev-bode-gain").value)  || 0;
        const Q      = parseFloat(document.getElementById("dev-bode-q").value)     || 0.707;
        const noise  = parseFloat(document.getElementById("dev-bode-noise").value) || 0;
        const K      = Math.pow(10, dcGain / 20);

        // Box-Muller normal sample, scaled to noise rms
        function rn(scale) {
          const u = Math.random() || 1e-10, v = Math.random();
          return Math.sqrt(-2 * Math.log(u)) * Math.cos(2 * Math.PI * v) * scale;
        }

        const N = 50, fMin = 10, fMax = 6000;
        bodePoints = [];
        for (let i = 0; i < N; i++) {
          const f = fMin * Math.pow(fMax / fMin, i / (N - 1));
          const r = f / fc;
          let gain, phase;

          if (type === "lp1") {
            gain  = 20 * Math.log10(K / Math.sqrt(1 + r * r));
            phase = -Math.atan(r) * 180 / Math.PI;
          } else if (type === "hp1") {
            gain  = 20 * Math.log10(K * r / Math.sqrt(1 + r * r));
            phase = 90 - Math.atan(r) * 180 / Math.PI;
          } else if (type === "lp2") {
            const re = 1 - r * r, im = r / Q;
            gain  = 20 * Math.log10(K / Math.sqrt(re * re + im * im));
            phase = -Math.atan2(im, re) * 180 / Math.PI;
          } else {                          // bp2
            const re = 1 - r * r, im = r / Q;
            gain  = 20 * Math.log10(K * (r / Q) / Math.sqrt(re * re + im * im));
            phase = Math.atan2(re, im) * 180 / Math.PI;
          }

          if (noise > 0) { gain += rn(noise); phase += rn(noise * 8); }
          bodePoints.push({ f, gain, phase });
        }

        enterBodeMode();
        drawBodePlot();
        const statusEl = document.getElementById("bode-status");
        if (statusEl) statusEl.textContent =
          `[DEV] ${type.toUpperCase()} \u2014 fc=${fc}Hz, Q=${Q}, ${N} pts injected.`;
      });
    })();

    // ── Help drawer ───────────────────────────────────────────────────────────
    const helpPages = [
      // 0: Home
      `<h2>MADAM3 Quick Start</h2>
      <p>Welcome to the MADAM3 bench instrument. This device provides four tools:</p>
      <ul>
        <li><strong>Oscilloscope</strong> &mdash; View and measure AC waveforms on two channels</li>
        <li><strong>DMM</strong> &mdash; Measure DC voltage, current, and resistance</li>
        <li><strong>Waveform Generator</strong> &mdash; Output sine, square, and triangle waves</li>
        <li><strong>Power Supply</strong> &mdash; Adjustable and fixed voltage rails for your circuits</li>
      </ul>
      <h3>Before You Begin</h3>
      <ol>
        <li>Connect a jumper wire from any <strong>GND</strong> pin on the MADAM3 to your breadboard&rsquo;s ground rail. Do this before making any other connections.</li>
        <li>All measurements and outputs share this common ground. Skipping this step will produce incorrect readings or damage components.</li>
      </ol>
      <h3>Pin Layout (Top to Bottom)</h3>
      <table>
        <tr><th>Location</th><th>Pins</th><th>Purpose</th></tr>
        <tr><td>Top (BNC)</td><td>Channel A, Channel B</td><td>Oscilloscope inputs</td></tr>
        <tr><td>Top (header)</td><td>GND, GND, Output</td><td>Waveform generator</td></tr>
        <tr><td>Upper header</td><td>V_adj, GND, GND, &minus;5V</td><td>Power supply</td></tr>
        <tr><td>Middle header</td><td>NC, NC, Current+, Current&minus;, GND, R out</td><td>DMM (current &amp; resistance)</td></tr>
        <tr><td>Bottom header</td><td>NC, NC, V+, R in (+5V), GND, V&minus;</td><td>DMM (voltage) / +5V rail</td></tr>
      </table>`,

      // 1: Oscilloscope
      `<h2>Oscilloscope</h2>
      <p>Use the oscilloscope to view and measure <strong>AC signals</strong>. For DC voltage measurements, use the DMM tab instead.</p>
      <h3>Pins Used</h3>
      <table>
        <tr><th>Pin</th><th>Location</th><th>Function</th></tr>
        <tr><td><strong>Channel A</strong> (BNC)</td><td>Top left</td><td>Scope input &mdash; channel A</td></tr>
        <tr><td><strong>Channel B</strong> (BNC)</td><td>Top right</td><td>Scope input &mdash; channel B</td></tr>
      </table>
      <p>Both channels are sampled simultaneously by the AD7356 dual 12-bit ADC at up to 60&thinsp;kHz.</p>
      <h3>Wiring</h3>
      <ol>
        <li>Connect <strong>Channel A</strong> probe to the node you want to observe (typically V_in).</li>
        <li>Connect <strong>Channel B</strong> probe to the second node (typically V_out).</li>
        <li>Clip the probe ground leads to your breadboard ground rail.</li>
      </ol>
      <h3>Common Measurements</h3>
      <p><strong>Amplitude</strong> &mdash; Read peak or peak-to-peak voltage directly from the display.</p>
      <p><strong>Phase shift</strong> &mdash; Measure &Delta;t between matching peaks on Ch A and Ch B, then:</p>
      <blockquote>&Delta;&phi; = 360 &times; &Delta;t &times; f</blockquote>
      <p><strong>Period (T)</strong> &mdash; Time between two consecutive peaks. Frequency = 1&thinsp;/&thinsp;T.</p>
      <p><strong>Time constant (&tau;)</strong> &mdash; Time for output to reach 63% of final value (charging) or decay to 37% (discharging).</p>
      <p><strong>AC RMS Voltage</strong> &mdash; Displayed automatically for each channel.</p>
      <h3>Tips</h3>
      <ul>
        <li>Adjust the <strong>timebase</strong> if the waveform looks too compressed or stretched.</li>
        <li>Use <strong>Slow mode</strong> (up to 3&thinsp;s window) for low-frequency transient signals.</li>
      </ul>`,

      // 2: DMM Voltage
      `<h2>DMM &mdash; DC Voltage</h2>
      <p>Use this mode to measure DC voltage between two points in your circuit.</p>
      <h3>Pins Used</h3>
      <table>
        <tr><th>Pin</th><th>Location</th><th>Function</th></tr>
        <tr><td><strong>V+</strong></td><td>Bottom header, pin 3</td><td>Differential voltage input (+)</td></tr>
        <tr><td><strong>V&minus;</strong></td><td>Bottom header, pin 6</td><td>Differential voltage input (&minus;)</td></tr>
      </table>
      <blockquote>V+ and V&minus; are <strong>measurement inputs only</strong> &mdash; they do not output any voltage. Do not use these pins to power your circuit.</blockquote>
      <h3>Wiring</h3>
      <ol>
        <li>Connect <strong>V+</strong> to the higher-potential node in your circuit.</li>
        <li>Connect <strong>V&minus;</strong> to the lower-potential node (often your ground rail).</li>
        <li>Read the DC voltage from this screen.</li>
      </ol>
      <h3>Measuring Across a Component</h3>
      <p>Place <strong>V+</strong> on one side and <strong>V&minus;</strong> on the other side of the component &mdash; not to ground.</p>
      <h3>Verifying Your Power Supply</h3>
      <ol>
        <li>Connect <strong>V+</strong> to the V_adj output wire.</li>
        <li>Connect <strong>V&minus;</strong> to GND.</li>
        <li>Record the reading &mdash; this is your true supply voltage.</li>
      </ol>`,

      // 3: DMM Current
      `<h2>DMM &mdash; Current</h2>
      <p>Use this mode to measure DC current through a branch of your circuit. Current is always measured <strong>in series</strong> &mdash; the MADAM3 must be inserted into the current path.</p>
      <h3>Pins Used</h3>
      <table>
        <tr><th>Pin</th><th>Location</th><th>Function</th></tr>
        <tr><td><strong>Current+</strong></td><td>Middle header, pin 3</td><td>Connect to the more positive side</td></tr>
        <tr><td><strong>Current&minus;</strong></td><td>Middle header, pin 4</td><td>Connect to the more negative side</td></tr>
      </table>
      <h3>Wiring</h3>
      <ol>
        <li><strong>Break</strong> the circuit at the point where you want to measure current.</li>
        <li>Connect <strong>Current+</strong> to the side closer to the positive voltage source.</li>
        <li>Connect <strong>Current&minus;</strong> to the side closer to ground.</li>
        <li>The circuit is now complete through the MADAM3&rsquo;s internal sense path.</li>
        <li>Read the current from this screen.</li>
      </ol>
      <h3>Example</h3>
      <pre><code>Before:  ... &mdash;&mdash; R_L &mdash;&mdash; ...
After:   ... &mdash; Current+ &mdash; MADAM3 &mdash; Current&minus; &mdash; R_L &mdash; ...</code></pre>
      <h3>Safety</h3>
      <ul>
        <li>Supports up to <strong>&plusmn;5&thinsp;A</strong>.</li>
        <li><strong>Never</strong> connect Current+ and Current&minus; directly across a voltage source &mdash; near-short-circuit.</li>
        <li>Only insert into a branch that already has a load limiting the current.</li>
      </ul>`,

      // 4: DMM Resistance
      `<h2>DMM &mdash; Resistance</h2>
      <p>Use this mode to measure the resistance of a component. The component <strong>must be disconnected</strong> from your circuit before measuring.</p>
      <h3>Pins Used</h3>
      <table>
        <tr><th>Pin</th><th>Location</th><th>Function</th></tr>
        <tr><td><strong>R+</strong></td><td>Bottom header, pin 4 (R in / +5V)</td><td>5&thinsp;V source &mdash; connect to one leg</td></tr>
        <tr><td><strong>R out</strong></td><td>Middle header, pin 6</td><td>Sense input &mdash; connect to the other leg</td></tr>
      </table>
      <h3>Wiring</h3>
      <ol>
        <li><strong>Remove</strong> the component from your breadboard circuit.</li>
        <li>Connect <strong>R+</strong> to one leg of the component.</li>
        <li>Connect the other leg to <strong>R out</strong>.</li>
        <li>Read the resistance from this screen.</li>
      </ol>
      <h3>What You Can Measure</h3>
      <ul>
        <li>Fixed resistors &mdash; verify nominal values before building</li>
        <li>Potentiometers &mdash; at minimum and maximum settings</li>
        <li>Photoresistors &mdash; under different lighting conditions</li>
        <li>Thermistors &mdash; at different temperatures</li>
        <li>Inductor DC resistance (winding resistance, not inductance)</li>
      </ul>
      <blockquote><strong>Always disconnect</strong> the component first. Measuring in-circuit gives incorrect results because other components provide parallel current paths.</blockquote>
      <h3>R in (+5V) as a Power Rail</h3>
      <p>When not measuring resistance, <strong>R in (+5V)</strong> provides a fixed +5&thinsp;V source for op amp positive rails or logic gate V<sub>CC</sub>.</p>`,

      // 5: Wavegen
      `<h2>Waveform Generator</h2>
      <p>Use the waveform generator to output AC signals into your circuit. Generated by the AD9833 DDS chip over SPI.</p>
      <h3>Pins Used</h3>
      <table>
        <tr><th>Pin</th><th>Location</th><th>Function</th></tr>
        <tr><td><strong>Output</strong></td><td>Top header, pin 3</td><td>Signal output</td></tr>
        <tr><td><strong>GND</strong></td><td>Top header, pins 1&ndash;2</td><td>Signal return / ground reference</td></tr>
      </table>
      <h3>Wiring</h3>
      <ol>
        <li>Connect the <strong>Output</strong> pin to V_in of your circuit.</li>
        <li>Connect one of the adjacent <strong>GND</strong> pins to your breadboard ground rail.</li>
      </ol>
      <h3>Common Lab Settings</h3>
      <table>
        <tr><th>Lab</th><th>Waveform</th><th>Frequency</th><th>Amplitude</th></tr>
        <tr><td>RC/RL Transients</td><td>Square</td><td>20&thinsp;Hz</td><td>1&thinsp;V</td></tr>
        <tr><td>AC Signals</td><td>Sine</td><td>30&thinsp;Hz</td><td>2&thinsp;V</td></tr>
        <tr><td>Op Amp</td><td>Sine</td><td>30&thinsp;Hz</td><td>0.5&thinsp;V</td></tr>
        <tr><td>Op Amp Integrator</td><td>Sine / Square / Triangle</td><td>30&thinsp;Hz</td><td>0.5&thinsp;V</td></tr>
        <tr><td>Frequency Response</td><td>Sine</td><td>1&ndash;1200&thinsp;Hz</td><td>2&thinsp;V</td></tr>
      </table>
      <blockquote>The waveform generator <strong>does not support DC offset</strong>. All waveforms are centered around 0&thinsp;V.</blockquote>
      <h3>Tips</h3>
      <ul>
        <li>Always use the GND pins adjacent to Output &mdash; do not rely on a GND connection elsewhere on the board.</li>
        <li>When sweeping frequencies, adjust the oscilloscope timebase after each frequency change.</li>
      </ul>`,

      // 6: Power Supply
      `<h2>Power Supply</h2>
      <p>The MADAM3 provides three voltage rails for powering your circuits.</p>
      <h3>Pins Used</h3>
      <table>
        <tr><th>Pin</th><th>Location</th><th>Function</th></tr>
        <tr><td><strong>V_adj</strong></td><td>Upper header, pin 1</td><td>Adjustable: 0.64&ndash;5.25&thinsp;V</td></tr>
        <tr><td><strong>&minus;5V</strong></td><td>Upper header, pin 4</td><td>Fixed &minus;5&thinsp;V output</td></tr>
        <tr><td><strong>R in (+5V)</strong></td><td>Bottom header, pin 4</td><td>Fixed +5&thinsp;V output</td></tr>
        <tr><td><strong>GND</strong></td><td>Multiple locations</td><td>Common ground (use any GND pin)</td></tr>
      </table>
      <blockquote>The <strong>V+</strong> and <strong>V&minus;</strong> pins on the bottom header are DMM measurement inputs only &mdash; they do not output voltage.</blockquote>
      <h3>V_adj &mdash; Adjustable Supply</h3>
      <ol>
        <li>Connect <strong>V_adj</strong> to the positive rail of your breadboard.</li>
        <li>Connect any <strong>GND</strong> pin to the breadboard ground rail.</li>
        <li>Set the voltage on this screen and enable the output.</li>
      </ol>
      <h3>&minus;5V &mdash; Fixed Negative Rail</h3>
      <p>Fixed &minus;5&thinsp;V output. No configuration needed. Connect to op amp V&minus; or V<sub>EE</sub> pin.</p>
      <h3>R in (+5V) &mdash; Fixed +5V Rail</h3>
      <p>Shared with the resistance checker. Use for op amp positive supply or logic gate V<sub>CC</sub> when not measuring resistance.</p>
      <h3>Op Amp Wiring Summary</h3>
      <table>
        <tr><th>What</th><th>DC Input Labs</th><th>AC Input Labs</th></tr>
        <tr><td>V_in (signal)</td><td>V_adj (e.g. 0.5&thinsp;V)</td><td>Waveform Generator Output</td></tr>
        <tr><td>Op amp + rail</td><td>R in (+5V)</td><td>V_adj (set to 5.0&thinsp;V)</td></tr>
        <tr><td>Op amp &minus; rail</td><td>&minus;5V</td><td>&minus;5V</td></tr>
      </table>
      <blockquote><strong>WARNING:</strong> Connecting +5&thinsp;V and &minus;5&thinsp;V to the wrong op amp pins will destroy the chip. Double-check polarity before enabling the supply.</blockquote>`
    ];

    (function () {
      const drawer   = document.getElementById("help-drawer");
      const overlay  = document.getElementById("help-overlay");
      const openBtn  = document.getElementById("help-open-btn");
      const closeBtn = document.getElementById("help-close-btn");
      const content  = document.getElementById("help-content");
      const tabs     = document.querySelectorAll(".help-tab");

      let activeTab = 0;

      function showPage(idx) {
        activeTab = idx;
        content.innerHTML = helpPages[idx];
        content.scrollTop = 0;
        tabs.forEach((t, i) => t.classList.toggle("active", i === idx));
      }

      function openDrawer()  { drawer.classList.add("open"); overlay.classList.add("open"); }
      function closeDrawer() { drawer.classList.remove("open"); overlay.classList.remove("open"); }

      openBtn.addEventListener("click", openDrawer);
      closeBtn.addEventListener("click", closeDrawer);
      overlay.addEventListener("click", closeDrawer);
      tabs.forEach((tab, i) => tab.addEventListener("click", () => showPage(i)));

      showPage(0);
    })();
  </script>
</body>
</html>
)HTML";
