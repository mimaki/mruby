assert('WatchdogTimer', 'class') do
  WatchdogTimer.class == Class
end

assert('WatchdogTimer', 'superclass') do
  WatchdogTimer.superclass == Object
end

assert('WatchdogTimer', 'include?(ENZI)') do
  WatchdogTimer.include?(ENZI)
end

assert('WatchdogTimer::start', 'few args') do
  e = nil
  begin
    WatchdogTimer::start
  rescue => e
  end
  e
end

assert('WatchdogTimer::start') do
  WatchdogTimer::start(1) == nil
end

assert('WatchdogTimer::start', 'arg type error') do
  e = nil
  begin
    WatchdogTimer::start(:DEFAULT)
  rescue => e
  end
  e
end

assert('WatchdogTimer::cancel') do
  WatchdogTimer::cancel() == nil
end

assert('WatchdogTimer::stop') do
  WatchdogTimer::stop() == nil
end

assert('WatchdogTimer::start?') do
  s1, s2 = nil, nil
  WatchdogTimer::start(1)
  s1 = WatchdogTimer::start?
  WatchdogTimer::stop()
  s2 = WatchdogTimer::start?
  s1 == true && s2 == false
end
