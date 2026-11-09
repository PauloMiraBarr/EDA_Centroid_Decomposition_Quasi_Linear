#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "centroid_decomposition_folklore.h"
#include "centroid_decomposition_quasi_linear.h"

#include "complete_gen.h"
#include "path_gen.h"
#include "random_gen.h"
#include "star_gen.h"

using Graph = std::vector<std::vector<int>>;

struct Options {
    std::vector<std::string> families;
    std::vector<int> sizes;
    int reps = 5;
    unsigned int seed = 42;
    std::string csv_path = "output/benchmark.csv";
    bool validate = false;
};

struct Stats {
    double min_ms = 0.0;
    double median_ms = 0.0;
    double mean_ms = 0.0;
};

static volatile long long benchmark_sink = 0;

static void print_usage(const char* program) {
    std::cout
        << "Uso:\n"
        << "  " << program << " -random [-n 1000,10000,100000] [-reps 5] [-seed 42] [-csv output/benchmark.csv] [-validate]\n"
        << "  " << program << " -star -n 1000000\n"
        << "  " << program << " -complete -n 100000\n"
        << "  " << program << " -path -n 50000\n"
        << "  " << program << " -all -n 1000,10000,100000 -reps 5\n";
}

static std::vector<int> parse_sizes(const std::string& text) {
    std::vector<int> sizes;
    std::stringstream stream(text);
    std::string token;

    while (std::getline(stream, token, ',')) {
        if (token.empty()) {
            throw std::runtime_error("lista de tamanos invalida: " + text);
        }

        std::size_t position = 0;
        int value = 0;

        try {
            value = std::stoi(token, &position);
        } catch (const std::exception&) {
            throw std::runtime_error("tamano invalido: " + token);
        }

        if (position != token.size() || value <= 0) {
            throw std::runtime_error("tamano invalido: " + token);
        }

        sizes.push_back(value);
    }

    return sizes;
}

static int parse_positive_int(const std::string& text, const std::string& option) {
    std::size_t position = 0;
    int value = 0;

    try {
        value = std::stoi(text, &position);
    } catch (const std::exception&) {
        throw std::runtime_error(option + " requiere un entero positivo");
    }

    if (position != text.size() || value <= 0) {
        throw std::runtime_error(option + " requiere un entero positivo");
    }

    return value;
}

static unsigned int parse_seed(const std::string& text) {
    std::size_t position = 0;
    unsigned long value = 0;

    try {
        value = std::stoul(text, &position);
    } catch (const std::exception&) {
        throw std::runtime_error("-seed requiere un entero no negativo");
    }

    if (position != text.size()) {
        throw std::runtime_error("-seed requiere un entero no negativo");
    }

    return static_cast<unsigned int>(value);
}

static Options parse_options(int argc, char** argv) {
    Options options;

    for (int i = 1; i < argc; ++i) {
        std::string argument = argv[i];

        if (argument == "-random" || argument == "-star" ||
            argument == "-complete" || argument == "-path") {
            options.families.push_back(argument.substr(1));
        } else if (argument == "-all") {
            options.families = {"random", "star", "complete", "path"};
        } else if (argument == "-n") {
            if (++i >= argc) throw std::runtime_error("-n requiere un valor");
            std::vector<int> parsed = parse_sizes(argv[i]);
            options.sizes.insert(options.sizes.end(), parsed.begin(), parsed.end());
        } else if (argument == "-reps") {
            if (++i >= argc) throw std::runtime_error("-reps requiere un valor");
            options.reps = parse_positive_int(argv[i], "-reps");
        } else if (argument == "-seed") {
            if (++i >= argc) throw std::runtime_error("-seed requiere un valor");
            options.seed = parse_seed(argv[i]);
        } else if (argument == "-csv") {
            if (++i >= argc) throw std::runtime_error("-csv requiere una ruta");
            options.csv_path = argv[i];
        } else if (argument == "-validate") {
            options.validate = true;
        } else if (argument == "-h" || argument == "--help") {
            print_usage(argv[0]);
            std::exit(0);
        } else {
            throw std::runtime_error("opcion desconocida: " + argument);
        }
    }

    if (options.families.empty()) {
        options.families = {"random", "star", "complete"};
    }

    std::vector<std::string> unique_families;
    for (const std::string& family : options.families) {
        if (std::find(unique_families.begin(), unique_families.end(), family) == unique_families.end()) {
            unique_families.push_back(family);
        }
    }
    options.families = unique_families;

    if (options.sizes.empty()) {
        options.sizes = {1000, 10000, 100000};
    }

    std::sort(options.sizes.begin(), options.sizes.end());
    options.sizes.erase(std::unique(options.sizes.begin(), options.sizes.end()), options.sizes.end());

    return options;
}

