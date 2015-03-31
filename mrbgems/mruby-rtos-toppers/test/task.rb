# RTOS::Task class

assert('Task', "class") do
  RTOS::Task.class == Class
end

assert('Task', 'new') do
  RTOS::Task.new(1)
end

assert('Task', 'new: few argument') do
  e = nil
  begin
    RTOS::Task.new
  rescue => e
  end
  e.class == ArgumentError
end

assert('Task', 'activate') do
  RTOS::Task.new(1).activate == nil
end

assert('Task', 'suspend') do
  RTOS::Task.new(1).suspend == nil
end

assert('Task', 'resume') do
  RTOS::Task.new(1).resume == nil
end

assert('Task', 'terminate') do
  RTOS::Task.new(1).terminate == nil
end
