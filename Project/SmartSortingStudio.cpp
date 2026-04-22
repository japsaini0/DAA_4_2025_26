#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <string>
#include <cmath>
#include <climits>
#include <fstream>
#include <sstream>

using namespace std;
using namespace std::chrono;

struct SortMetrics {
    string name;
    long long comparisons = 0;
    long long swaps = 0;
    double timeMs = 0.0;
};

void merge(vector<int>& arr, int left, int mid, int right, SortMetrics& m) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        m.comparisons++;
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        m.swaps++;
        k++;
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
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n) {
        m.comparisons++;
        if (arr[left] > arr[largest]) largest = left;
    }
    if (right < n) {
        m.comparisons++;
        if (arr[right] > arr[largest]) largest = right;
    }
    if (largest != i) {
        swap(arr[i], arr[largest]);
        m.swaps++;
        heapify(arr, n, largest, m);
    }
}

void heapSort(vector<int>& arr, SortMetrics& m) {
    m.name = "Heap Sort";
    int n = (int)arr.size();
    auto start = high_resolution_clock::now();
    for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i, m);
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        m.swaps++;
        heapify(arr, i, 0, m);
    }
    auto end = high_resolution_clock::now();
    m.timeMs = duration<double, milli>(end - start).count();
}

int partition(vector<int>& arr, int low, int high, SortMetrics& m) {
    int mid = low + (high - low) / 2;
    m.comparisons++;
    if (arr[mid] < arr[low]) { swap(arr[low], arr[mid]); m.swaps++; }
    m.comparisons++;
    if (arr[high] < arr[low]) { swap(arr[low], arr[high]); m.swaps++; }
    m.comparisons++;
    if (arr[mid] < arr[high]) { swap(arr[mid], arr[high]); m.swaps++; }

    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j <= high - 1; j++) {
        m.comparisons++;
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
            m.swaps++;
        }
    }
    swap(arr[i + 1], arr[high]);
    m.swaps++;
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

void printLine(int width = 62) {
    cout << "  +";
    for (int i = 0; i < width; i++) cout << "-";
    cout << "+" << endl;
}

void printBar(const string& label, long long value, long long maxVal, int barWidth = 35) {
    int filled = (maxVal > 0) ? (int)((double)value / maxVal * barWidth) : 0;
    if (value > 0 && filled == 0) filled = 1;

    cout << "  | " << setw(12) << left << label << " | ";
    for (int i = 0; i < filled; i++) cout << "#";
    for (int i = filled; i < barWidth; i++) cout << " ";
    cout << " | " << setw(6) << right << value << " |" << endl;
}

void printTimeBar(const string& label, double value, double maxVal, int barWidth = 35) {
    int filled = (maxVal > 0) ? (int)(value / maxVal * barWidth) : 0;
    if (value > 0 && filled == 0) filled = 1;

    cout << "  | " << setw(12) << left << label << " | ";
    for (int i = 0; i < filled; i++) cout << "=";
    for (int i = filled; i < barWidth; i++) cout << " ";
    cout << " | " << fixed << setprecision(4) << setw(6) << right << value << " |" << endl;
}

