##
# EV3 Test

# EV3 module

assert('EV3', 'module') do
  EV3.class == Module
end

assert('EV3', 'Object.include?') do
  Object.include?(EV3)
end

# RTOS API

assert('EV3', 'delay') do
  e = nil
  begin
    delay 1
  rescue => e
  end
  !e
end

assert('EV3', 'delay: no argument') do
  e = nil
  begin
    delay
  rescue => e
  end
  e && e.class == ArgumentError
end
