// core/io/ntrx_reader.h
//
// Reader for .ntrx files produced by ScanWriter. Minimal, streaming:
// reads the 256-byte header and channel-name table once, then iterates
// scans on demand. Designed so a transcoder can walk hour-long files
// without loading them into RAM.
//
// Header-only for simplicity. The format is documented in scan_writer.h.

#ifndef NTRX_READER_H
#define NTRX_READER_H

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "scan_writer.h"   // reuses scan_writer_format::Header / constants

/// Streaming reader for .ntrx scan recordings.
class NtrxReader {
public:
    struct Info {
        double sample_rate_hz;
        uint32_t scan_size;
        uint32_t num_channels;
        int64_t start_time_unix_ns;
        uint64_t num_scans;              // authoritative count (recovered
                                         // from file size if header's
                                         // num_scans_written is 0)
        std::vector<std::string> channel_names;
    };

    /// Open a file. On failure returns false and populates last_error().
    bool open(const std::string& path) {
        m_stream.reset(new std::ifstream(path, std::ios::binary));
        if (!m_stream || !m_stream->good()) {
            m_last_error = "cannot open " + path;
            return false;
        }

        // Read and validate the fixed header.
        scan_writer_format::Header h{};
        m_stream->read(reinterpret_cast<char*>(&h), sizeof(h));
        if (m_stream->gcount() != sizeof(h)) {
            m_last_error = "file smaller than header";
            return false;
        }
        if (h.magic != scan_writer_format::kMagic) {
            m_last_error = "bad magic (not an .ntrx file)";
            return false;
        }
        if (h.format_version != scan_writer_format::kVersion) {
            m_last_error = "unsupported format version " +
                           std::to_string(h.format_version);
            return false;
        }
        if (h.scan_size == 0 || h.num_channels == 0) {
            m_last_error = "invalid header: zero scan_size / num_channels";
            return false;
        }

        m_info.sample_rate_hz = h.sample_rate_hz;
        m_info.scan_size = h.scan_size;
        m_info.num_channels = h.num_channels;
        m_info.start_time_unix_ns = h.start_time_unix_ns;

        // Channel-name table.
        m_info.channel_names.clear();
        m_info.channel_names.reserve(h.num_channels);
        char buf[scan_writer_format::kChannelNameBytes];
        for (uint32_t i = 0; i < h.num_channels; ++i) {
            m_stream->read(buf, sizeof(buf));
            if (m_stream->gcount() != sizeof(buf)) {
                m_last_error = "truncated channel-name table at channel " +
                               std::to_string(i);
                return false;
            }
            // Find NUL terminator; names are NUL-padded.
            std::size_t n = 0;
            while (n < sizeof(buf) && buf[n] != '\0') ++n;
            m_info.channel_names.emplace_back(buf, n);
        }

        // Seek to data offset from header to allow forward compatibility
        // if the on-disk format grows.
        m_data_offset = h.data_offset;
        m_stream->seekg(static_cast<std::streamoff>(m_data_offset),
                        std::ios::beg);

        // Determine authoritative scan count: if header has it, trust
        // it; otherwise compute from file size (crash-recovery path).
        if (h.num_scans_written > 0) {
            m_info.num_scans = h.num_scans_written;
        } else {
            // Seek to end to get file size, then back.
            m_stream->seekg(0, std::ios::end);
            const auto total = m_stream->tellg();
            if (total == std::streampos(-1)) {
                m_last_error = "could not determine file size";
                return false;
            }
            const uint64_t data_bytes =
                static_cast<uint64_t>(total) - m_data_offset;
            m_info.num_scans = data_bytes / (h.scan_size * sizeof(double));
            m_stream->seekg(static_cast<std::streamoff>(m_data_offset),
                            std::ios::beg);
        }

        m_next_scan = 0;
        m_row_buf.resize(h.scan_size);
        return true;
    }

    const Info& info() const { return m_info; }
    const std::string& last_error() const { return m_last_error; }

    /// Read the next scan row into `out` (length = info().scan_size).
    /// Returns false at EOF or on read error.
    bool read_next(std::vector<double>& out) {
        if (m_next_scan >= m_info.num_scans) return false;
        out.resize(m_info.scan_size);
        m_stream->read(reinterpret_cast<char*>(out.data()),
                       m_info.scan_size * sizeof(double));
        const auto got = m_stream->gcount();
        if (got != static_cast<std::streamsize>(
                m_info.scan_size * sizeof(double))) {
            m_last_error = "short read on scan " +
                           std::to_string(m_next_scan);
            return false;
        }
        ++m_next_scan;
        return true;
    }

    /// Convenience: read the next scan into a raw pointer (row-major).
    /// Caller guarantees `out` has room for info().scan_size doubles.
    bool read_next(double* out) {
        if (m_next_scan >= m_info.num_scans) return false;
        m_stream->read(reinterpret_cast<char*>(out),
                       m_info.scan_size * sizeof(double));
        const auto got = m_stream->gcount();
        if (got != static_cast<std::streamsize>(
                m_info.scan_size * sizeof(double))) {
            m_last_error = "short read on scan " +
                           std::to_string(m_next_scan);
            return false;
        }
        ++m_next_scan;
        return true;
    }

    /// Scan index of the next row to be returned.
    uint64_t next_scan_index() const { return m_next_scan; }

    /// Monotonic time (seconds) of scan index k.
    double time_of(uint64_t scan_index) const {
        if (m_info.sample_rate_hz <= 0) return 0;
        return static_cast<double>(scan_index) / m_info.sample_rate_hz;
    }

private:
    std::unique_ptr<std::ifstream> m_stream;
    Info m_info{};
    uint64_t m_data_offset = 0;
    uint64_t m_next_scan = 0;
    std::vector<double> m_row_buf;
    std::string m_last_error;
};

#endif  // NTRX_READER_H
