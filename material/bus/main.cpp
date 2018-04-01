#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <tuple>

#include <stdio.h>
#include <time.h>

#include "sdl_app.h"

enum class Kind {
    GPS,
    Schedule,
    Passed
};

class Time {
    public:
        Time();
        Time(int h, int m);
        int hour;
        int minute;
};

Time::Time()
    : hour(0),
    minute(0)
{
}

Time::Time(int h, int m)
    : hour(h),
    minute(m)
{
}

// t2 - t1 i.e. >0 if t1 is earlier than t2
int time_diff(const Time& t1, const Time& t2) {
    int min_diff = t2.minute - t1.minute;
    int hr_diff  = t2.hour   - t1.hour;
    int diff = hr_diff * 60 + min_diff;
    // wrap around
    if(diff < -720)
        diff += 1440;
    return diff;
}

class BusInfo {
    public:
        BusInfo(int rnr, int snr, const Time& t, Kind k);
        BusInfo(int rnr, int snr, int h, int m, Kind k);
        int route_nr = 0;
        int start_nr = 0;
        Time time;
        Kind kind = Kind::GPS;
};

BusInfo::BusInfo(int rnr, int snr, const Time& t, Kind k)
    : route_nr(rnr),
    start_nr(snr),
    time(t),
    kind(k)
{
}

BusInfo::BusInfo(int rnr, int snr, int h, int m, Kind k)
    : route_nr(rnr),
    start_nr(snr),
    time(h, m),
    kind(k)
{
}

class GpsInfo {
    public:
        int route_nr = 0;
        int start_nr = 0;
        float x = 0.0f;
        float y = 0.0f;
        Kind kind = Kind::GPS;
};

class HistoricalGps {
    public:
        int route_nr = 0;
        float time = 0.0f;
        float x = 0.0f;
        float y = 0.0f;
};

class Scheduled {
    public:
        Scheduled(int rnr, int snr, int h, int m);
        int route_nr = 0;
        int start_nr = 0;
        Time time;
};

Scheduled::Scheduled(int rnr, int snr, int h, int m)
    : route_nr(rnr),
    start_nr(snr),
    time(h, m)
{
}

class App {
    public:
        App(const char* stop_name,
                const char* gps_raw,
                const char* gps,
                const char* sched,
                const char* time);
        void run();

    private:
        void read_time();
        void update_all();
        void parse_raw_gps();
        void update_gps_info();
        void update_sched_info();
        void update_bus_info();
        void set_labels();
        static std::string time_to_string(const Time& t);
        void cache_gps(const char* gps);
        void cache_sched(const char* sched);

        std::string m_stop_name;
        std::string m_gps_raw_filename;
        std::vector<HistoricalGps> m_historical_gps;
        std::vector<Scheduled> m_scheduled;
        std::vector<GpsInfo> m_gps_raw;
        std::vector<BusInfo> m_sched_info;
        std::vector<BusInfo> m_gps_info;
        std::vector<BusInfo> m_bus_info;
        std::map<int, std::string> m_route_names;
        label_container m_labels;
        Time m_time;
        bool m_fixed_time;
};

App::App(const char* stop_name,
                const char* gps_raw,
                const char* gps,
                const char* sched,
                const char* time)
    : m_stop_name(stop_name),
    m_gps_raw_filename(gps_raw)
{
    m_fixed_time = time != NULL;
    if(m_fixed_time) {
        std::string t(time);
        auto ind = t.find(":");
        if(ind == std::string::npos)
            throw std::runtime_error("Could not parse time");
        int h = std::stoi(t.substr(0, ind));
        int m = std::stoi(t.substr(ind + 1));
        if(h < 0 || h > 23 || m < 0 || m > 59)
            throw std::runtime_error("Invalid time");
        m_time = Time(h, m);
    }
    m_route_names[3]   = "Park Street";
    m_route_names[29]  = "Central Park";
    m_route_names[848] = "Wall Street";
    m_route_names[6]   = "Manhattan Square";
    m_route_names[72]  = "Hill Street";
    cache_gps(gps);
    cache_sched(sched);
    update_all();
}

