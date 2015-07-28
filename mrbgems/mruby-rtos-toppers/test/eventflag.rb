# RTOS::EventFlag class

assert('EventFlag', "class") do
  RTOS::EventFlag.class == Class
end

assert('EventFlag', 'new') do
  f1 = RTOS::EventFlag.new
  f2 = RTOS::EventFlag.new(1)
  f3 = RTOS::EventFlag.new(2, :manual)
  f4 = RTOS::EventFlag.new(3, :auto, :multiple, :fifo)
  f1 && f2 && f3 && f4
end

assert('EvetnFlag', 'ATTRIBUTE') do
  atr = RTOS::EventFlag::ATTRIBUTE
  atr &&
  atr[:auto] && atr[:manual] &&
  atr[:fifo] && atr[:priority] &&
  atr[:single] && atr[:multiple]
end

# assert('EventFlag', 'WAITMODE') do
#   mode = RTOS::EventFlag::WAITMODE
#   mode && mode[:and] && mode[:or]
# end

# assert('EventFlag', 'TIMEOUT') do
#   tmo = RTOS::EventFlag::TIMEOUT
#   tmo && tmo[:polling] && tmo[:forever]
# end
#
assert('EventFlag', 'set') do
  RTOS::EventFlag.new.set == 1 &&
  RTOS::EventFlag.new.set(0xf) == 0xf
end

assert('EventFlag', 'wait') do
  f = RTOS::EventFlag.new
  f.wait == 1
end

assert('EventFlag', 'wait(flg)') do
  f = RTOS::EventFlag.new
  f.wait(0xf) == 0xf
end

assert('EventFlag', 'wait(flg, mode)') do
  f = RTOS::EventFlag.new
  f.wait(0xf, :and) == 0xf &&
  f.wait(0x1, :or) == 0x1
end

assert('EventFlag', 'wait(flg, mode, tmo)') do
  f = RTOS::EventFlag.new
  f.wait(0xf, :and, :forever) == 0xf &&
  f.wait(0xf, :and, :polling) == 0xf &&
  f.wait(0xf, :and, 10) == 0xf
end

assert('EventFlag', 'wait: parameter error') do
  e1, e2, e3 = nil, nil, nil
  f = RTOS::EventFlag.new
  begin
    f.wait(:hoge)
  rescue => e1
  end
  begin
    f.wait(1, :xor)
  rescue => e2
  end
  begin
    f.wait(1, :or, :unknown)
  rescue => e3
  end
  e1 && e2 && e3
end

assert('EventFlag', 'clear') do
  f = RTOS::EventFlag.new
  f.clear == nil && f.clear(0x0f) == nil
end
