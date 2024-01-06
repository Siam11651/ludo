#include <time.hpp>
#include <thread>

ludo::clk::time_point ludo::time::s_start_time_point;
ludo::clk::time_point ludo::time::s_frame_begin_time_point;
ludo::clk::time_point ludo::time::s_frame_end_time_point;
float ludo::time::s_delta_time_s = 0.0f;

const float &ludo::time::FPS()
{
    return s_FPS;
}

int64_t ludo::time::frame_time_ns()
{
    return (int64_t)(1000000000 / s_FPS);
}

void ludo::time::initialise()
{
    s_start_time_point = ludo::clk::now();
}

void ludo::time::start_frame()
{
    s_frame_begin_time_point = ludo::clk::now();
}

void ludo::time::end_frame()
{
    s_frame_end_time_point = ludo::clk::now();
    ludo::clk::duration nanoseconds = s_frame_end_time_point - s_frame_begin_time_point;

    if(nanoseconds.count() < frame_time_ns())
    {
        ludo::clk::duration sleep_time(frame_time_ns() - nanoseconds.count());

        std::this_thread::sleep_for(sleep_time);

        s_frame_end_time_point = clk::now();
        nanoseconds = s_frame_end_time_point - s_frame_begin_time_point;
    }

    s_delta_time_s = nanoseconds.count() / 1e9f;
}

const float &ludo::time::get_delta_time_s()
{
    return s_delta_time_s;
}

int64_t ludo::time::get_now_ns()
{
    ludo::clk::time_point now_time_point(ludo::clk::now());
    ludo::clk::duration time_point_ns(now_time_point - s_start_time_point);

    return time_point_ns.count();
}

float ludo::time::get_now_s()
{
    return get_now_ns() / 1e9f;
}