int main() {
    cout << endl;
    cout << "  +------------------------------------------------------------+" << endl;
    cout << "  |                                                            |" << endl;
    cout << "  |              SMART SORTING STUDIO                          |" << endl;
    cout << "  |     Merge Sort  |  Heap Sort  |  Quick Sort                |" << endl;
    cout << "  |     Performance Analysis & Comparison Tool                 |" << endl;
    cout << "  |                                                            |" << endl;
    cout << "  +------------------------------------------------------------+" << endl;
    cout << endl;

    int n;
    cout << "  Enter number of elements: ";
    cin >> n;

    if (n <= 0) {
        cout << "  ERROR: Number of elements must be > 0." << endl;
        return 1;
    }

    vector<int> original(n);
    cout << "  Enter " << n << " elements (space-separated): ";
    for (int i = 0; i < n; i++) cin >> original[i];

    cout << endl;
    cout << "  ORIGINAL ARRAY:" << endl;
    cout << "  [ ";
    int limit = min(n, 20);
    for (int i = 0; i < limit; i++) {
        cout << original[i];
        if (i < limit - 1) cout << ", ";
    }
    if (n > 20) cout << ", ... (" << n - 20 << " more)";
    cout << " ]" << endl;

    vector<int> arrMerge = original;
    vector<int> arrHeap = original;
    vector<int> arrQuick = original;

    SortMetrics mergeM, heapM, quickM;
    mergeSort(arrMerge, mergeM);
    heapSort(arrHeap, heapM);
    quickSort(arrQuick, quickM);

    cout << endl;
    cout << "  SORTED ARRAY:" << endl;
    cout << "  [ ";
    for (int i = 0; i < limit; i++) {
        cout << arrMerge[i];
        if (i < limit - 1) cout << ", ";
    }
    if (n > 20) cout << ", ... (" << n - 20 << " more)";
    cout << " ]" << endl;

    long long maxComp = max({mergeM.comparisons, heapM.comparisons, quickM.comparisons});
    long long maxSwap = max({mergeM.swaps, heapM.swaps, quickM.swaps});
    double maxTime = max({mergeM.timeMs, heapM.timeMs, quickM.timeMs});

    cout << endl;
    cout << "  ============== COMPARISONS (# = scale) =====================" << endl;
    printLine();
    printBar("Merge Sort", mergeM.comparisons, maxComp);
    printBar("Heap Sort", heapM.comparisons, maxComp);
    printBar("Quick Sort", quickM.comparisons, maxComp);
    printLine();

    cout << endl;
    cout << "  ============== SWAPS / MOVES ================================" << endl;
    printLine();
    printBar("Merge Sort", mergeM.swaps, maxSwap);
    printBar("Heap Sort", heapM.swaps, maxSwap);
    printBar("Quick Sort", quickM.swaps, maxSwap);
    printLine();

    cout << endl;
    cout << "  ============== EXECUTION TIME (ms) =========================" << endl;
    printLine();
    printTimeBar("Merge Sort", mergeM.timeMs, maxTime);
    printTimeBar("Heap Sort", heapM.timeMs, maxTime);
    printTimeBar("Quick Sort", quickM.timeMs, maxTime);
    printLine();

    cout << endl;
    cout << "  +----------------+---------------+---------------+---------------+" << endl;
    cout << "  | Algorithm      | Comparisons   | Swaps/Moves   | Time (ms)     |" << endl;
    cout << "  +----------------+---------------+---------------+---------------+" << endl;

    auto printRow = [](const SortMetrics& m) {
        cout << "  | " << setw(14) << left << m.name
             << " | " << setw(13) << right << m.comparisons
             << " | " << setw(13) << right << m.swaps
             << " | " << setw(9) << right << fixed << setprecision(4) << m.timeMs << " ms |" << endl;
    };

    printRow(mergeM);
    printRow(heapM);
    printRow(quickM);
    cout << "  +----------------+---------------+---------------+---------------+" << endl;

    struct AlgoInfo { string name; long long comp; long long swp; double time; };
    vector<AlgoInfo> algos = {
        {"Merge Sort", mergeM.comparisons, mergeM.swaps, mergeM.timeMs},
        {"Heap Sort", heapM.comparisons, heapM.swaps, heapM.timeMs},
        {"Quick Sort", quickM.comparisons, quickM.swaps, quickM.timeMs}
    };

    AlgoInfo best = algos[0];
    for (const auto& a : algos) {
        if (a.comp < best.comp ||
            (a.comp == best.comp && a.swp < best.swp) ||
            (a.comp == best.comp && a.swp == best.swp && a.time < best.time)) {
            best = a;
        }
    }

    cout << endl;
    cout << "  +------------------------------------------------------------+" << endl;
    cout << "  |                                                            |" << endl;
    cout << "  |   BEST TECHNIQUE:  " << setw(39) << left << best.name << "|" << endl;
    cout << "  |                                                            |" << endl;
    cout << "  |   Comparisons : " << setw(41) << left << best.comp << "|" << endl;
    cout << "  |   Swaps/Moves : " << setw(41) << left << best.swp << "|" << endl;
    cout << "  |   Time        : " << setw(37) << left << (to_string(best.time).substr(0, 8) + " ms") << "|" << endl;
    cout << "  |                                                            |" << endl;
    cout << "  +------------------------------------------------------------+" << endl;
    cout << endl;

    cout << "  Generate HTML report with charts? (y/n): ";
    char choice;
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        ofstream html("report.html");
        if (!html.is_open()) {
            cout << "  ERROR: Could not create report.html" << endl;
            return 1;
        }

        stringstream origSS, sortSS;
        for (int i = 0; i < n; i++) {
            origSS << original[i];
            sortSS << arrMerge[i];
            if (i < n - 1) { origSS << ", "; sortSS << ", "; }
        }

        html << R"(<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Smart Sorting Studio - Report</title>
<script src="https://cdn.jsdelivr.net/npm/chart.js@4.4.0/dist/chart.umd.min.js"></script>
<style>
  @import url('https://fonts.googleapis.com/css2?family=Inter:wght@300;400;500;600;700;800;900&family=JetBrains+Mono:wght@400;600&display=swap');
  * { margin: 0; padding: 0; box-sizing: border-box; }
  :root {
    --bg: #0a0e17; --card: #1a2332; --text: #e8edf5; --dim: #8b95a8;
    --cyan: #06d6a0; --purple: #a855f7; --orange: #f59e0b;
    --blue: #3b82f6; --border: rgba(255,255,255,0.06);
  }
  body { font-family: 'Inter', sans-serif; background: var(--bg); color: var(--text); min-height: 100vh; }
  body::before { content: ''; position: fixed; inset: 0; background: radial-gradient(ellipse at 20% 20%, rgba(6,214,160,0.08) 0%, transparent 50%), radial-gradient(ellipse at 80% 80%, rgba(168,85,247,0.08) 0%, transparent 50%); z-index: -1; }
  .container { max-width: 1100px; margin: 0 auto; padding: 40px 24px; }
  .hero { text-align: center; padding: 50px 20px 30px; }
  .hero h1 { font-size: 2.8rem; font-weight: 900; background: linear-gradient(135deg, #06d6a0, #3b82f6, #a855f7); -webkit-background-clip: text; background-clip: text; -webkit-text-fill-color: transparent; letter-spacing: -1px; margin-bottom: 6px; }
  .hero .sub { font-size: 1rem; color: var(--dim); letter-spacing: 3px; text-transform: uppercase; }
  .badge { display: inline-block; margin-top: 16px; padding: 5px 18px; border-radius: 100px; font-size: 0.78rem; font-weight: 600; letter-spacing: 1px; text-transform: uppercase; background: linear-gradient(135deg, rgba(6,214,160,0.15), rgba(168,85,247,0.15)); border: 1px solid rgba(6,214,160,0.3); color: var(--cyan); }
  .card { background: var(--card); border: 1px solid var(--border); border-radius: 14px; padding: 28px; margin-bottom: 20px; transition: transform 0.3s, box-shadow 0.3s; }
  .card:hover { transform: translateY(-2px); box-shadow: 0 10px 30px rgba(0,0,0,0.3); }
  .card h2 { font-size: 1.2rem; font-weight: 700; margin-bottom: 16px; }
  .winner { background: linear-gradient(135deg, rgba(6,214,160,0.08), rgba(59,130,246,0.08)); border: 1px solid rgba(6,214,160,0.25); border-radius: 14px; padding: 36px; text-align: center; margin-bottom: 20px; }
  .winner .trophy { font-size: 2.8rem; margin-bottom: 10px; }
  .winner h2 { font-size: 1.8rem; font-weight: 800; color: var(--cyan); justify-content: center; }
  .winner p { color: var(--dim); }
  .stats { display: grid; grid-template-columns: repeat(3, 1fr); gap: 16px; margin-bottom: 20px; }
  .stat { background: var(--card); border: 1px solid var(--border); border-radius: 14px; padding: 24px; text-align: center; position: relative; overflow: hidden; transition: transform 0.3s; }
  .stat:hover { transform: translateY(-2px); }
  .stat::before { content: ''; position: absolute; top: 0; left: 0; right: 0; height: 3px; }
  .stat:nth-child(1)::before { background: linear-gradient(90deg, var(--cyan), var(--blue)); }
  .stat:nth-child(2)::before { background: linear-gradient(90deg, var(--purple), #ec4899); }
  .stat:nth-child(3)::before { background: linear-gradient(90deg, var(--orange), #ef4444); }
  .stat .alg { font-size: 0.8rem; text-transform: uppercase; letter-spacing: 2px; color: var(--dim); margin-bottom: 10px; font-weight: 600; }
  .stat .val { font-size: 2rem; font-weight: 800; font-family: 'JetBrains Mono', monospace; margin-bottom: 4px; }
  .stat:nth-child(1) .val { color: var(--cyan); }
  .stat:nth-child(2) .val { color: var(--purple); }
  .stat:nth-child(3) .val { color: var(--orange); }
  .stat .lbl { font-size: 0.78rem; color: var(--dim); }
  .charts { display: grid; grid-template-columns: repeat(2, 1fr); gap: 20px; margin-bottom: 20px; }
  .chart-card { background: var(--card); border: 1px solid var(--border); border-radius: 14px; padding: 24px; }
  .chart-card h3 { font-size: 0.9rem; font-weight: 600; margin-bottom: 16px; color: var(--dim); text-transform: uppercase; letter-spacing: 1px; }
  .chart-card canvas { max-height: 280px; }
  table { width: 100%%; border-collapse: collapse; font-size: 0.9rem; }
  th { padding: 12px 18px; text-align: left; font-weight: 600; color: var(--dim); border-bottom: 1px solid var(--border); text-transform: uppercase; font-size: 0.72rem; letter-spacing: 1.5px; }
  td { padding: 14px 18px; border-bottom: 1px solid var(--border); font-family: 'JetBrains Mono', monospace; font-size: 0.85rem; }
  tr:hover td { background: rgba(255,255,255,0.02); }
  .best td { color: var(--cyan); font-weight: 600; }
  .dot { display: inline-block; width: 9px; height: 9px; border-radius: 50%%; margin-right: 7px; }
  .arr { font-family: 'JetBrains Mono', monospace; font-size: 0.82rem; color: var(--dim); background: rgba(0,0,0,0.3); padding: 14px 18px; border-radius: 8px; line-height: 1.7; word-break: break-all; max-height: 110px; overflow-y: auto; }
  .footer { text-align: center; padding: 36px; color: var(--dim); font-size: 0.78rem; }
  @media (max-width: 768px) { .hero h1 { font-size: 1.8rem; } .stats, .charts { grid-template-columns: 1fr; } }
</style>
</head>
<body>
<div class="container">
  <div class="hero">
    <h1>Smart Sorting Studio</h1>
    <div class="sub">Performance Analysis Report</div>
    <div class="badge">)" << n << R"( Elements Analyzed</div>
  </div>
  <div class="winner">
    <div class="trophy">&#127942;</div>
    <h2>)" << best.name << R"( Wins!</h2>
    <p>Based on fewest comparisons, this algorithm performed the best for your dataset.</p>
  </div>
  <div class="stats">
    <div class="stat"><div class="alg">Merge Sort</div><div class="val">)" << mergeM.comparisons << R"(</div><div class="lbl">comparisons</div></div>
    <div class="stat"><div class="alg">Heap Sort</div><div class="val">)" << heapM.comparisons << R"(</div><div class="lbl">comparisons</div></div>
    <div class="stat"><div class="alg">Quick Sort</div><div class="val">)" << quickM.comparisons << R"(</div><div class="lbl">comparisons</div></div>
  </div>
  <div class="charts">
    <div class="chart-card"><h3>Comparisons</h3><canvas id="compChart"></canvas></div>
    <div class="chart-card"><h3>Swaps / Moves</h3><canvas id="swapChart"></canvas></div>
    <div class="chart-card"><h3>Execution Time (ms)</h3><canvas id="timeChart"></canvas></div>
    <div class="chart-card"><h3>Overall Comparison</h3><canvas id="radarChart"></canvas></div>
  </div>
  <div class="card">
    <h2>Detailed Metrics</h2>
    <table>
      <thead><tr><th>Algorithm</th><th>Comparisons</th><th>Swaps/Moves</th><th>Time (ms)</th><th>Status</th></tr></thead>
      <tbody>)";

        long long minC = min({mergeM.comparisons, heapM.comparisons, quickM.comparisons});
        auto row = [&](const SortMetrics& s, const string& clr) {
            bool isBest = (s.comparisons == minC);
            html << "<tr" << (isBest ? " class=\"best\"" : "") << ">"
                 << "<td><span class='dot' style='background:" << clr << "'></span>" << s.name << "</td>"
                 << "<td>" << s.comparisons << "</td>"
                 << "<td>" << s.swaps << "</td>"
                 << "<td>" << fixed << setprecision(4) << s.timeMs << "</td>"
                 << "<td>" << (isBest ? "Best" : "-") << "</td></tr>";
        };
        row(mergeM, "#06d6a0");
        row(heapM, "#a855f7");
        row(quickM, "#f59e0b");

        html << R"(
      </tbody>
    </table>
  </div>
  <div class="card"><h2>Input Array</h2><div class="arr">)" << origSS.str() << R"(</div></div>
  <div class="card"><h2>Sorted Array</h2><div class="arr">)" << sortSS.str() << R"(</div></div>
  <div class="footer">Smart Sorting Studio - DAA Project</div>
</div>
<script>
const L=['Merge Sort','Heap Sort','Quick Sort'],C=['#06d6a0','#a855f7','#f59e0b'];
const opt={responsive:true,plugins:{legend:{display:false},tooltip:{backgroundColor:'#1a2332',titleColor:'#e8edf5',bodyColor:'#8b95a8',borderColor:'rgba(255,255,255,0.1)',borderWidth:1,cornerRadius:8,padding:12}},scales:{x:{ticks:{color:'#8b95a8'},grid:{color:'rgba(255,255,255,0.04)'}},y:{ticks:{color:'#8b95a8'},grid:{color:'rgba(255,255,255,0.04)'},beginAtZero:true}}};
new Chart(document.getElementById('compChart'),{type:'bar',data:{labels:L,datasets:[{data:[)" << mergeM.comparisons << "," << heapM.comparisons << "," << quickM.comparisons << R"(],backgroundColor:C.map(c=>c+'33'),borderColor:C,borderWidth:2,borderRadius:8,borderSkipped:false}]},options:opt});
new Chart(document.getElementById('swapChart'),{type:'bar',data:{labels:L,datasets:[{data:[)" << mergeM.swaps << "," << heapM.swaps << "," << quickM.swaps << R"(],backgroundColor:C.map(c=>c+'33'),borderColor:C,borderWidth:2,borderRadius:8,borderSkipped:false}]},options:opt});
new Chart(document.getElementById('timeChart'),{type:'doughnut',data:{labels:L,datasets:[{data:[)" << fixed << setprecision(4) << mergeM.timeMs << "," << heapM.timeMs << "," << quickM.timeMs << R"(],backgroundColor:C.map(c=>c+'55'),borderColor:C,borderWidth:2,hoverOffset:10}]},options:{responsive:true,cutout:'60%',plugins:{legend:{position:'bottom',labels:{color:'#8b95a8',padding:14,font:{family:'Inter',size:11}}}}}});
const mx=Math.max()" << mergeM.comparisons << "," << heapM.comparisons << "," << quickM.comparisons << R"(),ms=Math.max()" << mergeM.swaps << "," << heapM.swaps << "," << quickM.swaps << R"(),mt=Math.max()" << fixed << setprecision(4) << mergeM.timeMs << "," << heapM.timeMs << "," << quickM.timeMs << R"();
function N(v,m){return m>0?(v/m*100):0;}
new Chart(document.getElementById('radarChart'),{type:'radar',data:{labels:['Comparisons','Swaps','Time'],datasets:[{label:'Merge',data:[N()" << mergeM.comparisons << R"(,mx),N()" << mergeM.swaps << R"(,ms),N()" << fixed << setprecision(4) << mergeM.timeMs << R"(,mt)],borderColor:'#06d6a0',backgroundColor:'rgba(6,214,160,0.1)',borderWidth:2,pointRadius:4},{label:'Heap',data:[N()" << heapM.comparisons << R"(,mx),N()" << heapM.swaps << R"(,ms),N()" << fixed << setprecision(4) << heapM.timeMs << R"(,mt)],borderColor:'#a855f7',backgroundColor:'rgba(168,85,247,0.1)',borderWidth:2,pointRadius:4},{label:'Quick',data:[N()" << quickM.comparisons << R"(,mx),N()" << quickM.swaps << R"(,ms),N()" << fixed << setprecision(4) << quickM.timeMs << R"(,mt)],borderColor:'#f59e0b',backgroundColor:'rgba(245,158,11,0.1)',borderWidth:2,pointRadius:4}]},options:{responsive:true,scales:{r:{ticks:{display:false},grid:{color:'rgba(255,255,255,0.06)'},pointLabels:{color:'#8b95a8',font:{family:'Inter',size:11}},angleLines:{color:'rgba(255,255,255,0.06)'}}},plugins:{legend:{position:'bottom',labels:{color:'#8b95a8',padding:14,font:{family:'Inter',size:11}}}}}});
</script>
</body></html>)";

        html.close();
        cout << endl;
        cout << "  report.html generated successfully!" << endl;
        cout << "  Open it in your browser for interactive charts." << endl;
        cout << endl;
    } else {
        cout << endl;
        cout << "  Skipped HTML report. Done!" << endl;
        cout << endl;
    }

    return 0;
}
