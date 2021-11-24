#pragma once

#include <uv.h>

class UVTimer;

class UVTimerObserver
{
public:
  virtual ~UVTimerObserver() = default;

  virtual void onTimeout(UVTimer* timer) = 0;
};

template<typename Callback>
class UVTimerObserverLambda final : public UVTimerObserver
{
public:
  UVTimerObserverLambda(Callback callback)
    : m_callback(callback)
  {}

  void onTimeout(UVTimer* timer) override { m_callback(timer); }

private:
  Callback m_callback;
};

class UVTimer final
{
public:
  UVTimer(uv_loop_t* loop, UVTimerObserver* observer);

  UVTimer(const UVTimer&) = delete;

  ~UVTimer();

  int start(uint64_t timeout, uint64_t repeat);

  int stop();

private:
  static void onTimeout(uv_timer_t* handle);

private:
  UVTimerObserver* m_observer;

  uv_timer_t m_handle;
};
