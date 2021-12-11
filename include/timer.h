#ifndef _timer
#define _timer

#include <chrono>
#include "MWEngineLow.h"
#include "TMP.h"

class Timer
{
public:
	using Func_t = void( * )( int );
	using Time_t = float;
	using FPS_t = Time_t;

	void setFPS( const FPS_t& fps )
	{
		objfps = fps;
		curfps = fps;
		ms_per_frame = 1000 / fps;
	}

	void setfunc( Func_t func ) { this->func = func; }

	void run()
	{
		is_running = true;
		glutTimerFunc( ms_per_frame, func, id_component.getid() );
	}

	void stop()
	{
		is_running = false;
		is_last_tp_invalid = true;
	}

	const float getlag() const { return lag; }
	const float getFPS_setted() const { return objfps; }
	const float getFPS() const { return curfps; }
	const float get_frame_time() const { return frame_time; }
	const float get_frame_time_setted() const { return ms_per_frame; }
	const float get_time() const { return ms_time; }

	Timer( Func_t func = nullptr, const FPS_t fps = static_cast< FPS_t >( 60 ) )
		: func{ func }, frame_time { 0 }, lag{ 0 }, ms_per_frame{ 1000 / fps }, ms_time{ 0 }, curfps{ fps }, objfps{ fps },
		is_running{ false }, last_tp{}, is_last_tp_invalid{ true }, id_component{} {}

private:
	Time_t frame_time;
	Time_t lag;
	Time_t ms_per_frame;
	Time_t ms_time;
	FPS_t curfps;
	FPS_t objfps;
	std::chrono::system_clock::time_point last_tp;
	bool is_running;
	bool is_last_tp_invalid;
	ComponentID id_component;
	Func_t func;

	void update()
	{
		if ( is_running )
		{
			update_time();
			prevent_overflow();
			update_curfps( frame_time );
			
			glutTimerFunc( ms_per_frame, func, id_component.getid() );
		}
	}

	void update_time()
	{
		using namespace std::chrono;

		if ( is_last_tp_invalid )
		{
			last_tp = system_clock::now();
			is_last_tp_invalid = false;
		}

		auto cur_tp = system_clock::now();
		frame_time = duration_cast< nanoseconds >( cur_tp - last_tp ).count()
			/ static_cast< float >( nanoseconds::period::den / milliseconds::period::den );

		ms_time += frame_time;
		lag = std::max( lag + frame_time - ms_time, static_cast< Time_t >( 0 ) );

		last_tp = cur_tp;
	}

	void prevent_overflow()
	{
		static constexpr Time_t limit = static_cast< Time_t >( 10'000'000 );
		if ( ms_time > limit ) ms_time -= limit;
	}

	void update_curfps( const float frame_time ) { curfps = 1000 / frame_time; }
};

#endif