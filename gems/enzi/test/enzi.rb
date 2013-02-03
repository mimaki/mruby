assert('ENZI', 'module') do
  ENZI.class == Module
end

assert('ENZI', 'Object.include?') do
  Object.include?(ENZI)
end

assert('ENZI', 'LOW') do
  LOW == 0
end

assert('ENZI', 'HIGH') do
  HIGH == 1
end

assert('ENZI', 'INPUT') do
  INPUT == 0
end

assert('ENZI', 'OUTPUT') do
  OUTPUT == 1
end

assert('ENZI', 'INPUT_PULLUP') do
  INPUT_PULLUP == 2
end

assert('ENZI', 'A0-A5') do
  A0 == 0 &&
  A1 == 1 &&
  A2 == 2 &&
  A3 == 3 &&
  A4 == 4 &&
  A5 == 5
end

assert('ENZI', 'D0-D13') do
  D0 == 0 &&
  D1 == 1 &&
  D2 == 2 &&
  D3 == 3 &&
  D4 == 4 &&
  D5 == 5 &&
  D6 == 6 &&
  D7 == 7 &&
  D8 == 8 &&
  D9 == 9 &&
  D10 == 10 &&
  D11 == 11 &&
  D12 == 12 &&
  D13 == 13
end

assert('ENZI', 'D14-D19') do
  D14 == 0 &&
  D15 == 1 &&
  D16 == 2 &&
  D17 == 3 &&
  D18 == 4 &&
  D19 == 5
end

assert('ENZI', 'DLED') do
  DLED == 14
end

assert('ENZI', 'PWM0-PWM5') do
  PWM0 == 3 &&
  PWM1 == 5 &&
  PWM2 == 6 &&
  PWM3 == 9 &&
  PWM4 == 10 &&
  PWM5 == 11
end

assert('ENZI', 'LEVEL') do
  LEVEL[:LOW] == LOW &&
  LEVEL[:HIGH] == HIGH
end

assert('ENZI', 'IOMODE') do
  IOMODE[:INPUT] == INPUT &&
  IOMODE[:OUTPUT] == OUTPUT &&
  IOMODE[:INPUT_PULLUP] == INPUT_PULLUP
end

assert('ENZI', 'DIOPIN') do
  DIOPIN[:D0] == D0 &&
  DIOPIN[:D1] == D1 &&
  DIOPIN[:D2] == D2 &&
  DIOPIN[:D3] == D3 &&
  DIOPIN[:D4] == D4 &&
  DIOPIN[:D5] == D5 &&
  DIOPIN[:D6] == D6 &&
  DIOPIN[:D7] == D7 &&
  DIOPIN[:D8] == D8 &&
  DIOPIN[:D9] == D9 &&
  DIOPIN[:D10] == D10 &&
  DIOPIN[:D11] == D11 &&
  DIOPIN[:D12] == D12 &&
  DIOPIN[:D13] == D13 &&
  DIOPIN[:D14] == A0 &&
  DIOPIN[:D15] == A1 &&
  DIOPIN[:D16] == A2 &&
  DIOPIN[:D17] == A3 &&
  DIOPIN[:D18] == A4 &&
  DIOPIN[:D19] == A5 &&
  DIOPIN[:DLED] == DLED
end

assert('ENZI', 'AIPIN') do
  AIPIN[:A0] == A0 &&
  AIPIN[:A1] == A1 &&
  AIPIN[:A2] == A2 &&
  AIPIN[:A3] == A3 &&
  AIPIN[:A4] == A4 &&
  AIPIN[:A5] == A5
end

assert('ENZI', 'PWMPIN') do
  PWMPIN[:PWM0] == PWM0 &&
  PWMPIN[:PWM1] == PWM1 &&
  PWMPIN[:PWM2] == PWM2 &&
  PWMPIN[:PWM3] == PWM3 &&
  PWMPIN[:PWM4] == PWM4 &&
  PWMPIN[:PWM5] == PWM5
end

assert('ENZI.delay', 'few args') do
  e = nil
  begin
    delay()
  rescue => e
  end
  e
end

assert('ENZI.delay') do
  delay(1)
  true
end

assert('ENZI.millis') do
  millis >= 0
end

assert('ENZI.sleep', 'few args') do
  e = nil
  begin
    delay()
  rescue => e
  end
  e
end

assert('ENZI.sleep') do
  e = nil
  begin
    sleep(1)
    sleep(0.001)
  rescue => e
  end
  e == nil
end
