#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <climits>

using namespace std;
using namespace std::chrono;

struct SortMetrics {
    string name;
    long long comparisons = 0;
    long long swaps = 0;
    double timeMs = 0.0;
};

void merge(vector<int>& arr, int left, int mid, int right, SortMetrics& m) {
    int n1 = mid - left + 1, n2 = right - mid;
    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        m.comparisons++;
        if (L[i] <= R[j]) { arr[k] = L[i]; i++; }
        else { arr[k] = R[j]; j++; }
        m.swaps++; k++;
    }
    while (i < n1) { arr[k] = L[i]; m.swaps++; i++; k++; }
    while (j < n2) { arr[k] = R[j]; m.swaps++; j++; k++; }
}

void mergeSortHelper(vector<int>& arr, int left, int right, SortMetrics& m) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortHelper(arr, left, mid, m);
        mergeSortHelper(arr, mid + 1, right, m);
        merge(arr, left, mid, right, m);
    }
}

void mergeSort(vector<int>& arr, SortMetrics& m) {
    m.name = "Merge Sort";
    auto start = high_resolution_clock::now();
    mergeSortHelper(arr, 0, (int)arr.size() - 1, m);
    auto end = high_resolution_clock::now();
    m.timeMs = duration<double, milli>(end - start).count();
}

void heapify(vector<int>& arr, int n, int i, SortMetrics& m) {
    int largest = i, left = 2 * i + 1, right = 2 * i + 2;
    if (left < n) { m.comparisons++; if (arr[left] > arr[largest]) largest = left; }
    if (right < n) { m.comparisons++; if (arr[right] > arr[largest]) largest = right; }
    if (largest != i) { swap(arr[i], arr[largest]); m.swaps++; heapify(arr, n, largest, m); }
}

void heapSort(vector<int>& arr, SortMetrics& m) {
    m.name = "Heap Sort";
    int n = (int)arr.size();
    auto start = high_resolution_clock::now();
    for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i, m);
    for (int i = n - 1; i > 0; i--) { swap(arr[0], arr[i]); m.swaps++; heapify(arr, i, 0, m); }
    auto end = high_resolution_clock::now();
    m.timeMs = duration<double, milli>(end - start).count();
}

int partition(vector<int>& arr, int low, int high, SortMetrics& m) {
    int mid = low + (high - low) / 2;
    m.comparisons++; if (arr[mid] < arr[low]) { swap(arr[low], arr[mid]); m.swaps++; }
    m.comparisons++; if (arr[high] < arr[low]) { swap(arr[low], arr[high]); m.swaps++; }
    m.comparisons++; if (arr[mid] < arr[high]) { swap(arr[mid], arr[high]); m.swaps++; }
    int pivot = arr[high], i = low - 1;
    for (int j = low; j <= high - 1; j++) {
        m.comparisons++;
        if (arr[j] < pivot) { i++; swap(arr[i], arr[j]); m.swaps++; }
    }
    swap(arr[i + 1], arr[high]); m.swaps++;
    return i + 1;
}

void quickSortHelper(vector<int>& arr, int low, int high, SortMetrics& m) {
    if (low < high) {
        int pi = partition(arr, low, high, m);
        quickSortHelper(arr, low, pi - 1, m);
        quickSortHelper(arr, pi + 1, high, m);
    }
}

void quickSort(vector<int>& arr, SortMetrics& m) {
    m.name = "Quick Sort";
    auto start = high_resolution_clock::now();
    quickSortHelper(arr, 0, (int)arr.size() - 1, m);
    auto end = high_resolution_clock::now();
    m.timeMs = duration<double, milli>(end - start).count();
}

vector<int> parseJsonArray(const string& body) {
    vector<int> arr;
    size_t s = body.find('['), e = body.find(']');
    if (s == string::npos || e == string::npos) return arr;
    stringstream ss(body.substr(s + 1, e - s - 1));
    string tok;
    while (getline(ss, tok, ',')) {
        try { arr.push_back(stoi(tok)); } catch (...) {}
    }
    return arr;
}