void App::cache_gps(const char* gps)
{
    std::ifstream infile(gps);
    std::string line;
    while(std::getline(infile, line)) {
        std::istringstream iss(line);
        HistoricalGps g;
        if(!(iss >> g.route_nr >> g.time >> g.x >> g.y)) {
            throw std::runtime_error("Could not parse gps data");
        }
        m_historical_gps.push_back(g);
    }
}

void App::cache_sched(const char* sched)
{
    std::ifstream infile(sched);
    std::string line;
    while(std::getline(infile, line)) {
        std::istringstream iss(line);
        Scheduled s(0, 0, 0, 0);
        if(!(iss >> s.route_nr >> s.start_nr >> s.time.hour >> s.time.minute)) {
            throw std::runtime_error("Could not parse schedule data");
        }
        m_scheduled.push_back(s);
    }
}

void App::update_all()
{
    read_time();
    parse_raw_gps();
    update_gps_info();
    update_sched_info();
    update_bus_info();
    set_labels();
}

void App::parse_raw_gps()
{
    std::vector<GpsInfo> gps_new;
    std::ifstream infile(m_gps_raw_filename);
    std::string line;
    while(std::getline(infile, line)) {
        std::istringstream iss(line);
        GpsInfo g;
        int kind;
        if(!(iss >> g.route_nr >> g.start_nr >> g.x >> g.y >> kind)) {
            fprintf(stderr, "Could not parse raw gps data\n");
            return;
        }
        g.kind = Kind(kind);
        gps_new.push_back(g);
    }
    m_gps_raw = gps_new;
}

void App::update_gps_info()
{
    /* 1. find entries in historical data that are
       within 100m of current gps data, and note the times
       2. for all (not passed) buses with noted times,
       calculate the average time
       3. sort the entries by average time, keep the
       newest 20
       4. store these + all passed buses in m_gps_info
       */
    m_gps_info.clear();
    std::vector<std::tuple<GpsInfo, float>> entries;
    for(const auto& g : m_gps_raw) {
        if(g.kind == Kind::Passed) {
            m_gps_info.push_back(BusInfo(g.route_nr,
                        g.start_nr,
                        0, 0,
                        Kind::Passed));
            continue;
        }
        float sum_times = 0.0f;
        int num_times = 0;
        for(const auto& h : m_historical_gps) {
            auto dx = h.x - g.x;
            auto dy = h.y - g.y;
            auto dist = dx * dx + dy * dy;
            if(dist < 10000.0f) {
                sum_times += h.time;
                num_times++;
            }
        }
        if(num_times) {
            float avg_time = sum_times / (float)num_times;
            entries.push_back(std::make_tuple(g, avg_time));
        }
    }
    std::sort(entries.begin(), entries.end(),
        [](const std::tuple<GpsInfo, float>& b1,
            const std::tuple<GpsInfo, float>& b2) {
            return std::get<1>(b1) < std::get<1>(b2);
        });
    int num_added = 0;
    for(const auto& g : entries) {
        auto h = m_time.hour;
        auto m = m_time.minute + std::get<1>(g);
        while(m >= 60) {
            m -= 60;
            h++;
        }
        if(h > 23)
            h -= 24;
        m_gps_info.push_back(BusInfo(std::get<0>(g).route_nr,
                    std::get<0>(g).start_nr, h, m, Kind::GPS));
        num_added++;
        if(num_added == 20)
            break;
    }
}

