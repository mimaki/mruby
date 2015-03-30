##
# RTOS Test

# RTOS module

assert('RTOS', 'module') do
  RTOS.class == Module
end

assert('RTOS', 'Object.include?') do
  Object.include?(RTOS)
end

# RTOS API

assert('RTOS', 'delay') do
  e = nil
  begin
    delay 1
  rescue => e
  end
  !e
end

assert('RTOS', 'delay: no argument') do
  e = nil
  begin
    delay
  rescue => e
  end
  e && e.class == ArgumentError
end

assert('RTOS', 'millis') do
  millis
end