string buildJson(const vector<int>& orig, const vector<int>& sorted,
                 const SortMetrics& mg, const SortMetrics& hp, const SortMetrics& qk,
                 const string& best) {
    stringstream s;
    s << fixed << setprecision(4);
    s << "{\"original\":[";
    for (int i = 0; i < (int)orig.size(); i++) { if (i) s << ","; s << orig[i]; }
    s << "],\"sorted\":[";
    for (int i = 0; i < (int)sorted.size(); i++) { if (i) s << ","; s << sorted[i]; }
    s << "],";
    auto w = [&](const string& k, const SortMetrics& m) {
        s << "\"" << k << "\":{\"name\":\"" << m.name
          << "\",\"comparisons\":" << m.comparisons
          << ",\"swaps\":" << m.swaps
          << ",\"timeMs\":" << m.timeMs << "}";
    };
    w("merge", mg); s << ","; w("heap", hp); s << ","; w("quick", qk);
    s << ",\"best\":\"" << best << "\"}";
    return s.str();
}

const string HTML_PAGE = R"RAWHTML(<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Smart Sorting Studio</title>
<script src="https://cdn.jsdelivr.net/npm/chart.js@4.4.0/dist/chart.umd.min.js"></script>
<style>
@import url('https://fonts.googleapis.com/css2?family=Inter:wght@300;400;500;600;700;800;900&family=JetBrains+Mono:wght@400;600&display=swap');
*{margin:0;padding:0;box-sizing:border-box}
:root{--bg:#0a0e17;--card:#1a2332;--card2:#141d2b;--text:#e8edf5;--dim:#8b95a8;--cyan:#06d6a0;--purple:#a855f7;--orange:#f59e0b;--blue:#3b82f6;--pink:#ec4899;--red:#ef4444;--border:rgba(255,255,255,0.06)}
body{font-family:'Inter',sans-serif;background:var(--bg);color:var(--text);min-height:100vh;overflow-x:hidden}
body::before{content:'';position:fixed;inset:0;background:radial-gradient(ellipse at 20% 20%,rgba(6,214,160,0.07) 0%,transparent 50%),radial-gradient(ellipse at 80% 80%,rgba(168,85,247,0.07) 0%,transparent 50%);z-index:-1}
.container{max-width:1100px;margin:0 auto;padding:30px 24px}
.hero{text-align:center;padding:40px 20px 24px}
.hero h1{font-size:2.6rem;font-weight:900;background:linear-gradient(135deg,#06d6a0,#3b82f6,#a855f7);-webkit-background-clip:text;background-clip:text;-webkit-text-fill-color:transparent;letter-spacing:-1px;margin-bottom:6px}
.hero .sub{font-size:0.9rem;color:var(--dim);letter-spacing:3px;text-transform:uppercase}

.input-card{background:var(--card);border:1px solid var(--border);border-radius:16px;padding:32px;margin-bottom:24px;text-align:center}
.input-card label{display:block;font-size:0.85rem;color:var(--dim);margin-bottom:12px;text-transform:uppercase;letter-spacing:1px;font-weight:600}
.input-row{display:flex;gap:12px;max-width:700px;margin:0 auto 16px}
.input-row input{flex:1;padding:14px 20px;border-radius:10px;border:1px solid var(--border);background:var(--card2);color:var(--text);font-family:'JetBrains Mono',monospace;font-size:0.95rem;outline:none;transition:border 0.3s}
.input-row input:focus{border-color:var(--cyan)}
.input-row input::placeholder{color:rgba(139,149,168,0.5)}
.btn{padding:14px 28px;border-radius:10px;border:none;font-family:'Inter',sans-serif;font-size:0.9rem;font-weight:700;cursor:pointer;transition:all 0.3s;text-transform:uppercase;letter-spacing:1px}
.btn-sort{background:linear-gradient(135deg,var(--cyan),var(--blue));color:#0a0e17}
.btn-sort:hover{transform:translateY(-2px);box-shadow:0 6px 20px rgba(6,214,160,0.3)}
.btn-sort:active{transform:translateY(0)}
.btn-rand{background:var(--card2);color:var(--dim);border:1px solid var(--border);padding:14px 20px}
.btn-rand:hover{border-color:var(--purple);color:var(--purple)}
.btn-row{display:flex;justify-content:center;gap:10px}
.helper{font-size:0.78rem;color:rgba(139,149,168,0.6);margin-top:10px}

.results{display:none;animation:fadeUp 0.6s ease}
@keyframes fadeUp{from{opacity:0;transform:translateY(20px)}to{opacity:1;transform:translateY(0)}}

.winner{background:linear-gradient(135deg,rgba(6,214,160,0.08),rgba(59,130,246,0.08));border:1px solid rgba(6,214,160,0.25);border-radius:16px;padding:36px;text-align:center;margin-bottom:20px;position:relative;overflow:hidden}
.winner::before{content:'';position:absolute;top:-50%;left:-50%;width:200%;height:200%;background:radial-gradient(circle,rgba(6,214,160,0.04) 0%,transparent 60%);animation:glow 4s ease-in-out infinite}
@keyframes glow{0%,100%{transform:translate(0,0)}50%{transform:translate(3%,3%)}}
.winner .trophy{font-size:2.6rem;margin-bottom:8px}
.winner h2{font-size:1.7rem;font-weight:800;color:var(--cyan);margin-bottom:6px}
.winner p{color:var(--dim);font-size:0.9rem}

.stats{display:grid;grid-template-columns:repeat(3,1fr);gap:16px;margin-bottom:20px}
.stat{background:var(--card);border:1px solid var(--border);border-radius:14px;padding:22px;text-align:center;position:relative;overflow:hidden;transition:transform 0.3s}
.stat:hover{transform:translateY(-2px)}
.stat::before{content:'';position:absolute;top:0;left:0;right:0;height:3px}
.stat:nth-child(1)::before{background:linear-gradient(90deg,var(--cyan),var(--blue))}
.stat:nth-child(2)::before{background:linear-gradient(90deg,var(--purple),var(--pink))}
.stat:nth-child(3)::before{background:linear-gradient(90deg,var(--orange),var(--red))}
.stat .alg{font-size:0.75rem;text-transform:uppercase;letter-spacing:2px;color:var(--dim);margin-bottom:8px;font-weight:600}
.stat .val{font-size:1.8rem;font-weight:800;font-family:'JetBrains Mono',monospace;margin-bottom:2px}
.stat:nth-child(1) .val{color:var(--cyan)}
.stat:nth-child(2) .val{color:var(--purple)}
.stat:nth-child(3) .val{color:var(--orange)}
.stat .lbl{font-size:0.72rem;color:var(--dim)}

.charts{display:grid;grid-template-columns:repeat(2,1fr);gap:20px;margin-bottom:20px}
.chart-card{background:var(--card);border:1px solid var(--border);border-radius:14px;padding:24px}
.chart-card h3{font-size:0.82rem;font-weight:600;margin-bottom:16px;color:var(--dim);text-transform:uppercase;letter-spacing:1px}
.chart-card canvas{max-height:260px}

.card{background:var(--card);border:1px solid var(--border);border-radius:14px;padding:24px;margin-bottom:20px;transition:transform 0.3s}
.card:hover{transform:translateY(-1px)}
.card h2{font-size:1.1rem;font-weight:700;margin-bottom:14px}

table{width:100%;border-collapse:collapse;font-size:0.85rem}
th{padding:10px 16px;text-align:left;font-weight:600;color:var(--dim);border-bottom:1px solid var(--border);text-transform:uppercase;font-size:0.7rem;letter-spacing:1.5px}
td{padding:12px 16px;border-bottom:1px solid var(--border);font-family:'JetBrains Mono',monospace;font-size:0.82rem}
tr:hover td{background:rgba(255,255,255,0.02)}
.best-row td{color:var(--cyan);font-weight:600}
.dot{display:inline-block;width:8px;height:8px;border-radius:50%;margin-right:6px}

.arr-display{font-family:'JetBrains Mono',monospace;font-size:0.8rem;color:var(--dim);background:rgba(0,0,0,0.3);padding:12px 16px;border-radius:8px;line-height:1.7;word-break:break-all;max-height:100px;overflow-y:auto}
.arr-display::-webkit-scrollbar{width:5px}
.arr-display::-webkit-scrollbar-track{background:transparent}
.arr-display::-webkit-scrollbar-thumb{background:rgba(255,255,255,0.1);border-radius:3px}

.footer{text-align:center;padding:30px;color:var(--dim);font-size:0.75rem}

.loading{display:none;text-align:center;padding:40px}
.spinner{display:inline-block;width:36px;height:36px;border:3px solid var(--border);border-top-color:var(--cyan);border-radius:50%;animation:spin 0.8s linear infinite}
@keyframes spin{to{transform:rotate(360deg)}}

@media(max-width:768px){
  .hero h1{font-size:1.8rem}
  .stats,.charts{grid-template-columns:1fr}
  .input-row{flex-direction:column}
}
</style>
</head>
<body>
<div class="container">
  <div class="hero">
    <h1>Smart Sorting Studio</h1>
    <div class="sub">Performance Analysis Tool</div>
  </div>

  <div class="input-card">
    <label>Enter array elements</label>
    <div class="input-row">
      <input type="text" id="arrayInput" placeholder="e.g. 45, 12, 89, 3, 67, 23, 1, 56" autofocus>
      <button class="btn btn-sort" onclick="sortArray()">Sort</button>
    </div>
    <div class="btn-row">
      <button class="btn btn-rand" onclick="randomArray(10)">Random 10</button>
      <button class="btn btn-rand" onclick="randomArray(20)">Random 20</button>
      <button class="btn btn-rand" onclick="randomArray(50)">Random 50</button>
    </div>
    <div class="helper">Separate numbers with commas or spaces</div>
  </div>

  <div class="loading" id="loading"><div class="spinner"></div></div>

  <div class="results" id="results">
    <div class="winner" id="winner"></div>
    <div class="stats" id="stats"></div>
    <div class="charts">
      <div class="chart-card"><h3>Comparisons</h3><canvas id="compChart"></canvas></div>
      <div class="chart-card"><h3>Swaps / Moves</h3><canvas id="swapChart"></canvas></div>
      <div class="chart-card"><h3>Execution Time</h3><canvas id="timeChart"></canvas></div>
      <div class="chart-card"><h3>Overall Comparison</h3><canvas id="radarChart"></canvas></div>
    </div>
    <div class="card" id="tableCard"></div>
    <div class="card" id="arraysCard"></div>
  </div>

  <div class="footer">Smart Sorting Studio &bull; DAA Project &bull; Merge Sort &middot; Heap Sort &middot; Quick Sort</div>
</div>

<script>
let charts = [];

function randomArray(n) {
  const arr = Array.from({length: n}, () => Math.floor(Math.random() * 999) + 1);
  document.getElementById('arrayInput').value = arr.join(', ');
}

async function sortArray() {
  const raw = document.getElementById('arrayInput').value;
  const elements = raw.split(/[,\s]+/).map(s => parseInt(s.trim())).filter(n => !isNaN(n));
  if (elements.length === 0) { alert('Please enter at least one number.'); return; }

  document.getElementById('loading').style.display = 'block';
  document.getElementById('results').style.display = 'none';

  try {
    const res = await fetch('/sort', {
      method: 'POST',
      headers: {'Content-Type': 'application/json'},
      body: JSON.stringify({elements})
    });
    const data = await res.json();
    document.getElementById('loading').style.display = 'none';
    renderResults(data);
  } catch (err) {
    document.getElementById('loading').style.display = 'none';
    alert('Error connecting to server.');
  }
}

function renderResults(d) {
  document.getElementById('results').style.display = 'block';

  document.getElementById('winner').innerHTML =
    '<div class="trophy">&#127942;</div>' +
    '<h2>' + d.best + ' Wins!</h2>' +
    '<p>Fewest comparisons among all three algorithms for this dataset.</p>';

  const algos = [d.merge, d.heap, d.quick];
  document.getElementById('stats').innerHTML = algos.map(a =>
    '<div class="stat"><div class="alg">' + a.name + '</div>' +
    '<div class="val">' + a.comparisons + '</div>' +
    '<div class="lbl">comparisons</div></div>'
  ).join('');

  const colors = ['#06d6a0','#a855f7','#f59e0b'];
  const labels = ['Merge Sort','Heap Sort','Quick Sort'];
  const minComp = Math.min(...algos.map(a=>a.comparisons));

  let tableHTML = '<h2>Detailed Metrics</h2><table><thead><tr>' +
    '<th>Algorithm</th><th>Comparisons</th><th>Swaps/Moves</th><th>Time (ms)</th><th>Status</th>' +
    '</tr></thead><tbody>';
  algos.forEach((a,i) => {
    const best = a.comparisons === minComp;
    tableHTML += '<tr' + (best?' class="best-row"':'') + '>' +
      '<td><span class="dot" style="background:'+colors[i]+'"></span>'+a.name+'</td>' +
      '<td>'+a.comparisons+'</td><td>'+a.swaps+'</td>' +
      '<td>'+a.timeMs.toFixed(4)+'</td>' +
      '<td>'+(best?'Best':'-')+'</td></tr>';
  });
  tableHTML += '</tbody></table>';
  document.getElementById('tableCard').innerHTML = tableHTML;

  document.getElementById('arraysCard').innerHTML =
    '<h2>Input Array</h2><div class="arr-display">' + d.original.join(', ') + '</div>' +
    '<br><h2>Sorted Array</h2><div class="arr-display">' + d.sorted.join(', ') + '</div>';

  charts.forEach(c => c.destroy());
  charts = [];

  const barOpt = {
    responsive:true,
    plugins:{legend:{display:false},tooltip:{backgroundColor:'#1a2332',titleColor:'#e8edf5',bodyColor:'#8b95a8',borderColor:'rgba(255,255,255,0.1)',borderWidth:1,cornerRadius:8,padding:12}},
    scales:{x:{ticks:{color:'#8b95a8'},grid:{color:'rgba(255,255,255,0.04)'}},y:{ticks:{color:'#8b95a8'},grid:{color:'rgba(255,255,255,0.04)'},beginAtZero:true}},
    animation:{duration:800,easing:'easeOutQuart'}
  };

  charts.push(new Chart(document.getElementById('compChart'),{type:'bar',data:{labels,datasets:[{data:algos.map(a=>a.comparisons),backgroundColor:colors.map(c=>c+'33'),borderColor:colors,borderWidth:2,borderRadius:8,borderSkipped:false}]},options:barOpt}));
  charts.push(new Chart(document.getElementById('swapChart'),{type:'bar',data:{labels,datasets:[{data:algos.map(a=>a.swaps),backgroundColor:colors.map(c=>c+'33'),borderColor:colors,borderWidth:2,borderRadius:8,borderSkipped:false}]},options:barOpt}));

  charts.push(new Chart(document.getElementById('timeChart'),{type:'doughnut',data:{labels,datasets:[{data:algos.map(a=>a.timeMs),backgroundColor:colors.map(c=>c+'55'),borderColor:colors,borderWidth:2,hoverOffset:10}]},options:{responsive:true,cutout:'60%',animation:{duration:800},plugins:{legend:{position:'bottom',labels:{color:'#8b95a8',padding:14,font:{family:'Inter',size:11}}}}}}));

  const mx=Math.max(...algos.map(a=>a.comparisons))||1;
  const ms=Math.max(...algos.map(a=>a.swaps))||1;
  const mt=Math.max(...algos.map(a=>a.timeMs))||1;
  const N=(v,m)=>m>0?(v/m*100):0;

  charts.push(new Chart(document.getElementById('radarChart'),{type:'radar',data:{labels:['Comparisons','Swaps','Time'],datasets:[
    {label:'Merge',data:[N(d.merge.comparisons,mx),N(d.merge.swaps,ms),N(d.merge.timeMs,mt)],borderColor:'#06d6a0',backgroundColor:'rgba(6,214,160,0.1)',borderWidth:2,pointRadius:4},
    {label:'Heap',data:[N(d.heap.comparisons,mx),N(d.heap.swaps,ms),N(d.heap.timeMs,mt)],borderColor:'#a855f7',backgroundColor:'rgba(168,85,247,0.1)',borderWidth:2,pointRadius:4},
    {label:'Quick',data:[N(d.quick.comparisons,mx),N(d.quick.swaps,ms),N(d.quick.timeMs,mt)],borderColor:'#f59e0b',backgroundColor:'rgba(245,158,11,0.1)',borderWidth:2,pointRadius:4}
  ]},options:{responsive:true,animation:{duration:800},scales:{r:{ticks:{display:false},grid:{color:'rgba(255,255,255,0.06)'},pointLabels:{color:'#8b95a8',font:{family:'Inter',size:11}},angleLines:{color:'rgba(255,255,255,0.06)'}}},plugins:{legend:{position:'bottom',labels:{color:'#8b95a8',padding:14,font:{family:'Inter',size:11}}}}}}));

  document.getElementById('results').scrollIntoView({behavior:'smooth',block:'start'});
}

document.getElementById('arrayInput').addEventListener('keydown', e => {
  if (e.key === 'Enter') sortArray();
});
</script>
</body>
</html>)RAWHTML";

void sendResponse(SOCKET client, int code, const string& type, const string& body) {
    string status = (code == 200) ? "OK" : (code == 400) ? "Bad Request" : "Not Found";
    stringstream ss;
    ss << "HTTP/1.1 " << code << " " << status << "\r\n"
       << "Content-Type: " << type << "\r\n"
       << "Content-Length: " << body.size() << "\r\n"
       << "Connection: close\r\n\r\n"
       << body;
    string r = ss.str();
    send(client, r.c_str(), (int)r.size(), 0);
}

int main() {
    WSADATA wsData;
    if (WSAStartup(MAKEWORD(2, 2), &wsData) != 0) {
        cout << "  Failed to initialize network." << endl;
        return 1;
    }

    SOCKET server = socket(AF_INET, SOCK_STREAM, 0);
    if (server == INVALID_SOCKET) {
        cout << "  Failed to create socket." << endl;
        WSACleanup();
        return 1;
    }

    int optval = 1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;

    int port = 8080;
    addr.sin_port = htons(port);
    while (bind(server, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        port++;
        addr.sin_port = htons(port);
        if (port > 9000) {
            cout << "  No available port found." << endl;
            closesocket(server);
            WSACleanup();
            return 1;
        }
    }

    listen(server, 5);

    cout << endl;
    cout << "  +------------------------------------------------------------+" << endl;
    cout << "  |                                                            |" << endl;
    cout << "  |              SMART SORTING STUDIO                          |" << endl;
    cout << "  |              Web Interface                                 |" << endl;
    cout << "  |                                                            |" << endl;
    cout << "  +------------------------------------------------------------+" << endl;
    cout << endl;
    cout << "  Server running at http://localhost:" << port << endl;
    cout << "  Opening browser..." << endl;
    cout << "  Press Ctrl+C to stop." << endl;
    cout << endl;

    system(("start http://localhost:" + to_string(port)).c_str());

    while (true) {
        SOCKET client = accept(server, NULL, NULL);
        if (client == INVALID_SOCKET) continue;

        char buf[65536] = {0};
        int bytesRead = recv(client, buf, sizeof(buf) - 1, 0);
        if (bytesRead <= 0) { closesocket(client); continue; }

        string req(buf, bytesRead);
        string method, path;
        istringstream rs(req);
        rs >> method >> path;

        if (method == "GET" && path == "/") {
            sendResponse(client, 200, "text/html; charset=utf-8", HTML_PAGE);
        }
        else if (method == "POST" && path == "/sort") {
            size_t bpos = req.find("\r\n\r\n");
            string body = (bpos != string::npos) ? req.substr(bpos + 4) : "";

            vector<int> arr = parseJsonArray(body);
            if (arr.empty()) {
                sendResponse(client, 400, "application/json", "{\"error\":\"No valid numbers\"}");
            } else {
                vector<int> orig = arr;
                vector<int> a1 = arr, a2 = arr, a3 = arr;
                SortMetrics mg, hp, qk;
                mergeSort(a1, mg);
                heapSort(a2, hp);
                quickSort(a3, qk);

                struct AI { string n; long long c, s; double t; };
                vector<AI> al = {
                    {mg.name, mg.comparisons, mg.swaps, mg.timeMs},
                    {hp.name, hp.comparisons, hp.swaps, hp.timeMs},
                    {qk.name, qk.comparisons, qk.swaps, qk.timeMs}
                };
                AI best = al[0];
                for (auto& a : al) {
                    if (a.c < best.c || (a.c == best.c && a.s < best.s) ||
                        (a.c == best.c && a.s == best.s && a.t < best.t))
                        best = a;
                }

                cout << "  Sorted " << arr.size() << " elements -> Best: " << best.n << endl;
                sendResponse(client, 200, "application/json", buildJson(orig, a1, mg, hp, qk, best.n));
            }
        }
        else {
            sendResponse(client, 404, "text/plain", "Not Found");
        }

        closesocket(client);
    }

    closesocket(server);
    WSACleanup();
    return 0;
}