static Graph generate_graph(const std::string& family, int n, unsigned int seed) {
    if (family == "random") return generate_random_tree(n, seed);
    if (family == "star") return generate_star_tree(n);
    if (family == "complete") return generate_complete_binary_tree(n);
    if (family == "path") return generate_path_tree(n);

    throw std::runtime_error("familia desconocida: " + family);
}

template <typename Function>
static double measure_ms(Function&& function) {
    const std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    function();
    const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    return std::chrono::duration<double, std::milli>(end - start).count();
}

static double run_folklore(const Graph& graph, int n) {
    CentroidDecompositionFolklore* decomposition = nullptr;

    const double elapsed = measure_ms([&]() {
        decomposition = new CentroidDecompositionFolklore(n, graph);
    });

    benchmark_sink += static_cast<long long>(decomposition->centroid_parent.size());
    delete decomposition;

    return elapsed;
}

static double run_qlinear(const Graph& graph, int n) {
    CentroidDecompositionQLinear* decomposition = nullptr;

    const double elapsed = measure_ms([&]() {
        decomposition = new CentroidDecompositionQLinear(n, graph);
    });

    benchmark_sink += static_cast<long long>(decomposition->centroid_parent.size());
    delete decomposition;

    return elapsed;
}

static Stats summarize(std::vector<double> values) {
    std::sort(values.begin(), values.end());

    Stats stats;
    stats.min_ms = values.front();

    const std::size_t middle = values.size() / 2;
    if (values.size() % 2 == 0) {
        stats.median_ms = (values[middle - 1] + values[middle]) / 2.0;
    } else {
        stats.median_ms = values[middle];
    }

    double sum = 0.0;
    for (double value : values) sum += value;
    stats.mean_ms = sum / static_cast<double>(values.size());

    return stats;
}

static bool validate_graph(const Graph& graph, int n, const std::string& family) {
    if (static_cast<int>(graph.size()) < n + 1) {
        std::cerr << "[validacion] " << family << ": el grafo tiene menos de n+1 nodos (n=" << n << ")\n";
        return false;
    }

    long long degree_sum = 0;
    for (int u = 1; u <= n; ++u) {
        degree_sum += static_cast<long long>(graph[u].size());

        for (int v : graph[u]) {
            if (v < 1 || v > n) {
                std::cerr << "[validacion] " << family << ": arista invalida " << u << " -> " << v << "\n";
                return false;
            }
            if (v == u) {
                std::cerr << "[validacion] " << family << ": auto-arista en el nodo " << u << "\n";
                return false;
            }
        }
    }

    if (degree_sum != 2LL * static_cast<long long>(n - 1)) {
        std::cerr << "[validacion] " << family << ": cantidad de aristas invalida (n=" << n << ")\n";
        return false;
    }

    std::vector<char> visited(n + 1, 0);
    std::vector<int> stack;
    stack.push_back(1);
    visited[1] = 1;

    int reached = 0;
    while (!stack.empty()) {
        int u = stack.back();
        stack.pop_back();
        ++reached;

        for (int v : graph[u]) {
            if (!visited[v]) {
                visited[v] = 1;
                stack.push_back(v);
            }
        }
    }

    if (reached != n) {
        std::cerr << "[validacion] " << family << ": el grafo no es conexo (n=" << n << ")\n";
        return false;
    }

    return true;
}

