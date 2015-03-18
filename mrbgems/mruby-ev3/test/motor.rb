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

assert('Motor', 'constants') do
  Motor::PORT_A   == 0 &&
  Motor::PORT_B   == 1 &&
  Motor::PORT_C   == 2 &&
  Motor::PORT_D   == 3 &&
  Motor::NONE     == 0 &&
  Motor::MEDIUM   == 1 &&
  Motor::LARGE    == 2 &&
  Motor::UNKNOWN  == 3
end

assert('Motor', 'new') do
  mtr = []
  mtr << Motor.new(Motor::PORT_A, Motor::LARGE)
  mtr << Motor.new(Motor::PORT_B, Motor::MEDIUM)
  mtr << Motor.new(Motor::PORT_C, Motor::NONE)
  mtr << Motor.new(Motor::PORT_D, Motor::UNKNOWN)
  mtr.size == 4
end

assert('Motor', 'new: parameter error') do
  e1 = e2 = nil
  begin
    Motor.new
  rescue => e1
  end
  begin
    Motor.new(Motor::PORT_A)
  rescue => e2
  end
  e1 && e2
end

assert('Motor', 'port') do
  Motor.new(Motor::PORT_A, Motor::NONE).port    == Motor::PORT_A &&
  Motor.new(Motor::PORT_B, Motor::MEDIUM).port  == Motor::PORT_B &&
  Motor.new(Motor::PORT_C, Motor::LARGE).port   == Motor::PORT_C &&
  Motor.new(Motor::PORT_D, Motor::UNKNOWN).port == Motor::PORT_D
end

assert('Motor', 'type') do
  Motor.new(Motor::PORT_A, Motor::NONE).type    == Motor::NONE &&
  Motor.new(Motor::PORT_B, Motor::MEDIUM).type  == Motor::MEDIUM &&
  Motor.new(Motor::PORT_C, Motor::LARGE).type   == Motor::LARGE &&
  Motor.new(Motor::PORT_D, Motor::UNKNOWN).type == Motor::UNKNOWN
end

assert('Motor', 'power') do
  b = true
  mtr = Motor.new(Motor::PORT_A, Motor::LARGE)
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
    mtr = Motor.new(Motor::PORT_A, Motor::LARGE)
    mtr.stop
    mtr.stop(true)
  rescue => e
  end
  !e
end

assert('Motor', 'rotate') do
  mtr = Motor.new(Motor::PORT_A, Motor::LARGE)
  mtr.rotate(10, 20)
  b = (mtr.count == 10)
  mtr.rotate(-100, 100, 1)
  b & (mtr.count == -100)
end

assert('Motor', 'rotate: parameter error') do
  mtr = Motor.new(Motor::PORT_A, Motor::LARGE)
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
  mtr = Motor.new(Motor::PORT_A, Motor::LARGE)
  mtr.rotate(123, 100)
  mtr.count == 123
end

assert('Motor', 'reset_count') do
  mtr = Motor.new(Motor::PORT_A, Motor::LARGE)
  mtr.rotate(10, 20)
  mtr.reset_count
  mtr.count == 0
end