void App::update_sched_info()
{
    m_sched_info.clear();
    /* 1. pick all scheduled buses from now to end of day
       2. sort them by how far off in time they are
       3. store the first 50
       */
    for(const auto& s : m_scheduled) {
        if(time_diff(m_time, s.time) > 0) {
            m_sched_info.push_back(BusInfo(s.route_nr, s.start_nr, s.time, Kind::Schedule));
        }
    }
    std::sort(m_sched_info.begin(), m_sched_info.end(),
        [](const BusInfo& b1, const BusInfo& b2) {
            return time_diff(b2.time, b1.time) < 0;
        });
    if(m_sched_info.size() > 50)
        m_sched_info.erase(m_sched_info.begin() + 50, m_sched_info.end() - 1);
}

// merge gps and sched info, fill out bus info
void App::update_bus_info()
{
    // combine gps_info and sched_info
    // find out which buses have passed, and filter them out
    // find out which buses have gps data, and filter out their scheduled entries
    // sort by time
    m_bus_info = m_gps_info;
    m_bus_info.insert(m_bus_info.end(), m_sched_info.begin(), m_sched_info.end());

    std::vector<std::tuple<int, int>> passed;
    std::vector<std::tuple<int, int>> have_gps;
    for(const auto& bi : m_bus_info) {
        if(bi.kind == Kind::Passed)
            passed.push_back(std::make_tuple(bi.route_nr, bi.start_nr));
        else if(bi.kind == Kind::GPS)
            have_gps.push_back(std::make_tuple(bi.route_nr, bi.start_nr));
    }

    std::remove_if(m_bus_info.begin(), m_bus_info.end(),
            [&](const BusInfo& bi) {
                return std::find(passed.begin(), passed.end(),
                        std::make_tuple(bi.route_nr, bi.start_nr)) != passed.end();
            });

    std::remove_if(m_bus_info.begin(), m_bus_info.end(),
            [&](const BusInfo& bi) {
                return bi.kind == Kind::Schedule && std::find(have_gps.begin(), have_gps.end(),
                        std::make_tuple(bi.route_nr, bi.start_nr)) != have_gps.end();
            });

    std::sort(m_bus_info.begin(), m_bus_info.end(),
        [&](const BusInfo& b1, const BusInfo& b2) {
            return time_diff(m_time, b1.time) < time_diff(m_time, b2.time);
        });
}

std::string App::time_to_string(const Time& t)
{
    char buf[10] = {0};
    snprintf(buf, sizeof(buf) - 1, "%02d:%02d", t.hour, t.minute);
    return std::string(buf);
}

void App::set_labels()
{
    m_labels[0] = m_stop_name;
    m_labels[1] = time_to_string(m_time);
    for(int i = 0; i < (num_labels - 2) / 3; i++) {
        m_labels[i * 3 + 2] = std::to_string(m_bus_info[i].route_nr);
        m_labels[i * 3 + 3] = m_route_names[m_bus_info[i].route_nr];
        if(m_bus_info[i].kind == Kind::Schedule) {
            m_labels[i * 3 + 4] = "ca. " + time_to_string(m_bus_info[i].time);
        } else {
            m_labels[i * 3 + 4] = time_to_string(m_bus_info[i].time);
        }
    }
}

void App::read_time()
{
    if(m_fixed_time)
        return;
    time_t t;
    time(&t);
    tm* now = localtime(&t);
    m_time.hour   = now->tm_hour;
    m_time.minute = now->tm_min;
}

void App::run()
{
    SDL_Context ctxt(m_labels);
    while(1) {
        bool cont = ctxt.run(m_labels, 10000);
        if(!cont)
            return;
        update_all();
    }
}

int main(int argc, char** argv)
{
    if(argc == 2) {
        SDL_Context ctxt(argv[1]);
        ctxt.run();
    } else if(argc == 5 || argc == 6) {
        const char* time = argc == 6 ? argv[5] : NULL;
        App app(argv[1], argv[2], argv[3], argv[4], time);
        app.run();
    }
    else {
        fprintf(stderr, "Usage: %s <stop name> <gps_raw.txt> <gps.txt> <sched.txt> [time]\n", argv[0]);
        fprintf(stderr, "Alternative sage: %s <label file>\n", argv[0]);
    }
}
