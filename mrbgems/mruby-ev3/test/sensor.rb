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

assert('Sensor', 'constants') do
  Sensor::PORT_1      == 0 &&
  Sensor::PORT_2      == 1 &&
  Sensor::PORT_3      == 2 &&
  Sensor::PORT_4      == 3 &&
  Sensor::NONE        == 0 &&
  Sensor::ULTRASONIC  == 1 &&
  Sensor::GYRO        == 2 &&
  Sensor::TOUCH       == 3 &&
  Sensor::COLOR       == 4
end

assert('Sensor', 'port') do
  ColorSensor.new(Sensor::PORT_1).port      == Sensor::PORT_1 &&
  GyroSensor.new(Sensor::PORT_2).port       == Sensor::PORT_2 &&
  TouchSensor.new(Sensor::PORT_3).port      == Sensor::PORT_3 &&
  UltrasonicSensor.new(Sensor::PORT_4).port == Sensor::PORT_4
end

assert('Sensor', 'type') do
  ColorSensor.new(Sensor::PORT_1).type      == Sensor::COLOR &&
  GyroSensor.new(Sensor::PORT_2).type       == Sensor::GYRO &&
  TouchSensor.new(Sensor::PORT_3).type      == Sensor::TOUCH &&
  UltrasonicSensor.new(Sensor::PORT_4).type == Sensor::ULTRASONIC
end


#
# Color sensor
#

assert('ColorSensor', 'new') do
  sen = ColorSensor.new(Sensor::PORT_1)
  sen &&
  sen.class.superclass == EV3::Sensor
  sen.instance_variable_get("@port") == Sensor::PORT_1 &&
  sen.instance_variable_get("@type") == Sensor::COLOR
end

assert('ColorSensor', 'constants') do
  ColorSensor::NONE   == 0 &&
  ColorSensor::BLACK  == 1 &&
  ColorSensor::BLUE   == 2 &&
  ColorSensor::GREEN  == 3 &&
  ColorSensor::YELLOW == 4 &&
  ColorSensor::RED    == 5 &&
  ColorSensor::WHITE  == 6 &&
  ColorSensor::BROWN  == 7
end

assert('ColorSensor', 'ambient') do
  sen = ColorSensor.new(Sensor::PORT_2)
  sen.ambient
end

assert('ColorSensor', 'color') do
  sen = ColorSensor.new(Sensor::PORT_3)
  sen.color
end

assert('ColorSensor', 'reflect') do
  sen = ColorSensor.new(Sensor::PORT_4)
  sen.reflect
end

assert('ColorSensor', 'black?') do
  ColorSensor.new(Sensor::PORT_1).black? == false
end

assert('ColorSensor', 'blue?') do
  ColorSensor.new(Sensor::PORT_1).blue? == false
end

assert('ColorSensor', 'green?') do
  ColorSensor.new(Sensor::PORT_1).green? == false
end

assert('ColorSensor', 'yellow?') do
  ColorSensor.new(Sensor::PORT_1).yellow? == false
end

assert('ColorSensor', 'red?') do
  ColorSensor.new(Sensor::PORT_1).red? == false
end

assert('ColorSensor', 'white?') do
  ColorSensor.new(Sensor::PORT_1).white? == false
end

assert('ColorSensor', 'brown?') do
  ColorSensor.new(Sensor::PORT_1).brown? == false
end

#
# Gyro sensor
#

assert('GyroSensor', 'new') do
  sen = GyroSensor.new(Sensor::PORT_1)
  sen &&
  sen.class.superclass == EV3::Sensor
  sen.instance_variable_get("@port") == Sensor::PORT_1 &&
  sen.instance_variable_get("@type") == Sensor::GYRO
end

assert('GyroSensor', 'angle') do
  sen = GyroSensor.new(Sensor::PORT_2)
  sen.angle
end

assert('GyroSensor', 'rate') do
  sen = GyroSensor.new(Sensor::PORT_3)
  sen.rate
end

assert('GyroSensor', 'reset') do
  sen = GyroSensor.new(Sensor::PORT_4)
  sen.reset == nil
end

assert('GyroSensor', 'calibrate') do
  sen = GyroSensor.new(Sensor::PORT_1)
  sen.calibrate == nil
end

assert('GyroSensor', 'offset') do
  sen = GyroSensor.new(Sensor::PORT_2)
  sen.offset
end


#
# Touch sensor
#

assert('TouchSensor', 'new') do
  sen = TouchSensor.new(Sensor::PORT_1)
  sen &&
  sen.class.superclass == EV3::Sensor
  sen.instance_variable_get("@port") == Sensor::PORT_1 &&
  sen.instance_variable_get("@type") == Sensor::TOUCH
end

assert('TouchSensor', 'pressed?') do
  sen = TouchSensor.new(Sensor::PORT_2)
  sen.pressed? == false
end


#
# Ultrasonic sensor
#

assert('UltrasonicSensor', 'new') do
  sen = UltrasonicSensor.new(Sensor::PORT_1)
  sen &&
  sen.class.superclass == EV3::Sensor
  sen.instance_variable_get("@port") == Sensor::PORT_1 &&
  sen.instance_variable_get("@type") == Sensor::ULTRASONIC
end

assert('UltrasonicSensor', 'distance') do
  sen = UltrasonicSensor.new(Sensor::PORT_2)
  sen.distance
end

assert('UltrasonicSensor', 'listen') do
  sen = UltrasonicSensor.new(Sensor::PORT_3)
  sen.listen == false
end
