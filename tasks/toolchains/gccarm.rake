MRuby::Toolchain.new(:gccarm) do |conf|
  # C compiler settings
  [conf.cc, conf.cxx, conf.objc, conf.asm].each do |cc|
    cc.command = ENV['CC'] || 'arm-none-eabi-gcc'
    cc.flags = [ENV['CFLAGS'] || %w(
      -std=gnu99
      -mlittle-endian
      -O2
      -Wall
      -Wl,--no-warn-mismatch
    )]
    cc.defines = %w(
    )
  end

  # Archiver settings
  conf.archiver do |archiver|
    archiver.command = ENV['AR'] || 'arm-none-eabi-ar'
    archiver.archive_options = 'rcs %{outfile} %{objs}'
  end

  # [conf.cc, conf.objc, conf.asm].each do |cc|
  #   cc.command = ENV['CC'] || GCC_PREFIX + 'gcc'
  #   # cc.flags = [ENV['CFLAGS'] || %w(-g -std=gnu99 -O3 -Wall -Werror-implicit-function-declaration -Wdeclaration-after-statement)]
  #   cc.flags = [ENV['CFLAGS'] || %w(-g -std=gnu99 -O2 -Wall -mcpu=arm926ej-s -mlittle-endian)]
  #   cc.defines = %w(DISABLE_GEMS)
  #   cc.option_include_path = '-I%s'
  #   cc.option_define = '-D%s'
  #   cc.compile_options = '%{flags} -MMD -o %{outfile} -c %{infile}'
  # end

  # [conf.cxx].each do |cxx|
  #   cxx.command = ENV['CXX'] || GCC_PREFIX + 'g++'
  #   cxx.flags = [ENV['CXXFLAGS'] || ENV['CFLAGS'] || %w(-g -O3 -Wall -Werror-implicit-function-declaration)]
  #   cxx.defines = %w(DISABLE_GEMS)
  #   cxx.option_include_path = '-I%s'
  #   cxx.option_define = '-D%s'
  #   cxx.compile_options = '%{flags} -MMD -o %{outfile} -c %{infile}'
  # end

  # conf.linker do |linker|
  #   linker.command = ENV['LD'] || GCC_PREFIX + 'gcc'
  #   linker.flags = [ENV['LDFLAGS'] || %w()]
  #   linker.libraries = %w(m)
  #   linker.library_paths = []
  #   linker.option_library = '-l%s'
  #   linker.option_library_path = '-L%s'
  #   linker.link_options = '%{flags} -o %{outfile} %{objs} %{flags_before_libraries} %{libs} %{flags_after_libraries}'
  # end

  # [[conf.cc, 'c'], [conf.cxx, 'c++']].each do |cc, lang|
  #   cc.instance_variable_set :@header_search_language, lang
  #   def cc.header_search_paths
  #     if @header_search_command != command
  #       result = `echo | #{build.filename command} -x#{@header_search_language} -Wp,-v - -fsyntax-only 2>&1`
  #       result = `echo | #{command} -x#{@header_search_language} -Wp,-v - -fsyntax-only 2>&1` if $?.exitstatus != 0
  #       return include_paths if  $?.exitstatus != 0

  #       @frameworks = []
  #       @header_search_paths = result.lines.map { |v|
  #         framework = v.match(/^ (.*)(?: \(framework directory\))$/)
  #         if framework
  #           @frameworks << framework[1]
  #           next nil
  #         end

  #         v.match(/^ (.*)$/)
  #       }.compact.map { |v| v[1] }.select { |v| File.directory? v }
  #       @header_search_paths += include_paths
  #       @header_search_command = command
  #     end
  #     @header_search_paths
  #   end
  # end
end
