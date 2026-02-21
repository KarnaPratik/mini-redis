const BASE_URL = "http://localhost:18080";
const MAX_LOG = 12;

// ── Result display ──
function showResult(id, text, type = "ok") {
    const el = document.getElementById(id);
    el.innerHTML = `<div class="result-inner ${type}">&gt; ${text}</div>`;
    el.classList.add("visible");
}

// ── Activity log ──
function logActivity(op, detail) {
    const container = document.getElementById("log-entries");
    const empty = container.querySelector(".log-empty");
    if (empty) empty.remove();

    const now = new Date().toTimeString().slice(0, 8);
    const entry = document.createElement("div");
    entry.className = "log-entry";
    entry.innerHTML = `
        <span class="log-op-tag op-${op.toLowerCase()}">${op}</span>
        <span class="log-detail">${detail}</span>
        <span class="log-time">${now}</span>
    `;
    container.prepend(entry);

    // Trim to max
    const entries = container.querySelectorAll(".log-entry");
    if (entries.length > MAX_LOG) entries[entries.length - 1].remove();
}

function clearLog() {
    const container = document.getElementById("log-entries");
    container.innerHTML = `<div class="log-empty">No activity yet</div>`;
}

// ── SET ──
function setKeyValue() {
    const key   = document.getElementById("set-key").value.trim();
    const value = document.getElementById("set-value").value.trim();
    const ttl   = document.getElementById("set-ttl").value;

    if (!key || !value) {
        showResult("set-result", "key and value are required", "err");
        return;
    }

    let payload = { key, value };
    if (ttl) payload.ttl = parseInt(ttl);

    fetch(`${BASE_URL}/set`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(payload)
    })
    .then(res => res.text())
    .then(data => {
        showResult("set-result", data, "ok");
        logActivity("SET", `${key} = "${value}"${ttl ? ` · ttl=${ttl}s` : ""}`);
        getAllKeys();
        getStats();
    })
    .catch(err => showResult("set-result", err, "err"));
}

// ── GET ──
function getValue() {
    const key = document.getElementById("get-key").value.trim();
    if (!key) { showResult("get-result", "key is required", "err"); return; }

    fetch(`${BASE_URL}/get?key=${encodeURIComponent(key)}`)
    .then(res => res.text())
    .then(data => {
        const isErr = data.toLowerCase().includes("not found");
        showResult("get-result", data, isErr ? "err" : "info");
        logActivity("GET", `${key} → ${isErr ? "MISS" : `"${data}"`}`);
        getStats();
    })
    .catch(err => showResult("get-result", err, "err"));
}

// ── DELETE ──
function deleteKey() {
    const key = document.getElementById("del-key").value.trim();
    if (!key) { showResult("del-result", "key is required", "err"); return; }

    fetch(`${BASE_URL}/delete?key=${encodeURIComponent(key)}`)
    .then(res => res.text())
    .then(data => {
        showResult("del-result", data, "ok");
        logActivity("DEL", key);
        getAllKeys();
        getStats();
    })
    .catch(err => showResult("del-result", err, "err"));
}

// ── KEYS ──
function getAllKeys() {
    const btn = document.querySelector(".refresh-btn");
    if (btn) btn.classList.add("spinning");

    fetch(`${BASE_URL}/keys`)
    .then(res => res.json())
    .then(keys => {
        const ul    = document.getElementById("keys-list");
        const empty = document.getElementById("keys-empty");
        ul.innerHTML = "";

        if (btn) btn.classList.remove("spinning");

        if (keys && Array.isArray(keys) && keys.length > 0) {
            empty.style.display = "none";
            keys.forEach(k => {
                const li = document.createElement("li");
                li.textContent = k;
                ul.appendChild(li);
            });
        } else {
            empty.style.display = "flex";
        }
    })
    .catch(() => { if (btn) btn.classList.remove("spinning"); });
}

// ── STATS ──
function getStats() {
    fetch(`${BASE_URL}/stats`)
    .then(res => res.json())
    .then(data => {
        const total   = data.total_requests || 0;
        const hits    = data.hits   || 0;
        const misses  = data.misses || 0;
        const hitRate = total > 0 ? ((hits / total) * 100).toFixed(1) : "0.0";

        animateValue("stat-total",     total);
        animateValue("stat-hits",      hits);
        animateValue("stat-misses",    misses);
        animateValue("stat-evictions", data.evictions || 0);
        animateValue("stat-keys",      data.current_keys || 0);

        document.getElementById("stat-hitrate").textContent = hitRate + "%";
        document.getElementById("hit-bar").style.width      = hitRate + "%";

        const now = new Date().toTimeString().slice(0, 8);
        document.getElementById("last-updated").textContent = now;
    })
    .catch(err => console.log("Stats error:", err));
}

// ── Animate number change ──
function animateValue(id, newVal) {
    const el = document.getElementById(id);
    const prev = parseInt(el.textContent) || 0;
    if (prev === newVal) return;

    const diff = newVal - prev;
    const steps = 12;
    const stepVal = diff / steps;
    let current = prev;
    let step = 0;

    const timer = setInterval(() => {
        step++;
        current += stepVal;
        el.textContent = Math.round(current);
        if (step >= steps) {
            clearInterval(timer);
            el.textContent = newVal;
        }
    }, 30);
}

// ── Enter key support ──
document.addEventListener("DOMContentLoaded", () => {
    document.getElementById("set-ttl").addEventListener("keydown",   e => e.key === "Enter" && setKeyValue());
    document.getElementById("set-value").addEventListener("keydown", e => e.key === "Enter" && setKeyValue());
    document.getElementById("get-key").addEventListener("keydown",   e => e.key === "Enter" && getValue());
    document.getElementById("del-key").addEventListener("keydown",   e => e.key === "Enter" && deleteKey());
});

// ── Init ──
window.onload = () => {
    getAllKeys();
    getStats();
    setInterval(getStats, 5000);
};