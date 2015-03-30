# Task class

assert('Task', "class") do
  Task.class == Class
end

assert('Task', 'include?(RTOS)') do
  Task.include?(RTOS)
end

assert('Task', 'new') do
  Task.new(1)
end

assert('Task', 'new: few argument') do
  e = nil
  begin
    Task.new
  rescue => e
  end
  e.class == ArgumentError
end

assert('Task', 'activate') do
  Task.new(1).activate == nil
end

assert('Task', 'suspend') do
  Task.new(1).suspend == nil
end

assert('Task', 'resume') do
  Task.new(1).resume == nil
end

assert('Task', 'terminate') do
  Task.new(1).terminate == nil
end
