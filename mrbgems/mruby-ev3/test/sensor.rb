# Sensor class

assert('Sensor', "class") do
  Sensor.class == Class
end

assert('Sensor', 'include?(EV3)') do
  Sensor.include?(EV3)
end

assert('Sensor', 'superclass') do
  Sensor.superclass == Device
end

assert('Sensor', 'PORT') do
  Sensor::PORT[:port_1] == 0 &&
  Sensor::PORT[:port_2] == 1 &&
  Sensor::PORT[:port_3] == 2 &&
  Sensor::PORT[:port_4] == 3
end

assert('Sensor', 'TYPE') do
  Sensor::TYPE[:ultrasonic] == 1 &&
  Sensor::TYPE[:gyro]       == 2 &&
  Sensor::TYPE[:touch]      == 3 &&
  Sensor::TYPE[:color]      == 4
end

assert('Sensor', 'port') do
  ColorSensor.new(:port_1).port       == Sensor::PORT[:port_1] &&
  GyroSensor.new("port_2").port       == Sensor::PORT[:port_2] &&
  TouchSensor.new(:port_3).port       == Sensor::PORT[:port_3] &&
  UltrasonicSensor.new("port_4").port == Sensor::PORT[:port_4]
end

assert('Sensor', 'type') do
  ColorSensor.new(:port_1).type      == Sensor::TYPE[:color] &&
  GyroSensor.new(:port_2).type       == Sensor::TYPE[:gyro] &&
  TouchSensor.new(:port_3).type      == Sensor::TYPE[:touch] &&
  UltrasonicSensor.new(:port_4).type == Sensor::TYPE[:ultrasonic]
end


#
# Color sensor
#

assert('ColorSensor', 'new') do
  sen = ColorSensor.new(:port_1)
  sen &&
  sen.class.superclass == EV3::Sensor
  sen.instance_variable_get("@port") == Sensor::PORT[:port_1] &&
  sen.instance_variable_get("@type") == Sensor::TYPE[:color]
end

assert('ColorSensor', 'COLOR') do
  ColorSensor::COLOR[:none]   == 0 &&
  ColorSensor::COLOR[:black]  == 1 &&
  ColorSensor::COLOR[:blue]   == 2 &&
  ColorSensor::COLOR[:green]  == 3 &&
  ColorSensor::COLOR[:yellow] == 4 &&
  ColorSensor::COLOR[:red]    == 5 &&
  ColorSensor::COLOR[:white]  == 6 &&
  ColorSensor::COLOR[:brown]  == 7
end

assert('ColorSensor', 'ambient') do
  sen = ColorSensor.new(:port_2)
  sen.ambient
end

assert('ColorSensor', 'color') do
  sen = ColorSensor.new(:port_3)
  sen.color
end

assert('ColorSensor', 'reflect') do
  sen = ColorSensor.new(:port_4)
  sen.reflect
end

assert('ColorSensor', 'black?') do
  ColorSensor.new(:port_1).black? == false
end

assert('ColorSensor', 'blue?') do
  ColorSensor.new(:port_1).blue? == false
end

assert('ColorSensor', 'green?') do
  ColorSensor.new(:port_1).green? == false
end

assert('ColorSensor', 'yellow?') do
  ColorSensor.new(:port_1).yellow? == false
end

assert('ColorSensor', 'red?') do
  ColorSensor.new(:port_1).red? == false
end

assert('ColorSensor', 'white?') do
  ColorSensor.new(:port_1).white? == false
end

assert('ColorSensor', 'brown?') do
  ColorSensor.new(:port_1).brown? == false
end

#
# Gyro sensor
#

assert('GyroSensor', 'new') do
  sen = GyroSensor.new(:port_1)
  sen &&
  sen.class.superclass == EV3::Sensor
  sen.instance_variable_get("@port") == Sensor::PORT[:port_1] &&
  sen.instance_variable_get("@type") == Sensor::TYPE[:gyro]
end

assert('GyroSensor', 'angle') do
  sen = GyroSensor.new(:port_2)
  sen.angle
end

assert('GyroSensor', 'rate') do
  sen = GyroSensor.new(:port_3)
  sen.rate
end

assert('GyroSensor', 'reset') do
  sen = GyroSensor.new(:port_4)
  sen.reset == nil
end

assert('GyroSensor', 'calibrate') do
  sen = GyroSensor.new(:port_1)
  sen.calibrate == nil
end

assert('GyroSensor', 'offset') do
  sen = GyroSensor.new(:port_2)
  sen.offset
end


#
# Touch sensor
#

assert('TouchSensor', 'new') do
  sen = TouchSensor.new(:port_1)
  sen &&
  sen.class.superclass == EV3::Sensor
  sen.instance_variable_get("@port") == Sensor::PORT[:port_1] &&
  sen.instance_variable_get("@type") == Sensor::TYPE[:touch]
end

assert('TouchSensor', 'pressed?') do
  sen = TouchSensor.new(:port_2)
  sen.pressed? == false
end


#
# Ultrasonic sensor
#

assert('UltrasonicSensor', 'new') do
  sen = UltrasonicSensor.new(:port_1)
  sen &&
  sen.class.superclass == EV3::Sensor
  sen.instance_variable_get("@port") == Sensor::PORT[:port_1] &&
  sen.instance_variable_get("@type") == Sensor::TYPE[:ultrasonic]
end

assert('UltrasonicSensor', 'distance') do
  sen = UltrasonicSensor.new(:port_2)
  sen.distance
end

assert('UltrasonicSensor', 'listen') do
  sen = UltrasonicSensor.new(:port_3)
  sen.listen == false
end
