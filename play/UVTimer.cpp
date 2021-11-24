#include "UVTimer.h"

UVTimer::UVTimer(uv_loop_t* loop, UVTimerObserver* observer)
  : m_observer(observer)
{
  uv_timer_init(loop, &m_handle);

  uv_handle_set_data((uv_handle_t*)&m_handle, this);
}

UVTimer::~UVTimer()
{
  uv_close((uv_handle_t*)&m_handle, nullptr);
}

int
UVTimer::start(uint64_t timeout, uint64_t repeat)
{
  return uv_timer_start(&m_handle, &onTimeout, timeout, repeat);
}

int
UVTimer::stop()
{
  return uv_timer_stop(&m_handle);
}

void
UVTimer::onTimeout(uv_timer_t* handle)
{
  UVTimer* timer = (UVTimer*)uv_handle_get_data((uv_handle_t*)handle);

  timer->m_observer->onTimeout(timer);
}
