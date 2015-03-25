# Motor class

assert('Motor', "class") do
  Motor.class == Class
end

assert('Motor', 'include?(EV3)') do
  Motor.include?(EV3)
end

assert('Motor', 'superclass') do
  Motor.superclass == Device
end

assert('Motor', 'PORT') do
  Motor::PORT[:port_a] == 0 &&
  Motor::PORT[:port_b] == 1 &&
  Motor::PORT[:port_c] == 2 &&
  Motor::PORT[:port_d] == 3
end

assert('Motor', 'TYPE') do
  Motor::TYPE[:medium]  == 1 &&
  Motor::TYPE[:large]   == 2 &&
  Motor::TYPE[:unknown] == 3
end

assert('Motor', 'new') do
  mtr = []
  mtr << Motor.new(:port_a)
  mtr << Motor.new("port_b", "medium")
  mtr << Motor.new(:port_c,  "large")
  mtr << Motor.new("port_d", "unknown")
  mtr.size == 4
end

assert('Motor', 'new: parameter error') do
  e1 = e2 = nil
  begin
    Motor.new
  rescue => e1
  end
  begin
    Motor.new(:port_a)
  rescue => e2
  end
  e1 && !e2
end

assert('Motor', 'port') do
  Motor.new(:port_a).port == Motor::PORT[:port_a] &&
  Motor.new(:port_b).port == Motor::PORT[:port_b] &&
  Motor.new(:port_c).port == Motor::PORT[:port_c] &&
  Motor.new(:port_d).port == Motor::PORT[:port_d]
end

assert('Motor', 'type') do
  Motor.new(:port_a).type           == Motor::TYPE[:large] &&
  Motor.new(:port_b, :medium).type  == Motor::TYPE[:medium] &&
  Motor.new(:port_c, :large).type   == Motor::TYPE[:large] &&
  Motor.new(:port_d, :unknown).type == Motor::TYPE[:unknown]
end

assert('Motor', 'power') do
  b = true
  mtr = Motor.new(:port_a)
  [0, 100, -100, 200, -200].each {|pwr|
    mtr.power = pwr
    pwr = 100 if pwr > 100
    pwr = -100 if pwr < -100
    b &= (mtr.power == pwr)
  }
  b
end

assert('Motor', 'stop') do
  e = nil
  begin
    mtr = Motor.new(:port_a)
    mtr.stop
    mtr.stop(true)
  rescue => e
  end
  !e
end

assert('Motor', 'rotate') do
  mtr = Motor.new(:port_a)
  mtr.rotate(10, 20)
  b = (mtr.count == 10)
  mtr.rotate(-100, 100, 1)
  b & (mtr.count == -100)
end

assert('Motor', 'rotate: parameter error') do
  mtr = Motor.new(:port_a)
  e1 = e2 = nil
  begin
    mtr.rotate
  rescue => e1
  end
  begin
    mtr.rotate(10)
  rescue => e2
  end
  e1 && e2
end

assert('Motor', 'count') do
  mtr = Motor.new(:port_a)
  mtr.rotate(123, 100)
  mtr.count == 123
end

assert('Motor', 'reset_count') do
  mtr = Motor.new(:port_a)
  mtr.rotate(10, 20)
  mtr.reset_count
  mtr.count == 0
end