static bool validate_centroid_parent(const std::vector<int>& parent, int n,
                                     const std::string& description) {
    if (static_cast<int>(parent.size()) < n + 1) {
        std::cerr << "[validacion] " << description << ": centroid_parent tiene menos de n+1 nodos\n";
        return false;
    }

    for (int u = 1; u <= n; ++u) {
        const int p = parent[u];

        if (p < 0 || p > n) {
            std::cerr << "[validacion] " << description << ": padre invalido para " << u << "\n";
            return false;
        }

        if (p == u) {
            std::cerr << "[validacion] " << description << ": auto-padre en " << u << "\n";
            return false;
        }
    }

    int roots = 0;
    for (int u = 1; u <= n; ++u) {
        if (parent[u] == 0) ++roots;
    }

    if (roots != 1) {
        std::cerr << "[validacion] " << description << ": se esperaba una raiz y se encontraron " << roots << "\n";
        return false;
    }

    std::vector<unsigned char> state(n + 1, 0);

    for (int start = 1; start <= n; ++start) {
        if (state[start] != 0) continue;

        int u = start;
        while (u != 0 && state[u] == 0) {
            state[u] = 1;
            u = parent[u];
        }

        if (u != 0 && state[u] == 1) {
            std::cerr << "[validacion] " << description << ": ciclo detectado\n";
            return false;
        }

        u = start;
        while (u != 0 && state[u] == 1) {
            state[u] = 2;
            u = parent[u];
        }
    }

    return true;
}

static bool validate_structures(const Graph& graph, int n, const std::string& family) {
    bool valid = true;

    if (!validate_graph(graph, n, family)) valid = false;

    CentroidDecompositionFolklore folklore(n, graph);
    if (!validate_centroid_parent(folklore.centroid_parent, n, family + "/folklore")) {
        valid = false;
    }

    CentroidDecompositionQLinear qlinear(n, graph);
    if (!validate_centroid_parent(qlinear.centroid_parent, n, family + "/qlinear")) {
        valid = false;
    }

    return valid;
}

static void print_header() {
    std::cout << std::left << std::setw(10) << "family"
              << std::right << std::setw(10) << "n" << "  "
              << std::left << std::setw(10) << "algorithm"
              << std::right << std::setw(12) << "min(ms)"
              << std::setw(13) << "median(ms)"
              << std::setw(12) << "mean(ms)"
              << "\n";
}

static void print_stats(const std::string& family, int n, const std::string& algorithm,
                        const Stats& stats) {
    std::cout << std::left << std::setw(10) << family
              << std::right << std::setw(10) << n << "  "
              << std::left << std::setw(10) << algorithm
              << std::right << std::fixed << std::setprecision(3)
              << std::setw(12) << stats.min_ms
              << std::setw(13) << stats.median_ms
              << std::setw(12) << stats.mean_ms
              << "\n";
}

int main(int argc, char** argv) {
    try {
        const Options options = parse_options(argc, argv);

        std::ofstream csv(options.csv_path);
        if (!csv) {
            std::cerr << "Error: no se pudo abrir el CSV: " << options.csv_path << "\n";
            std::cerr << "Crea el directorio correspondiente o ejecuta con benchmark.sh.\n";
            return 1;
        }

        csv << "family,n,algorithm,rep,time_ms,seed\n";
        csv << std::fixed << std::setprecision(6);

        print_header();

        for (const std::string& family : options.families) {
            for (int n : options.sizes) {
                const Graph graph = generate_graph(family, n, options.seed);

                if (options.validate) {
                    if (!validate_structures(graph, n, family)) {
                        std::cerr << "[validacion] fallo para family=" << family << " n=" << n << "\n";
                    }
                }

                run_folklore(graph, n);
                run_qlinear(graph, n);

                std::vector<double> folklore_times;
                std::vector<double> qlinear_times;
                folklore_times.reserve(options.reps);
                qlinear_times.reserve(options.reps);

                for (int repetition = 0; repetition < options.reps; ++repetition) {
                    double folklore_ms = 0.0;
                    double qlinear_ms = 0.0;

                    if (repetition % 2 == 0) {
                        folklore_ms = run_folklore(graph, n);
                        qlinear_ms = run_qlinear(graph, n);
                    } else {
                        qlinear_ms = run_qlinear(graph, n);
                        folklore_ms = run_folklore(graph, n);
                    }

                    folklore_times.push_back(folklore_ms);
                    qlinear_times.push_back(qlinear_ms);

                    csv << family << ',' << n << ",folklore," << repetition + 1 << ','
                        << folklore_ms << ',' << options.seed << '\n';
                    csv << family << ',' << n << ",qlinear," << repetition + 1 << ','
                        << qlinear_ms << ',' << options.seed << '\n';
                }

                print_stats(family, n, "folklore", summarize(folklore_times));
                print_stats(family, n, "qlinear", summarize(qlinear_times));
            }
        }

        std::cout << "\n[benchmark] CSV: " << options.csv_path << "\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << "Error: " << exception.what() << "\n";
        return 1;
    }
}
