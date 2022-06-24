#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <chrono>

template<class T, class D>
class Pair {
public:
	T first;
	D second;

	Pair(T f, D s) {
		first = f;
		second = s;
	}
};

enum class TimerStatus { UNUSED, RUNNING, STOPPED };

class Timer {
public:
	void Start() {
		_start = std::chrono::high_resolution_clock::now();
		_status = TimerStatus::RUNNING;
	}

	void Stop() {
		_elapsed = _MeasureDuration();
		_status = TimerStatus::STOPPED;
	}

	void Reset() {
		_elapsed = 0;
		if (_status == TimerStatus::STOPPED) {
			_status = TimerStatus::UNUSED;
		}
		else if (_status == TimerStatus::RUNNING) {
			_start = std::chrono::high_resolution_clock::now();
		}
	}

	double Measure() {
		if (_status == TimerStatus::RUNNING) {
			return _MeasureDuration();
		}
		else if (_status == TimerStatus::STOPPED) {
			return _elapsed;
		}
	}

	TimerStatus GetStatus() {
		return _status;
	}

private:
	double _MeasureDuration() {
		return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - _start).count() / 1000000.0;
	}

	std::chrono::high_resolution_clock::time_point _start;
	double _elapsed = 0;
	TimerStatus _status = TimerStatus::UNUSED;
};

#endif
