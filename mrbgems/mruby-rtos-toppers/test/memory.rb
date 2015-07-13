# RTOS::MemoryPool class

assert('MemoryPool', "class") do
  RTOS::MemoryPool.class == Class
end

assert('MemoryPool', 'new') do
  m0 = RTOS::MemoryPool.new
  m1 = RTOS::MemoryPool.new(1)
  m2 = RTOS::MemoryPool.new(nil, 32)
  m3 = RTOS::MemoryPool.new(nil, 64, 4)
  m0 && m1 && m2 && m3
end

assert('MemoryPool', 'get') do
  mp = RTOS::MemoryPool.new
  m = mp.get(0)
  m.class == RTOS::MemoryBuffer
end

assert('MemoryPool', 'release') do
  mp = RTOS::MemoryPool.new
  mp.get.release == nil
end

# RTOS::MemoryBuffer class

assert('MemoryBuffer', 'new') do
  m1 = RTOS::MemoryBuffer.new(1, 256)
  m2 = RTOS::MemoryBuffer.new(1, 256, 0)
  m3 = RTOS::MemoryBuffer.new(1, 256, :forever)
  m4 = RTOS::MemoryBuffer.new(1, 256, :polling)
  m1 && m2 && m3 && m4
end

assert('MemoryBuffer', 'new: few parameter') do
  e0, e1 = nil, nil
  begin
    RTOS::MemoryBuffer.new
  rescue => e0
  end
  begin
    RTOS::MemoryBuffer.new(1)
  rescue => e1
  end
  e0 && e1
end

assert('MemoryBuffer', 'release') do
  m1 = RTOS::MemoryPool.new.get(0)
  m2 = RTOS::MemoryBuffer.new(0, 256)
  m1.release == nil && m2.release == nil
end

assert('MemoryBuffer', 'data, data=') do
  mp = RTOS::MemoryPool.new(nil, 16, 8)
  m1 = mp.get;  m1.data = "a"
  m2 = mp.get;  m2.data = "1234567890123456"
  m3 = mp.get;  m3.data = "12345678901234567"
  m4 = mp.get;  m4.data = [1,2,3]
  m1.data == "a" &&
  m1.data(true) == "a" &&
  m1.data(false) == "a" + "\0" * 15 &&
  m2.data == "1234567890123456" &&
  m3.data == "1234567890123456" &&
  m4.data == [1,2,3].to_s
end

assert('MemoryBuffer', 'release -> data') do
  e1, e2 = nil, nil
  m1 = RTOS::MemoryPool.new.get
  m1.data = "12345"
  m1.release
  begin
    m1.data
  rescue => e1
  end
  begin
    m1.data = "abcdefg"
  rescue => e2
  end
  e1 && e2
end
