# RTOS::DataQueue class

assert('DataQueue', 'class') do
  RTOS::DataQueue.class == Class
end

assert('DataQueue', 'new') do
  q1 = RTOS::DataQueue.new
  q2 = RTOS::DataQueue.new(1)
  q3 = RTOS::DataQueue.new(2, 3)
  q1 && q2 && q3
end

assert('DataQueue', 'send') do
  q = RTOS::DataQueue.new
  d1 = q.send 1
  d2 = q.send "abc"
  d3 = q.send [1,2,3]
  d1 && d2 && d3
end

assert('DataQueue', 'send: timeout') do
  q = RTOS::DataQueue.new
  d1 = q.send(10, 0)
  d2 = q.send(20, :polling)
  d3 = q.send(30, :forever)
  d1 && d2 && d3
end

assert('DataQueue', 'send: bad timeout') do
  e1, e2 = nil, nil
  q = RTOS::DataQueue.new
  begin
    q.send(1, :unknown)
  rescue => e1
  end
  begin
    q.send(2, "Invalid type")
  rescue => e2
  end
  e1 && e2
end

assert('DataQueue', 'get') do
  q = RTOS::DataQueue.new
  dt = [1, -273.15, "ABCdef123", [1,2,3,"xyz"], q]
  dt.each do |d|
    q.send d
  end
  b = true
  dt.each do |d|
    b &= (q.get == d.to_s)
  end
  b
end

assert('DataQueue', 'get: timeout') do
  q = RTOS::DataQueue.new
  dt = [[1, 100], [42.195, :polling], ["abc", :forever]]
  dt.each do |d|
    q.send d[0]
  end
  b = true
  dt.each do |d|
    b &= (q.get(d[1]) == d[0].to_s)
  end
  b
end

assert('DataQueue', 'get: bad timeout') do
  e1, e2 = nil, nil
  q = RTOS::DataQueue.new
  begin
    q.get(:unknown)
  rescue => e1
  end
  begin
    q.get("Invalid type")
  rescue => e2
  end
  e1 && e2
end
