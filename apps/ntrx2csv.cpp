// apps/ntrx2csv.cpp
//
// Command-line utility that converts a .ntrx binary recording into a
// CSV file suitable for Excel / MATLAB / Python / R. Streams scan by
// scan — works on files larger than RAM.
//
// Usage:
//   ntrx2csv <input.ntrx> [<output.csv>]
//
// If the output path is omitted it defaults to `<input>.csv`.
//
// CSV layout:
//   Time (s), <chan0>, <chan1>, …
//   0.000000, …
//   0.000100, …
//
// The first column is derived from the sample rate in the header, so
// the user never has to worry about "what timestamp goes with what row".

#include "ntrx_reader.h"

#include <cerrno>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

static int fail(const std::string& msg) {
    std::fprintf(stderr, "ntrx2csv: %s\n", msg.c_str());
    return 1;
}

int main(int argc, char** argv) {
    if (argc < 2 || argc > 3) {
        std::fprintf(stderr,
            "Usage: %s <input.ntrx> [<output.csv>]\n", argv[0]);
        return 2;
    }
    std::string in_path = argv[1];
    std::string out_path = (argc >= 3) ? argv[2] : (in_path + ".csv");

    NtrxReader r;
    if (!r.open(in_path)) {
        return fail("open " + in_path + ": " + r.last_error());
    }

    std::FILE* out = std::fopen(out_path.c_str(), "w");
    if (!out) {
        return fail("open " + out_path + ": errno=" +
                    std::to_string(errno));
    }
    // Big stdio buffer so we're not syscalling on every row.
    static char iobuf[1 << 20];
    std::setvbuf(out, iobuf, _IOFBF, sizeof(iobuf));

    // --- Header row -----------------------------------------------------
    std::fprintf(out, "Time (s)");
    for (const auto& name : r.info().channel_names) {
        // Escape commas and quotes per RFC 4180.
        bool needs_quotes = name.find_first_of(",\"\n") != std::string::npos;
        if (needs_quotes) {
            std::fputc(',', out);
            std::fputc('"', out);
            for (char c : name) {
                if (c == '"') std::fputc('"', out);  // double-up
                std::fputc(c, out);
            }
            std::fputc('"', out);
        } else {
            std::fprintf(out, ",%s", name.c_str());
        }
    }
    std::fputc('\n', out);

    // --- Data rows ------------------------------------------------------
    std::vector<double> row(r.info().scan_size);
    const uint64_t total = r.info().num_scans;
    const double rate = r.info().sample_rate_hz;
    const double dt = (rate > 0) ? 1.0 / rate : 0.0;

    std::fprintf(stderr, "ntrx2csv: %llu scans × %u channels @ %.1f Hz\n",
        (unsigned long long)total,
        (unsigned)r.info().num_channels,
        rate);

    uint64_t n = 0;
    while (r.read_next(row.data())) {
        // Time column — use multiplication rather than accumulated adds
        // to avoid drift over long recordings.
        const double t = n * dt;
        std::fprintf(out, "%.6f", t);
        for (double v : row) {
            std::fprintf(out, ",%g", v);
        }
        std::fputc('\n', out);
        ++n;

        // Periodic progress for long files (every ~1 s of recording).
        if (rate > 0 && n > 0 &&
            (n % static_cast<uint64_t>(rate)) == 0) {
            std::fprintf(stderr, "  %.1f / %.1f s\r",
                n * dt, total * dt);
            std::fflush(stderr);
        }
    }

    std::fflush(out);
    std::fclose(out);

    if (n != total) {
        std::fprintf(stderr, "\nntrx2csv: warning, read %llu of %llu scans "
                     "before stopping (%s)\n",
                     (unsigned long long)n,
                     (unsigned long long)total,
                     r.last_error().c_str());
        return 1;
    }

    std::fprintf(stderr, "\nntrx2csv: wrote %s\n", out_path.c_str());
    return 0;
}
